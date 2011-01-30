#include <stdio.h>

typedef int TDados;
typedef unsigned long int TTam;

#define TAM 10240

void merge (TDados *vetor, TTam p, TTam q, TTam r) {
    TTam n1, n2, i, j, k;
    int f;
    static TDados L[TAM/2+1], R[TAM/2+1];
    n1 = q - p + 1;
    n2 = r - q;
    for (i = 1; i <= n1; i++) {
        L[i-1] = vetor[p+i-1-1];
    }
    for (j = 1; j <= n2; j++) {
        R[j-1] = vetor[q+j-1];
    }
    /* L[n1+1-1] = INFINITO; */
    /* R[n2+1-1] = INFINITO; */
    i = 1;
    j = 1;
    for (k = p; k <= r; k++) {
        f = 0;
        if (j > n2) {
            f = 1;
        } else if (i <= n1) {
            if (L[i-1] <= R[j-1]) {
                f = 1;
            }
        }
        if (f != 0) {
            vetor[k-1] = L[i-1];
            i++;
        } else {
            vetor[k-1] = R[j-1];
            j++;
        }
    }
}

void mergesort (TDados *vetor, TTam p, TTam r) {
    TTam q;
    if (p < r) {
        q = (p + r) / 2;
        mergesort (vetor, p, q);
        mergesort (vetor, q+1, r);
        merge (vetor, p, q, r);
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

int main (void) {
    TDados entrada[TAM];
    leitura_vetor (entrada, TAM);
    mergesort (entrada, 1, TAM);
    escrita_vetor (entrada, TAM);
    return (0);
}
