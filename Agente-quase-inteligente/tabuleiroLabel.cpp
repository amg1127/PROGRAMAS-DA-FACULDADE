#include "asserts.h"
#include "tabuleiroLabel.h"
#include "templates.h"

TabuleiroLabel* TabuleiroLabel::_labelComAgente = NULL;
QPixmap* TabuleiroLabel::pixmapMuro = NULL;
QPixmap* TabuleiroLabel::pixmapVazio = NULL;
QPixmap* TabuleiroLabel::pixmapAgente = NULL;
QPixmap* TabuleiroLabel::pixmapsPoliticas[4] = { NULL, NULL, NULL, NULL };
QPixmap* TabuleiroLabel::pixmapEstadoFinal = NULL;
double TabuleiroLabel::minRecompensa = 0;
double TabuleiroLabel::maxRecompensa = 0;
bool TabuleiroLabel::_first = true;

void TabuleiroLabel::tiraAgente () {
    if (TabuleiroLabel::_labelComAgente == this) {
        TabuleiroLabel::_labelComAgente = NULL;
        this->redesenha ();
    }
}

void TabuleiroLabel::poeAgente () {
    if (TabuleiroLabel::_labelComAgente != this) {
        if (TabuleiroLabel::_labelComAgente != NULL) {
            TabuleiroLabel::_labelComAgente->tiraAgente ();
        }
        TabuleiroLabel::_labelComAgente = this;
        this->_temMuro = false;
        this->_mostrandoUtilidade = false;
        this->_mostraPoliticaOtima = false;
        this->redesenha ();
    }
}

void TabuleiroLabel::tiraMuro () {
    if (this->_temMuro) {
        this->_temMuro = false;
        this->redesenha ();
    }
}

void TabuleiroLabel::poeMuro () {
    if (! this->_temMuro) {
        this->_final = false;
        this->_temMuro = true;
        this->_mostrandoUtilidade = false;
        this->_mostraPoliticaOtima = false;
        if (TabuleiroLabel::_labelComAgente == this) {
            TabuleiroLabel::_labelComAgente = NULL;
        }
        this->redesenha ();
    }
}

void TabuleiroLabel::desseleciona () {
    if (this->_estaSelecionado) {
        this->_estaSelecionado = false;
        this->redesenha ();
    }
}

void TabuleiroLabel::seleciona () {
    if (! this->_estaSelecionado) {
        this->_estaSelecionado = true;
        this->redesenha ();
    }
}

TabuleiroLabel::~TabuleiroLabel () {
    if (TabuleiroLabel::_labelComAgente == this) {
        TabuleiroLabel::_labelComAgente = NULL;
    }
}

TabuleiroLabel::TabuleiroLabel (int x, int y, QWidget * parent, Qt::WindowFlags f) : QLabel (parent, f) {
    QSizePolicy::Policy pllbls = QSizePolicy::Maximum;
    if (TabuleiroLabel::_first) {
        TabuleiroLabel::_first = false;
        TabuleiroLabel::pixmapMuro = new QPixmap (":/muro.png");
        TabuleiroLabel::pixmapVazio = new QPixmap (":/vazio.png");
        TabuleiroLabel::pixmapAgente = new QPixmap (":/boneco.png");
        TabuleiroLabel::pixmapsPoliticas[0] = new QPixmap (":/seta-cima.png");
        TabuleiroLabel::pixmapsPoliticas[1] = new QPixmap (":/seta-esquerda.png");
        TabuleiroLabel::pixmapsPoliticas[2] = new QPixmap (":/seta-direita.png");
        TabuleiroLabel::pixmapsPoliticas[3] = new QPixmap (":/seta-baixo.png");
        TabuleiroLabel::pixmapEstadoFinal = new QPixmap (":/estado-final.png");
    }
    this->prbNorte.setNorte (1);
    this->prbOeste.setOeste (1);
    this->prbLeste.setLeste (1);
    this->prbSul.setSul (1);
    this->_final = false;
    this->_recompensa = 0;
    this->utilidade = 0;
    this->utilidadeAnt = 0;
    this->_mostrandoUtilidade = false;
    this->_mostraPoliticaOtima = false;
    this->_politicaOtima = 0;
    this->x = x;
    this->y = y;
    this->_temMuro = false;
    this->_estaSelecionado = false;
    this->setAlignment (Qt::AlignCenter);
    this->setAcceptDrops (true);
    this->setFrameStyle (QFrame::WinPanel | QFrame::Sunken);
    this->setMargin (0);
    this->setSizePolicy (pllbls, pllbls);
    this->redesenha ();
    this->show ();
}

void TabuleiroLabel::setRecompensa (double v) {
    if (this->_recompensa != v) {
        this->_recompensa = v;
        this->redesenha ();
    }
}

