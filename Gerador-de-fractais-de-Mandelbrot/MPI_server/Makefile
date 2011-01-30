all: MPI_server

MPI_server: MPI_server.o
	mpicc -Wall -g -o MPI_server MPI_server.o -lgd

MPI_server.o: Makefile MPI_server.c
	mpicc -Wall -g -o MPI_server.o -c MPI_server.c -DVERBOSE -DHOSTQUEESCUTA=\"`hostname -s`\" -DPORTATCP=1111

clean:
	rm -fv MPI_server *.o
