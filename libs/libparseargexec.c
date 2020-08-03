#include "libparseargexec.h"

#include <string.h>

#include "libstrutils.h"
#include "libstrlist.h"

char **parseArgExec(char *cmd) {
    char **list;
    int listSize = 0;

    strlistQuoteSplit(cmd, ' ', &list, &listSize);

    // Add NULL terminator (last list element is NULL)
    list = expandStrList(list, ++listSize);

    return list;
}
