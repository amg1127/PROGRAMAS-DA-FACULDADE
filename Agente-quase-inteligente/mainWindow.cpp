#include "asserts.h"
#include "mainWindow.h"
#include <cmath>

const char* MIMETYPE = "application/x-amg1127-agente_g";
const char* MOSTRAUTILI = "Mostrar função de &utilidade";
const char* MOSTRAUTILI_TOOLTIP = "Mostra os valores de utilidade que foram calculados para cada quadrante";
const char* PROPSELE = "Propriedades da seleção...";
const char* PROPSELE_TOOLTIP = "Altera as propriedades da seleção";
const int ANIMACAO_AGENTE_START = 1000;
const int ANIMACAO_AGENTE_END = 2000;
const int ANIMACAO_AGENTE_BEFORESTART = 3000;

void MainWindow::janelas_close (QObject*, QEvent* ev) {
    this->querContinuar = false;
    ev->accept ();
}

void MainWindow::desselecionaTudo () {
    int x, y;
    for (x = 0; x < this->dx; x++) {
        for (y = 0; y < this->dy; y++) {
            this->tabuleiroLabels[x][y]->desseleciona ();
        }
    }
}

TabuleiroLabel* MainWindow::quadroSobOPonteiro (int px, int py) {
    return (this->quadroSobOPonteiro (QPoint (px, py)));
}

TabuleiroLabel* MainWindow::quadroSobOPonteiro (QPoint p) {
    int x, y;
    TabuleiroLabel* resp;
    for (x = 0; x < this->dx; x++) {
        for (y = 0; y < this->dy; y++) {
            resp = this->tabuleiroLabels[x][y];
            if (resp->frameGeometry().contains (p)) {
                return (resp);
            }
        }
    }
    return (NULL);
}

void MainWindow::frmTabuleiro_mousePress (QObject* o, QEvent* e) {
    int x, y;
    int xin, yin, xfi, yfi;
    if (this->lblQuadroDeSelecao->isVisible() || this->mostrandoPoliticaOtima) {
        return;
    }
    QMouseEvent* ev = (QMouseEvent*) e;
    TabuleiroLabel* tst = this->quadroSobOPonteiro (ev->pos ());
    if (ev->button () != Qt::LeftButton) {
        if (ev->button () != Qt::RightButton) {
            return;
        }
        if (tst != NULL) {
            if (tst->estaSelecionado ()) {
                this->wantPopup = true;
                return;
            }
        }
    }
    this->wantPopup = false;
    this->segurouShift = ev->modifiers() & Qt::ShiftModifier;
    this->segurouCTRL = ev->modifiers() & Qt::ControlModifier;
    this->lblQuadroDeSelecao->show ();
    this->lblQuadroDeSelecao->raise ();
    this->movimento.setTopLeft (ev->pos());
    if (! segurouCTRL) {
        this->desselecionaTudo ();
    }
    if (this->segurouShift) {
        if (tst != NULL) {
            if (this->ultSelecX >= 0 && this->ultSelecY >= 0) {
                if (this->ultSelecX < tst->x) {
                    xin = this->ultSelecX;
                    xfi = tst->x;
                } else {
                    xin = tst->x;
                    xfi = this->ultSelecX;
                }
                if (this->ultSelecY < tst->y) {
                    yin = this->ultSelecY;
                    yfi = tst->y;
                } else {
                    yin = tst->y;
                    yfi = this->ultSelecY;
                }
                for (x = xin; x <= xfi; x++) {
                    for (y = yin; y <= yfi; y++) {
                        this->tabuleiroLabels[x][y]->seleciona ();
                    }
                }
                this->ultSelecX = tst->x;
                this->ultSelecY = tst->y;
            }
        }
    }
    for (x = 0; x < this->dx; x++) {
        for (y = 0; y < this->dy; y++) {
            this->tabuleiroLabels[x][y]->salvaEstadoSelecionado ();
        }
    }
    this->frmTabuleiro_mouseMove (o, e);
}

void MainWindow::frmTabuleiro_mouseMove (QObject*, QEvent* e) {
    if (this->lblQuadroDeSelecao->isVisible()) {
        QMouseEvent* ev = (QMouseEvent*) e;
        int x, y;
        TabuleiroLabel* ult;
        bool naovai;
        bool fl1, fl2;
        this->movimento.setBottomRight (ev->pos());
        this->scrlTrabuleiro->ensureVisible (ev->x(), ev->y());
        QRect r = this->movimento.normalized();
        this->lblQuadroDeSelecao->move (r.topLeft());
        this->lblQuadroDeSelecao->resize (r.size());
        if (this->segurouShift && (! this->segurouCTRL)) {
            naovai = true;
            ult = this->quadroSobOPonteiro (ev->pos ());
            if (ult != NULL) {
                if (ult->x == this->ultSelecX && ult->y == this->ultSelecY) {
                    naovai = false;
                }
            }
            if (naovai) {
                this->segurouShift = false;
                for (x = 0; x < this->dx; x++) {
                    for (y = 0; y < this->dy; y++) {
                        this->tabuleiroLabels[x][y]->desseleciona ();
                        this->tabuleiroLabels[x][y]->salvaEstadoSelecionado ();
                    }
                }
            }
        }
        for (x = 0; x < this->dx; x++) {
            for (y = 0; y < this->dy; y++) {
                ult = this->tabuleiroLabels[x][y];
                fl1 = this->movimento.intersects (ult->frameGeometry());
                fl2 = ult->estavaSelecionado();
                if ((fl1 && (! (fl2 && this->segurouCTRL))) || ((! fl1) && fl2 && this->segurouCTRL) || (this->segurouShift && (fl1 || fl2))) {
                    ult->seleciona ();
                } else {
                    ult->desseleciona ();
                }
            }
        }
    }
}

