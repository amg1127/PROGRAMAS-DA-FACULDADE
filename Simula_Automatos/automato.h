#ifndef __AUTOMATO_H
#define __AUTOMATO_H

#include "common.h"

class vertice;
class arco;
class automato;

class vertice {
    public:
        QString nome;
        QPoint pos;
        bool inicial, final;
        QVector<arco> arcos;
        vertice() { this->inicial = this->final = false; }
};

class arco {
    public:
        QChar simb;
        QString destino;
};

class automato {
    private:
        QVector<vertice> vertices;
        QVector<QChar> simbolos;
        void ajustaVertice (QString);
        bool revalidar;
        bool estavalido;
        QMutex* m;
        bool reconhece (QString, QString, QLinkedList<QString>&, QLinkedList<QString>&);
    public:
        automato ();
        ~automato ();
        // Verificar se a palavra passada eh reconhecida pelo automato
        bool reconhece (QString);
        // Adiciona um simbolo ao automato. Retorna 'true' se tudo deu certo.
        bool addSimb (QChar);
        // Obtem a lista de simbolos ja adicionados
        QVector<QChar> getSimb (void);
        // Muda um simbolo (por exemplo, o simbolo que era 'a' passa a ser 'b'). Retorna 'true' se tudo deu certo.
        // Parametros (em ordem): nome antigo, nome novo
        bool mvSimb (QChar, QChar);
        // Remove um simbolo do automato. Retorna 'true' se tudo deu certo.
        bool rmSimb (QChar);
        // Adiciona um estado ao automato. Retorna 'true' se tudo deu certo.
        bool addEstado (QString);
        // Obtem a lista de estados ja adicionados
        QVector<QString> getEstado (void);
        // Muda o nome de um estado. Retorna 'true' se tudo deu certo.
        // Parametros (em ordem): nome antigo, nome novo
        bool mvEstado (QString, QString);
        // Remove um estado. Retorna 'true' se tudo deu certo.
        bool rmEstado (QString);
        // Marca um estado (previamente adicionado) como inicial ou nao. Retorna 'true' se tudo deu certo.
        bool setEstadoInicial (QString, bool);
        // Pergunta se o estado especificado foi marcado como inicial.
        bool isEstadoInicial (QString);
        // Obtem o estado que foi marcado como inicial, se houver
        QString getEstadoInicial (void);
        // Marca um estado (previamente adicionado) como final ou nao. Retorna 'true' se tudo deu certo.
        bool setEstadoFinal (QString, bool);
        // Pergunta se o estado especificado foi marcado como final
        bool isEstadoFinal (QString);
        // Obtem a lista dos estados que foram marcados como final
        QVector<QString> getEstadoFinal (void);
        // Obtem as coordenadas (x,y) do estado (faz sentido ao desenhar o automato na tela)
        QPoint getPosEstado (QString);
        // Define as coordenadas (x,y) do estado (faz sentido ao desenhar o automato na tela)
        bool setPosEstado (QString, QPoint);
        // Adiciona um argumento para a funcao que recebe um par estado-simbolo e fornece um conjunto de estados. Retorna 'true' se tudo deu certo
        // Parametros (em ordem): estado atual, simbolo, proximo estado
        bool addFuncTrans (QString, QChar, QString);
        // Retorna o conjunto de proximos estados para o estado atual e simbolo de entrada dados
        // Parametros (em ordem): estado atual, simbolo
        QVector<QString> getFuncTrans (QString, QChar);
        // Retorna o conjunto de simbolos para o estado atual e proximo estado dados
        // Parametros (em ordem): estado atual, proximo estado
        QVector<QChar> getFuncTrans (QString, QString);
        // Remove um trio estado atual - simbolo - proximo estado. Retorna 'true' se tudo deu certo.
        // Parametros (em ordem): estado atual, simbolo, proximo estado
        bool rmFuncTrans (QString, QChar, QString);
        // Remove todos os trios estado atual - simbolo - proximo estado. Retorna 'true' se tudo deu certo.
        // Parametros (em ordem): estado atual, proximo estado
        bool rmFuncTrans (QString, QString);
        // Le um automato de um arquivo
        bool readFromFile (QString);
        // Grava um automato em um arquivo
        bool writeToFile (QString);
        // Verificar consistencia interna da classe
        bool validaAutomato (void);
        // Acaba com tudo
        void clear (void);
};
#endif // __AUTOMATO_H
