#include <stdio.h>
#include <time.h>
#include <stdlib.h>

typedef int TDados;
typedef long int TTam;

void troca (TDados *a, TDados *b) {
    TDados tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

TTam marilton (TTam p, TTam r) {
    TTam i, diff;
    diff = r - p + 1;
    i = (int) (1.0 * diff * rand () / (RAND_MAX + 1.0));
    return (i + p);
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

TTam rnd_partition (TDados *vetor, TTam p, TTam r) {
    TTam i;
    i = marilton (p, r);
    troca (&vetor[p], &vetor[i]);
    return (partition (vetor, p, r));
}

void rnd_quicksort (TDados *vetor, TTam p, TTam r) {
    TTam q;
    if (p < r) {
        q = rnd_partition (vetor, p, r);
        rnd_quicksort (vetor, p, q - 1);
        rnd_quicksort (vetor, q + 1, r);
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

#define TAM 10000

int main (void) {
    TDados entrada[TAM];
    srand (time (NULL));
    leitura_vetor (entrada, TAM);
    rnd_quicksort (entrada, 0, TAM-1);
    escrita_vetor (entrada, TAM);
    return (0);
}