void MainWindow::frmTabuleiro_mouseRelease (QObject* o, QEvent* e) {
    if (this->lblQuadroDeSelecao->isVisible ()) {
        TabuleiroLabel* ult1;
        TabuleiroLabel* ult2;
        this->frmTabuleiro_mouseMove (o, e);
        this->lblQuadroDeSelecao->hide ();
        this->frmTabuleiro->update ();
        ult1 = this->quadroSobOPonteiro (this->movimento.topLeft ());
        ult2 = this->quadroSobOPonteiro (this->movimento.bottomRight ());
        if (ult1 != NULL && ult1 == ult2) {
            this->ultSelecX = ult1->x;
            this->ultSelecY = ult1->y;
        } else {
            this->ultSelecX = -1;
            this->ultSelecY = -1;
        }
    }
    QMouseEvent* ev = (QMouseEvent*) e;
    if ((this->wantPopup || ev->button() == Qt::RightButton) && (! this->mostrandoPoliticaOtima)) {
        QMenu menu;
        QAction* a;
        int x, y;
        bool multiple = false;
        bool first = false;
        bool tem = false;
        bool marFi = false;
        bool marNFi = false;
        TabuleiroLabel* t;
        for (x = 0; x < dx; x++) {
            for (y = 0; y < dy; y++) {
                t = this->tabuleiroLabels[x][y];
                if (t->estaSelecionado ()) {
                    if (first) {
                        multiple = true;
                    } else {
                        first = true;
                    }
                    if (t->ehEstadoFinal ()) {
                        marNFi = true;
                    } else {
                        marFi = true;
                    }
                    tem = (tem || t->temAgente() || t->temMuro());
                }
            }
        }
        a = menu.addAction (QString::fromUtf8 ("Adicionar agente"), this, SLOT(menuAcaoPoeAgente_triggered()));
        if (multiple || (! first)) {
            a->setEnabled (false);
        }
        if (multiple) {
            a = menu.addAction (QString::fromUtf8 ("Adicionar muros"), this, SLOT(menuAcaoPoeMuro_triggered()));
        } else {
            a = menu.addAction (QString::fromUtf8 ("Adicionar muro"), this, SLOT(menuAcaoPoeMuro_triggered()));
            if (! first) {
                a->setEnabled (false);
            }
        }
        a = menu.addAction (QString::fromUtf8 ("Limpar"), this, SLOT(menuAcaoRemoverElemento_triggered()));
        a->setEnabled (tem && first);
        menu.addSeparator ();
        if (multiple) {
            a = menu.addAction (QString::fromUtf8 ("Marcar estados como terminais"), this, SLOT(menuAcaoMarcaFinal_triggered()));
        } else {
            a = menu.addAction (QString::fromUtf8 ("Marcar estado como terminal"), this, SLOT(menuAcaoMarcaFinal_triggered()));
        }
        a->setEnabled (marFi);
        if (multiple) {
            a = menu.addAction (QString::fromUtf8 ("Marcar estados como não-terminais"), this, SLOT(menuAcaoMarcaFinal_triggered()));
        } else {
            a = menu.addAction (QString::fromUtf8 ("Marcar estado como não-terminal"), this, SLOT(menuAcaoMarcaFinal_triggered()));
        }
        a->setEnabled (marNFi);
        menu.addSeparator ();
        a = menu.addAction (this->mw.btnEditarPropriedades->text(), this, SLOT(btnEditarPropriedades_clicked()));
        a->setEnabled (first);
        if (this->quadroSobOPonteiro (ev->pos()) != NULL) {
            menu.exec (ev->globalPos ());
        }
    }
}

void MainWindow::lblsAgenteMuro_mousePress (QObject* obj, QEvent*) {
    QDrag* drag;
    QMimeData* dados = new QMimeData();
    QSize meio;
    if (obj == this->mw.lblAgente) {
        drag = new QDrag (this->mw.lblAgente);
        drag->setPixmap (QPixmap (":/boneco.png"));
        dados->setData (MIMETYPE, QByteArray("agente"));
    } else if (obj == this->mw.lblFinal) {
        drag = new QDrag (this->mw.lblFinal);
        drag->setPixmap (QPixmap (":/estado-final.png"));
        dados->setData (MIMETYPE, QByteArray("estado-final"));
    } else {
        Q_ASSERT (obj == this->mw.lblMuro);
        drag = new QDrag (this->mw.lblMuro);
        drag->setPixmap (QPixmap (":/muro.png"));
        dados->setData (MIMETYPE, QByteArray("muro"));
    }
    drag->setMimeData (dados);
    meio = drag->pixmap().size() / 2;
    drag->setHotSpot (QPoint (meio.width(), meio.height()));
    drag->exec (Qt::CopyAction);
}

