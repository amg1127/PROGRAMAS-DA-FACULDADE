#ifndef __SEGMRETA_H
#define __SEGMRETA_H

#include "common.h"

class segmreta;

class segmreta : public QRect {
    private:
        double dist_pts ();
    public:
        segmreta ();
        segmreta (const QPoint&, const QPoint&);
        segmreta (const QPoint&, const QSize&);
        segmreta (int, int, int, int);
        QPoint deslocamento (int);
        double cosang ();
        double sinang ();
        bool isValid ();
        bool isEmpty ();
        bool isNull ();
        int diagoLength ();
};
#endif // __SEGMRETA_H
