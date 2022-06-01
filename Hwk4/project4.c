#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/time.h>

#define BUF_SIZE 64

int check (FILE *tmp_fd);
char **args_set (char *arglist, int *nofargs, char *exename);
static void terminator (int sig);

extern int errno;

int main (int argc, char *argv[])
{

    if (argc != 6)
    {
        fprintf (stderr, "\n./project4 <filename>.c <filename>.args <filename>.in <filename>.out <time>\n");
        return 1;
    }
    FILE *tmp_fd;
    pid_t pid, tmp_pid;
    int namesize, fd = -2, status = 0, compilation = 0, mem = 0, timeout = 0, \
    output = 0, grade = 0, time = atoi (argv[5]), sys_check;

    char *exename= NULL, *errfile = NULL, *arglist = argv[2];
    namesize = strlen (argv[1]);

    exename = (char *)malloc (sizeof (char) * (namesize - 1));
    errfile = (char *)malloc (sizeof (char) * (namesize + 3));

    strncpy (exename, argv[1], namesize - 2);
    strncpy (errfile, argv[1], namesize - 2);
    strncpy (errfile + (namesize - 2), ".err", 5);
    exename[namesize - 2] = '\0';

    fd = open (errfile, O_CREAT | O_RDWR | O_TRUNC, S_IRWXU);
    if (fd == -1)
        perror ("0");

    // Compilation proccess.(P1)
    if (!(pid = fork()))
    {
        sys_check = dup2 (fd, STDERR_FILENO);
        if (sys_check == -1)
            perror ("1");
        sys_check = execlp ("gcc", "gcc", "-Wall", argv[1], "-o", exename, NULL);
        if (sys_check == -1)
            perror ("2");
    }
    sys_check = waitpid (pid, &status, 0);
    close (fd);
    tmp_fd = fopen (errfile, "r");
    compilation = check (tmp_fd);

    if (compilation)
    {
        if (compilation > -100)
            status = 0;
    }

////////////////////////////////////////////////////////////////////////////////////

    char *filename= NULL;

    filename = (char *)malloc (sizeof (char) * (namesize + 1));
    strncpy (filename, "./", 3);
    strcpy (filename + 2, exename);
    filename[namesize] = '\0';

    // Initiallizing timer and sigaction.
    struct sigaction action = {{0}};
    struct itimerval timer = {{0}};

    action.sa_handler = terminator;
    sigaction (SIGALRM, &action, NULL);
    
    int newargc = 0;
    char **newargv = args_set(arglist, &newargc, exename);

    int pip[2];

    sys_check = pipe (pip);
    if (sys_check == -1)
        perror ("3");

    if (!status)
    {
        // Set alarm.
        timer.it_value.tv_sec = time;
        timer.it_value.tv_usec = 0;
        timer.it_interval.tv_sec = 0;
        timer.it_interval.tv_usec = 0;
        setitimer (ITIMER_REAL, &timer, NULL);

        // Exec running proccess.(P2)
        if (!(pid = fork()))
        {
            sys_check = dup2 (pip[1], STDOUT_FILENO);
            if (sys_check == -1)
                perror ("4");
            fd = open (argv[3], O_RDONLY);
            if (fd == -1)
                perror ("5");
            sys_check = dup2 (fd, STDIN_FILENO);
            if (sys_check == -1)
                perror ("6");
            close (pip[0]);
            sys_check = execv (filename, newargv);
            if (sys_check == -1)
                perror ("7");
        }
        // Setting p4diff.(P3)
        if (!(tmp_pid = fork()))
        {
            sys_check = dup2 (pip[0], STDIN_FILENO);
            if (sys_check == -1)
                perror ("8");
            close (pip[1]);
            sys_check = execl ("./p4diff", "p4diff", argv[4], NULL);
            if (sys_check == -1)
                perror ("9");
        }
        close (pip[0]);
        close (pip[1]);
        sys_check = waitpid (pid, &status, 0);
        // Case where parent was interrupted by SIGALRM.
        if (sys_check < 0)
        {
            if (errno == EINTR)
                kill (pid, SIGKILL);
            sys_check = waitpid (pid, &status, 0);
            if (WIFSIGNALED(status))
            {
                if (WTERMSIG(status) == SIGKILL)
                    timeout = -100;
                else if (WTERMSIG(status) == SIGSEGV || WTERMSIG(status) == SIGABRT || WTERMSIG(status) == SIGBUS)
                    mem += -15;
                // Wait for p4diff to exit.
                sys_check = waitpid (tmp_pid, &status, 0);
                if (sys_check == tmp_pid && WIFEXITED(status))
                    output = WEXITSTATUS(status);
            }
        }
        // Case where P2 exited.
        else
        {
            if (WIFSIGNALED(status))
            {
                if (WTERMSIG(status) == SIGKILL)
                    timeout = -100;
                else if (WTERMSIG(status) == SIGSEGV || WTERMSIG(status) == SIGABRT || WTERMSIG(status) == SIGBUS)
                    mem += -15;
            }
            // Wait for p4diff to exit.
            sys_check = waitpid (tmp_pid, &status, 0);
        }
        if (sys_check == tmp_pid && WIFEXITED(status))
            output = WEXITSTATUS(status);
    }

    // Reset alarm.
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;
    setitimer (ITIMER_REAL, &timer, NULL);

    int i;

    for (i = 0; i < newargc; free (newargv[i]), i++);
    free (newargv);
    free (exename);
    free (errfile);
    free (filename);
    grade = compilation + timeout + output + mem;
    if (grade < 0)
        grade = 0;
    printf ("\nCompilation: %d\n\nTermination: %d\n\nOutput: %d\n\nMemory access: %d\n\nScore: %d\n", \
     compilation, timeout, output, mem, grade);
    return 0;
}
// SIG handler.
static void terminator (int sig) {}
// Checks for error or warning messages and evaluates the penalty.
int check (FILE *tmp_fd)
{
    int penalty = 0, result;
    char errstr[] = "error:", warningstr[] = "warning:", *buf;

    while ((result = fscanf (tmp_fd, "%ms", &buf)) > 0)
    {
        if (!strcmp (buf, errstr))
        {
            free (buf);
            return -100;
        }

        if (!strcmp (buf, warningstr))
            penalty -=5;
        free (buf);
    }
    return penalty;
}
// Sets the argument list for the compiled program.
char **args_set (char *arglist, int *nofargs, char *exename)
{
    FILE *fd;
    int size = 0, strsize;
    char **list = NULL, *ptr = NULL, **tmp_list, *tmp_str;

    fd = fopen (arglist, "r");
    if (!fd)
        perror ("args");

    tmp_list = (char **)malloc (sizeof (char *));
    if (tmp_list)
        list = tmp_list;
    tmp_str = (char *)malloc (sizeof (char) * (strlen (exename) + 1));
    if (tmp_str)
        list[size] = tmp_str;
    strcpy (list[size], exename);
    list[size][strlen (exename)] = '\0';
    size++;

    while (fscanf (fd, "%ms", &ptr) != EOF)
    {
        size++;
        tmp_list = (char **) realloc (list, sizeof (char *) * size);
        if (tmp_list)
            list = tmp_list;
        strsize = strlen (ptr);
        tmp_str = (char *)malloc (sizeof (char) * (strsize + 1));
        if (tmp_str)
        {
            list[size - 1] = tmp_str;
            strcpy (list[size - 1], ptr);
            list[size - 1][strsize] = '\0';
        }
        free (ptr);
    }

    tmp_list = (char **) realloc (list, sizeof (char *) * (size + 1));
    if (tmp_list)
        list = tmp_list;
    list[size] = NULL;

    *nofargs = size;
    return list;
}
