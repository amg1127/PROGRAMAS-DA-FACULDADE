#include "imageContainer.h"

imageContainer::imageContainer (QWidget* parent, Qt::WindowFlags f) : QWidget (parent, f) {
    int winicial, hinicial, x, y;
    QRgb corinicial;
    this->corCaneta.setRgb (255, 255, 255);
    corinicial = this->corCaneta.rgba ();
    this->setMaximumSize (MAXDIMENSION, MAXDIMENSION);
    winicial = 640;
    hinicial = 480;
    this->resize (winicial, hinicial);
    this->memoryImage = new QImage (winicial, hinicial, QImage::Format_ARGB32);
    for (x = 0; x < winicial; x++) {
        for (y = 0; y < hinicial; y++) {
            this->memoryImage->setPixel (x, y, corinicial);
        }
    }
    this->setAttribute (Qt::WA_NoSystemBackground, true);
    this->setAttribute (Qt::WA_OpaquePaintEvent, true);
    this->setAutoFillBackground (false);
    this->tool = no_tool;
    this->tSpray = new QTimer (this);
    this->tSpray->setSingleShot (false);
    this->tSpray->setInterval (INTERVALO_SPRAY);
    QObject::connect (this->tSpray, SIGNAL(timeout()), this, SLOT(tSpray_timeout()));
    this->mouseIsDown = false;
    this->show ();
}

void imageContainer::resizeEvent (QResizeEvent* ev) {
    int neww, newh, oldw, oldh, x, y, menorx;
    QRgb cor;
    cor = this->corCaneta.rgba ();
    oldw = this->memoryImage->width ();
    oldh = this->memoryImage->height ();
    neww = ev->size().width ();
    newh = ev->size().height ();
    (*this->memoryImage) = this->memoryImage->copy (0, 0, neww, newh);
    if (neww > oldw) {
        menorx = oldw;
    } else {
        menorx = neww;
    }
    for (x = 0; x < menorx; x++) {
        for (y = oldh; y < newh; y++) {
            this->memoryImage->setPixel (x, y, cor);
        }
    }
    for (x = oldw; x < neww; x++) {
        for (y = 0; y < newh; y++) {
            this->memoryImage->setPixel (x, y, cor);
        }
    }
}

void imageContainer::paintEvent (QPaintEvent* ev) {
    // Apenas para informar... Tentei desenhar a imagem diretamente na tela, mas nao deu... Ficou lento demais!
    QRect rt (ev->rect ());
    QPainter pintor (this);
    QColor cor;
    pintor.setRenderHints (QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform, false);
    pintor.drawImage (rt, (*this->memoryImage), rt);
    if (this->mouseIsDown) {
        (void) this->drawFigure (&pintor);
    }
}

void imageContainer::eraseAll () {
    int x, y;
    QRgb cor;
    cor = this->corCaneta.rgba ();
    for (x = this->memoryImage->width() - 1; x >= 0; x--) {
        for (y = this->memoryImage->height() - 1; y >= 0; y--) {
            this->memoryImage->setPixel (x, y, cor);
        }
    }
    this->update ();
}

bool imageContainer::readFromFile (QString fname) {
    QImage img(fname);
    if (! img.isNull ()) {
        if (img.width() <= MAXDIMENSION && img.height() <= MAXDIMENSION) {
            (*this->memoryImage) = img.convertToFormat (QImage::Format_ARGB32);
            this->resize (this->memoryImage->size ());
            this->update ();
            return (true);
        }
    }
    return (false);
}

bool imageContainer::saveToFile (QString fname) {
    return (this->memoryImage->save (fname, (char*) 0, 100));
}

imageTools imageContainer::setTool (imageTools i) {
    if (! this->mouseIsDown) {
        if (this->tool == i) {
            this->tool = no_tool;
        } else {
            this->tool = i;
        }
    }
    return (this->tool);
}

void imageContainer::mousePressEvent (QMouseEvent* ev) {
    QLinkedList<QPoint> pilha;
    QRgb corIn, corOut;
    if (ev->button () == Qt::LeftButton) {
        this->mx = this->mpress_x = ev->x ();
        this->my = this->mpress_y = ev->y ();
        this->adjustLimits (this->mx, this->my);
        this->adjustLimits (this->mpress_x, this->mpress_y);
        if (this->tool == fill_tool) {
            QPoint pd (this->mx, this->my);
            corIn = this->memoryImage->pixel (pd);
            corOut = this->corCaneta.rgba ();
            if (qRed (corIn) != qRed (corOut) || qGreen (corIn) != qGreen (corOut) || qBlue (corIn) != qBlue (corOut)) {
                pilha << pd;
                while (! pilha.isEmpty ()) {
                    pd = pilha.takeLast ();
                    this->memoryImage->setPixel (pd, corOut);
                    // Ponto superior
                    if (pd.y() > 0) {
                        this->avaliaCor (&pilha, QPoint (pd.x(), pd.y() - 1), corIn, corOut);
                    }
                    // Ponto esquerdo
                    if (pd.x() > 0) {
                        this->avaliaCor (&pilha, QPoint (pd.x() - 1, pd.y()), corIn, corOut);
                    }
                    // Ponto direito
                    if ((pd.x() + 1) < this->memoryImage->width ()) {
                        this->avaliaCor (&pilha, QPoint (pd.x() + 1, pd.y()), corIn, corOut);
                    }
                    // Ponto inferior
                    if ((pd.y() + 1) < this->memoryImage->height ()) {
                        this->avaliaCor (&pilha, QPoint (pd.x(), pd.y() + 1), corIn, corOut);
                    }
                }
                emit altered ();
            }
        } else if (this->tool != no_tool) {
            this->mouseIsDown = true;
            this->mouseMoveEvent (ev);
            if (this->tool == spray_tool) {
                this->tSpray_timeout ();
                this->tSpray->start ();
            }
        }
    }
}

