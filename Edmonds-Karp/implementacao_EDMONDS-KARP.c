/* Algoritmo de Edmonds-Karp em C */

/* Arquivos necessarios para iteracao minima com o usuario e alocacao de memoria */
#include <stdio.h>
#include <stdlib.h>

/* Constantes usadas no algoritmo de busca em largura */
#define BRANCO 0
#define CINZA 1
#define PRETO 2

/* Numero de vertices */
#define NVERT 7

/* Esta struct eh um no de um grafo representado em lista de adjacencias */
struct noAdj {
    struct vert *vertice; /* O vertice referenciado pelo no */
    int peso_arco; /* O peso do arco */
    struct noAdj *proximo; /* O proximo elemento */
};

/* Esta struct eh o cabecalho da lista de adjacencias. */
struct vert {
    struct vert *pai; /* O vertice-pai */
    struct noAdj *listaAdj; /* A lista de adjacencias para este vertice */
    int cor; /* A cor do vertice (utilizado no algoritmo de pesquisa em largura) */
};

/* Implementacao de uma fila de ponteiros para vertices */
struct fila {
    int inicio, fim;
    struct vert *dados [NVERT];
};

/* Funcao para calcular o numero de elementos guardados na fila */
int tamanhoFila (struct fila *f) {
    int tam;
    tam = (f->fim + NVERT - f->inicio) % NVERT;
    return (tam);
}

/* Funcao para enfilar um dado */
void enfila (struct fila *f, struct vert *d) {
    if (tamanhoFila(f) + 1 < NVERT) {
        f->dados[f->fim] = d;
        f->fim = (f->fim + 1) % NVERT;
    }
}

/* Funcao para desenfilar um dado */
struct vert *desenfila (struct fila *f) {
    struct vert *r = NULL;
    if (tamanhoFila(f) > 0) {
        r = f->dados[f->inicio];
        f->inicio = (f->inicio + 1) % NVERT;
    }
    return (r);
}

/* Funcao para inicializar uma fila */
void criaFila (struct fila *f) {
    f->inicio = 0;
    f->fim = 0;
}

/* Funcao de busca em largura - modificada para nao calcular distancia entre vertices. Motivo: desnecessidade. */
void BFS (struct vert *grafo, struct vert *origem) {
    int u;
    struct fila f;
    struct vert *nografo;
    struct noAdj *nolista;
    for (u = 0; u < NVERT; u++) {
        grafo[u].cor = BRANCO;
        grafo[u].pai = NULL;
    }
    origem->cor = CINZA;
    origem->pai = NULL;
    criaFila (&f);
    enfila (&f, origem);
    while (tamanhoFila (&f) != 0) {
        nografo = desenfila (&f);
        for (nolista = nografo->listaAdj; nolista != NULL; nolista = nolista->proximo) {
            if (nolista->peso_arco != 0) { /* Isso serve para o algoritmo nao navegar em arcos de capacidade igual a zero */
                if (nolista->vertice->cor == BRANCO) {
                    nolista->vertice->cor = CINZA;
                    nolista->vertice->pai = nografo;
                    enfila (&f, nolista->vertice);
                }
            }
        }
        nografo->cor = PRETO;
    }
}

/* Funcao para criar um novo no de adjacencia */
struct noAdj *criaNoAdjacencia (struct vert *vertDestino, int peso_arco, struct noAdj *proximo) {
    struct noAdj *novo;
    do
        novo = (struct noAdj *) malloc (sizeof (struct noAdj));
    while (novo == NULL);
    novo->vertice = vertDestino;
    novo->peso_arco = peso_arco;
    novo->proximo = proximo;
    return (novo);
}

