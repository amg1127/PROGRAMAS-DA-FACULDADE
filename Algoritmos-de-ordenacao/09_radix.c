#include <stdio.h>
#include <stdlib.h>

/* Nesse algoritmo, os dados sao usados para indicar posicao de vetor... Entao, os dados a sortear OBRIGATORIAMENTE serao INT */

typedef struct nodo_t {
    int valor;
    struct nodo_t *prox;
} TNodo;

typedef struct lista_t {
    TNodo *inicio;
    TNodo *fim;
} TLista;

TNodo* novoNodo () {
    TNodo *novo;
    do {
        novo = (TNodo*) malloc (sizeof (TNodo));
    } while (novo == NULL);
    novo->prox = NULL;
    return (novo);
}

void criaLista (TLista *lista) {
    lista->inicio = novoNodo ();
    lista->fim = lista->inicio;
}

void criaVetorLista (TLista *lista, int tam) {
    int i;
    for (i = 0; i < tam; i++) {
        criaLista (&lista[i]);
    }
}

void adicionaNoFim (TLista *lista, int valor) {
    lista->fim->valor = valor;
    if (lista->fim->prox == NULL)
        lista->fim->prox = novoNodo ();
    lista->fim = lista->fim->prox;
}

int removeDoInicio (TLista *lista, int *valor) {
    TNodo *mover;
    if (lista->inicio == lista->fim) {
        return (0);
    }
    mover = lista->inicio;
    *valor = mover->valor;
    lista->inicio = mover->prox;
    mover->prox = lista->fim->prox;
    lista->fim->prox = mover;
    return (1);
}

int digito (int numero, int base, int pos, int *saida) {
    while (pos > 0) {
        numero /= base;
        pos--;
    }
    *saida = numero % base;
    return (numero != 0);
}

void radixsort (int *vetor, TLista *vetor_lista, int tam) {
    int i, gira, dig, posicao, j;
    dig = 0;
    do {
        gira = 0;
        for (i = 0; i < tam; i++) {
            if (digito (vetor[i], tam, dig, &posicao)) {
                gira = 1;
            }
            adicionaNoFim (&vetor_lista[posicao], vetor[i]);
        }
        i = 0;
        j = 0;
        while (i < tam && j < tam) {
            if (removeDoInicio (&vetor_lista[i], &vetor[j])) {
                j++;
            } else {
                i++;
            }
        }
        dig++;
    } while (gira);
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

int main (void) {
    int entrada[TAM];
    TLista lista[TAM];
    criaVetorLista (lista, TAM);
    leitura_vetor (entrada, TAM);
    radixsort (entrada, lista, TAM);
    escrita_vetor (entrada, TAM);
    return (0);
}
