#include <stdio.h>

typedef int TDados;
typedef unsigned long int TTam;

void troca (TDados *a, TDados *b) {
    TDados tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

void insertionsort (TDados *vetor, TTam tam) {
    TTam i, j;
    TDados chave;
    for (j = 1; j < tam; j++) {
        chave = vetor[j];
        i = j - 1;
        while (i >= 0 && vetor[i] > chave) {
            vetor[i+1] = vetor[i];
            i--;
            vetor[i+1] = chave;
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
    insertionsort (entrada, TAM);
    escrita_vetor (entrada, TAM);
    return (0);
}
