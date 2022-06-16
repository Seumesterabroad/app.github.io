#Makefile

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0` -Wall -Wextra -pthread -g -std=c99 -O3
LDLIBS = `pkg-config --libs gtk+-3.0` -lSDL `sdl-config --libs` -lSDL_gfx `pkg-config --libs sdl SDL_image` -lm -lSDL_ttf

all: interface

interface: graph_operation.o graph_processing.o interface.h interface.c construction.o pixel_operations.o client/print_page.o

graph_operation.o: graph_operation.h graph_operation.c

graph_processing.o: graph_processing.h graph_operation.o 

pixel_operations.o: pixel_operations.c pixel_operations.h

construction.o: pixel_operations.o construction.h 

#######################################################################

client/print_page.o: client/print_page.c client/print_page.h

#server/queue.o: server/queue.c server/queue.h

#server/shared_queue.o: server/shared_queue.c server/shared_queue.h server/queue.o

#server/server.o: server/server.c server/shared_queue.o


.PHONY: clean

clean:
	rm *.o
	rm *.d
	rm client/print_page.d
	rm client/print_page.o
	rm interface

#END
