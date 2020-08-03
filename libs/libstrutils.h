#ifndef _LIBSTRUTILS_H_
#define _LIBSTRUTILS_H_
    char *trimWhitespace(char *str);
    char *trimQuotes(char *str);
    void strlistQuoteSplit(char *str, const char delim, char ***list, int *listSize);
#endif