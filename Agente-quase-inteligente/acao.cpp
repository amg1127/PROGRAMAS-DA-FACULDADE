#include "asserts.h"
#include <cstdlib>
#include "acao.h"
#include "templates.h"

Acao::Acao () {
    this->probabs[0] = 0.25;
    this->probabs[1] = 0.25;
    this->probabs[2] = 0.25;
    this->probabs[3] = 0.25;
    this->criaOrdemAleatoria ();
}

Acao::Acao (double n, double o, double l, double s) {
    this->probabs[0] = n;
    this->probabs[1] = o;
    this->probabs[2] = l;
    this->probabs[3] = s;
    this->criaOrdemAleatoria ();
    this->balanco ();
}

Acao::Acao (double n, double o, double l, double s, int p0, int p1, int p2, int p3) {
    this->probabs[0] = n;
    this->probabs[1] = o;
    this->probabs[2] = l;
    this->probabs[3] = s;
    this->ordem_alteracao[0] = p0;
    this->ordem_alteracao[1] = p1;
    this->ordem_alteracao[2] = p2;
    this->ordem_alteracao[3] = p3;
    this->balanco ();
}

Acao::Acao (int p0, int p1, int p2, int p3) {
    this->probabs[0] = 0.25;
    this->probabs[1] = 0.25;
    this->probabs[2] = 0.25;
    this->probabs[3] = 0.25;
    this->ordem_alteracao[0] = p0;
    this->ordem_alteracao[1] = p1;
    this->ordem_alteracao[2] = p2;
    this->ordem_alteracao[3] = p3;
    this->balanco ();
}

Acao::Acao (Acao& outro) {
    (*this) = outro;
}

Acao& Acao::operator= (Acao& outro) {
    int i;
    for (i = 0; i < 4; i++) {
        this->probabs[i] = outro.probabs[i];
        this->ordem_alteracao[i] = outro.ordem_alteracao[i];
    }
    return (*this);
}

void Acao::balanco () {
    int i, k;
    int hits[4];
    double sum = 0;
    double diff, v;
    for (i = 0; i < 4; i++) {
        hits[i] = -1;
    }
    for (i = 0; i < 4; i++) {
        poeDentroDaFaixa (this->ordem_alteracao[i], 0, 3);
        if (hits[this->ordem_alteracao[i]] < 0) {
            hits[this->ordem_alteracao[i]] = i;
        } else {
            for (k = 0; k < 4; k++) {
                if (hits[k] < 0) {
                    hits[k] = i;
                    this->ordem_alteracao[i] = k;
                    break;
                }
            }
        }
        poeDentroDaFaixa (this->probabs[i], (double) 0, (double) 1);
        sum += this->probabs[i];
    }
    if (sum > 1) {
        diff = sum - 1;
        for (i = 3; i > 0 && diff > 0; i--) {
            v = this->probabs[ this->ordem_alteracao[i] ];
            diff -= v;
            if (diff > 0) {
                v = 0;
            } else {
                v = -diff;
            }
            this->probabs[ this->ordem_alteracao[i] ] = v;
        }
    } else if (sum < 1) {
        this->probabs[ this->ordem_alteracao[3] ] += 1 - sum;
    }
}

void Acao::criaOrdemAleatoria () {
    int chooses[4];
    int i, rnum;
    for (i = 0; i < 4; i++) {
        chooses[i] = i;
    }
    for (i = 3; i >= 0; i--) {
        rnum = (int) ((double) rand () * (i+1) / ((double) RAND_MAX + 1));
        this->ordem_alteracao[3 - i] = chooses[rnum];
        chooses[rnum] = chooses[i];
    }
}

void Acao::setProbab (int pos, double v) {
    poeDentroDaFaixa (pos, 0, 3);
    if (this->probabs[pos] != v) {
        this->probabs[pos] = v;
        this->maximizaPrioridade (pos);
        this->balanco ();
    }
}

void Acao::setProbabs (double n, double o, double l, double s) {
    this->probabs[0] = n;
    this->probabs[1] = o;
    this->probabs[2] = l;
    this->probabs[3] = s;
    this->balanco ();
}

void Acao::prioridades (int& p0, int& p1, int& p2, int& p3) {
    p0 = this->ordem_alteracao[0];
    p1 = this->ordem_alteracao[1];
    p2 = this->ordem_alteracao[2];
    p3 = this->ordem_alteracao[3];
}

void Acao::setPrioridades (int p0, int p1, int p2, int p3) {
    this->ordem_alteracao[0] = p0;
    this->ordem_alteracao[1] = p1;
    this->ordem_alteracao[2] = p2;
    this->ordem_alteracao[3] = p3;
    this->balanco ();
}

void Acao::maximizaPrioridade (int pos) {
    int i;
    for (i = 0; i < 3; i++) {
        if (this->ordem_alteracao[i] == pos) {
            break;
        }
    }
    while (i > 0) {
        this->ordem_alteracao[i] = this->ordem_alteracao[i-1];
        i--;
    }
    this->ordem_alteracao[0] = pos;
}
