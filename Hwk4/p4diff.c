#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 64

int myread (int fd, void *buf, int size);

int main (int argc, char *argv[])
{

    char mystdin[BUF_SIZE + 1], buf[BUF_SIZE + 1];
    int counter1, counter2, total = 0, size, Nofsame = 0, fd, i, Numtodiv, finalgrade, bytestoread;

    fd = open (argv[1], O_RDONLY);
    if (fd == -1)
    {
        perror ("Open");
        exit (-1);
    }
    size = lseek (fd, 0, SEEK_END);
    lseek (fd, 0, SEEK_SET);

    do
    {
        // Determine Num of bytes to read.
        if (size - total > BUF_SIZE)
            bytestoread = BUF_SIZE;
        else if (size - total > 0)
            bytestoread = size - total;
        else
            bytestoread = 0;
        // Reading from file.out.
        counter1 = myread (fd, buf, bytestoread);
        if (counter1 == -1)
            perror ("First read");
        // Reading from stdin.
        counter2 = myread (STDIN_FILENO, mystdin, BUF_SIZE);
        if (counter2 == -1)
            perror ("Second read");
        // EOF.
        if (!counter2)
            break;
        if (counter1)
        {
            for (i = 0; i < counter1; i++)
            {
                if (mystdin[i] == buf[i])
                    Nofsame++;
            }
        }
        if (counter2 > 0)
            total += counter2;
    }
    while (total != size);
    // Case where 0 bytes are in stdin and file.out.
    if (!total && !size)
        return 100;

    if (total > size)
        Numtodiv = total;
    else
        Numtodiv = size;

    finalgrade = (Nofsame * 100) / Numtodiv;

    return finalgrade;
}
// Reads from file until it reaches size or end of file.
int myread (int fd, void *buf, int size)
{
    int counter, total;

    counter = read (fd, buf, size);
    if (!counter)
        return 0;
    total = counter;
    while (total < size)
    {
        counter = read (fd, buf + total, size - total);
        if (!counter)
            return total;
        total += counter;
    }
    return total;
}