#include <QLinkedList>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QRgb>
#include <QTimer>
#include <QWidget>
#include <cmath>
#include <cstdlib>
#include "defines.h"

enum imageTools {
    no_tool,
    pen_tool,
    line_tool,
    rectangle_tool,
    circle_tool,
    spray_tool,
    fill_tool
};

class imageContainer : public QWidget {
    Q_OBJECT;
    private:
        QImage* memoryImage;
        void resizeEvent (QResizeEvent*);
        void paintEvent (QPaintEvent*);
        void mousePressEvent (QMouseEvent*);
        void mouseMoveEvent (QMouseEvent*);
        void mouseReleaseEvent (QMouseEvent*);
        imageTools tool;
        int mpress_x, mpress_y;
        int mx, my;
        bool mouseIsDown;
        inline void adjustLimits (int&, int&);
        bool drawFigure (QPainter*);
        bool drawFigure (QPaintDevice*);
        QTimer* tSpray;
        void avaliaCor (QLinkedList<QPoint>*, QPoint, QRgb, QRgb);
    signals:
        void altered ();
    public:
        imageTools setTool (imageTools);
        QColor corCaneta;
        imageContainer (QWidget* = 0, Qt::WindowFlags = 0);
        void eraseAll ();
        bool readFromFile (QString);
        bool saveToFile (QString);
    public slots:
        void tSpray_timeout ();
};

void imageContainer::adjustLimits (int& x, int& y) {
    if (x < 0) {
        x = 0;
    }
    if (y < 0) {
        y = 0;
    }
    if (x >= this->width()) {
        x = this->width() - 1;
    }
    if (y >= this->height()) {
        y = this->height() - 1;
    }
}
