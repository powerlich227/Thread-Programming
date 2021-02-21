all: sort.exe

foo2.exe: sort.c
	gcc -o sort.exe sort.c -lm -lpthread