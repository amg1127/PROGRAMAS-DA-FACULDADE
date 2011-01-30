CXXFLAGS=-g -Wall -O2

all: compacta_adaptive.exe descompacta_adaptive.exe test.exe

common.h: tcell.h tadaphuff.h
	touch common.h

tadaphuff.o: tadaphuff.cpp common.h 
	g++ $(CXXFLAGS) -o tadaphuff.o -c tadaphuff.cpp

tcell.o: tcell.cpp common.h 
	g++ $(CXXFLAGS) -o tcell.o -c tcell.cpp

compacta_adaptive.o: compacta_adaptive.cpp common.h 
	g++ $(CXXFLAGS) -o compacta_adaptive.o -c compacta_adaptive.cpp

descompacta_adaptive.o: descompacta_adaptive.cpp common.h 
	g++ $(CXXFLAGS) -o descompacta_adaptive.o -c descompacta_adaptive.cpp

test.o: test.cpp common.h 
	g++ $(CXXFLAGS) -o test.o -c test.cpp

compacta_adaptive.exe: compacta_adaptive.o tcell.o tadaphuff.o
	g++ $(CXXFLAGS) -o compacta_adaptive.exe compacta_adaptive.o tcell.o tadaphuff.o

descompacta_adaptive.exe: descompacta_adaptive.o tcell.o tadaphuff.o
	g++ $(CXXFLAGS) -o descompacta_adaptive.exe descompacta_adaptive.o tcell.o tadaphuff.o

test.exe: test.o tcell.o tadaphuff.o
	g++ $(CXXFLAGS) -o test.exe test.o tcell.o tadaphuff.o

clean:
	rm -fv *.exe *.o
