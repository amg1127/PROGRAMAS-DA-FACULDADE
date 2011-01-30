// paintbrush.h
#ifndef __PAINTBRUSH_H
#define __PAINTBRUSH_H

#include <QPixmap>
#include <QWidget>
#include <QPoint>
#include <QResizeEvent>
#include <QPaintEvent>
#include <QRect>

#include "common.h"

class paintbrush : public QWidget {
    Q_OBJECT;
    private:
        QPixmap pixNaTelaSelecionado;
        QPoint p_antes, p_desceu, p_agora;
        bool clicando;
        void resizeEvent (QResizeEvent *);
        void mousePressEvent (QMouseEvent *);
        void mouseReleaseEvent (QMouseEvent *);
        void mouseMoveEvent (QMouseEvent *);
        void paintEvent (QPaintEvent *);
        void redimensiona ();
        void desenhaRetangulo (bool);
        void acertaPontos ();
    public:
        QPixmap pixNaTelaOriginal;
        paintbrush ();
        void limpaImg ();
        void recarrega ();
    signals:
        void selecionouArea (QRect, bool);
};

#endif // __PAINTBRUSH_H
// paintbrush.h
