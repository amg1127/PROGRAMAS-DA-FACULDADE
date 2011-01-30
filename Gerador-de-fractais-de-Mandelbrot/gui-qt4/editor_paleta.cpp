// editor_paleta.cpp
#include "editor_paleta.h"

void editor_paleta::btnSalvaArquivo_clicked () {
    static QString arquivo_salvar;
    QString novo;
    int i, l, e;
    char chr, chg, chb;
    int ichr, ichg, ichb;
    QColor el_color;
    l = this->mostrador->count ();
    if (l == 0) {
        QMessageBox::warning (this, this->windowTitle () + " - Erro", "Voce nao pode salvar uma paleta de cores vazia!", QMessageBox::Ok, QMessageBox::NoButton);
    } else {
        novo = QFileDialog::getSaveFileName (this, this->windowTitle() + " - Salvar como...", arquivo_salvar, QString(), 0, 0);
        if (novo.isEmpty()) return;
        arquivo_salvar = novo;
        QFile arq (arquivo_salvar);
        if (arq.open (QIODevice::WriteOnly)) {
            e = 0;
            for (i = 0; i < l; i++) {
                el_color = this->mostrador->item(i)->backgroundColor();
                ichr = el_color.red ();
                ichg = el_color.green ();
                ichb = el_color.blue ();
                if (ichr > 127)
                    ichr -= 256;
                if (ichg > 127)
                    ichg -= 256;
                if (ichb > 127)
                    ichb -= 256;
                chr = ichr;
                chg = ichg;
                chb = ichb;
                if (! arq.putChar (chr)) {
                    e = 1;
                } else if (! arq.putChar (chg)) {
                    e = 1;
                } else if (! arq.putChar (chb)) {
                    e = 1;
                }
                if (e) {
                    QMessageBox::warning (this, this->windowTitle () + " - Erro", QString("O arquivo nao pode ser gravado:\n\n\"") + arq.errorString() + "\"", QMessageBox::Ok, QMessageBox::NoButton);
                    break;
                }
            }
            arq.close ();
        } else {
            QMessageBox::warning (this, this->windowTitle () + " - Erro", QString("O arquivo nao pode ser salvo:\n\n\"") + arq.errorString() + "\"", QMessageBox::Ok, QMessageBox::NoButton);
        }
    }
}

void editor_paleta::btnAbreArquivo_clicked () {
    static QString arquivo_abrir;
    QString ab;
    QListWidgetItem *m;
    int i, e;
    char chr, chg, chb;
    int ichr, ichg, ichb;
    ab = QFileDialog::getOpenFileName (this, this->windowTitle() + " - Abrir...", arquivo_abrir, QString(), 0, 0);
    if (ab.isEmpty()) return;
    arquivo_abrir = ab;
    QFile arq (arquivo_abrir);
    if (arq.open (QIODevice::ReadOnly)) {
        this->lop = true;
        this->mostrador->clear ();
        e = 0;
        for (i = 0; ! arq.atEnd (); i++) {
            chg = 0;
            chb = 0;
            if (! arq.getChar (&chr)) {
                e = 1;
            } else if (! arq.atEnd ()) {
                if (! arq.getChar (&chg)) {
                    e = 1;
                } else if (! arq.atEnd ()) {
                    if (! arq.getChar (&chb)) {
                        e = 1;
                    }
                }
            }
            if (e) {
                QMessageBox::warning (this, this->windowTitle () + " - Erro", QString("O arquivo nao pode ser lido:\n\n\"") + arq.errorString() + "\"", QMessageBox::Ok, QMessageBox::NoButton);
                break;
            }
            ichr = chr;
            ichg = chg;
            ichb = chb;
            if (ichr < 0)
                ichr += 256;
            if (ichg < 0)
                ichg += 256;
            if (ichb < 0)
                ichb += 256;
            NOVO_OBJETO (m, QListWidgetItem (""));
            this->mostrador->insertItem (i, m);
            m->setBackgroundColor (QColor (ichr, ichg, ichb));
            m->setTextAlignment (Qt::AlignCenter);
            if ((ichr + ichg + ichb) > 384) {
                m->setTextColor (QColor (0, 0, 0));
            } else {
                m->setTextColor (QColor (255, 255, 255));
            }
        }
        arq.close ();
        this->lop = false;
    } else {
        QMessageBox::warning (this, this->windowTitle () + " - Erro", QString("O arquivo nao pode ser aberto:\n\n\"") + arq.errorString() + "\"", QMessageBox::Ok, QMessageBox::NoButton);
    }
}

