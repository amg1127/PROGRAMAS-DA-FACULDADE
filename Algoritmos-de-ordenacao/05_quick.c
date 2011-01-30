/* Um produto Nestle!!! */

#include <stdio.h>

typedef int TDados;
typedef long int TTam;

void troca (TDados *a, TDados *b) {
    TDados tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

TTam partition (TDados *vetor, TTam p, TTam r) {
    TDados x;
    TTam i, j;
    x = vetor[r];
    i = p - 1;
    for (j = p; j < r; j++) {
        if (vetor[j] <= x) {
            i++;
            troca (&vetor[i], &vetor[j]);
        }
    }
    troca (&vetor[i+1], &vetor[r]);
    return (i+1);
}

void quicksort (TDados *vetor, TTam p, TTam r) {
    TTam q;
    if (p < r) {
        q = partition (vetor, p, r);
        quicksort (vetor, p, q - 1);
        quicksort (vetor, q + 1, r);
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
    quicksort (entrada, 0, TAM-1);
    escrita_vetor (entrada, TAM);
    return (0);
}