void imageContainer::mouseMoveEvent (QMouseEvent* ev) {
    if (this->mouseIsDown) {
        this->mx = ev->x ();
        this->my = ev->y ();
        this->adjustLimits (this->mx, this->my);
        if (this->tool == pen_tool) {
            QPainter pintor (this->memoryImage);
            pintor.setBrush (Qt::NoBrush);
            pintor.setPen (this->corCaneta);
            pintor.drawLine (this->mx, this->my, this->mpress_x, this->mpress_y);
            this->mpress_x = this->mx;
            this->mpress_y = this->my;
            emit altered ();
        }
        this->update ();
    }
}

void imageContainer::mouseReleaseEvent (QMouseEvent* ev) {
    if (ev->button () == Qt::LeftButton) {
        this->mx = ev->x ();
        this->my = ev->y ();
        this->adjustLimits (this->mx, this->my);
        this->mouseMoveEvent (ev);
        if (this->drawFigure (this->memoryImage)) {
            emit altered ();
        }
        if (this->tool == spray_tool) {
            this->tSpray->stop ();
        }
        this->mouseIsDown = false;
        this->update ();
    }
}

bool imageContainer::drawFigure (QPainter* pintor) {
    int x, y, w, h, i;
    double angu, raio;
    pintor->setPen (this->corCaneta);
    pintor->setBrush (Qt::NoBrush);
    if (this->mouseIsDown) {
        if (this->tool == line_tool) {
            pintor->drawLine (this->mx, this->my, this->mpress_x, this->mpress_y);
            return (true);
        } else if (this->tool == rectangle_tool || this->tool == circle_tool) {
            if (this->mx < this->mpress_x) {
                x = this->mx;
                w = this->mpress_x - this->mx;
            } else {
                x = this->mpress_x;
                w = this->mx - this->mpress_x;
            }
            if (this->my < this->mpress_y) {
                y = this->my;
                h = this->mpress_y - this->my;
            } else {
                y = this->mpress_y;
                h = this->my - this->mpress_y;
            }
            if (this->tool == rectangle_tool) {
                pintor->drawRect (x, y, w, h);
            } else {
                pintor->drawEllipse (x, y, w, h);
            }
            return (true);
        } else if (this->tool == spray_tool) {
            for (i = 0; i < NUMPONTOS_SPRAY; i++) {
                angu = (double) 2 * M_PI * rand () / (1.0 + RAND_MAX);
                raio = RAIO_SPRAY * rand() / (1.0 + RAND_MAX);
                x = (int) round (this->mx + raio * cos (angu));
                y = (int) round (this->my + raio * sin (angu));
                pintor->drawPoint (x, y);
            }
        }
    }
    return (false);
}

bool imageContainer::drawFigure (QPaintDevice* dev) {
    QPainter pintor (dev);
    return (this->drawFigure (&pintor));
}

void imageContainer::tSpray_timeout () {
    this->drawFigure (this->memoryImage);
    this->update ();
    emit altered ();
}

void imageContainer::avaliaCor (QLinkedList<QPoint>* pilha, QPoint ponto, QRgb corIn, QRgb corOut) {
    QRgb corpon;
    int diffr, diffg, diffb, diff;
    corpon = this->memoryImage->pixel (ponto);
    if (qRed (corpon) != qRed (corOut) || qGreen (corpon) != qGreen (corOut) || qBlue (corpon) != qBlue (corOut)) {
        diffr = qRed (corpon) - qRed (corIn);
        if (diffr < 0) {
            diffr = -diffr;
        }
        diffg = qGreen (corpon) - qGreen (corIn);
        if (diffg < 0) {
            diffg = -diffg;
        }
        diffb = qBlue (corpon) - qBlue (corIn);
        if (diffb < 0) {
            diffb = -diffb;
        }
        diff = diffr + diffg + diffb;
        if (diff <= TOLERANCIA_PREENCHIMENTO) {
            (*pilha) << ponto;
        }
    }
}
