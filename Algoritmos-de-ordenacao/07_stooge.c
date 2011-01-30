#include <stdio.h>

typedef int TDados;
typedef unsigned long int TTam;

void troca (TDados *a, TDados *b) {
    TDados tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

void stoogesort (int *vetor, int ultimo) {
    TTam k;
    if (vetor[0] > vetor[ultimo]) {
        troca (&vetor[0], &vetor[ultimo]);
    }
    if (1 < ultimo) {   /* 1 = número um, não L minúsculo */
        k = (ultimo+1) / 3;
        stoogesort (&vetor[0], ultimo-k);
        stoogesort (&vetor[k], ultimo-k);
        stoogesort (&vetor[0], ultimo-k);
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

#define TAM 10240

int main (void) {
    TDados entrada[TAM];
    leitura_vetor (entrada, TAM);
    stoogesort (entrada, TAM-1);
    escrita_vetor (entrada, TAM);
    return (0);
}
