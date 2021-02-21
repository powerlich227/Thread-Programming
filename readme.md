The Makefile can generates the executable file sort.exe from my source code sort.c

If Makefile does not work, the source file could be compiled and executed as followed

	gcc -o sort.exe sort.c -lm -lpthread

    ./sort.exe ./input.txt -r

All these conmmands are verified by ubuntu 16.0

The source code could also be compiled in OS system, 
    gcc -o sort.exe sort.c

but OS do not support the function sem_init and sem_destory, so the file does not work well in OS
Please try to run it in Linux System.

The input.txt record all the test number to sort

The sema.init and Design.doc describe the use of semaphores and how to perform barrier synchronization.
The results of test running are also printed in Design.doc.
