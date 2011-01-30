#include <stdio.h>

typedef int TDados;
typedef unsigned long int TTam;

void troca (TDados *a, TDados *b) {
    TDados tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

void bubblesort (TDados *vetor, TTam tam) {
    TTam i, j;
    for (i = 0; i < tam; i++) {
        for (j = tam - 1; j > i; j--) {
            if (vetor[j] < vetor[j-1]) {
                troca (&vetor[j], &vetor[j-1]);
            }
        }
    }
}

void imprime (TDados d) {
    printf ("%d\n", d);
}

void leitura_vetor (TDados *vetor, int tam) {
    int i;
    int valor;
    for (i = 0; i < tam; i++) {
        scanf ("%d", &valor);
        vetor[i] = valor;
    }
}

void escrita_vetor (TDados *vetor, int tam) {
    int i;
    for (i = 0; i < tam; i++) {
        imprime (vetor[i]);
    }
}

#define TAM 40000

int main (void) {
    TDados entrada[TAM];
    leitura_vetor (entrada, TAM);
    bubblesort (entrada, TAM);
    escrita_vetor (entrada, TAM);
    return (0);
}
