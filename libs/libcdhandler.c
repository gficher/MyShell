#include "libcdhandler.h"

#include <stddef.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

int cdHandler(char **args) {
    if (args[1] != NULL) {
        if (args[1][0] == '~') {
            // Go home
            chdir(getenv("HOME"));
        } else if (args[1][0] == '*') {
            // This is the path my source code was located in Windows from WSL
            // I got tired of retyping it after a while
            chdir("/mnt/c/Users/gficher/OneDrive/Documentos/Faculdade/7sem/SO2/aula5");
        } else {
            // Go wherever else
            if (chdir(args[1]) < 0) {
                fprintf(stderr, "Error while changing dir: %s\n", strerror(errno));
                return -1;
            }
        }
    } else {
        chdir(getenv("HOME"));
    }

    return 0;
}
