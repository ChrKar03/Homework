CC = gcc
CFLAGS = -Wall -g -fsanitize=address
OBJ = project4 p4diff
OBJS = $(widcard $(OBJ))

all: $(OBJ)

project4: project4.c
	$(CC) $(CFLAGS) $< -o $@

p4diff: p4diff.c
	$(CC) $(CFLAGS) $< -o $@