#Makefile

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0` -Wall -Wextra -pthread -g -std=c99 -O3
LDLIBS = `pkg-config --libs gtk+-3.0` -lSDL `sdl-config --libs` -lSDL_gfx `pkg-config --libs sdl SDL_image` -lm -lSDL_ttf

all: interface

interface: graph_operation.o graph_processing.o interface.h interface.c construction.o pixel_operations.o

graph_operation.o: graph_operation.h graph_operation.c

graph_processing.o: graph_processing.h graph_operation.o graph_operation.o

pixel_operations.o: pixel_operations.c pixel_operations.h

construction.o : pixel_operations.o construction.h construction.h 

.PHONY: clean

clean:
	rm *.o
	rm *.d
	rm interface
	rm *.bmp

#END