MainWindow::MainWindow (QWidget* parent, Qt::WindowFlags f) : QWidget (parent, f) {
    QShortcut* atalho;
    QVBoxLayout* vlay;
    mw.setupUi (this);
    this->setWindowTitle (QString::fromUtf8("Agente inteligente (ou quase) em um tabuleiro estocástico - by AMG1127"));
    vlay = new QVBoxLayout (this->mw.frmTabuleiro);
    this->scrlTrabuleiro = new QScrollArea (this->mw.frmTabuleiro);
    this->scrlTrabuleiro->show ();
    vlay->addWidget (this->scrlTrabuleiro);
    this->frmTabuleiro = new QWidget (this->scrlTrabuleiro);
    this->frmTabuleiro->show ();
    this->scrlTrabuleiro->setWidget (this->frmTabuleiro);
    this->scrlTrabuleiro->setWidgetResizable (true);
    this->boneco = new AgenteAnimado (this->frmTabuleiro);
    QObject::connect (this->boneco, SIGNAL(endedAnimation()), this, SLOT(boneco_endedAnimation()));
    QObject::connect (this->mw.btnAlterarDimsTab, SIGNAL(clicked()), this, SLOT(btnAlterarDimsTab_clicked()));
    QObject::connect (this->mw.btnEditarPropriedades, SIGNAL(clicked()), this, SLOT(btnEditarPropriedades_clicked()));
    QObject::connect (this->mw.btnPoliticaOtima, SIGNAL(clicked()), this, SLOT(btnPoliticaOtima_clicked()));
    atalho = new QShortcut (QKeySequence::Open, this->mw.btnCarregar);
    QObject::connect (atalho, SIGNAL(activated()), this, SLOT(btnCarregar_clicked()));
    QObject::connect (this->mw.btnCarregar, SIGNAL(clicked()), this, SLOT(btnCarregar_clicked()));
    atalho = new QShortcut (QKeySequence::Save, this->mw.btnSalvar);
    QObject::connect (atalho, SIGNAL(activated()), this, SLOT(btnSalvar_clicked()));
    QObject::connect (this->mw.btnSalvar, SIGNAL(clicked()), this, SLOT(btnSalvar_clicked()));
    atalho = new QShortcut (QKeySequence::SelectAll, this->mw.btnSelectTudo);
    QObject::connect (atalho, SIGNAL(activated()), this, SLOT(btnSelectTudo_clicked()));
    QObject::connect (this->mw.btnSelectTudo, SIGNAL(clicked()), this, SLOT(btnSelectTudo_clicked()));
    atalho = new QShortcut (QKeySequence::Delete, this);
    QObject::connect (atalho, SIGNAL(activated()), this, SLOT(menuAcaoRemoverElemento_triggered()));
    EventFilter::IniciaMonitoramento (this->mw.lblAgente, QEvent::MouseButtonPress, this, "lblsAgenteMuro_mousePress");
    EventFilter::IniciaMonitoramento (this->mw.lblMuro, QEvent::MouseButtonPress, this, "lblsAgenteMuro_mousePress");
    EventFilter::IniciaMonitoramento (this->mw.lblFinal, QEvent::MouseButtonPress, this, "lblsAgenteMuro_mousePress");
    EventFilter::IniciaMonitoramento (this->frmTabuleiro, QEvent::MouseButtonPress, this, "frmTabuleiro_mousePress");
    EventFilter::IniciaMonitoramento (this->frmTabuleiro, QEvent::MouseButtonRelease, this, "frmTabuleiro_mouseRelease");
    EventFilter::IniciaMonitoramento (this->frmTabuleiro, QEvent::MouseMove, this, "frmTabuleiro_mouseMove");
    EventFilter::IniciaMonitoramento (this, QEvent::Close, this, "janelas_close");
    this->dx = 4;
    this->dy = 3;
    this->ultSelecX = -1;
    this->ultSelecY = -1;
    this->mostrandoPoliticaOtima = false;
    this->lblQuadroDeSelecao = new QLabel (this->frmTabuleiro);
    this->lblQuadroDeSelecao->setFrameStyle (QFrame::StyledPanel | QFrame::Plain);
    this->lblQuadroDeSelecao->setScaledContents (true);
    this->lblQuadroDeSelecao->hide ();
    this->criaTabuleiro ();
    this->mw.btnEditarPropriedades->setText (QString::fromUtf8(PROPSELE));
    habilitaOuDesabilita ();
    this->show ();
}

MainWindow::~MainWindow() {
    this->destroiTabuleiro ();
}

void MainWindow::destroiTabuleiro () {
    int ix, iy;
    this->mostrandoPoliticaOtima = false;
    this->habilitaOuDesabilita ();
    this->frmTabuleiro->hide ();
    this->boneco->stopAnimation ();
    delete this->grlay;
    delete this->spcTopLeft;
    delete this->spcBottomRight;
    for (ix = 0; ix < dx; ix++) {
        for (iy = 0; iy < dy; iy++) {
            EventFilter::EncerraMonitoramento (this->tabuleiroLabels[ix][iy]);
        }
    }
    LimpaMatriz<TabuleiroLabel*> (this->tabuleiroLabels, dx, dy);
    this->dx = 0;
    this->dy = 0;
    this->ultSelecX = -1;
    this->ultSelecY = -1;
}

void MainWindow::criaTabuleiro () {
    QSizePolicy::Policy plspc = QSizePolicy::Preferred;
    int ix, iy;
    TabuleiroLabel* newLabel;
    this->grlay = new QGridLayout (this->frmTabuleiro);
    this->spcTopLeft = new QWidget ();
    this->spcTopLeft->setSizePolicy (plspc, plspc);
    this->spcBottomRight = new QWidget ();
    this->spcBottomRight->setSizePolicy (plspc, plspc);
    this->grlay->addWidget (this->spcTopLeft, 0, 0);
    this->grlay->addWidget (this->spcBottomRight, this->dy + 1, this->dx + 1);
    CriaMatriz<TabuleiroLabel*> (this->tabuleiroLabels, this->dx, this->dy);
    for (ix = 0; ix < this->dx; ix++) {
        for (iy = 0; iy < this->dy; iy++) {
            newLabel = new TabuleiroLabel (ix, iy, this->frmTabuleiro);
            this->grlay->addWidget (newLabel, iy + 1, ix + 1);
            this->tabuleiroLabels[ix][iy] = newLabel;
            EventFilter::IniciaMonitoramento (newLabel, QEvent::DragEnter, this, "tabuleiroLabels_dragEnter");
            EventFilter::IniciaMonitoramento (newLabel, QEvent::DragLeave, this, "tabuleiroLabels_dragLeave");
            EventFilter::IniciaMonitoramento (newLabel, QEvent::Drop, this, "tabuleiroLabels_drop");
        }
    }
    this->frmTabuleiro->show ();
    this->mw.lblDimsTabResp->setText (QString("%1x%2").arg(this->dy).arg(this->dx));
    this->lblQuadroDeSelecao->raise ();
}

void MainWindow::tabuleiroLabels_dragEnter (QObject* obj, QEvent* e) {
    QDragEnterEvent* ev = (QDragEnterEvent*) e;
    QWidget* src;
    TabuleiroLabel* dst;
    if (ev->mimeData()->hasFormat(MIMETYPE)) {
        src = ev->source();
        if (src == this->mw.lblAgente || src == this->mw.lblMuro || src == this->mw.lblFinal) {
            dst = (TabuleiroLabel*) obj;
            dst->seleciona ();
            ev->acceptProposedAction();
            return;
        }
    }
    ev->ignore();
}

