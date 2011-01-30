all: single

single: single.o
	gcc -Wall -g -o single single.o -lgd

single.o: Makefile single.c
	gcc -Wall -g -o single.o -c single.c

clean:
	rm -fv single *.o