void editor_paleta::btnAdicionar_clicked () {
    QListWidgetItem *m;
    QColor novaCor (0, 0, 0);
    int pos = this->oCaraSelecionado (m) + 1;
    novaCor = QColorDialog::getColor (novaCor, this);
    if (novaCor.isValid()) {
        if (pos)
            m->setText ("");
        NOVO_OBJETO (m, QListWidgetItem (defotexto));
        this->mostrador->insertItem (pos, m);
        m->setBackgroundColor (novaCor);
        m->setTextAlignment (Qt::AlignCenter);
        if ((novaCor.red() + novaCor.green() + novaCor.blue()) > 384) {
            m->setTextColor (QColor (0, 0, 0));
        } else {
            m->setTextColor (QColor (255, 255, 255));
        }
    }
}

void editor_paleta::btnEditar_clicked () {
    QListWidgetItem *m;
    QColor novaCor (0, 0, 0);
    int pos = this->oCaraSelecionado (m);
    if (pos < 0) {
        QMessageBox::warning (this, this->windowTitle () + " - Erro", "Voce deve selecionar uma cor para editar!", QMessageBox::Ok, QMessageBox::NoButton);
    } else {
        novaCor = QColorDialog::getColor (m->backgroundColor (), this);
        if (novaCor.isValid()) {
            m->setBackgroundColor (novaCor);
            if ((novaCor.red() + novaCor.green() + novaCor.blue()) > 384) {
                m->setTextColor (QColor (0, 0, 0));
            } else {
                m->setTextColor (QColor (255, 255, 255));
            }
        }
    }
}

void editor_paleta::btnRemover_clicked () {
    QListWidgetItem *m;
    int pos = this->oCaraSelecionado (m);
    int lM = this->mostrador->count () - 1;
    if (pos < 0) {
        QMessageBox::warning (this, this->windowTitle () + " - Erro", "Voce deve selecionar uma cor para remover!", QMessageBox::Ok, QMessageBox::NoButton);
    } else {
        this->mostrador->takeItem (pos);
        APAGA_OBJETO (m);
        if (pos < lM) {
            this->mostrador_aconteceuAlgumaCoisa (this->mostrador->item (pos));
        } else if (pos > 0) {
            this->mostrador_aconteceuAlgumaCoisa (this->mostrador->item (pos - 1));
        }
    }
}

void editor_paleta::btnLimpar_clicked () {
    this->mostrador->clear ();
}

void editor_paleta::btnParaBaixo_clicked () {
    QListWidgetItem *m;
    int pos = this->oCaraSelecionado (m);
    int lM = this->mostrador->count ();
    if (pos < 0) {
        QMessageBox::warning (this, this->windowTitle () + " - Erro", "Voce deve selecionar uma cor para mover!", QMessageBox::Ok, QMessageBox::NoButton);
    } else if (pos + 1 < lM) {
        m = this->mostrador->takeItem (pos + 1);
        this->mostrador->insertItem (pos, m);
        this->mostrador_aconteceuAlgumaCoisa (this->mostrador->item (pos + 1));
    }
}

void editor_paleta::btnParaCima_clicked () {
    QListWidgetItem *m;
    int pos = this->oCaraSelecionado (m);
    if (pos < 0) {
        QMessageBox::warning (this, this->windowTitle () + " - Erro", "Voce deve selecionar uma cor para mover!", QMessageBox::Ok, QMessageBox::NoButton);
    } else if (pos > 0) {
        m = this->mostrador->takeItem (pos);
        this->mostrador->insertItem (pos - 1, m);
        this->mostrador_aconteceuAlgumaCoisa (this->mostrador->item (pos - 1));
    }
}

