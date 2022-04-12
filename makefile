# Compiler Flags
CC = gcc
CCFLAGS += -Wall -Wextra
LDFLAGS = -lSDL2

all: main.o

main.o: sdl_main.c instructions.c cpu.c memory.c display.c
	$(CC) $(CCFLAGS) sdl_main.c instructions.c cpu.c memory.c display.c -o main.o $(LDFLAGS)

clean:
	rm -f main.o