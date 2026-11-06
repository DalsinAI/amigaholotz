/* IMG_Load and IMG_Load_RW for Holotz's Castle on AmigaOS (from the Wizznic,
 * OpenLieroX and Gigalomania ports; see include/SDL/SDL_image.h). PNGs (on
 * libpng) and Truevision TGAs (plain or RLE, 24 or 32 bits: the game's
 * pictures are RLE TGAs) become 32-bit surfaces, with SDL_SRCALPHA when a
 * pixel is not opaque; anything else IMG_Load hands to SDL_LoadBMP.
 * The AmiCradle Team. GPL 3 or later, like Wizznic (Holotz's Castle is GPL 2
 * or later). */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL_image.h"
#include "png_rgba.h"

static unsigned char *read_file(const char *name, size_t *size)
{
	FILE *f = fopen(name, "rb");
	unsigned char *buf = NULL;
	long len;

	if (!f)
		return NULL;
	if (fseek(f, 0, SEEK_END) == 0 && (len = ftell(f)) > 0 && fseek(f, 0, SEEK_SET) == 0) {
		buf = (unsigned char *)malloc((size_t)len);
		if (buf && fread(buf, 1, (size_t)len, f) != (size_t)len) {
			free(buf);
			buf = NULL;
		}
		*size = (size_t)len;
	}
	fclose(f);
	return buf;
}

/* A TGA of colour type 2 (plain) or 10 (RLE), 24 or 32 bits, to RGBA rows
 * top first, R first. */
static unsigned char *tga_decode(const unsigned char *d, size_t size, int *w, int *h, int *has_alpha)
{
	int type, bpp, desc, x, y, Bpp;
	size_t pos, i, n, total;
	unsigned char *rgba, *p;

	if (size < 18)
		return NULL;
	type = d[2];
	bpp = d[16];
	desc = d[17];
	*w = d[12] | d[13] << 8;
	*h = d[14] | d[15] << 8;
	if ((type != 2 && type != 10) || (bpp != 24 && bpp != 32) || d[1] != 0 || *w <= 0 || *h <= 0)
		return NULL;
	Bpp = bpp / 8;
	pos = 18 + d[0];
	total = (size_t)*w * *h;
	rgba = (unsigned char *)malloc(total * 4);
	if (!rgba)
		return NULL;
	*has_alpha = 0;
	/* into rgba in file order (B, G, R[, A]), then flipped below */
	for (i = 0, p = rgba; i < total; ) {
		int rle = 0;
		n = 1;
		if (type == 10) {
			if (pos >= size)
				goto bad;
			rle = d[pos] & 0x80;
			n = (size_t)(d[pos] & 0x7f) + 1;
			pos++;
		}
		if (i + n > total)
			n = total - i;
		for (x = 0; x < (int)n; x++) {
			const unsigned char *px;
			if (pos + Bpp > size)
				goto bad;
			px = d + pos;
			if (!rle || x == (int)n - 1)
				pos += Bpp;
			p[0] = px[2];
			p[1] = px[1];
			p[2] = px[0];
			p[3] = Bpp == 4 ? px[3] : 255;
			if (p[3] != 255)
				*has_alpha = 1;
			p += 4;
		}
		i += n;
	}
	if (!(desc & 0x20)) {	/* bottom row first: turn it over */
		size_t row = (size_t)*w * 4;
		unsigned char *tmp = (unsigned char *)malloc(row);
		if (!tmp)
			goto bad;
		for (y = 0; y < *h / 2; y++) {
			memcpy(tmp, rgba + (size_t)y * row, row);
			memcpy(rgba + (size_t)y * row, rgba + (size_t)(*h - 1 - y) * row, row);
			memcpy(rgba + (size_t)(*h - 1 - y) * row, tmp, row);
		}
		free(tmp);
	}
	return rgba;
bad:
	free(rgba);
	return NULL;
}

/* PNG or TGA in memory to a surface; NULL (no error set) if it is neither */
static SDL_Surface *decode(const unsigned char *data, size_t size, const char *what)
{
	unsigned char *rgba;
	SDL_Surface *s;
	int w, h, alpha = 0, y;

	if (size >= 8 && memcmp(data, "\211PNG\r\n\032\n", 8) == 0)
		rgba = amiga_png_decode(data, size, &w, &h, &alpha);
	else
		rgba = tga_decode(data, size, &w, &h, &alpha);
	if (!rgba)
		return NULL;
	/* bytes R, G, B, A in memory, as big-endian 32-bit pixels */
	s = SDL_CreateRGBSurface(SDL_SWSURFACE | (alpha ? SDL_SRCALPHA : 0), w, h, 32,
	                         0xFF000000u, 0x00FF0000u, 0x0000FF00u, alpha ? 0x000000FFu : 0);
	if (s) {
		for (y = 0; y < h; y++)
			memcpy((Uint8 *)s->pixels + (size_t)y * s->pitch, rgba + (size_t)y * w * 4, (size_t)w * 4);
	}
	else
		printf("W: IMG_Load: no %dx%d surface for %s: %s\n", w, h, what, SDL_GetError());
	free(rgba);
	return s;
}

SDL_Surface *IMG_Load(const char *file)
{
	size_t size = 0;
	unsigned char *data;
	SDL_Surface *s;

	if (!file) {
		SDL_SetError("IMG_Load: no file name");
		return NULL;
	}
	data = read_file(file, &size);
	if (!data) {
		SDL_SetError("IMG_Load: cannot read %s", file);
		printf("W: IMG_Load: cannot read %s (%lu bytes)\n", file, (unsigned long)size);
		return NULL;
	}
	s = decode(data, size, file);
	free(data);
	if (s)
		return s;
	return SDL_LoadBMP(file);
}

/* The rest of the stream (JLib reads pictures from inside its own files) */
SDL_Surface *IMG_Load_RW(SDL_RWops *src, int freesrc)
{
	unsigned char *data = NULL, *more;
	size_t size = 0, cap = 0;
	int got;
	SDL_Surface *s = NULL;

	if (!src) {
		SDL_SetError("IMG_Load_RW: no RWops");
		return NULL;
	}
	for (;;) {
		if (size == cap) {
			cap = cap ? cap * 2 : 65536;
			more = (unsigned char *)realloc(data, cap);
			if (!more)
				break;
			data = more;
		}
		got = SDL_RWread(src, data + size, 1, (int)(cap - size));
		if (got <= 0)
			break;
		size += (size_t)got;
	}
	if (data && size)
		s = decode(data, size, "an RWops");
	if (!s)
		SDL_SetError("IMG_Load_RW: not a PNG or TGA picture");
	free(data);
	if (freesrc)
		SDL_RWclose(src);
	return s;
}
