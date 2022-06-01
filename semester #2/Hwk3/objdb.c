#include "objdb.h"

// Opens an already existed database or creates it.
int open_db (int fd, char *db_name)
{
    int check;

    if (fd != -1)
        close_db (&fd);
    fd = open (db_name, O_RDWR | O_CREAT, S_IRWXU);
    if (fd == -1)
    {
        fprintf(stderr, "\nError opening %s.\n", db_name);
        return -1;
    }
    check = check_db (fd);

    switch (check)
    {
        case -2:
            init_db (fd);
            break;
        case -1:
            fprintf(stderr, "\nInvalid db file %s.\n", db_name);
            return -1;
    }
    return fd;
}

// Checks if the file we are trying to open is a database we have made/are about to make.
int check_db (int fd)
{
    char bus[10];
    off_t size = file_size (fd);

    if (size >= CODE_SIZE)
        read_db (fd, bus, CODE_SIZE, size);
    bus[9] = '\0';
    
    if (!size)
    {
        return -2;
    }
    else
    {
        if (strcmp (PASSWORD_DB, bus))
            return -1;
    }
    return 0;
}

int check_dir ();

// General-use search function inside the database.
off_t traverse_db (int fd)
{
    off_t filesize;
    read_db (fd, &filesize, sizeof (off_t), sizeof (off_t));
    return lseek (fd, filesize, SEEK_CUR);
}

// Initiallazes the database with a specific password.
int init_db (int fd)
{
    write_db (fd, PASSWORD_DB, CODE_SIZE);
    return 0;
}

// Initiallazes the imported file.
int file_init (int fd, char *objname, off_t *size)
{
    lseek (fd, 0, SEEK_END);
    int namesize = strlen (objname);
    write_db (fd, &namesize, sizeof (int));
    write_db (fd, objname, namesize);
    write_db (fd, size, sizeof (off_t));
    return 0;
}

int dir_init (int fd, char *dir_name, DIR *dir, char *path, int pathsize)
{
    int namesize;
    off_t dirsize = 0;

    namesize = strlen (dir_name) + DIR_CODE_SIZE;
    write_db (fd, &namesize, sizeof (int));
    write_db (fd, dir_name, namesize - DIR_CODE_SIZE);
    write_db (fd, DIR_CODE, DIR_CODE_SIZE);
    // dir_size (dir_name, &dirsize, dir, path, pathsize);
    write_db (fd, &dirsize, sizeof (off_t));
    return 0;
}

// Finds the size of the file.
off_t file_size (int fd)
{
    off_t size;

    lseek (fd, 0, SEEK_SET);
    size = lseek (fd, 0, SEEK_END);
    lseek (fd, 0, SEEK_SET);

    return size;
}

void dir_size (char *dir_name, off_t *dirsize, DIR *dir, char *path, int pathsize)
{
    char *tmp_path = NULL;
    int fd = -1, namesize;
    struct dirent *store = NULL;
    off_t total = 0;
    DIR *tmp = NULL;

    do
    {
        store = readdir (dir);
        if (!store)
            return;
        if (dir && store && (!strcmp (store->d_name, "..") || !strcmp (store->d_name, ".")))
        {
            continue;
        }
        namesize = strlen (store->d_name);
        tmp_path = (char *)realloc (path, sizeof (char) * (pathsize + namesize + 2));
        if (!tmp_path)
        {
            *dirsize = -1;
            return;
        }
        path = tmp_path;
        strncpy (path + pathsize + 1, store->d_name, namesize);
        path[pathsize + namesize + 1] = '\0';
        tmp = opendir (path);
        if (tmp)
            dir_size (store->d_name, dirsize, tmp, path, pathsize + namesize + 2);
        fd = open (path, O_RDONLY, S_IRWXU);
        if (fd <= 0)
        {
            *dirsize = -2;
            return;
        }
        total = file_size (fd);
        *dirsize += total;
        close (fd);
    }
    while (store);
}

