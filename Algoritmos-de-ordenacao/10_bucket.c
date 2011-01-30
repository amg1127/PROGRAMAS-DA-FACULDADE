#include <stdio.h>
#include <stdlib.h>

#define BLOCO 1024

typedef struct nodo_t {
    double valor;
    struct nodo_t *prox;
} TNodo;

typedef struct lista_t {
    TNodo *inicio;
    TNodo *fim;
    int length;
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
    lista->length = 0;
}

void criaVetorLista (TLista *lista, int tam) {
    int i;
    for (i = 0; i < tam; i++) {
        criaLista (&lista[i]);
    }
}

void adicionaNoFim (TLista *lista, double valor) {
    lista->fim->valor = valor;
    if (lista->fim->prox == NULL)
        lista->fim->prox = novoNodo ();
    lista->fim = lista->fim->prox;
    lista->length++;
}

int removeDoInicio (TLista *lista, double *valor) {
    TNodo *mover;
    if (lista->inicio == lista->fim) {
        return (0);
    }
    mover = lista->inicio;
    *valor = mover->valor;
    lista->inicio = mover->prox;
    mover->prox = lista->fim->prox;
    lista->fim->prox = mover;
    lista->length--;
    return (1);
}

/* quicksort entra na conversa... */

void troca (double *a, double *b) {
    double tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

int partition (double *vetor, int p, int r) {
    double x;
    int i, j;
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

void quicksort (double *vetor, int p, int r) {
    int q;
    if (p < r) {
        q = partition (vetor, p, r);
        quicksort (vetor, p, q - 1);
        quicksort (vetor, q + 1, r);
    }
}

void sorteiaALista (TLista *lista) {
    static double *vect = NULL;
    static int vecttam = 0;
    int i;
    TNodo *navega;
    if (vecttam < lista->length) {
        if (vect != NULL) {
            free (vect);
        }
        vecttam = lista->length;
        do {
            vect = (double*) malloc (sizeof (double) * vecttam);
        } while (vect == NULL);
    }
    for (i = 0, navega = lista->inicio; i < lista->length; i++, navega = navega->prox) {
        vect[i] = navega->valor;
    }
    quicksort (vect, 0, lista->length - 1);
    for (i = 0, navega = lista->inicio; i < lista->length; i++, navega = navega->prox) {
        navega->valor = vect[i];
    }
}

void bucketsort (double *vetor, TLista *auxiliar, int tam) {
    int i;
    int pos;
    double v;
    for (i = 0; i < tam; i++) {
        pos = (int) (vetor[i] * tam);
        adicionaNoFim (&auxiliar[pos], vetor[i]);
    }
    for (i = 0; i < tam; i++) {
        sorteiaALista (&auxiliar[i]);
    }
    for (i = 0, pos = 0; i < tam; i++) {
        while (! removeDoInicio (&auxiliar[pos], &v)) {
            pos++;
        }
        vetor[i] = v;
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

int main (void) {
    int entrada[TAM];
    TLista aux[TAM];
    double entrada_doub[TAM];
    int i;
    criaVetorLista (aux, TAM);
    leitura_vetor (entrada, TAM);
    for (i = 0; i < TAM; i++) {
        entrada_doub[i] = 1.0 * entrada[i] / 65536;
    }
    bucketsort (entrada_doub, aux, TAM);
    for (i = 0; i < TAM; i++) {
        entrada[i] = 65536 * entrada_doub[i];
    }
    escrita_vetor (entrada, TAM);
    return (0);
}
