all: game.c sprites.h sprites.c renderFunctions.h renderFunctions.c text.h text.c
	gcc -g -Wall renderFunctions.c game.c sprites.c text.c -I/usr/local/include/freetype2 -I/usr/local/include -D_THREAD_SAFE -L/usr/local/lib -lSDL2 -lfreetype
