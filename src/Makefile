# Makefile Para Holotz's Castle. (C) Juan Carlos Seijo Pérez - 2004.
# Makefile for Holotz's Castle.

ifndef SDL_CONFIG
SDL_CONFIG = sdl-config
endif

# Uncomment these lines if you don't want to install the game. Then
# you'll only be able to execute it from the directory it is in.
# Also comment lines bellow.
# Descomenta estas l�neas si no quieres instalar el juego. S�lo 
# podr�s ejecutarlo desde el directorio donde est�.
# Comenta las l�neas m�s abajo.
ifdef I486_BUILD
HC_DATA = res/
HCED_DATA = HCedHome/res/
endif

# Edit these lines to change the data and game installation dirs or
# comment out these lines to not install the game (also uncomment lines above)
# Edita estas l�neas para cambiar el directorio de instalaci�n de datos y del juego o bien
# comenta estas l�neas si no quieres instalar el juego (y descomenta las de arriba)

ifndef I486_BUILD
HC_BASE = /usr/share/games/holotz-castle/
HC_DATA = $(HC_BASE)game/
HCED_DATA = $(HC_BASE)editor/
HC_BIN_DIR = /usr/local/bin/
endif

DEFINES = -DHC_DATA_DIR=\"$(HC_DATA)\" -DHCED_DATA_DIR=\"$(HCED_DATA)\"

all: JLib HC HCed

HC_OBJ_MAIN=HolotzCastle.o
HC_OBJS=HCUtil.o HCTimer.o HCLoadSaveSlot.o HCPlaylist.o HCPreferences.o HCTheme.o HCText.o \
				HCBreak.o HCRope.o HCContFloor.o HCObject.o HCMap.o HCExit.o HCCharacter.o \
		    HCEnemy.o HCEnemyBall.o HCEnemyChaser.o HCEnemyMaker.o HCEnemyRandom.o \
				HCEnemyStatic.o HCScriptAction.o HCScriptBlock.o HCScript.o HCLevel.o

HCED_OBJ_MAIN=HCed.o

# Definir -D_JLIB_DEBUG para debug con JLib. Ponerlo también en el Makefile de JLib.
# Define -D_JLIB_DEBUG for JLib debug mode. Put it also in JLib's Makefile.
CFLAGS=-fno-strict-aliasing -I. -I../JLib `$(SDL_CONFIG) --cflags` -I/usr/local/include/ -I/usr/local/include/SDL -O3 -Wall -Werror -Wshadow $(CPU_OPTS) -ffast-math -c
LDFLAGS=-L. -ffast-math -lSDL_image -lSDL_ttf -lSDL_mixer `$(SDL_CONFIG) --libs`


JLib: 
	$(MAKE) -C ../JLib;

%.o: %.cpp %.h
	g++ $(CFLAGS) $(DEFINES) $< -o $@

HC: $(HC_OBJS) $(HC_OBJ_MAIN) ../JLib/libJLib.a
	g++ $(LDFLAGS) $? -o $@;\
	mv HC ../holotz-castle

# HCed (Holotz's Castle editor).
HCed: $(HC_OBJS) $(HCED_OBJ_MAIN) ../JLib/libJLib.a
	g++ $(LDFLAGS) $? -o $@;\
	mv HCed ../holotz-castle-editor

install:
	mkdir -p $(HC_DATA);\
  mkdir -p $(HCED_DATA);\
	chmod -R a+rx $(HC_DATA) $(HCED_DATA);\
  mkdir -p $(HC_BIN_DIR);\
	cp -r ../res/[^H]* $(HC_DATA) && cp -r ../HCedHome/res/* $(HCED_DATA);\
	chmod -R a+x ../holotz-castle ../holotz-castle-editor;\
	cp ../holotz-castle ../holotz-castle-editor $(HC_BIN_DIR);\
	chmod -R a+r $(HC_DATA) $(HCED_DATA);

uninstall:
	rm -rf $(HC_BASE);\
	rm -f $(HC_BIN_DIR)holotz-castle $(HC_BIN_DIR)holotz-castle-editor;\

.PHONY: clean
clean:
	rm -f ../holotz-castle;\
	rm -f ../holotz-castle-editor;\
	rm -f *\.o;\
  $(MAKE) -C ../JLib clean
