#include "asserts.h"
#include "agenteAnimado.h"
#include <cmath>

QList<QPixmap>* AgenteAnimado::_passos = NULL;
int AgenteAnimado::_maxsteps = 12;
int AgenteAnimado::_interval = 111;
double AgenteAnimado::_razIncr = 0.3;

double AgenteAnimado::conversion (double n, double mini, double maxi, double minf, double maxf) {
    return (minf + (n - mini) * (maxf - minf) / (maxi - mini));
}

AgenteAnimado::AgenteAnimado (QWidget* p, Qt::WindowFlags f) : QLabel (p, f) {
    if (AgenteAnimado::_passos == NULL) {
        int i;
        AgenteAnimado::_passos = new QList<QPixmap>;
        QPixmap p (":/boneco.png");
        for (i = 0; i < AgenteAnimado::_maxsteps; i++) {
            *(AgenteAnimado::_passos) << (p.scaled ((QSizeF (p.size()) * ((double) 1 + AgenteAnimado::_razIncr * sin (AgenteAnimado::conversion (i, 0, AgenteAnimado::_maxsteps - 1, 0, M_PI)))).toSize (), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }
    this->_timeout_start = 0;
    this->_timeout_end = 0;
}

void AgenteAnimado::centerMove (int x, int y) {
    this->adjustSize ();
    this->move (x - this->width () / 2, y - this->height () / 2);
}

void AgenteAnimado::animatedMoving (QPoint i, QPoint f, double direction, double dist, bool hide, int timeout_start, int timeout_end) {
    this->animatedMoving (i.x(), i.y(), f.x(), f.y(), direction, dist, hide, timeout_start, timeout_end);
}

void AgenteAnimado::animatedMoving (int xi, int yi, int xf, int yf, double direction, double dist, bool hide, int timeout_start, int timeout_end) {
    this->xi = xi;
    this->yi = yi;
    this->xf = xf;
    this->yf = yf;
    if (timeout_start >= 0) {
        this->_timeout_start = timeout_start;
    }
    if (timeout_end >= 0) {
        this->_timeout_end = timeout_end;
    }
    this->_hide = hide;
    this->_step = 0;
    this->_stop = false;
    this->_direction = direction;
    this->_dist = dist;
    this->position (false);
    this->show ();
    this->raise ();
    QTimer::singleShot (this->_timeout_start, this, SLOT(timer_timeout()));
}

void AgenteAnimado::position (bool emitsignal) {
    int x, y;
    double k;
    k = sin (AgenteAnimado::conversion (this->_step, 0, AgenteAnimado::_maxsteps - 1, -M_PI_2, M_PI_2));
    if (this->xi != this->xf || this->yi != this->yf) {
        x = (int) AgenteAnimado::conversion (k, -1, 1, this->xi, this->xf);
        y = (int) AgenteAnimado::conversion (k, -1, 1, this->yi, this->yf);
    } else {
        if (k > 0) {
            k = -k;
        }
        x = (int) AgenteAnimado::conversion (k, -1, 0, this->xi, this->xi + this->_dist * cos (this->_direction));
        y = (int) AgenteAnimado::conversion (k, -1, 0, this->yi, this->yi + this->_dist * (-sin (this->_direction)));
    }
    this->setPixmap (AgenteAnimado::_passos[0][this->_step]);
    this->centerMove (x, y);
    if (emitsignal) {
        emit stepAnimation ();
        emit stepAnimation (x, y);
        emit stepAnimation (QPoint (x, y));
    }
}

void AgenteAnimado::timer_timeout () {
    if (this->_stop) {
        this->animationIsAboutToEnd ();
    } else {
        this->position (true);
        this->_step++;
        if (this->_step >= AgenteAnimado::_maxsteps) {
            QTimer::singleShot (this->_timeout_end, this, SLOT(animationIsAboutToEnd()));
        } else {
            QTimer::singleShot (this->_interval, this, SLOT(timer_timeout()));
        }
    }
}

void AgenteAnimado::animationIsAboutToEnd () {
    if (this->_hide) {
        this->hide ();
    }
    emit endedAnimation ();
    emit endedAnimation ((int) this->xf, (int) this->yf);
    emit endedAnimation (QPoint ((int) this->xf, (int) this->yf));
}