/* Programa principal */
int main (void) {
    struct vert grafo[NVERT];
    struct vert *navegante;
    struct noAdj *noAdjasc;
    int somafluxo;
    int menorcapacidade;
    /* Criar a lista de adjacencia correspondente ao exemplo */
    /* p=0, q=1, r=2, s=3, t=4, u=5, v=6 */
    /* Obs: arcos com capacidade = 0 sao os arcos simetricos */
    grafo[0].listaAdj = criaNoAdjacencia (&grafo[5], 12, NULL); /* p >= 12 => u */
    grafo[0].listaAdj = criaNoAdjacencia (&grafo[3], 0, grafo[0].listaAdj); /* p  >= 0 => s */

    grafo[1].listaAdj = criaNoAdjacencia (&grafo[5], 6, NULL); /* q >= 6 => u */
    grafo[1].listaAdj = criaNoAdjacencia (&grafo[3], 0, grafo[1].listaAdj); /* q >= 0 => s */

    grafo[2].listaAdj = criaNoAdjacencia (&grafo[6], 20, NULL); /* r >= 20 => v */
    grafo[2].listaAdj = criaNoAdjacencia (&grafo[5], 0, grafo[2].listaAdj); /* r >= 0 => u */
    grafo[2].listaAdj = criaNoAdjacencia (&grafo[3], 0, grafo[2].listaAdj); /* r >= 0 => s */

    grafo[3].listaAdj = criaNoAdjacencia (&grafo[0], 14, NULL); /* s >= 14 => p */
    grafo[3].listaAdj = criaNoAdjacencia (&grafo[1], 7, grafo[3].listaAdj); /* s >= 7 => q */
    grafo[3].listaAdj = criaNoAdjacencia (&grafo[2], 14, grafo[3].listaAdj); /* s >= 14 => r */

    grafo[4].listaAdj = criaNoAdjacencia (&grafo[5], 0, NULL); /* t >= 0 => u */
    grafo[4].listaAdj = criaNoAdjacencia (&grafo[6], 0, grafo[4].listaAdj); /* t >= 0 => v */

    grafo[5].listaAdj = criaNoAdjacencia (&grafo[0], 0, NULL); /* u >= 0 => p */
    grafo[5].listaAdj = criaNoAdjacencia (&grafo[1], 0, grafo[5].listaAdj); /* u >= 0 => q */
    grafo[5].listaAdj = criaNoAdjacencia (&grafo[2], 11, grafo[5].listaAdj); /* u >= 11 => r */
    grafo[5].listaAdj = criaNoAdjacencia (&grafo[4], 5, grafo[5].listaAdj); /* u >= 5 => t */

    grafo[6].listaAdj = criaNoAdjacencia (&grafo[4], 18, NULL);  /* v >= 18 => t */
    grafo[6].listaAdj = criaNoAdjacencia (&grafo[2], 0, grafo[6].listaAdj); /* v >= 0 => r */

    /* Aplicar o algoritmo de Edmonds-Karp */
    /* O algoritmo descrito tem fins educacionais, nao de eficiencia */
    somafluxo = 0;
    while (1) {
        /* Aplicar a busca em largura */
        BFS (grafo, &grafo[3]);
        /* Testar se existe um caminho de 's' para 't' */
        /* Navegar de 't' ate chegar no 's' ou no fim do grafo */
        navegante = &grafo[4]; /* Vertice 't' */
        do {
            navegante = navegante->pai;
            if (navegante == NULL) { /* Encontrou um vertice sem pai antes de encontrar o no de origem ? */
                printf ("O fluxo total eh %d KB/s.\n", somafluxo);
                return (0);
            }
        } while (navegante != &grafo[3]); /* Vertice 's' */
        /* Saiu do while. Portanto, ainda existe um caminho valido */
        /* Ele deve ser novamente percorrido, para que seja localizada a menor capacidade de fluxo */
        navegante = &grafo[4]; /* Vertice 't' */
        menorcapacidade = 9999999;
        do {
            noAdjasc = navegante->pai->listaAdj;
            while (noAdjasc->vertice != navegante) {
                noAdjasc = noAdjasc->proximo;;
            }
            /* 'noAdjasc' eh o arco que parte do pai do vertice indicado por 'navegante' e que se destina a ele. */
            if (menorcapacidade > noAdjasc->peso_arco) {
                menorcapacidade = noAdjasc->peso_arco;
            }
            navegante = navegante->pai;
        } while (navegante != &grafo[3]); /* Vertice 's' */
        somafluxo += menorcapacidade;
        /* Agora, o caminho deve ser percorrido para modificar as capacidades */
        navegante = &grafo[4]; /* Vertice 't' */
        do {
            noAdjasc = navegante->pai->listaAdj;
            while (noAdjasc->vertice != navegante) {
                noAdjasc = noAdjasc->proximo;;
            }
            /* 'noAdjasc' eh o arco que parte do pai do vertice indicado por 'navegante' e que se destina a ele. */
            noAdjasc->peso_arco -= menorcapacidade;
            noAdjasc = navegante->listaAdj;
            while (noAdjasc->vertice != navegante->pai) {
                noAdjasc = noAdjasc->proximo;;
            }
            /* 'noAdjasc' eh o arco que se destina ao pai do vertice indicado por 'navegante'. */
            noAdjasc->peso_arco += menorcapacidade;
            navegante = navegante->pai;
        } while (navegante != &grafo[3]); /* Vertice 's' */
    }
}
