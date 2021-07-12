all: test

test: test.o fs.o
        gcc -o test test.o fs.o

test.o: test.c fs_struct.h
        gcc -c test.c

fs.o: fs.c fs_struct.h
        gcc -c fs.c
