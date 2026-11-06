# Makefile para JLib.
# (C) Juan Carlos Seijo Pérez - 2003.

GRAPHICS=JLib/Graphics
UTIL=JLib/Util
MATH=JLib/Math
SOUND=JLib/Sound
PHYSICS=JLib/Physics
HARDENING := $(shell dpkg-buildflags --get CFLAGS)
HARDENING += $(shell dpkg-buildflags --get CPPFLAGS)
HARDENING += $(shell dpkg-buildflags --get CXXFLAGS)
HARDENING_LDFLAGS = $(shell dpkg-buildflags --get LDFLAGS)

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
MORE_CFLAGS= -O3
CFLAGS=-I. -I/usr/local/include/SDL $(MORE_CFLAGS) $(HARDENING) -ffast-math -fPIC -Wall -Wshadow `$(SDL_CONFIG) --cflags`

# Sample LDFLAGS for applications
# LDFLAGS=-L. -ffast-math -lSDL_image -lSDL_ttf -lSDL_mixer `$(SDL_CONFIG) --libs`

all: libJLib

# JLib
libJLib: $(JLIB_OBJS)
	$(CXX) $(HARDENING_LDFLAGS) -shared -L/usr/lib -fPIC -o $@.so $? \
	&& ar rvus $@.a $? \

$(GRAPHICS)/%.o: $(GRAPHICS)/%.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<
$(UTIL)/%.o: $(UTIL)/%.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<
$(MATH)/%.o: $(MATH)/%.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

.PHONY: install
install:
	cp libJLib* /usr/local/lib/.; \
	find . -name *.h -exec 'cp' '--parents' '{}' '/usr/local/include/' ';' ; \
	ldconfig;

.PHONY: clean
clean:
	rm -rf JLib/*/*\.o libJLib.a libJLib.so