void MainWindow::tabuleiroLabels_dragLeave (QObject* obj, QEvent* ev) {
    TabuleiroLabel* dst = (TabuleiroLabel*) obj;
    dst->desseleciona ();
    ev->accept();
}

void MainWindow::tabuleiroLabels_drop (QObject* obj, QEvent* e) {
    QDropEvent* ev = (QDropEvent*) e;
    QWidget* src;
    TabuleiroLabel* dst;
    if (ev->mimeData()->hasFormat(MIMETYPE)) {
        src = ev->source();
        if (src == this->mw.lblAgente || src == this->mw.lblMuro || src == this->mw.lblFinal) {
            dst = (TabuleiroLabel*) obj;
            if (src == this->mw.lblAgente) {
                if (dst->temAgente()) {
                    dst->tiraAgente ();
                } else {
                    dst->poeAgente ();
                }
            } else if (src == this->mw.lblFinal) {
                dst->setEstadoFinal (! dst->ehEstadoFinal ());
            } else {
                if (dst->temMuro ()) {
                    dst->tiraMuro ();
                } else {
                    dst->poeMuro ();
                }
            }
            ev->acceptProposedAction();
            dst->desseleciona ();
            return;
        }
    }
    ev->ignore();
}

void MainWindow::btnAlterarDimsTab_clicked () {
    QString resp;
    QStringList lsts;
    QRegExp nums ("\\s*(\\d+)\\s*[^\\d\\s]\\s*(\\d+)\\s*");
    bool ok;
    int x, y;
    if (this->lblQuadroDeSelecao->isVisible()) {
        return;
    }
    do {
        resp = QInputDialog::getText (0,
                                      this->windowTitle (),
                                      QString::fromUtf8 ("Digite as dimensões do tabuleiro, no formato ROWxCOLS ou ROW,COLS.\n\nCuidado: todos os objetos que estão atualmente no tabuleiro serão removidos!"),
                                      QLineEdit::Normal,
                                      QString("%1x%2").arg(this->dy).arg(this->dx),
                                      &ok);
        if (ok) {
            if (nums.exactMatch (resp)) {
                lsts = nums.capturedTexts ();
                y = lsts[1].toInt();
                x = lsts[2].toInt();
                if (x < 1) {
                    x = 1;
                } else if (x > 99) {
                    x = 99;
                }
                if (y < 1) {
                    y = 1;
                } else if (y > 99) {
                    y = 99;
                }
                this->destroiTabuleiro ();
                this->dx = x;
                this->dy = y;
                this->criaTabuleiro ();
                ok = false;
            }
        }
        if (ok) {
            QMessageBox::warning (0,
                                  this->windowTitle (),
                                  QString::fromUtf8 ("Valor de dimensões inválido!"));
        }
    } while (ok);
}

void MainWindow::btnEditarPropriedades_clicked () {
    int x, y;
    if (this->lblQuadroDeSelecao->isVisible()) {
        return;
    }
    if (this->mostrandoPoliticaOtima) {
        if (this->mw.btnEditarPropriedades->isChecked ()) {
            for (x = 0; x < this->dx; x++) {
                for (y = 0; y < this->dy; y++) {
                    this->tabuleiroLabels[x][y]->mostraUtilidade ();
                }
            }
            if (this->navAgente != NULL) {
                this->boneco->stopAnimation ();
            }
        } else {
            for (x = 0; x < this->dx; x++) {
                for (y = 0; y < this->dy; y++) {
                    this->tabuleiroLabels[x][y]->ocultaUtilidade ();
                }
            }
            if (this->xAgente >= 0 && this->yAgente >= 0) {
                this->navAgente = this->tabuleiroLabels[this->xAgente][this->yAgente];
                QTimer::singleShot (ANIMACAO_AGENTE_BEFORESTART, this, SLOT(iniciaAnimacao()));
            }
        }
        this->frmTabuleiro->update ();
    } else {
        for (x = 0; x < this->dx; x++) {
            for (y = 0; y < this->dy; y++) {
                if (this->tabuleiroLabels[x][y]->estaSelecionado ()) {
                    OpcoesSelecao o (x, y, this->tabuleiroLabels, this->dx, this->dy);
                    o.setWindowTitle (this->windowTitle ());
                    o.exec ();
                    this->determinaIntervRecompensa ();
                    return;
                }
            }
        }
        QMessageBox::warning (0,
                              this->windowTitle (),
                              QString::fromUtf8 ("Selecione um ou mais quadros primeiro!"));
    }
}

void MainWindow::menuAcaoPoeAgente_triggered () {
    int x, y;
    TabuleiroLabel* t;
    for (x = 0; x < dx; x++) {
        for (y = 0; y < dy; y++) {
            t = this->tabuleiroLabels[x][y];
            if (t->estaSelecionado ()) {
                t->poeAgente ();
                break;
            }
        }
    }
}

void MainWindow::menuAcaoPoeMuro_triggered () {
    int x, y;
    TabuleiroLabel* t;
    for (x = 0; x < dx; x++) {
        for (y = 0; y < dy; y++) {
            t = this->tabuleiroLabels[x][y];
            if (t->estaSelecionado ()) {
                t->poeMuro ();
            }
        }
    }
}

void MainWindow::menuAcaoMarcaFinal_triggered () {
    this->menuAcaoMarcacaoFinal (true);
}

void MainWindow::menuAcaoMarcaNaoFinal_triggered () {
    this->menuAcaoMarcacaoFinal (false);
}

void MainWindow::menuAcaoMarcacaoFinal (bool flag) {
    int x, y;
    TabuleiroLabel* t;
    for (x = 0; x < dx; x++) {
        for (y = 0; y < dy; y++) {
            t = this->tabuleiroLabels[x][y];
            if (t->estaSelecionado ()) {
                t->setEstadoFinal (flag);
            }
        }
    }
}

