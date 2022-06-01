#ifndef OBJDB_H
#define OBJDB_H 1

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <limits.h>

#define MAX_BUF_SIZE 512
#define CODE_SIZE 9
#define DIR_CODE_SIZE 1
#define DIR_CODE "/"
#define PATH_INIT_SIZE 2
#define PATH_INIT "./"
#define PASSWORD_DB "sKARAbeos"

typedef enum mode {single, all} modeT;

int open_db (int fd, char *db_name);

int check_db (int fd);

off_t traverse_db (int fd);

int init_db (int fd);

int file_init (int fd, char *objname, off_t *size);

int dir_init (int fd, char *dir_name, DIR *dir, char *path, int pathsize);

off_t file_size (int fd);

void dir_size (char *dir_name, off_t *dirsize, DIR *dir, char *path, int pathsize);

int read_db (int fd, void *bus, int byteblock, off_t size);

int write_db (int fd, void *buf, int byteblock);

int close_db (int *fd);

char *find_db (int fd, char *objname, off_t *bytesread, modeT mode, DIR *dir);

int import (int fd, char *file_name, char *objname, DIR *dir, int dbsize);

int export (int fd, char *objname, char *file_name, DIR *dir);

int delete (int fd, char *dbname, char *objname, off_t filesize);

void quit (int *fd);

int import_dir ();

#endif