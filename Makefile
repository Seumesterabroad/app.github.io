#Makefile

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O3 `pkg-config --cflags gtk+-3.0`
LDLIBS= `sdl-config --libs` -lSDL_gfx `pkg-config --libs sdl SDL_image` -lm `pkg-config --libs gtk+-3.0` -lSDL_ttf
all: main

main: construction.o pixel_operations.o parser.o graph_operation.o graph_processing.o graph.h main.c

parser.o: parser.h parser.c

graph_operation.o: graph_operation.h graph_operation.c

graph_processing.o: graph_processing.h graph_operation.o graph_processing.c 

pixel_operations.o: pixel_operations.c pixel_operations.h

construction.o : pixel_operations.o construction.h construction.h 


.PHONY: clean

clean:
	${RM} *.o
	${RM} *.d
	${RM} main
	rm test.bmp
	

#END
