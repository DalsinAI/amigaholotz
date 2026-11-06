Holotz's Castle 1.3.14 for AmigaOS 3.x
======================================

Holotz's Castle (Juan Carlos Seijo Pérez, Mainreactor), the platform
puzzle game: Ybelle and Ludar escape the castle of Holotz, room after room,
by ropes, ladders and breaking floors. For AmigaOS 3.x on a 68040 or 68060
with an FPU and an RTG screen, on OpenGPU's SDL 1.2 (sdl12-compat).

Base: holotz-castle_1.3.14.orig.tar.gz from Debian (upstream's site is
gone; sha256 42903559a9821b39eac6eba7730be0fcba12c9ad861ba2691f372c54371039b1),
with Debian's patches 1.3.14-13 (second commit). Branch
team2/holotz-amiga.

Status: builds; not yet run in the lab.


What you need
-------------

- A 68040 or 68060 with an FPU, AmigaOS 3.x, an RTG card (or the
  AmigaChrome lab).
- OpenGPU 0.6 or later: opengpu.library and LIBS:OpenGPU/SDL2.module
  (installed by OpenUp). Everything else is in the program.


How to play
-----------

Open the HolotzCastle icon, or run HolotzCastle from a Shell. The game
reads res/ in its drawer and keeps its preferences and saved games there
too (res/savedata).


Building
--------

    make -f amiga/Makefile.amigaos -j2 dist

with GFOpenAmigaGCC 16.2.0-open4, the OpenNDK headers, the OpenGPU kit and
the Team's static FreeType, libpng and zlib (see the Makefile's head). The
drawer is dist/HolotzCastle (and dist/HolotzCastle.info). The level editor
is not built.


What was changed
----------------

- src/HolotzCastle.cpp: main() first moves into the drawer
  (amiga/amiga_start.c), and closes at exit the screens SDL2.module 0.6 can
  leave (amiga/amiga_screens.c). amiga/stack.c: 512 KB of stack.
- src/HCPreferences.cpp, src/HCLoadSaveSlot.cpp: preferences and saved
  games in the drawer's res/, as on Windows (upstream on Unix:
  ~/.holotz-castle).
- No SDL_image, SDL_mixer or SDL_ttf 1.2 for OpenGPU's SDL 1.2, so:
  amiga/sdl_image_lite.c (IMG_Load and IMG_Load_RW: PNG on libpng, and the
  game's RLE TGAs), amiga/sdl_mixer_lite.c (WAV; the Team's, from the
  Wizznic and Gigalomania ports, with the channel calls JLib makes added)
  and SDL_ttf 2.0.11 itself (amiga/sdl_ttf, zlib licence; outlined text,
  which the game does not use, left out: the Team's FreeType has no
  stroker).
- amiga/amiga_rwops.c: SDL_RWFromFile on stdio (-Wl,--wrap): sdl12-compat's
  does not come back on OpenGPU.


Licences
--------

Holotz's Castle: GPL 2 or later (LICENSE.txt); JLib: LGPL 2 or later.
SDL_ttf 2.0.11: zlib (SDL_ttf-COPYING.txt). The Amiga files (amiga/): the
AmiCradle Team; the SDL_image, SDL_mixer and rwops shims come from the
Team's Wizznic and Gigalomania ports and are GPL 3 or later, so the program
as built here is GPL 3 or later.