// Reads from a file.
int read_db (int fd, void *bus, int byteblock, off_t size)
{
    int total, counter;

    if (!size)
    {
        return -1;
    }
    else
    {
        counter = read (fd, bus, byteblock);
        total = counter;
        if (counter < byteblock && size >= byteblock)
        {
            while (total != byteblock)
            {
                counter = read (fd, bus + total, byteblock - total);
                total += counter;
            }
        }
    }
    return total;
}

// Writes to a file.
int write_db (int fd, void *buf, int byteblock)
{
    int total, counter;

    counter = write (fd, buf, byteblock);
    if (counter < byteblock)
    {
        total = counter;
        while (total != byteblock)
        {
            
            counter = write (fd, buf + total, byteblock - total);
            total += counter;
        }
    }
    return total;
}

// Closes a database we have opened.
int close_db (int *fd)
{
    close (*fd);
    *fd = -1;
    return 0;
}

// Searches for an object that is stored in our database.
char *find_db (int fd, char *objname, off_t *bytesread, modeT mode, DIR *dir)
{
    int namesize, size = 0, fullstringlen = 0, operator = 0;
    char *tmp = NULL, *buf = NULL, *finalstr = NULL, *check = NULL;

    size = file_size (fd);
    *bytesread = lseek (fd, CODE_SIZE, SEEK_SET);
    while (*bytesread != size)
    {
        *bytesread += read_db (fd, &namesize, 4, 4);
        buf = (char *)malloc (sizeof (char) * (namesize + 1));
        *bytesread += read_db (fd, buf, namesize, namesize);
        buf[namesize] = '\0';
        if (mode == single && !strcmp (buf, objname))
        {
            *bytesread = size;
            return buf;
        }
        operator = strcmp (objname, "*");
        if (mode == all && operator)
            tmp = strstr (buf, objname);
        if (mode == all && (tmp || !operator))
        {
            check = (char *)realloc (finalstr, sizeof (char) * (fullstringlen + namesize + 1));
            if (check)
                finalstr = check;
            strncpy (finalstr + fullstringlen, buf, namesize);
            fullstringlen += namesize;
            if (strstr (buf, "_d"))
                fullstringlen -= DIR_CODE_SIZE;
            finalstr[fullstringlen] = '\n';
            fullstringlen++;
        }
        free (buf);
        *bytesread = traverse_db (fd);
    }

    if (finalstr)
        finalstr[fullstringlen - 1] = '\0';
    return finalstr;
}

// Imports a file into our database.
int import (int fd, char *file_name, char *objname, DIR *dir, int dbsize)
{
    int total = 0, Numofbytes, check = 0, pathsize = 0, namesize = 0, tmp_fd = -2;
    char *buf = NULL, *path = NULL, *tmp = NULL;
    off_t size;
    struct dirent *store = NULL;

    if (!dir)
        tmp_fd = open (file_name, O_RDONLY);
    if (tmp_fd == -1)
    {
        fprintf(stderr, "\nFile %s not found.\n", file_name);
        return -1;
    }
    if (dir)
    {
        
        if (file_name[0] != '.' && file_name[1] != '/')
        {
            tmp = (char *)malloc (sizeof (char) * (PATH_INIT_SIZE + 1));
            if (tmp)
                path = tmp;
            strncpy (path, PATH_INIT, PATH_INIT_SIZE + 1);
            pathsize = PATH_INIT_SIZE;
            path[pathsize] = '\0';
        }
        pathsize += strlen (file_name);
        tmp = (char *)realloc (path, sizeof (char) * (pathsize + 1));
        if (tmp)
            path = tmp;
        strcat (path, file_name);
        path[pathsize] = '/';
    }
    ////////////////////////////////////////////////////////////////////////////////////
    do
    {
        if (dir)
        {
            store = readdir (dir);
            if (!store)
                break;
            namesize = strlen (store->d_name);
            tmp = (char *)realloc (path, sizeof (char) * (namesize + pathsize + 2));
            if (tmp)
                path = tmp;
            strncpy (path + pathsize + 1, store->d_name, namesize);
            path[pathsize + namesize + 1] = '\0';
            tmp_fd = open (path, O_RDONLY);
        }
        if (tmp_fd == -1)
        {
            fprintf(stderr, "\nFile %s not found.\n", file_name);
            return -1;
        }
        if (dir && store && (!strcmp (store->d_name, "..") || !strcmp (store->d_name, ".")))
        {
            close (tmp_fd);
            continue;
        }
        /////////////////////////////////////////////////////////////////////////////////
        size = 0;
        buf = (char *)malloc (sizeof (char) * MAX_BUF_SIZE);
        if (!buf)
            return 42;
        size = file_size (tmp_fd);
        if (dir && !check)
        {
            check++;
            dir_init (fd, objname, dir, path, pathsize);
        }
        if(dir && store)
            file_init (fd, store->d_name, &size);
        else
            file_init (fd, objname, &size);
        ///////////////////////////////////////////////////////////////////////////////////////
        while (total != size)
        {
            if ((size - total) > MAX_BUF_SIZE)
                Numofbytes = MAX_BUF_SIZE;
            else
                Numofbytes = size - total;
            total += read_db (tmp_fd, buf, MAX_BUF_SIZE, Numofbytes);
            write_db (fd, buf, Numofbytes);
        }
        close_db(&tmp_fd);
    }
    while (dir && store);
    free (path);
    free (buf);
    return 0;
}

