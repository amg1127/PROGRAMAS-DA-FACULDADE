all: MPI_client

MPI_client: MPI_client.o
	gcc -Wall -g -o MPI_client MPI_client.o

MPI_client.o: Makefile MPI_client.c
	gcc -Wall -g -o MPI_client.o -c MPI_client.c

clean:
	rm -fv MPI_client *.o
