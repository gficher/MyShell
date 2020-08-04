#include "libstrlist.h"

#include <stdlib.h>
#include <stddef.h>
#include <stddef.h>
#include <ctype.h>
#include <string.h>

#include <stdio.h>

char **createStrList() {
    char **list;

    list = malloc(sizeof(char **));
    list[0] = NULL;

    return list;
}

char **expandStrList(char **list, int size) {
    char **newList = realloc(list, sizeof(char *) * size);
    newList[size-1] = NULL;

    return newList;
}

void appendStrList(char *str, char ***list, int *size) {
    printf("'%s'\n", str);
    
    if (*size == 0) {
        *list = createStrList();
    } else {
        *list = expandStrList(*list, *size+1);
    }

    (*list)[*size] = malloc(strlen(str)+1);
    strcpy((*list)[*size], str);

    (*size)++;
}

int getStrListSize(char **list) {
    int count = 0;

    while (list[count++] != NULL);

    return count-1;
}

void freeStrList(char **list) {
    int i = 0;
    
    while (list[i] != NULL) {
        free(list[i++]);
    }

    free(list);
}