CC = gcc
CFLAGS = -Wall -Wextra -O2 -ggdb
LFLAGS = -lncurses

main: main.c
	$(CC) $(CFLAGS) main.c -o main $(LFLAGS)
