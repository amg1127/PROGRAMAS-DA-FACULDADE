// main.cpp
#include <QApplication>
#include "unistd.h"
#include "common.h"

int maior2 (int n1, int n2) {
    return ((n1 > n2) ? n1 : n2);
}

int maior3 (int n1, int n2, int n3) {
    return (maior2 (n1, maior2 (n2, n3)));
}

int maior4 (int n1, int n2, int n3, int n4) {
    return (maior2 (maior2 (n1, n2), maior2 (n3, n4)));
}

int maior5 (int n1, int n2, int n3, int n4, int n5) {
    return (maior2 (maior2 (n1, n2), maior3 (n3, n4, n5)));
}

int main (int argc, char **argv) {
    srand (time(NULL));
    QApplication prog (argc, argv);
    visualizacao v;
    v.show ();
    return (prog.exec ());
}
// main.cpp
