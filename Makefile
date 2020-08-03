CC = gcc

INCLUDE = libs/libcdhandler.c libs/libstrutils.c libs/libstrlist.c libs/libparseargexec.c libs/libparsepipe.c
INCLUDEDIR = libs

build: mysh

mysh: mysh.c
	${CC} $^ $(INCLUDE) -o $@ -I$(INCLUDEDIR) -Wall

clean:
	rm mysh
