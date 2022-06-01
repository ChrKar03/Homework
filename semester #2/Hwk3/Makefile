CC = gcc
CFLAGS = -Wall -g -fsanitize=address
OBJ = objdb.o project3.o

all: project3 clean

project3: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

%.o: %.c objdb.h
	$(CC) $(CFLAGS) $< -c

clean:
	rm *.o