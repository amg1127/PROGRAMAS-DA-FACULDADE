#ifndef __ACOES_H
#define __ACOES_H

class Acao {
    private:
        double probabs[4];
        int ordem_alteracao[4];
        void balanco ();
        void criaOrdemAleatoria ();
    public:
        Acao ();
        Acao (double, double, double, double);
        Acao (double, double, double, double, int, int, int, int);
        Acao (int, int, int, int);
        Acao (Acao&);
        Acao& operator= (Acao&);
        inline double probab (int);
        inline double norte ();
        inline double oeste ();
        inline double leste ();
        inline double sul ();
        void setProbab (int, double);
        inline void setNorte (double);
        inline void setOeste (double);
        inline void setLeste (double);
        inline void setSul (double);
        void setProbabs (double, double, double, double);
        inline int prioridade (int);
        void maximizaPrioridade (int);
        void prioridades (int&, int&, int&, int&);
        void setPrioridades (int, int, int, int);
};

inline double Acao::probab (int i) {
    return (this->probabs[i]);
}

inline double Acao::norte () {
    return (this->probabs[0]);
}

inline double Acao::oeste () {
    return (this->probabs[1]);
}

inline double Acao::leste () {
    return (this->probabs[2]);
}

inline double Acao::sul () {
    return (this->probabs[3]);
}

inline void Acao::setNorte (double n) {
    this->setProbab (0, n);
}

inline void Acao::setOeste (double n) {
    this->setProbab (1, n);
}

inline void Acao::setLeste (double n) {
    this->setProbab (2, n);
}

inline void Acao::setSul (double n) {
    this->setProbab (3, n);
}

inline int Acao::prioridade (int n) {
    return (this->ordem_alteracao[n]);
}
#endif // __ACOES_H