void editor_paleta::btnEscolhaAleatoria_clicked () {
    bool ok;
    int i, pos, kx;
    static int quantas = 1;
    QListWidgetItem *m;
    kx = QInputDialog::getInteger (this,
                                            this->windowTitle(), 
                                            "Informe o numero de cores que sera gerado aleatoriamente:",
                                            quantas,
                                            1,
                                            maxint,
                                            1,
                                            &ok);
    if (!ok) return;
    quantas = kx;
    pos = this->oCaraSelecionado (m) + 1;
    this->lop = true;
    for (i = 0; i < quantas; i++) {
        QColor novaCor ((int) (256.0 * rand () / (1.0 + RAND_MAX)), (int) (256.0 * rand () / (1.0 + RAND_MAX)), (int) (256.0 * rand () / (1.0 + RAND_MAX)));
        NOVO_OBJETO (m, QListWidgetItem (QString("")));
        this->mostrador->insertItem (pos, m);
        m->setBackgroundColor (novaCor);
        m->setTextAlignment (Qt::AlignCenter);
        if ((novaCor.red() + novaCor.green() + novaCor.blue()) > 384) {
            m->setTextColor (QColor (0, 0, 0));
        } else {
            m->setTextColor (QColor (255, 255, 255));
        }
    }
    this->lop = false;
    if (pos) {
        this->mostrador_aconteceuAlgumaCoisa (this->mostrador->item (pos - 1));
    } else {
        this->mostrador_aconteceuAlgumaCoisa (this->mostrador->item (0));
    }
}

void editor_paleta::btnAdicionarGradiente_clicked () {
    bool ok;
    int i, pos, redInicio, redFim, redDiff, greenInicio, greenFim, greenDiff, blueInicio, blueFim, blueDiff, k, kx;
    static int quantas = 1;
    QListWidgetItem *m;
    QColor corInicio(0, 0, 0), corFim;
    pos = this->oCaraSelecionado (m) + 1;
    if (pos)
        corInicio = m->backgroundColor ();
    kx = QInputDialog::getInteger (this,
                                            this->windowTitle(), 
                                            "Informe agora o numero de cores do gradiente.\n\nNas proximas 2 caixas de dialogo, voce podera especificar o intervalo de cor.",
                                            quantas,
                                            1,
                                            maxint,
                                            1,
                                            &ok);
    if (!ok) return;
    quantas = kx;
    if (quantas == 1) {
        this->btnAdicionar_clicked ();
        return;
    }
    corInicio = QColorDialog::getColor (corInicio, this);
    if (! corInicio.isValid ()) return;
    corFim = QColorDialog::getColor (corInicio, this);
    if (! corFim.isValid ()) return;
    corFim.getRgb (&redInicio, &greenInicio, &blueInicio);
    corInicio.getRgb (&redFim, &greenFim, &blueFim);
    redDiff = redFim - redInicio;
    greenDiff = greenFim - greenInicio;
    blueDiff = blueFim - blueInicio;
    k = quantas - 1;
    this->lop = true;
    for (i = 0; i < quantas; i++) {
        QColor novaCor (redInicio + redDiff * i / k, greenInicio + greenDiff * i / k, blueInicio + blueDiff * i / k);
        NOVO_OBJETO (m, QListWidgetItem (QString("")));
        this->mostrador->insertItem (pos, m);
        m->setBackgroundColor (novaCor);
        m->setTextAlignment (Qt::AlignCenter);
        if ((novaCor.red() + novaCor.green() + novaCor.blue()) > 384) {
            m->setTextColor (QColor (0, 0, 0));
        } else {
            m->setTextColor (QColor (255, 255, 255));
        }
    }
    this->lop = false;
    if (pos) {
        this->mostrador_aconteceuAlgumaCoisa (this->mostrador->item (pos - 1));
    }
}

void editor_paleta::btnOk_clicked () {
    int i, lM;
    lM = this->mostrador->count ();
    if (lM) {
        if (! this->p->mexeuNaPaleta) {
            this->p->mexeuNaPaleta = (this->mostrador->count () != this->p->paletaDeCoresReal->count ());
        }
        if (! this->p->mexeuNaPaleta) {
            for (i=0; i < lM; i++) {
                if (this->mostrador->item(i)->backgroundColor () != this->p->paletaDeCoresReal->item(i)->backgroundColor ()) {
                    this->p->mexeuNaPaleta = true;
                    break;
                }
            }
        }
        if (this->p->mexeuNaPaleta) {
            this->p->paletaDeCoresReal->clear ();
            for (i=0; i < lM; i++) {
                this->p->paletaDeCoresReal->addItem (this->mostrador->item(i)->clone ());
            }
        }
        this->close ();
    } else {
        QMessageBox::warning (this, this->windowTitle () + " - Erro", "A paleta de cores esta vazia. Pelo menos 1 cor deve ser adicionada na paleta, antes de continuar.!", QMessageBox::Ok, QMessageBox::NoButton);
    }
}

void editor_paleta::btnCancelar_clicked () {
    this->close ();
}