void MainWindow::menuAcaoRemoverElemento_triggered () {
    int x, y;
    TabuleiroLabel* t;
    for (x = 0; x < dx; x++) {
        for (y = 0; y < dy; y++) {
            t = this->tabuleiroLabels[x][y];
            if (t->estaSelecionado ()) {
                t->tiraMuro ();
                t->tiraAgente ();
            }
        }
    }
}

void MainWindow::btnCarregar_clicked () {
    static QString arqname;
    QString newarqname;
    if (this->lblQuadroDeSelecao->isVisible()) {
        return;
    }
    newarqname = QFileDialog::getOpenFileName ((QWidget*)0, this->windowTitle() + " - Abrir estado", arqname, QString::fromUtf8("Modelos de ambientes estocásticos (*.agente_g)"));
    if (newarqname != "") {
        if (newarqname.right(9).toLower() != ".agente_g") {
            newarqname += ".agente_g";
        }
        arqname = newarqname;
        if (! this->LeEstadoDoArquivo (newarqname)) {
            QMessageBox::critical ((QWidget*)0, this->windowTitle() + " - Abrir estado", QString("Erro:\n\nNao foi possivel carregar o estado do arquivo '") + arqname + "'.");
        }
        this->update();
    }
}

void MainWindow::btnSalvar_clicked () {
    static QString arqname;
    QString newarqname;
    if (this->lblQuadroDeSelecao->isVisible()) {
        return;
    }
    newarqname = QFileDialog::getSaveFileName ((QWidget*)0, this->windowTitle() + " - Salvar estado", arqname, QString::fromUtf8("Modelos de ambientes estocásticos (*.agente_g)"));
    if (newarqname != "") {
        if (newarqname.right(9).toLower() != ".agente_g") {
            newarqname += ".agente_g";
            if (QFile::exists (newarqname)) {
                if (QMessageBox::warning ((QWidget*)0, this->windowTitle() + " - Salvar estado", QString::fromUtf8("Tem certeza que você deseja sobreescrever o arquivo já existente '") + newarqname + "'?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No) {
                    this->btnSalvar_clicked ();
                    return;
                }
            }
        }
        arqname = newarqname;
        if (! this->gravaEstadoNoArquivo (newarqname)) {
            QMessageBox::critical ((QWidget*)0, this->windowTitle() + " - Salvar estado", QString("Erro:\n\nNao foi possivel salvar o estado no arquivo '") + arqname + "'.");
        }
    }
}

bool MainWindow::gravaEstadoNoArquivo (QString file) {
    QFile fd (file);
    int x, y;
    QString k;
    TabuleiroLabel* t;
    // A ideia aqui e esvaziar o arquivo antes de gravar.
    if (! fd.open (QIODevice::Truncate | QIODevice::WriteOnly)) {
        return (false);
    }
    fd.close ();
    QSettings saida (file, QSettings::IniFormat);
    saida.setValue ("global/gamma", this->mw.dspbGamma->value());
    saida.setValue ("global/epsilon", this->mw.dspbEpsilon->value());
    saida.setValue ("global/dim_x", this->dx);
    saida.setValue ("global/dim_y", this->dy);
    for (x = 0; x < this->dx; x++) {
        for (y = 0; y < this->dy; y++) {
            k = QString("quad_%1_%2/").arg(x).arg(y);
            t = this->tabuleiroLabels[x][y];
            if (t->temAgente()) {
                saida.setValue (k + "objeto", "agente");
            } else if (t->temMuro()) {
                saida.setValue (k + "objeto", "muro");
            } else {
                saida.setValue (k + "objeto", "vazio");
            }
            if (t->ehEstadoFinal ()) {
                saida.setValue (k + "final", "true");
            } else {
                saida.setValue (k + "final", "false");
            }
            saida.setValue (k + "norte_probabs", QString("%1/%2/%3/%4").arg(t->prbNorte.norte()).arg(t->prbNorte.oeste()).arg(t->prbNorte.leste()).arg(t->prbNorte.sul()));
            saida.setValue (k + "norte_prio", QString("%1/%2/%3/%4").arg(t->prbNorte.prioridade(0)).arg(t->prbNorte.prioridade(1)).arg(t->prbNorte.prioridade(2)).arg(t->prbNorte.prioridade(3)));
            saida.setValue (k + "oeste_probabs", QString("%1/%2/%3/%4").arg(t->prbOeste.norte()).arg(t->prbOeste.oeste()).arg(t->prbOeste.leste()).arg(t->prbOeste.sul()));
            saida.setValue (k + "oeste_prio", QString("%1/%2/%3/%4").arg(t->prbOeste.prioridade(0)).arg(t->prbOeste.prioridade(1)).arg(t->prbOeste.prioridade(2)).arg(t->prbOeste.prioridade(3)));
            saida.setValue (k + "leste_probabs", QString("%1/%2/%3/%4").arg(t->prbLeste.norte()).arg(t->prbLeste.oeste()).arg(t->prbLeste.leste()).arg(t->prbLeste.sul()));
            saida.setValue (k + "leste_prio", QString("%1/%2/%3/%4").arg(t->prbLeste.prioridade(0)).arg(t->prbLeste.prioridade(1)).arg(t->prbLeste.prioridade(2)).arg(t->prbLeste.prioridade(3)));
            saida.setValue (k + "sul_probabs", QString("%1/%2/%3/%4").arg(t->prbSul.norte()).arg(t->prbSul.oeste()).arg(t->prbSul.leste()).arg(t->prbSul.sul()));
            saida.setValue (k + "sul_prio", QString("%1/%2/%3/%4").arg(t->prbSul.prioridade(0)).arg(t->prbSul.prioridade(1)).arg(t->prbSul.prioridade(2)).arg(t->prbSul.prioridade(3)));
            saida.setValue (k + "recompensa", t->recompensa());
        }
    }
    saida.sync ();
    if (saida.status () != QSettings::NoError) {
        return (false);
    }
    return (true);
}

bool MainWindow::LeEstadoDoArquivo (QString file) {
    double dtmp;
    double p0d, p1d, p2d, p3d;
    int p0i, p1i, p2i, p3i;
    int x, y;
    QString k;
    QString v;
    QFile fd (file);
    TabuleiroLabel* t;
    if (! fd.open (QIODevice::ReadOnly)) {
        return (false);
    }
    fd.close ();
    QSettings entrada (file, QSettings::IniFormat);
    if (entrada.status () != QSettings::NoError) {
        return (false);
    }
    entrada.sync ();
    dtmp = entrada.value("global/gamma", 1).toDouble ();
    poeDentroDaFaixa (dtmp, this->mw.dspbGamma->minimum(), this->mw.dspbGamma->maximum());
    this->mw.dspbGamma->setValue (dtmp);
    dtmp = entrada.value("global/epsilon", 1).toDouble ();
    poeDentroDaFaixa (dtmp, this->mw.dspbEpsilon->minimum(), this->mw.dspbEpsilon->maximum());
    this->mw.dspbEpsilon->setValue (dtmp);
    this->destroiTabuleiro ();
    this->dx = entrada.value("global/dim_x", 4).toInt();
    poeDentroDaFaixa (this->dx, 1, 99);
    this->dy = entrada.value("global/dim_y", 3).toInt();
    poeDentroDaFaixa (this->dy, 1, 99);
    this->criaTabuleiro ();
    for (x = 0; x < this->dx; x++) {
        for (y = 0; y < this->dy; y++) {
            k = QString("quad_%1_%2/").arg(x).arg(y);
            t = this->tabuleiroLabels[x][y];
            v = entrada.value (k + "objeto", "vazio").toString ();
            if (v == "agente") {
                t->poeAgente ();
            } else if (v == "muro") {
                t->poeMuro ();
            }
            dtmp = entrada.value (k + "recompensa", 0).toDouble ();
            poeDentroDaFaixa (dtmp, (double) -100, (double) 100);
            t->setRecompensa (dtmp);
            v = entrada.value (k + "norte_probabs", "0.25/0.25/0.25/0.25").toString ();
            this->parseiaProbabs (v, p0d, p1d, p2d, p3d);
            t->prbNorte.setProbabs (p0d, p1d, p2d, p3d);
            v = entrada.value (k + "oeste_probabs", "0.25/0.25/0.25/0.25").toString ();
            this->parseiaProbabs (v, p0d, p1d, p2d, p3d);
            t->prbOeste.setProbabs (p0d, p1d, p2d, p3d);
            v = entrada.value (k + "leste_probabs", "0.25/0.25/0.25/0.25").toString ();
            this->parseiaProbabs (v, p0d, p1d, p2d, p3d);
            t->prbLeste.setProbabs (p0d, p1d, p2d, p3d);
            v = entrada.value (k + "sul_probabs", "0.25/0.25/0.25/0.25").toString ();
            this->parseiaProbabs (v, p0d, p1d, p2d, p3d);
            t->prbSul.setProbabs (p0d, p1d, p2d, p3d);
            v = entrada.value (k + "norte_prio", "0/1/2/3").toString ();
            this->parseiaPrioridades (v, p0i, p1i, p2i, p3i);
            t->prbNorte.setPrioridades (p0i, p1i, p2i, p3i);
            v = entrada.value (k + "oeste_prio", "0/1/2/3").toString ();
            this->parseiaPrioridades (v, p0i, p1i, p2i, p3i);
            t->prbOeste.setPrioridades (p0i, p1i, p2i, p3i);
            v = entrada.value (k + "leste_prio", "0/1/2/3").toString ();
            this->parseiaPrioridades (v, p0i, p1i, p2i, p3i);
            t->prbLeste.setPrioridades (p0i, p1i, p2i, p3i);
            v = entrada.value (k + "sul_prio", "0/1/2/3").toString ();
            this->parseiaPrioridades (v, p0i, p1i, p2i, p3i);
            t->prbSul.setPrioridades (p0i, p1i, p2i, p3i);
            t->setEstadoFinal (entrada.value (k + "final", "false").toString () == "true");
        }
    }
    this->determinaIntervRecompensa ();
    return (true);
}

void MainWindow::parseiaProbabs (QString s, double& p1, double& p2, double& p3, double& p4) {
    QStringList spl = s.split ("/");
    p1 = spl[0].toDouble ();
    p2 = spl[1].toDouble ();
    p3 = spl[2].toDouble ();
    p4 = spl[3].toDouble ();
}

void MainWindow::parseiaPrioridades (QString s, int& p1, int& p2, int& p3, int& p4) {
    QStringList spl = s.split ("/");
    p1 = spl[0].toInt ();
    p2 = spl[1].toInt ();
    p3 = spl[2].toInt ();
    p4 = spl[3].toInt ();
}

void MainWindow::determinaIntervRecompensa () {
    int x, y;
    TabuleiroLabel* t;
    TabuleiroLabel::minRecompensa = this->tabuleiroLabels[0][0]->recompensa();;
    TabuleiroLabel::maxRecompensa = TabuleiroLabel::minRecompensa;
    for (x = 0; x < this->dx; x++) {
        for (y = 0; y < this->dy; y++) {
            t = this->tabuleiroLabels[x][y];
            if (TabuleiroLabel::maxRecompensa < t->recompensa ()) {
                TabuleiroLabel::maxRecompensa = t->recompensa ();
            }
            if (TabuleiroLabel::minRecompensa > t->recompensa ()) {
                TabuleiroLabel::minRecompensa = t->recompensa ();
            }
        }
    }
    for (x = 0; x < this->dx; x++) {
        for (y = 0; y < this->dy; y++) {
            this->tabuleiroLabels[x][y]->redesenha ();
        }
    }
}

void MainWindow::brainTimer_timeout () {
    static QPixmap* brain_light = NULL;
    static QPixmap* brain_dark = NULL;
    static bool _first = true;
    static const int max = 360;
    static const int min = 0;
    static const int step = 15;
    static int pos = min;
    static double conv;
    if (_first) {
        _first = false;
        brain_light = new QPixmap (":/pensando-claro.png");
        brain_dark = new QPixmap (":/pensando-escuro.png");
    }
    QPixmap definitivo (brain_light->size ());
    definitivo.fill (Qt::transparent);
    conv = sin ((double) pos * M_PI / 180);
    if (conv < 0) {
        conv = -conv;
    }
    pos += step;
    if (pos >= max) {
        pos = min;
    }
    {
        QPainter pintor (&definitivo);
        pintor.setOpacity (conv);
        pintor.drawPixmap (brain_light->rect(), *brain_dark);
        pintor.setOpacity (1 - conv);
        pintor.drawPixmap (brain_light->rect(), *brain_light);
    }
    this->brain.lblBrain->setPixmap (definitivo);
}

void MainWindow::btnSelectTudo_clicked () {
    int x, y;
    for (x = 0; x < this->dx; x++) {
        for (y = 0; y < this->dy; y++) {
            this->tabuleiroLabels[x][y]->seleciona ();
        }
    }
}

void MainWindow::habilitaOuDesabilita () {
    this->mw.dspbGamma->setEnabled (! this->mostrandoPoliticaOtima);
    this->mw.dspbEpsilon->setEnabled (! this->mostrandoPoliticaOtima);
    this->mw.btnAlterarDimsTab->setEnabled (! this->mostrandoPoliticaOtima);
    this->mw.lblAgente->setEnabled (! this->mostrandoPoliticaOtima);
    this->mw.lblMuro->setEnabled (! this->mostrandoPoliticaOtima);
    this->mw.lblFinal->setEnabled (! this->mostrandoPoliticaOtima);
    this->mw.btnEditarPropriedades->setCheckable (this->mostrandoPoliticaOtima);
    this->mw.btnEditarPropriedades->setChecked (false);
    this->mw.btnEditarPropriedades->setText (QString::fromUtf8 ((this->mostrandoPoliticaOtima) ? MOSTRAUTILI : PROPSELE));
    this->mw.btnEditarPropriedades->setToolTip (QString::fromUtf8 ((this->mostrandoPoliticaOtima) ? MOSTRAUTILI_TOOLTIP : PROPSELE_TOOLTIP));
    this->mw.btnSelectTudo->setEnabled (! this->mostrandoPoliticaOtima);
    this->mw.btnPoliticaOtima->setChecked (this->mostrandoPoliticaOtima);
}

void MainWindow::btnPoliticaOtima_clicked () {
    int x, y;
    TabuleiroLabel* t;
    if (this->lblQuadroDeSelecao->isVisible()) {
        return;
    }
    if (this->mostrandoPoliticaOtima) {
        for (x = 0; x < this->dx; x++) {
            for (y = 0; y < this->dy; y++) {
                t = this->tabuleiroLabels[x][y];
                t->ocultaPoliticaOtima ();
                t->ocultaUtilidade ();
            }
        }
        this->mostrandoPoliticaOtima = false;
        this->boneco->stopAnimation ();
        this->habilitaOuDesabilita ();
    } else {
        QTimer brainTimer;
        TabuleiroLabel* t;
        this->dial = new QDialog ();
        this->dial->resize (1, 1);
        this->brain.setupUi (dial);
        EventFilter::IniciaMonitoramento (this->dial, QEvent::Close, this, "janelas_close");
        dial->setWindowTitle (this->windowTitle ());
        brainTimer.setInterval (100);
        brainTimer.setSingleShot (false);
        QObject::connect (&brainTimer, SIGNAL(timeout()), this, SLOT(brainTimer_timeout()));
        brainTimer.start ();
        brainTimer_timeout ();
        this->querContinuar = true;
        QTimer::singleShot (20000, dial, SLOT(close()));
        dial->setWindowModality (Qt::ApplicationModal);
        dial->show ();
        this->desselecionaTudo ();
        this->calculaPoliticaOtima ();
        brainTimer.stop ();
        this->mostrandoPoliticaOtima = true;
        EventFilter::EncerraMonitoramento (this->dial);
        delete this->dial;
        this->habilitaOuDesabilita ();
        this->xAgente = -1;
        this->yAgente = -1;
        this->navAgente = NULL;
        for (x = 0; x < this->dx; x++) {
            for (y = 0; y < this->dy; y++) {
                t = this->tabuleiroLabels[x][y];
                t->mostraPoliticaOtima ();
                if (t->temAgente ()) {
                    this->xAgente = x;
                    this->yAgente = y;
                    this->navAgente = t;
                    QTimer::singleShot (ANIMACAO_AGENTE_BEFORESTART, this, SLOT(iniciaAnimacao()));
                }
            }
        }
    }
}

int MainWindow::calcProxEstado (Acao* a) {
    double rndn, s;
    int i;
    i = -1;
    if (this->mostrandoPoliticaOtima && this->navAgente != NULL) {
        rndn = (double) rand() / RAND_MAX;
        s = 0;
        for (i = 0; i < 3; i++) {
            s += a->probab (i);
            if (rndn < s) {
                break;
            }
        }
    }
    return (i);
}

void MainWindow::iniciaAnimacao () {
    if (this->mostrandoPoliticaOtima && this->navAgente != NULL && (! this->mw.btnEditarPropriedades->isChecked ())) {
        QPoint inicio (this->navAgente->frameGeometry().center());
        if (this->navAgente->ehEstadoFinal ()) {
            this->boneco->animatedMoving (inicio, inicio, 0, 0, true, ANIMACAO_AGENTE_START, ANIMACAO_AGENTE_END);
        } else {
            this->avancaAgenteAnimado ();
        }
    }
    this->frmTabuleiro->update ();
}

void MainWindow::boneco_endedAnimation () {
    if (this->mostrandoPoliticaOtima && this->navAgente != NULL && (! this->mw.btnEditarPropriedades->isChecked ())) {
        if (this->navAgente->ehEstadoFinal ()) {
            this->boneco->hide ();
            this->navAgente = this->tabuleiroLabels[this->xAgente][this->yAgente];
            QTimer::singleShot (ANIMACAO_AGENTE_BEFORESTART, this, SLOT(iniciaAnimacao()));
        } else {
            this->avancaAgenteAnimado ();
        }
    } else {
        this->boneco->hide ();
    }
    this->frmTabuleiro->update ();
}

void MainWindow::avancaAgenteAnimado () {
    int prox, xd, yd, ot;
    double dirt[4] = { M_PI_2, M_PI, 0, M_PI + M_PI_2 };
    Acao* a;
    if (this->mostrandoPoliticaOtima && this->navAgente != NULL && (! this->mw.btnEditarPropriedades->isChecked ())) {
        QPoint inicio (this->navAgente->frameGeometry().center());
        ot = this->navAgente->politicaOtima ();
        switch (ot) {
            case 0:
                a = &(this->navAgente->prbNorte);
                break;
            case 1:
                a = &(this->navAgente->prbOeste);
                break;
            case 2:
                a = &(this->navAgente->prbLeste);
                break;
            default:
                a = &(this->navAgente->prbSul);
        }
        prox = this->calcProxEstado (a);
        this->bateEVolta (this->navAgente->x, this->navAgente->y, prox, xd, yd);
        this->navAgente = this->tabuleiroLabels[xd][yd];
        this->boneco->animatedMoving (inicio, this->navAgente->frameGeometry().center(), dirt[prox], (double) this->navAgente->width () / 2, false, ANIMACAO_AGENTE_START, ANIMACAO_AGENTE_END);
    } else {
        this->boneco->hide ();
    }
    this->frmTabuleiro->update ();
}

bool MainWindow::bateEVolta (int x, int y, int acao, int& xok, int& yok) {
    int osxis[4] = { 0, -1, 1, 0 };
    int osypsilon[4] = { -1, 0, 0, 1 };
    int xt, yt;
    xok = x;
    yok = y;
    xt = x + osxis[acao];
    yt = y + osypsilon[acao];
    if (xt < 0 || xt >= this->dx) {
        return (true);
    }
    if (yt < 0 || yt >= this->dy) {
        return (true);
    }
    if (this->tabuleiroLabels[xt][yt]->temMuro ()) {
        return (true);
    }
    xok = xt;
    yok = yt;
    return (false);
}

void MainWindow::calculaPoliticaOtima () {
    int x, y, maxLoops, xd, yd, acao, proxEst, minLoops;
    double erro;
    double maxErro;
    double fatorImport;
    double utilAcao;
    double maxAcao;
    double diff;
    TabuleiroLabel* t;
    Acao* probabils[4];
    maxLoops = 1<<30;
    minLoops = 3;
    maxAcao = 0;
    maxErro = this->mw.dspbEpsilon->value ();
    fatorImport = this->mw.dspbGamma->value ();
    for (x = 0; x < this->dx; x++) {
        for (y = 0; y < this->dy; y++) {
            t = this->tabuleiroLabels[x][y];
            t->utilidade = t->recompensa ();
        }
    }
    do {
        for (x = 0; x < this->dx; x++) {
            for (y = 0; y < this->dy; y++) {
                t = this->tabuleiroLabels[x][y];
                t->utilidadeAnt = t->utilidade;
            }
        }
        erro = 0;
        for (x = 0; x < this->dx; x++) {
            for (y = 0; y < this->dy; y++) {
                t = this->tabuleiroLabels[x][y];
                if (t->ehEstadoFinal ()) {
                    t->utilidade = t->recompensa ();
                } else {
                    probabils[0] = &(t->prbNorte);
                    probabils[1] = &(t->prbOeste);
                    probabils[2] = &(t->prbLeste);
                    probabils[3] = &(t->prbSul);
                    for (acao = 0; acao < 4; acao++) {
                        utilAcao = 0;
                        for (proxEst = 0; proxEst < 4; proxEst++) {
                            this->bateEVolta (x, y, proxEst, xd, yd);
                            utilAcao += probabils[acao]->probab (proxEst) * this->tabuleiroLabels[xd][yd]->utilidadeAnt;
                        }
                        if (acao == 0 || maxAcao < utilAcao) {
                            maxAcao = utilAcao;
                        }
                    }
                    t->utilidade = t->recompensa() + fatorImport * maxAcao;
                    diff = t->utilidade - t->utilidadeAnt;
                    if (diff < 0) {
                        diff = -diff;
                    }
                    if (erro < diff) {
                        erro = diff;
                    }
                }
            }
        }
        QCoreApplication::processEvents ();
    } while (erro >= maxErro && (maxLoops--) > 0 && (this->querContinuar || (minLoops--) > 0));
    for (x = 0; x < this->dx; x++) {
        for (y = 0; y < this->dy; y++) {
            t = this->tabuleiroLabels[x][y];
            if (! t->temMuro ()) {
                probabils[0] = &(t->prbNorte);
                probabils[1] = &(t->prbOeste);
                probabils[2] = &(t->prbLeste);
                probabils[3] = &(t->prbSul);
                for (acao = 0; acao < 4; acao++) {
                    utilAcao = 0;
                    for (proxEst = 0; proxEst < 4; proxEst++) {
                        this->bateEVolta (x, y, proxEst, xd, yd);
                        utilAcao += probabils[acao]->probab (proxEst) * this->tabuleiroLabels[xd][yd]->utilidade;
                    }
                    if (acao == 0 || maxAcao < utilAcao) {
                        maxAcao = utilAcao;
                        t->setPoliticaOtima (acao);
                        t->redesenha ();
                    }
                }
            }
        }
    }
    return;
}
