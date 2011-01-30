CXXFLAGS=-g -Wall -O2

all: compacta.exe descompacta.exe test.exe

common.h: tcell.h thuff.h
	touch common.h

thuff.o: thuff.cpp common.h 
	g++ $(CXXFLAGS) -o thuff.o -c thuff.cpp

tcell.o: tcell.cpp common.h 
	g++ $(CXXFLAGS) -o tcell.o -c tcell.cpp

compacta.o: compacta.cpp common.h 
	g++ $(CXXFLAGS) -o compacta.o -c compacta.cpp

descompacta.o: descompacta.cpp common.h 
	g++ $(CXXFLAGS) -o descompacta.o -c descompacta.cpp

test.o: test.cpp common.h 
	g++ $(CXXFLAGS) -o test.o -c test.cpp

compacta.exe: compacta.o tcell.o thuff.o
	g++ $(CXXFLAGS) -o compacta.exe compacta.o tcell.o thuff.o

descompacta.exe: descompacta.o tcell.o thuff.o
	g++ $(CXXFLAGS) -o descompacta.exe descompacta.o tcell.o thuff.o

test.exe: test.o tcell.o thuff.o
	g++ $(CXXFLAGS) -o test.exe test.o tcell.o thuff.o

clean:
	rm -fv *.exe *.o
