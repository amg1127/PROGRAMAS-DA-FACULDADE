#ifndef __TEMPLATES_H
#define __TEMPLATES_H

#include <QtCore>

template <class T>
void LimpaMatriz (T**& matriz, int dx, int dy) {
    int px, py;
    for (px = 0; px < dx; px++) {
        for (py = 0; py < dy; py++) {
            if (matriz[px][py] != NULL) {
                delete matriz[px][py];
            }
        }
        delete[] matriz[px];
    }
    delete[] matriz;
    matriz = NULL;
}

template <class T>
void CriaMatriz (T**& matriz, int dx, int dy) {
    int px;
    matriz = new T* [dx];
    for (px = 0; px < dx; px++) {
        matriz[px] = new T [dy];
    }
}

template <class T>
inline void poeDentroDaFaixa (T& n1, T min, T max) {
    if (n1 < min) {
        n1 = min;
    } else if (n1 > max) {
        n1 = max;
    }
}

#endif // __TEMPLATES_H
