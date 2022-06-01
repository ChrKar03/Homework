CC=gcc
CFLAGS=-Wall -g -fsanitize=address -fsanitize=bounds 

project1: project1.o
	$(CC) $(CFLAGS) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm *.o
