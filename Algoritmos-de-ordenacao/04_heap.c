#include <stdio.h>

typedef int TDados;
typedef unsigned long int TTam;

/* Comentar o define abaixo para compilar a versao iterativa */
#define COMPILA_RECURSIVO

void troca (TDados *a, TDados *b) {
    TDados tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

TTam esquerda (TTam i) {
    return (2 * i);
}

TTam direita (TTam i) {
    return (2 * i + 1);
}

#ifdef COMPILA_RECURSIVO
void max_heapfy (TDados *vetor, TTam i, TTam tam) {
    TTam l, r, maior;
    l = esquerda (i);
    r = direita (i);
    maior = i;
    if (l <= tam) {
        if (vetor[l-1] > vetor[maior-1]) {
            maior = l;
        }
    }
    if (r <= tam) {
        if (vetor[r-1] > vetor[maior-1]) {
            maior = r;
        }
    }
    if (maior != i) {
        troca (&vetor[i-1], &vetor[maior-1]);
        max_heapfy (vetor, maior, tam);
    }
}
#else
void max_heapfy (TDados *vetor, TTam i, TTam tam) {
    TTam l, r, maior;
    do {
        l = esquerda (i);
        r = direita (i);
        maior = i;
        if (l <= tam) {
            if (vetor[l-1] > vetor[maior-1]) {
                maior = l;
            }
        }
        if (r <= tam) {
            if (vetor[r-1] > vetor[maior-1]) {
                maior = r;
            }
        }
        if (maior != i) {
            troca (&vetor[i-1], &vetor[maior-1]);
            i = maior;
            maior--;
        }
    } while (maior != i);
}
#endif

void build_max_heap (TDados *vetor, TTam tam) {
    TTam i;
    for (i = tam / 2; i > 0; i--) {
        max_heapfy (vetor, i, tam);
    }
}

void heapsort (TDados *vetor, TTam tam) {
    TTam i;
    build_max_heap (vetor, tam);
    for (i = tam; i > 1; i--) {
        troca (&vetor[0], &vetor[i-1]);
        max_heapfy (vetor, 1, i-1);
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
    heapsort (entrada, TAM);
    escrita_vetor (entrada, TAM);
    return (0);
}
