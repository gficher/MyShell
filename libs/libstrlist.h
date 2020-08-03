#ifndef _LIBSTRLIST_H_
#define _LIBSTRLIST_H_
    char **createStrList();
    char **expandStrList(char **list, int size);
    void appendStrList(char *str, char ***list, int *size);
    int getStrListSize(char **list);
    void freeStrList(char **list);
#endif