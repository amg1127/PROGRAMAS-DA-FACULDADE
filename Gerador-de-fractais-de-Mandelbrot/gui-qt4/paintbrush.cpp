// paintbrush.cpp
#include "paintbrush.h"

paintbrush::paintbrush () {
    this->pixNaTelaSelecionado = QPixmap (this->width(), this->height ());
    this->pixNaTelaOriginal = QPixmap (this->width(), this->height ());
    this->setMinimumSize (1, 1);
    this->clicando = false;
}

void paintbrush::redimensiona () {
    this->acertaPontos ();
    this->pixNaTelaOriginal = this->pixNaTelaOriginal.scaled (this->width(), this->height());
    this->pixNaTelaSelecionado = this->pixNaTelaOriginal;
    this->clicando = false;
    this->update ();
}

void paintbrush::desenhaRetangulo (bool param) {
    this->acertaPontos ();
    QRect r (this->p_desceu, this->p_agora);
    r = r.normalized ();
    this->limpaImg ();
    if (r.width() > 2 && r.height() > 2) {
        {
            QPainter b (&(this->pixNaTelaSelecionado));
            b.setPen (QColor (0, 0, 0));
            // Droga... Tem um bug no metodo drawRect()
            b.drawLine (r.topLeft(), r.topRight());
            b.drawLine (r.bottomLeft(), r.bottomRight());
            b.drawLine (r.topLeft(), r.bottomLeft());
            b.drawLine (r.topRight(), r.bottomRight());
        }
        this->update (r.adjusted (-1, -1, 1, 1));
        this->p_antes = this->p_agora;
    }
    emit selecionouArea (r.adjusted (1, 1, -1, -1), param);
}

void paintbrush::resizeEvent (QResizeEvent *ev) {
    this->redimensiona ();
    ev->accept ();
}

void paintbrush::mousePressEvent (QMouseEvent *ev) {
    ev->accept ();
    if (this->clicando || ev->button() != Qt::LeftButton) return;
    this->limpaImg ();
    this->clicando = true;
    this->p_desceu = ev->pos();
    this->p_agora = ev->pos();
}

void paintbrush::mouseReleaseEvent (QMouseEvent *ev) {
    this->p_agora = ev->pos();
    if (this->clicando && ev->button() == Qt::LeftButton) {
        this->clicando = false;
        this->desenhaRetangulo (true);
    }
    ev->accept ();
}

void paintbrush::mouseMoveEvent (QMouseEvent *ev) {
    this->p_agora = ev->pos();
    if (this->clicando) {
        this->desenhaRetangulo (false);
    }
    ev->accept ();
}

void paintbrush::recarrega () {
    this->pixNaTelaSelecionado = this->pixNaTelaOriginal;
}

void paintbrush::limpaImg () {
    this->acertaPontos ();
    QRect r (this->p_desceu, this->p_antes);
    r = r.normalized ().adjusted (-2, -2, 2, 2);
    {
        QPainter b (&(this->pixNaTelaSelecionado));
        b.drawPixmap (r.topLeft(), this->pixNaTelaOriginal, r);
    }
    this->update (r);
}

void paintbrush::paintEvent (QPaintEvent *ev) {
    QPainter p(this);
    p.drawPixmap (ev->rect().topLeft(), this->pixNaTelaSelecionado, ev->rect());
    ev->accept ();
}

void paintbrush::acertaPontos () {
    int nl, na;
    nl = this->width() - 1;
    na = this->height() - 1;
    if (this->p_antes.x() > nl)
        this->p_antes.setX (nl);
    else if (this->p_antes.x() < 0)
        this->p_antes.setX (0);
    if (this->p_antes.y() > na)
        this->p_antes.setY (na);
    else if (this->p_antes.y() < 0)
        this->p_antes.setY (0);
    if (this->p_agora.x() > nl)
        this->p_agora.setX (nl);
    else if (this->p_agora.x() < 0)
        this->p_agora.setX (0);
    if (this->p_agora.y() > na)
        this->p_agora.setY (na);
    else if (this->p_agora.y() < 0)
        this->p_agora.setY (0);
    if (this->p_desceu.x() > nl)
        this->p_desceu.setX (nl);
    else if (this->p_desceu.x() < 0)
        this->p_desceu.setX (0);
    if (this->p_desceu.y() > na)
        this->p_desceu.setY (na);
    else if (this->p_desceu.y() < 0)
        this->p_desceu.setY (0);
}
// paintbrush.cpp