void TabuleiroLabel::redesenha () {
    QColor minRecomp (0xFF, 0x40, 0x00, 0x20);
    QColor medioRecomp (0xFF, 0xFF, 0x00, 0x00);
    QColor maxRecomp (0x00, 0xC0, 0x00, 0x20);
    QColor desenhar;
    double proporcao;
    double meio;
    QPixmap r (TabuleiroLabel::pixmapVazio->size ());
    QRect tudo (r.rect());
    r.fill (Qt::transparent);
    QPainter pintor;
    if (TabuleiroLabel::minRecompensa >= TabuleiroLabel::maxRecompensa) {
        desenhar.setRgba (medioRecomp.rgba ());
    } else if (this->_recompensa < TabuleiroLabel::minRecompensa) {
        desenhar.setRgba (minRecomp.rgba ());
    } else if (this->_recompensa > TabuleiroLabel::maxRecompensa) {
        desenhar.setRgba (maxRecomp.rgba ());
    } else {
        meio = (TabuleiroLabel::minRecompensa + TabuleiroLabel::maxRecompensa) / 2;
        if (this->_recompensa > meio) {
            proporcao = (this->_recompensa - meio) / (TabuleiroLabel::maxRecompensa - meio);
            desenhar.setRed ((int) (medioRecomp.red () + (maxRecomp.red () - medioRecomp.red ()) * proporcao));
            desenhar.setGreen ((int) (medioRecomp.green () + (maxRecomp.green () - medioRecomp.green ()) * proporcao));
            desenhar.setBlue ((int) (medioRecomp.blue () + (maxRecomp.blue () - medioRecomp.blue ()) * proporcao));
            desenhar.setAlpha ((int) (medioRecomp.alpha () + (maxRecomp.alpha () - medioRecomp.alpha ()) * proporcao));
        } else {
            proporcao = (this->_recompensa - TabuleiroLabel::minRecompensa) / (meio - TabuleiroLabel::minRecompensa);
            desenhar.setRed ((int) (minRecomp.red () + (medioRecomp.red () - minRecomp.red ()) * proporcao));
            desenhar.setGreen ((int) (minRecomp.green () + (medioRecomp.green () - minRecomp.green ()) * proporcao));
            desenhar.setBlue ((int) (minRecomp.blue () + (medioRecomp.blue () - minRecomp.blue ()) * proporcao));
            desenhar.setAlpha ((int) (minRecomp.alpha () + (medioRecomp.alpha () - minRecomp.alpha ()) * proporcao));
        }
    }
    pintor.begin (&r);
    pintor.setPen (Qt::NoPen);
    pintor.setBrush (desenhar);
    pintor.drawRect (tudo);
    if (this->_final) {
        pintor.drawPixmap (tudo, *(TabuleiroLabel::pixmapEstadoFinal));
    }
    if (this->_temMuro) {
        pintor.drawPixmap (tudo, *(TabuleiroLabel::pixmapMuro));
    } else if (this->_mostrandoUtilidade || (this->_mostraPoliticaOtima && this->_final)) {
        pintor.setPen (QColor (0, 0, 0));
        pintor.drawText (tudo, Qt::TextSingleLine | Qt::AlignHCenter | Qt::AlignVCenter, QString::number (this->utilidade, 'f', 3));
        pintor.setPen (Qt::NoPen);
    } else if (this->_mostraPoliticaOtima) {
        pintor.drawPixmap (tudo, *(TabuleiroLabel::pixmapsPoliticas[this->_politicaOtima]));
    } else if (TabuleiroLabel::_labelComAgente == this) {
        pintor.drawPixmap (tudo, *(TabuleiroLabel::pixmapAgente));
    } else {
        pintor.drawPixmap (tudo, *(TabuleiroLabel::pixmapVazio));
    }
    if (this->_estaSelecionado) {
        pintor.setBrush (QColor (0x00, 0x00, 0x80, 0x20));
        pintor.drawRect (tudo);
    }
    pintor.end ();
    this->setPixmap (r);
    this->setToolTip (QString("( %1 , %2 )\nValor de recompensa: %3").arg(this->x).arg(this->y).arg(this->_recompensa));
}

void TabuleiroLabel::setEstadoFinal (bool f) {
    if (this->_final != f) {
        this->_final = f;
        if (f) {
            this->_temMuro = false;
        }
        this->redesenha ();
    }
}

void TabuleiroLabel::mostraUtilidade () {
    if (! this->_mostrandoUtilidade) {
        this->_mostrandoUtilidade = true;
        this->redesenha ();
    }
}

void TabuleiroLabel::ocultaUtilidade () {
    if (this->_mostrandoUtilidade) {
        this->_mostrandoUtilidade = false;
        this->redesenha ();
    }
}

void TabuleiroLabel::setPoliticaOtima (int v) {
    poeDentroDaFaixa (v, 0, 3);
    if (this->_politicaOtima != v) {
        this->_politicaOtima = v;
        this->redesenha ();
    }
}

void TabuleiroLabel::mostraPoliticaOtima () {
    if (! this->_mostraPoliticaOtima) {
        this->_mostraPoliticaOtima = true;
        this->redesenha ();
    }
}

void TabuleiroLabel::ocultaPoliticaOtima () {
    if (this->_mostraPoliticaOtima) {
        this->_mostraPoliticaOtima = false;
        this->redesenha ();
    }
}
