#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

void usage(const char *reason) {
    fprintf(stderr, "Syntax error (%s)\n", reason);
    fprintf(stderr, "Usage: chroot <directory> [executable]\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    int status;
    char *rootdir;
    char *executable = "/bin/sh";
    char *newargv[] = {NULL,NULL};
    char *newenviron[] = {NULL};

    /* Parse arguments */
    if (argc < 2)
        usage("argument count");

    if (argc >= 3)
        executable = argv[2];
    rootdir = argv[1];

    /* Do changeroot */
    status = chdir(rootdir);
    if (status) {
        fprintf(stderr, "chroot(chdir): %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    status = chroot(rootdir);
    if (status) {
        fprintf(stderr, "chroot(chroot): %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* Invoke executable */
    newargv[0] = executable;

    execve ( executable, &newargv[0], newenviron );

    fprintf(stderr, "chroot(execve): %s\n", strerror(errno));

    return EXIT_FAILURE;

}

