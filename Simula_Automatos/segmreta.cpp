#include "segmreta.h"

double segmreta::dist_pts () {
    double px = this->right() - this->left();
    double py = this->bottom() - this->top();
    return (sqrt (px*px + py*py));
}

double segmreta::cosang () {
    if (this->isValid()) {
        return ((this->right() - this->left()) / this->dist_pts());
    } else {
        return (0);
    }
}

double segmreta::sinang () {
    if (this->isValid()) {
        return ((this->bottom() - this->top()) / this->dist_pts());
    } else {
        return (0);
    }
}

segmreta::segmreta () : QRect() {
}

segmreta::segmreta (const QPoint& topLeft, const QPoint& bottomRight) : QRect (topLeft, bottomRight) {
}

segmreta::segmreta (const QPoint& topLeft, const QSize& size) : QRect (topLeft, size) {
}

segmreta::segmreta (int xp1, int yp1, int xp2, int yp2) : QRect (xp1, yp1, xp2, yp2) {
}

QPoint segmreta::deslocamento (int ds) {
    if (this->isValid()) {
        double px, py;
        if (ds >= 0) {
            px = ds * (this->right() - this->left()) / this->dist_pts() + this->left();
            py = ds * (this->bottom() - this->top()) / this->dist_pts() + this->top();
        } else {
            px = (-ds) * (this->left() - this->right()) / this->dist_pts() + this->right();
            py = (-ds) * (this->top() - this->bottom()) / this->dist_pts() + this->bottom();
        }
        return (QPoint ((int) rint(px), (int) rint(py)));
    } else {
        return (QPoint());
    }
}

bool segmreta::isValid () {
    return (this->topLeft() != this->bottomRight());
}

bool segmreta::isEmpty () {
    return (this->topLeft() == this->bottomRight());
}

bool segmreta::isNull () {
    return (this->topLeft() == this->bottomRight());
}

int segmreta::diagoLength () {
    return ((int) rint(this->dist_pts()));
}
