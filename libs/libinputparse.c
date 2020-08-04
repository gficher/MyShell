#include "libinputparse.h"

#include <string.h>

#include "libstrutils.h"
#include "libstrlist.h"

char **parsePipe(char *input) {
    char **list;
    int listSize = 0;

    strlistQuoteSplit(input, '|', &list, &listSize);

    // Add NULL terminator (last list element is NULL)
    list = expandStrList(list, ++listSize);

    return list;
}

char **parseArgExec(char *cmd) {
    char **list;
    int listSize = 0;

    strlistQuoteSplit(cmd, ' ', &list, &listSize);

    // Add NULL terminator (last list element is NULL)
    list = expandStrList(list, ++listSize);

    return list;
}

char **parseSemicolon(char *cmd) {
    char **list;
    int listSize = 0;

    strlistQuoteSplit(cmd, ';', &list, &listSize);

    // Add NULL terminator (last list element is NULL)
    list = expandStrList(list, ++listSize);

    return list;
}
