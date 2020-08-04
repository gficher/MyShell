#include "libcdhandler.h"

#include <stddef.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

int cdHandler(char **args) {
    if (args[1] != NULL) {
        if (args[1][0] == '~' && args[1][0] == '\0') {
            // Go home
            chdir(getenv("HOME"));
        } else if (args[1][0] == '~') {
            // Gome home
            chdir(getenv("HOME"));

            // Check if there is a relative path
            if (args[1][1] == '/') {
                // Continue navigation
                chdir(args[1]+2);
            }
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