int editor_paleta::oCaraSelecionado (QListWidgetItem *&l) {
    QListWidgetItem *selecionado;
    int i, lM;
    l = NULL;
    lM = this->mostrador->count();
    for (i = lM - 1; i >= 0; i--) {
        selecionado = this->mostrador->item(i);
        if (selecionado->text() != "") {
            l = selecionado;
            break;
        }
    }
    return (i);
}

void editor_paleta::mostrador_aconteceuAlgumaCoisa (QListWidgetItem *m2) {
    int i, lM;
    QListWidgetItem *m;
    if (this->lop) return;
    this->lop = true;
    lM = this->mostrador->count ();
    for (i=0; i < lM; i++) {
        m = this->mostrador->item(i);
        m->setText ("");
    }
    if (m2 != NULL)
        m2->setText (defotexto);
    this->lop = false;
}

editor_paleta::editor_paleta (configuracao *l) : QDialog (l, Qt::Dialog) {
    int tmpw, tmph, i;

    this->lop = false;

    this->setAttribute (Qt::WA_ShowModal, true);
    this->setWindowTitle ("Configurar paleta de cores");
    this->p = l;

    NOVO_OBJETO (this->grupo, QButtonGroup (this));

    NOVO_OBJETO (this->mostrador, QListWidget (this));
    this->mostrador->setSelectionMode (QAbstractItemView::NoSelection);
    this->loadConfig ();

    NOVO_OBJETO (this->btnAdicionar, QPushButton (this));
    this->btnAdicionar->setText ("Adicionar...");

    NOVO_OBJETO (this->btnEditar, QPushButton (this));
    this->btnEditar->setText ("Editar...");

    NOVO_OBJETO (this->btnRemover, QPushButton (this));
    this->btnRemover->setText ("Remover");

    NOVO_OBJETO (this->btnParaCima, QPushButton (this));
    this->btnParaCima->setText ("Mover a cor para cima");

    NOVO_OBJETO (this->btnParaBaixo, QPushButton (this));
    this->btnParaBaixo->setText ("Mover a cor para baixo");

    NOVO_OBJETO (this->btnEscolhaAleatoria, QPushButton (this));
    this->btnEscolhaAleatoria->setText ("Adicionar cores aleatorias...");

    NOVO_OBJETO (this->btnAdicionarGradiente, QPushButton (this));
    this->btnAdicionarGradiente->setText ("Adicionar um gradiente de cores...");

    NOVO_OBJETO (this->btnLimpar, QPushButton (this));
    this->btnLimpar->setText ("Limpar esta paleta");

    NOVO_OBJETO (this->btnAbreArquivo, QPushButton (this));
    this->btnAbreArquivo->setText ("Carregar uma paleta de um arquivo...");

    NOVO_OBJETO (this->btnSalvaArquivo, QPushButton (this));
    this->btnSalvaArquivo->setText ("Salvar a paleta em um arquivo...");

    NOVO_OBJETO (this->btnOk, QPushButton (this));
    this->btnOk->setText ("OK");
    this->btnOk->show ();
    this->btnOk->adjustSize ();

    NOVO_OBJETO (this->btnCancelar, QPushButton (this));
    this->btnCancelar->setText ("Cancelar");
    this->btnCancelar->show ();
    this->btnCancelar->adjustSize ();

    this->grupo->addButton (this->btnAdicionar);
    this->grupo->addButton (this->btnEditar);
    this->grupo->addButton (this->btnRemover);
    this->grupo->addButton (this->btnParaCima);
    this->grupo->addButton (this->btnParaBaixo);
    this->grupo->addButton (this->btnEscolhaAleatoria);
    this->grupo->addButton (this->btnAdicionarGradiente);
    this->grupo->addButton (this->btnLimpar);
    this->grupo->addButton (this->btnAbreArquivo);
    this->grupo->addButton (this->btnSalvaArquivo);
    this->grupo->addButton (this->btnOk);
    this->grupo->addButton (this->btnCancelar);

    tmpw = 0;
    tmph = 0;
    for (i = 0; i < 10; i++) {
        this->grupo->buttons().at(i)->show ();
        this->grupo->buttons().at(i)->adjustSize ();
    }
    for (i = 0; i < 10; i++) {
        tmpw = maior2 (tmpw, this->grupo->buttons().at(i)->width ());
        tmph = maior2 (tmph, this->grupo->buttons().at(i)->height ());
    }
    for (i = 0; i < 10; i++) {
        this->grupo->buttons().at(i)->resize (tmpw, tmph);
    }
    this->mostrador->move (20, 20);
    this->btnAdicionar->move (40 + tmpw, 20);
    for (i = 1; i < 10; i++) {
        this->grupo->buttons().at(i)->move (
        this->grupo->buttons().at(i-1)->x(),
        10 + this->grupo->buttons().at(i-1)->y() + ((i == 3 || i == 5 || i == 8) ? 2 : 1) * this->grupo->buttons().at(i-1)->height()
        );
    }
    this->mostrador->resize (tmpw, embaixo (this->grupo->buttons().at(i-1)) - 20);
    tmpw = maior2 (this->btnOk->width(), this->btnCancelar->width());
    tmph = maior2 (this->btnOk->height(), this->btnCancelar->height());
    this->btnOk->resize (tmpw, tmph);
    this->btnCancelar->resize (tmpw, tmph);

    i = 20 + direita (this->btnLimpar);
    this->btnOk->move ((i - tmpw - tmpw - 40) / 2, 40 + embaixo (this->btnSalvaArquivo));
    this->btnCancelar->move (direita (this->btnOk) + 40, this->btnOk->y());

    this->setFixedSize (i, 20 + embaixo (this->btnOk));

    QObject::connect (this->mostrador, SIGNAL(itemActivated (QListWidgetItem*)), this, SLOT(mostrador_aconteceuAlgumaCoisa(QListWidgetItem*)));
    QObject::connect (this->mostrador, SIGNAL(itemChanged (QListWidgetItem*)), this, SLOT(mostrador_aconteceuAlgumaCoisa(QListWidgetItem*)));
    QObject::connect (this->mostrador, SIGNAL(itemClicked (QListWidgetItem*)), this, SLOT(mostrador_aconteceuAlgumaCoisa(QListWidgetItem*)));
    QObject::connect (this->mostrador, SIGNAL(itemDoubleClicked (QListWidgetItem*)), this, SLOT(mostrador_aconteceuAlgumaCoisa(QListWidgetItem*)));
    QObject::connect (this->mostrador, SIGNAL(itemEntered (QListWidgetItem*)), this, SLOT(mostrador_aconteceuAlgumaCoisa(QListWidgetItem*)));
    QObject::connect (this->mostrador, SIGNAL(itemPressed (QListWidgetItem*)), this, SLOT(mostrador_aconteceuAlgumaCoisa(QListWidgetItem*)));
    QObject::connect (this->btnAdicionar, SIGNAL(clicked()), this, SLOT(btnAdicionar_clicked()));
    QObject::connect (this->btnEditar, SIGNAL(clicked()), this, SLOT(btnEditar_clicked()));
    QObject::connect (this->btnRemover, SIGNAL(clicked()), this, SLOT(btnRemover_clicked()));
    QObject::connect (this->btnLimpar, SIGNAL(clicked()), this, SLOT(btnLimpar_clicked()));
    QObject::connect (this->btnParaBaixo, SIGNAL(clicked()), this, SLOT(btnParaBaixo_clicked()));
    QObject::connect (this->btnParaCima, SIGNAL(clicked()), this, SLOT(btnParaCima_clicked()));
    QObject::connect (this->btnEscolhaAleatoria, SIGNAL(clicked()), this, SLOT(btnEscolhaAleatoria_clicked()));
    QObject::connect (this->btnAdicionarGradiente, SIGNAL(clicked()), this, SLOT(btnAdicionarGradiente_clicked()));
    QObject::connect (this->btnAbreArquivo, SIGNAL(clicked()), this, SLOT(btnAbreArquivo_clicked()));
    QObject::connect (this->btnSalvaArquivo, SIGNAL(clicked()), this, SLOT(btnSalvaArquivo_clicked()));
    QObject::connect (this->btnOk, SIGNAL(clicked()), this, SLOT(btnOk_clicked()));
    QObject::connect (this->btnCancelar, SIGNAL(clicked()), this, SLOT(btnCancelar_clicked()));
    this->update ();
}

editor_paleta::~editor_paleta () {
    this->p = NULL;
}

void editor_paleta::loadConfig () {
    int i, lM;
    this->mostrador->clear ();
    lM = this->p->paletaDeCoresReal->count ();
    for (i=0; i < lM; i++) {
        this->mostrador->addItem (this->p->paletaDeCoresReal->item(i)->clone ());
    }
}
// editor_paleta.cpp
