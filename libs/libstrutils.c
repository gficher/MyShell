#include "libstrutils.h"

#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "libstrlist.h"

void strlistQuoteSplit(char *str, const char delim, char ***list, int *listSize) {
	char *start = str;
	// State can have one of those values:
	//  delim, ' ', '\'', '\"' or 'T'
	int state = delim;

	while(*str) {
		if (state == delim) {
			if (*str == '\"' || *str == '\'') {
				// Begin quote
				start = str;
				state = *str;
			} else if (*str != delim) {
				start = str;
				state = 'T';
			}
		} else if (state == 'T') {
			if (*str == delim) {
				// End of quote

				// Copy substring
				char *appendStr = malloc(str - start + 1);
				memcpy(appendStr, start, str - start + 1);
				appendStr[str - start] = '\0';
				appendStrList(trimQuotes(trimWhitespace(appendStr)), list, listSize);
				free(appendStr);

				// Update state
				state = delim;
			} else if (*str == '\"' || *str == '\'') {
				// Begin quote
				state = *str;
			}
		} else if (state == '\"' || state == '\'') {
			if (*str == state) {
				// End quote
				state = 'T';
			}
		}

		str++;
	}

	if (state != delim) {
		// Copy substring
		char *appendStr = malloc(str - start + 1);
		memcpy(appendStr, start, str - start + 1);
		appendStr[str - start] = '\0';
		appendStrList(trimQuotes(trimWhitespace(appendStr)), list, listSize);
		free(appendStr);
	}
}

char *trimWhitespace(char *str) {
	char *end;

	// Trim leading space
	while(isspace((unsigned char) *str))
		str++;

	if (*str == 0)  // String is only spaces
		return str;

	// Trim trailing space
	end = str + strlen(str) - 1;
	while(end > str && isspace((unsigned char) *end))
		end--;

	end[1] = '\0';

	return str;
}

char *trimQuotes(char *str) {
	char *end = str + strlen(str) - 1;

	if ((*str == '\'' || *str == '\"') && (*end == *str)) {
		str++;
		*end = '\0';
	}

	return str;
}