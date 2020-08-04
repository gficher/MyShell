CC = gcc

INCLUDE = libs/libcdhandler.c libs/libstrutils.c libs/libstrlist.c libs/libinputparse.c
INCLUDEDIR = libs

build: mysh

mysh: mysh.c
	${CC} $^ $(INCLUDE) -o $@ -I$(INCLUDEDIR) -Wall -Wextra -pedantic

clean:
	rm -f mysh
