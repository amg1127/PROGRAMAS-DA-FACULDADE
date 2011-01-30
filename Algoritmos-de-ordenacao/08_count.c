#include <stdio.h>

void countingsort (int *vetorin, int *conta, int *vetorout, int max, int tam) {
    int i;
    for (i = 0; i < max; i++) {
        conta[i] = 0;
    }
    for (i = 0; i < tam; i++) {
        conta[ vetorin[i] ]++;
    }
    for (i = 1; i < max; i++) {
        conta[i] += conta[i-1];
    }
    for (i = tam - 1; i >= 0; i--) {
        vetorout[ conta[ vetorin[i] ] - 1 ] = vetorin[i];
        conta[ vetorin[i] ]--;
    }
}

void imprime (int d) {
    printf ("%d\n", d);
}

void leitura_vetor (int *vetor, int tam) {
    int i;
    int valor;
    for (i = 0; i < tam; i++) {
        scanf ("%d", &valor);
        vetor[i] = valor;
    }
}

void escrita_vetor (int *vetor, int tam) {
    int i;
    for (i = 0; i < tam; i++) {
        imprime (vetor[i]);
    }
}

#define TAM 10240
#define MAX 65536

int main (void) {
    int entrada[TAM];
    int aux[MAX], saida[TAM];
    leitura_vetor (entrada, TAM);
    countingsort (entrada, aux, saida, MAX, TAM);
    escrita_vetor (saida, TAM);
    return (0);
}
