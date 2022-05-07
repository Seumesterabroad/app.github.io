#Makefile

CC = gcc
CFLAGS = -Wall -Wextra -g

all: main

main: parser.o graph_operation.o graph_processing.o graph.h main.c

parser.o: parser.h parser.c

graph_operation.o: graph_operation.h graph_operation.c

graph_processing.o: graph_processing.h graph_operation.h graph_processing.c

.PHONY: clean

clean:
	rm *.o main
	

#END
