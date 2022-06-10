#Makefile

CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0` -Wall -Wextra -pthread -g
LDLIBS = `pkg-config --libs gtk+-3.0` -lSDL -lSDL_image

all: interface

interface: graph_operation.o graph_processing.o interface.h interface.c 

graph_operation.o: graph_operation.h graph_operation.c

graph_processing.o: graph_processing.h graph_operation.o graph_operation.o

.PHONY: clean

clean:
	rm *.o
	rm interface
	

#END
