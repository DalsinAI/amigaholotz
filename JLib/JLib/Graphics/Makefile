# Makefile para JLib.
# (C) Juan Carlos Seijo Pérez - 2003.

ADD_INCLUDE=-I. -I/usr/local/include
GRAPHICS=JLib/Graphics
UTIL=JLib/Util
MATH=JLib/Math
SOUND=JLib/Sound
PHYSICS=JLib/Physics
JSPRITED=JLib/JSprited
JSCENED=JLib/JScened
TARGET=libJLib.so libJLib.a JSprited JScened

DBG_FLAG=-g
OPTIM=-O4 -mcpu=pentium2 -march=pentium2

WITH_LIBS=-lSDL -lSDL_image -lGL -lGLU -lfltk -lfltk_images

all: *.o

*.o: *.cpp *.h
	echo $?
	g++ $(DBG_FLAG) $(ADD_INCLUDE) -c -o $@ $<

.PHONY: clean
clean:
	rm -rf *.o
