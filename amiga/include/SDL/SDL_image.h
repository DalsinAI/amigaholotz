/* Holotz's Castle for AmigaOS (from the Wizznic and Gigalomania ports): the SDL_image 1.2 calls it
 * makes, IMG_Load and IMG_Load_RW, for PNG (libpng) and TGA (amiga/sdl_image_lite.c). The OpenGPU kit's SDL 1.2
 * (sdl12-compat) comes without an SDL_image 1.2.
 * The AmiCradle Team. GPL 3 or later, like Wizznic. */
#ifndef AMIGA_SDL_IMAGE_LITE_H
#define AMIGA_SDL_IMAGE_LITE_H
#include <SDL.h>
#ifdef __cplusplus
extern "C" {
#endif
SDL_Surface *IMG_Load(const char *file);
SDL_Surface *IMG_Load_RW(SDL_RWops *src, int freesrc);
#define IMG_GetError SDL_GetError
#define IMG_SetError SDL_SetError
#ifdef __cplusplus
}
#endif
#endif