// Exports an object from our database.
int export (int fd, char *objname, char *file_name, DIR *dir)
{
    char *buf = NULL;
    off_t size, total = 0;
    int tmp_fd, Numofbytes;
    struct dirent *store;
    
    buf = (char *)malloc (sizeof (char) * MAX_BUF_SIZE);
    if (!buf)
        return 42;
    tmp_fd = open (file_name, O_EXCL | O_CREAT | O_WRONLY, S_IRWXU);
    if (tmp_fd == -1)
    {
        free (buf);
        fprintf(stderr, "\nCannot open file %s.\n", file_name);
        return -1;
    }
    read_db (fd, &size, sizeof (off_t), sizeof (off_t));
    while (total != size)
    {
        if ((size - total) > MAX_BUF_SIZE)
        {
            Numofbytes = MAX_BUF_SIZE;
        }
        else
        {
            Numofbytes = size - total;
        }
        total += read_db (fd, buf, Numofbytes, Numofbytes);
        write_db (tmp_fd, buf, Numofbytes);
    }
    free (buf);
    close_db (&tmp_fd);
    return 0;
}

// Deletes an object from our database.
int delete (int fd, char *dbname, char *objname, off_t filesize)
{
    char *buf = NULL;
    int tmp_fd = -1, namelen, Numofbytes, i;
    off_t total;

    buf = (char *)malloc (sizeof (char) * MAX_BUF_SIZE);
    if (!buf)
        return 42;
    total = lseek (fd, 0, SEEK_CUR);
    namelen = strlen (objname);
    tmp_fd = open_db (tmp_fd, dbname);
    i = lseek (tmp_fd, total - namelen - sizeof (int) - CODE_SIZE, SEEK_CUR);
    total = traverse_db (fd);
    while (total < filesize)
    {
        if ((filesize - total) > MAX_BUF_SIZE)
            Numofbytes = MAX_BUF_SIZE;
        else
            Numofbytes = filesize - total;
        total += read_db (fd, buf, MAX_BUF_SIZE, Numofbytes);
        write_db (tmp_fd, buf, Numofbytes);
    }
    free (buf);
    i = lseek (tmp_fd, 0, SEEK_CUR);
    lseek (fd, 0, SEEK_SET);
    ftruncate (tmp_fd, i);
    close_db (&tmp_fd);
    return 0;
}

// Quits the program.
void quit (int *fd)
{
    close_db (fd);
    exit (EXIT_SUCCESS);
}
