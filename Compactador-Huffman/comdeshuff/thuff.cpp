#include "common.h"

THuff::THuff () {
    NEW_ARRAY_OF_OBJECTS (this->vetPrioridades, int, 1);
    NEW_ARRAY_OF_OBJECTS (this->vetCaracteres, char, 1);
    NEW_ARRAY_OF_OBJECTS (this->vetCodificacoes, std::string, 1);
    this->recalculaCodificacao = false;
    this->tamvetores = 0;
}

THuff::~THuff () {
    DEL_ARRAY_OF_OBJECTS (this->vetPrioridades);
    DEL_ARRAY_OF_OBJECTS (this->vetCaracteres);
    DEL_ARRAY_OF_OBJECTS (this->vetCodificacoes);
}

void THuff::redimensionaVetores (int novotamanho) {
    if (novotamanho < 0) return;
    if (this->tamvetores < novotamanho) {
        char *vtchar;
        int *vtint;
        std::string *vtstr;
        int i;
        NEW_ARRAY_OF_OBJECTS (vtint, int, novotamanho);
        NEW_ARRAY_OF_OBJECTS (vtchar, char, novotamanho);
        NEW_ARRAY_OF_OBJECTS (vtstr, std::string, novotamanho);
        for (i=0; i<this->tamvetores; i++) {
            vtint[i] = this->vetPrioridades[i];
            vtchar[i] = this->vetCaracteres[i];
            vtstr[i] = this->vetCodificacoes[i];
        }
        DEL_ARRAY_OF_OBJECTS (this->vetCaracteres);
        DEL_ARRAY_OF_OBJECTS (this->vetPrioridades);
        DEL_ARRAY_OF_OBJECTS (this->vetCodificacoes);
        this->vetPrioridades = vtint;
        this->vetCaracteres = vtchar;
        this->vetCodificacoes = vtstr;
    }
    this->tamvetores = novotamanho;
}

void THuff::calculaCodificacao (void) {
    int i, p;
    TCell *arvoreHuff, tmp0, tmp1, *tmp;
    if (this->tamvetores == 0) return;
    NEW_ARRAY_OF_OBJECTS (arvoreHuff, TCell, this->tamvetores);
    for (i=0; i<this->tamvetores; i++) {
        arvoreHuff[i].setCharacter (this->vetCaracteres[i]);
        arvoreHuff[i].setWeight (this->vetPrioridades[i]);
    }
    p = 0;
    while (true) {
        sortCellInterval (arvoreHuff, p, this->tamvetores);
        if (++p == this->tamvetores) break;
        tmp0.copyFrom (arvoreHuff[p-1]);
        tmp1.copyFrom (arvoreHuff[p]);
        arvoreHuff[p].makeSons();
        arvoreHuff[p].son0()->copyFrom (tmp0);
        arvoreHuff[p].son1()->copyFrom (tmp1);
        arvoreHuff[p].setWeight (tmp0.weight() + tmp1.weight());
        arvoreHuff[p].setCharacter (0);
    }
    for (i=0; i<this->tamvetores; i++) {
        tmp = arvoreHuff[p-1].findSonByCharacter (this->vetCaracteres[i]);
        assert (tmp != NULL);
        this->vetCodificacoes[i] = arvoreHuff[p-1].pathToSon (tmp);
        assert (this->vetCodificacoes[i].length() > 1);
        this->vetCodificacoes[i].resize (this->vetCodificacoes[i].length() - 1);
    }
    DEL_ARRAY_OF_OBJECTS (arvoreHuff);
    this->recalculaCodificacao = false;
}

void THuff::sortCellInterval (TCell *vetCell, int beginCell, int endCell) {
    int i;
    bool troca;
    do {
        troca = false;
        for (i=beginCell+1; i<endCell; i++)
            if (vetCell[i-1].weight() > vetCell[i].weight()) {
                troca = true;
                vetCell[i-1].swap (vetCell[i]);
            }
    } while (troca);
}

void THuff::adicionaCaractere (char ch, int prioridade) {
    int i;
    if (ch == 0) return;
    for (i=0; i<this->tamvetores; i++) {
        if (this->vetCaracteres[i] == ch) {
            if (this->vetPrioridades[i] != prioridade) {
                this->recalculaCodificacao = true;
                this->vetPrioridades[i] = prioridade;
            }
            return;
        }
    }
    i = this->tamvetores;
    this->redimensionaVetores (i+1);
    this->vetCaracteres[i] = ch;
    this->vetPrioridades[i] = prioridade;
    this->recalculaCodificacao = true;
}

std::string THuff::obtemCodificacao (char ch) {
    int i;
    if (this->recalculaCodificacao)
        this->calculaCodificacao();
    for (i=0; i<this->tamvetores; i++)
        if (this->vetCaracteres[i] == ch)
            return (this->vetCodificacoes[i]);
    return (std::string (""));
}

int THuff::obtemPrioridade (char ch) {
    int i;
    for (i=0; i<this->tamvetores; i++)
        if (this->vetCaracteres[i] == ch)
            return (this->vetPrioridades[i]);
    return (-1);
}

char THuff::obtemCaractere (std::string cod) {
    int i;
    if (this->recalculaCodificacao)
        this->calculaCodificacao();
    for (i=0; i<this->tamvetores; i++)
        if (this->vetCodificacoes[i] == cod)
            return (this->vetCaracteres[i]);
    return (0);
}

void THuff::removeCaractere (char ch) {
    int i;
    for (i=0; i<this->tamvetores; i++)
        if (this->vetCaracteres[i] == ch) {
            this->recalculaCodificacao = true;
            if (i+1 < this->tamvetores) {
                this->vetPrioridades[i] = this->vetPrioridades[this->tamvetores-1];
                this->vetCaracteres[i] = this->vetCaracteres[this->tamvetores-1];
                this->vetCodificacoes[i] = this->vetCodificacoes[this->tamvetores-1];
            }
            this->redimensionaVetores (this->tamvetores - 1);
            break;
        }
}
