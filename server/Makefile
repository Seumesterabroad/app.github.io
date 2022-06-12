CC = gcc
CFLAGS = -Wall -Wextra
LDLIBS = -pthread -fsanitize=address -l sqlite3

all: server

server: server.o shared_queue.o queue.o
shared_queue.o: shared_queue.c shared_queue.h queue.o
queue.o: queue.c queue.h

.PHONY: clean

clean:
	${RM} server server.o
	${RM} shared_queue.o queue.o
