all:
	qmake -o Makefile.qt
	make -f Makefile.qt

clean:
	(test -e Makefile.qt && make -f Makefile.qt distclean) || true
