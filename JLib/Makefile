# Makefile para JLib.
# (C) Juan Carlos Seijo Pérez - 2003.

GRAPHICS=JLib/Graphics
UTIL=JLib/Util
MATH=JLib/Math
SOUND=JLib/Sound
PHYSICS=JLib/Physics

JLIB_OBJS=$(GRAPHICS)/JControl.o $(GRAPHICS)/JUI.o $(GRAPHICS)/JGLAxes.o $(GRAPHICS)/JGLCamera.o $(GRAPHICS)/JGLConsole.o\
					$(GRAPHICS)/JGLGrid.o $(GRAPHICS)/JGLLight.o $(GRAPHICS)/JGLTexture.o $(GRAPHICS)/JGLVector.o \
	  			$(GRAPHICS)/JControlImage.o $(GRAPHICS)/JImage.o $(GRAPHICS)/JImageSprite.o $(GRAPHICS)/JSprite.o $(GRAPHICS)/JFont.o\
					$(GRAPHICS)/JTextMenu.o $(GRAPHICS)/JImageMenu.o $(GRAPHICS)/JGLMesh.o $(GRAPHICS)/JGLImage.o $(UTIL)/JFile.o\
					$(UTIL)/JGLApp.o $(UTIL)/JObject.o $(UTIL)/JTextFile.o $(UTIL)/JTextUtil.o $(UTIL)/JTimer.o $(UTIL)/JUtil.o\
					$(UTIL)/JApp.o $(UTIL)/JFS.o $(UTIL)/JThread.o $(UTIL)/JRW.o $(MATH)/J2DPolygon.o

ifndef SDL_CONFIG
SDL_CONFIG = sdl-config
endif

# Define -D_JLIB_DEBUG to debug JLib. Compile the app to debug also with this flag
CFLAGS=-I. -I/usr/local/include/SDL -O3 -ffast-math -fPIC -Wall -Wshadow `$(SDL_CONFIG) --cflags`

# Sample LDFLAGS for applications
# LDFLAGS=-L. -ffast-math -lSDL_image -lSDL_ttf -lSDL_mixer `$(SDL_CONFIG) --libs`

all: libJLib

# JLib
libJLib: $(JLIB_OBJS)
	g++-4.1 -shared -L/usr/lib -fPIC -o $@.so $? \
	&& ar rvus $@.a $? \

$(GRAPHICS)/%.o: $(GRAPHICS)/%.cpp
	g++-4.1 $(CFLAGS) -c -o $@ $<
$(UTIL)/%.o: $(UTIL)/%.cpp
	g++-4.1 $(CFLAGS) -c -o $@ $<
$(MATH)/%.o: $(MATH)/%.cpp
	g++-4.1 $(CFLAGS) -c -o $@ $<

.PHONY: install
install:
	cp libJLib* /usr/local/lib/.; \
	find . -name *.h -exec 'cp' '--parents' '{}' '/usr/local/include/' ';' ; \
	ldconfig;

.PHONY: clean
clean:
	rm -rf JLib/*/*\.o libJLib.a libJLib.so
