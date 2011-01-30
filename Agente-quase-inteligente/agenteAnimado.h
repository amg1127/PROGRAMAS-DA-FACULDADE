#ifndef __AGENTEANIMADO_H
#define __AGENTEANIMADO_H

#include <QtGui>

class AgenteAnimado : public QLabel {
    Q_OBJECT;
    private:
        double xi, yi;
        double xf, yf;
        bool _hide;
        int _step;
        double _direction;
        double _dist;
        int _timeout_start;
        int _timeout_end;
        static QList<QPixmap>* _passos;
        static int _maxsteps;
        static int _interval;
        static double _razIncr;
        bool _stop;
        void centerMove (int, int);
        static double conversion (double, double, double, double, double);
        void position (bool);
    public:
        AgenteAnimado (QWidget* = 0, Qt::WindowFlags = 0);
        void animatedMoving (QPoint, QPoint, double, double, bool = true, int = -1, int = -1);
        void animatedMoving (int, int, int, int, double, double, bool = true, int = -1, int = -1);
        inline void stopAnimation ();
    signals:
        void endedAnimation ();
        void endedAnimation (QPoint);
        void endedAnimation (int, int);
        void stepAnimation ();
        void stepAnimation (QPoint);
        void stepAnimation (int, int);
    public slots:
        void timer_timeout ();
        void animationIsAboutToEnd ();
};

inline void AgenteAnimado::stopAnimation () {
    this->_stop = true;
}

#endif // __AGENTEANIMADO_H
