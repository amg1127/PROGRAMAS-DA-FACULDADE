#include <stdio.h>

#define TAM 10
#define ALFA "ab"

void recursao (char* buffer, char* lista, int len1, int len2) {
    int i;
    buffer[len1] = 0;
    printf ("%s\n", buffer);
    if (len1 < TAM) {
        for (i = 0; i < len2; i++) {
            buffer[len1] = lista[i];
            recursao (buffer, lista, len1 + 1, len2);
        }
    }
    buffer[len1] = 0;
}

int main () {
    char buffer[TAM + 1];
    buffer[0] = 0;
    recursao (buffer, ALFA, 0, 2);
    return (0);
}
