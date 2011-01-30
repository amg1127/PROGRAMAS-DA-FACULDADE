#ifndef __EDITAUTOMATO_H
#define __EDITAUTOMATO_H

#include "common.h"
#include "automato.h"
#include "segmreta.h"

class editautomato: public QWidget {
    Q_OBJECT;
    private:
        void desenhaSeta (QPainter&, QPoint, QPoint);
        void paintEvent (QPaintEvent *);
        void mouseMoveEvent (QMouseEvent *);
        void mousePressEvent (QMouseEvent *);
        void mouseReleaseEvent (QMouseEvent *);
        void mouseDoubleClickEvent (QMouseEvent *);
        QString estado_origem_selecionado;
        QString estado_destino_selecionado;
        bool movendoAlguem;
        bool criandoLink;
        bool jaSaiuArea;
        int procuraMaisProximo (QPoint, QVector<QString>, QString&);
        int procuraMaisProximo (QPoint, QVector<QString>, QString&, QString&);
        int x, y;
        int w, h;
    public:
        automato aut;
        editautomato (QWidget* = 0, Qt::WFlags = 0);
        void processaDEL();
        void processaCTRL_DEL();
        void processaCTRL_N();
        void processaCTRL_I();
        void processaCTRL_E();
        void processaCTRL_F();
        void processaCTRL_S();
        void processaCTRL_A();
        void processaCTRL_L();
};

#endif // __EDITAUTOMATO_H
