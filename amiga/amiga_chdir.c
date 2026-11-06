/* chdir() for Holotz's Castle on AmigaOS. The game walks its theme's
 * drawers with chdir("floor"), chdir("1"), ... and back with chdir("..")
 * (src/HCTheme.cpp). AmigaDOS has no "..": its parent is "/" ("/x" is the
 * parent's x, "a//" is a's parent), and libnix's chdir() passes the name
 * on as it is, so the first ".." failed, the game stayed in floor/1 and
 * the theme never loaded. Linked with -Wl,--wrap=chdir: "." and ".."
 * parts are put in AmigaDOS's terms, the rest is passed on.
 * getcwd() (-Wl,--wrap=getcwd) names the current drawer from its lock
 * (dos.library's NameFromLock): the theme goes back to where it started
 * with chdir(getcwd()), and the game's next files are named from there.
 * The AmiCradle Team. GPL 2 or later, like Holotz's Castle. */
#include <string.h>
#include <errno.h>
#include <proto/dos.h>

int __real_chdir(const char *path);

int __wrap_chdir(const char *path)
{
	char out[1024];
	size_t n = 0, len;
	const char *p = path, *slash;

	if (!path)
		return __real_chdir(path);
	if (strchr(path, ':'))	/* a volume or assign: AmigaDOS's own name */
		return __real_chdir(path);
	out[0] = 0;
	while (*p) {
		slash = strchr(p, '/');
		len = slash ? (size_t)(slash - p) : strlen(p);
		if (len == 2 && p[0] == '.' && p[1] == '.') {
			if (n && out[n - 1] != '/' && n + 1 < sizeof(out))
				out[n++] = '/';
			if (n + 1 < sizeof(out))
				out[n++] = '/';
		}
		else if (len && !(len == 1 && p[0] == '.')) {
			if (n && out[n - 1] != '/' && n + 1 < sizeof(out))
				out[n++] = '/';
			if (n + len < sizeof(out)) {
				memcpy(out + n, p, len);
				n += len;
			}
		}
		p += len;
		if (*p == '/')
			p++;
	}
	out[n] = 0;
	return __real_chdir(out);	/* "" is the current drawer itself */
}

char *__wrap_getcwd(char *buf, size_t size)
{
	BPTR here = CurrentDir(0);
	LONG ok;

	CurrentDir(here);
	if (!buf || size == 0) {
		errno = EINVAL;
		return NULL;
	}
	ok = here ? NameFromLock(here, (STRPTR)buf, (LONG)size) : 0;
	if (!ok) {
		if (here || size < 5) {
			errno = ERANGE;
			return NULL;
		}
		strcpy(buf, "SYS:");	/* no lock: the boot volume's root */
	}
	return buf;
}
