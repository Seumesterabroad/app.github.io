CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0` -Wall -Wextra -pthread -g
LDLIBS = `pkg-config --libs gtk+-3.0` -lSDL -lSDL_image

EXE = interface

all: $(EXE)

$(foreach f, $(EXE), $(eval $(f):))

.PHONY: clean

clean:
	${RM} $(EXE)