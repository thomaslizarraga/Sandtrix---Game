CFLAGS= -c -Wall -std=c99 -pedantic -g 
LDLIBS= -lm -lSDL2

all:sandtrix

sandtrix: sprite.o sprites.o imagen.o color.o test_sdl2.o juego.o
	gcc test_sdl2.o sprite.o sprites.o imagen.o color.o juego.o -o sandtrix $(LDLIBS)

test_sdl2.o: test_sdl2.c imagen.h color.h sprite.h sprites.h config.h juego.h
	gcc test_sdl2.c $(CFLAGS)

juego.o: imagen.h juego.c juego.h
	gcc juego.c $(CFLAGS)

imagen.o: imagen.h imagen.c color.h
	gcc imagen.c $(CFLAGS)

color.o: color.h color.c 
	gcc color.c $(CFLAGS)

sprites.o: sprite.h sprites.h sprites.c
	gcc sprites.c $(CFLAGS)

sprite.o: sprite.c sprite.h 
	gcc sprite.c $(CFLAGS)

clean:
	rm -vf *.o