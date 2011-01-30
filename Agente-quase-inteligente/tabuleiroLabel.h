#ifndef __TABULEIROLABEL_H
#define __TABULEIROLABEL_H

#include <QtGui>
#include "acao.h"

class TabuleiroLabel : public QLabel {
    Q_OBJECT;
    private:
        bool _temMuro;
        bool _estaSelecionado;
        bool _estavaSelecionado;
        bool _final;
        bool _mostrandoUtilidade;
        int _politicaOtima;
        bool _mostraPoliticaOtima;
        double _recompensa;
        static bool _first;
        static TabuleiroLabel* _labelComAgente;
        static QPixmap* pixmapMuro;
        static QPixmap* pixmapVazio;
        static QPixmap* pixmapAgente;
        static QPixmap* pixmapEstadoFinal;
        static QPixmap* pixmapsPoliticas[4];
    public:
        static double minRecompensa;
        static double maxRecompensa;
        int x, y;
        double utilidade, utilidadeAnt;
        Acao prbNorte;
        Acao prbOeste;
        Acao prbLeste;
        Acao prbSul;
        inline double recompensa ();
        void setRecompensa (double);
        TabuleiroLabel (int, int, QWidget * = 0, Qt::WindowFlags = 0);
        ~TabuleiroLabel ();
        void seleciona ();
        void desseleciona ();
        inline bool estaSelecionado ();
        inline void salvaEstadoSelecionado ();
        inline bool estavaSelecionado ();
        void poeMuro ();
        void tiraMuro ();
        inline bool temMuro ();
        void poeAgente ();
        void tiraAgente ();
        inline bool temAgente ();
        static inline TabuleiroLabel* ondeEstaOAgente ();
        void redesenha ();
        inline bool ehEstadoFinal ();
        void setEstadoFinal (bool);
        void mostraUtilidade ();
        void ocultaUtilidade ();
        inline int politicaOtima ();
        void setPoliticaOtima (int);
        void mostraPoliticaOtima ();
        void ocultaPoliticaOtima ();
};

inline int TabuleiroLabel::politicaOtima () {
    return (this->_politicaOtima);
}

inline bool TabuleiroLabel::ehEstadoFinal () {
    return (this->_final);
}

inline double TabuleiroLabel::recompensa () {
    return (this->_recompensa);
}

inline void TabuleiroLabel::salvaEstadoSelecionado () {
    this->_estavaSelecionado = this->_estaSelecionado;
}

inline bool TabuleiroLabel::estavaSelecionado () {
    return (this->_estavaSelecionado);
}

inline TabuleiroLabel* TabuleiroLabel::ondeEstaOAgente () {
    return (TabuleiroLabel::_labelComAgente);
}

inline bool TabuleiroLabel::temAgente () {
    return (TabuleiroLabel::_labelComAgente == this);
}

inline bool TabuleiroLabel::temMuro () {
    return (this->_temMuro);
}

inline bool TabuleiroLabel::estaSelecionado () {
    return (this->_estaSelecionado);
}

#endif // __TABULEIROLABEL_H
