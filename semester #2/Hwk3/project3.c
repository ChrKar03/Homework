#include "objdb.h"

int main (int argc, char *argv[])
{
    off_t bytes, size;
    int fd = -1;
    char *dbname = NULL, *buf = NULL, operator, *name, *objname, charbin;
    DIR *mydir = NULL;

    do
    {
        scanf (" %c", &operator);
        switch (operator)
        {
            case 'o':
                scanf ("%ms", &dbname);
                fd = open_db (fd, dbname);
                break;
            case 'i':
                if (fd < 0)
                {
                    fprintf(stderr, "\nNo open db file.\n");
                    charbin = '\0';
                    while ((charbin = getchar()) != '\n')
                        continue;
                    break;
                }
                name = NULL;
                scanf ("%ms %ms", &buf, &objname);
                mydir = opendir (buf);
                name = find_db (fd, objname, &size, single, mydir);
                if (!name)
                {
                    import (fd, buf, objname, mydir, size);
                    free (name);
                }
                else
                {
                    fprintf(stderr, "\nObject %s already in db.\n", objname);
                }
                free (buf);
                free (objname);
                break;
            case 'f':
                if (fd < 0)
                {
                    fprintf(stderr, "\nNo open db file.\n");
                    charbin = '\0';
                    while ((charbin = getchar()) != '\n')
                        continue;
                    break;
                }
                scanf ("%ms", &buf);
                name = find_db (fd, buf, &bytes, all, mydir);
                printf ("\n##\n");
                if (name)
                {
                    printf ("%s\n", name);
                    free (name);
                }
                free (buf);
                break;
            case 'e':
                if (fd < 0)
                {
                    fprintf(stderr, "\nNo open db file.\n");
                    charbin = '\0';
                    while ((charbin = getchar()) != '\n')
                        continue;
                    break;
                }
                scanf ("%ms %ms", &buf, &objname);
                name = find_db (fd, buf, &size, single, mydir);
                if (name)
                {
                    export (fd, buf, objname, mydir);
                    free (name);
                }
                else
                {
                    fprintf(stderr, "\nObject %s not in db.\n", buf);
                }
                free (buf);
                free (objname);
                break;
            case 'd':
                if (fd < 0)
                {
                    fprintf(stderr, "\nNo open db file.\n");
                    charbin = '\0';
                    while ((charbin = getchar()) != '\n')
                        continue;
                    break;
                }
                name = NULL;
                scanf ("%ms", &buf);
                name = find_db (fd, buf, &size, single, mydir);
                if (name)
                {
                    delete (fd, dbname, buf, size);
                    free (name);
                }
                else
                {
                    fprintf(stderr, "\nObject %s not in db.\n", buf);
                }
                free (buf);
                break;
            case 'c':
                if (fd < 0)
                {
                    fprintf(stderr, "\nNo open db file.\n");
                    charbin = '\0';
                    while ((charbin = getchar()) != '\n')
                        continue;
                    break;
                }
                free (dbname);
                dbname = NULL;
                close_db (&fd);
                break;
            case 'q':
                if (dbname)
                    free (dbname);
                quit (&fd);
                break;
            default:
                charbin = '\0';
                fprintf (stderr, "\nInvalid command.\n");
                while ((charbin = getchar()) != '\n')
                    continue;
                break;
        }
    }
    while (1);
}
