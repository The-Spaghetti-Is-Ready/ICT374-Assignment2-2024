CC = gcc
CFLAGS = -Wextra -Wall -Wpedantic

a2-part1: main.o command.o commandfunctions.o
	$(CC) $(CFLAGS) -o a2-part1 main.o

main.o: main.c command.h commandfunctions.h

command.o: command.h

commandfunctions.o: commandfunctions.c commandfunctions.h