#include "libparsepipe.h"

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
