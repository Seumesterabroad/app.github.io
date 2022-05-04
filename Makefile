#Makefile

CC = gcc
CFLAGS = -Wall -Wextra -g

all: graph

graph: graph.o
graph.o: graph.c graph.h

.PHONY: clean

clean:
	${RM} graph 
	

#END
