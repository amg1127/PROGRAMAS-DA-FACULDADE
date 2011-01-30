#include "common.h"
#include "editautomato.h"
#include "execautomatos.h"

class mainwindow : public QWidget {
    Q_OBJECT;
    private:
        QScrollArea *edicao;
        editautomato *edwindow;
        QPushButton *btnNovoEstado;
        QPushButton *btnNovaTransicao;
        QPushButton *btnExcluir;
        QPushButton *btnMarcaInicial;
        QPushButton *btnMarcaFinal;
        QLabel *lblMostraAlfabeto;
        QPushButton *btnEditarAlfabeto;
        QPushButton *btnAbrirAutomato;
        QPushButton *btnSalvarAutomato;
        QPushButton *btnExecutarAutomato;
        QPushButton *btnLimparAutomato;
        QPushButton *btnAjuda;
        QPushButton *btnSair;
        void setTxtLabel ();
        void keyPressEvent (QKeyEvent *);
        void resizeEvent (QResizeEvent *);
        void closeEvent (QCloseEvent *);
        template <typename T> T maior (T, T);
        template <typename T> T maior (T, T, T);
        template <typename T> T maior (T, T, T, T);
        template <typename T> T maior (T, T, T, T, T);
    public:
        mainwindow();
    public slots:
        void btnNovoEstado_clicked (bool);
        void btnNovaTransicao_clicked (bool);
        void btnExcluir_clicked (bool);
        void btnMarcaInicial_clicked (bool);
        void btnMarcaFinal_clicked (bool);
        void btnEditarAlfabeto_clicked (bool);
        void btnAbrirAutomato_clicked (bool);
        void btnSalvarAutomato_clicked (bool);
        void btnExecutarAutomato_clicked (bool);
        void btnLimparAutomato_clicked (bool);
        void btnAjuda_clicked (bool);
        void btnSair_clicked (bool);
};

template <typename T> T mainwindow::maior (T el1, T el2) {
    if (el1 > el2) {
        return (el1);
    } else {
        return (el2);
    }
}

template <typename T> T mainwindow::maior (T el1, T el2, T el3) {
    T el = this->maior (el1, el2);
    if (el > el3) {
        return (el);
    } else {
        return (el3);
    }
}

template <typename T> T mainwindow::maior (T el1, T el2, T el3, T el4) {
    return (this->maior (this->maior(el1, el2), this->maior (el3, el4)));
}

template <typename T> T mainwindow::maior (T el1, T el2, T el3, T el4, T el5) {
    return (this->maior (this->maior(el1, el2), this->maior (el3, el4, el5)));
}

void mainwindow::setTxtLabel () {
    QString resp = "";
    QVector<QChar> lista;
    QVector<QChar>::iterator i;
    lista = this->edwindow->aut.getSimb();
    for (i = lista.begin(); i != lista.end(); i++) {
        resp += (*i);
    }
    if (resp == "") {
        resp = "(nenhum)";
    } else {
        resp = "\"" + resp + "\"";
    }
    this->lblMostraAlfabeto->setText (QString("Alfabeto em utilizacao:\n") + resp);
}

mainwindow::mainwindow () {
    this->edicao = new QScrollArea (this);
    Q_ASSERT (this->edicao != NULL);
    this->edicao->setWidgetResizable (true);
    this->edicao->move (ESPACO, ESPACO);
    this->edicao->setMouseTracking (true);
    if (this->edicao->viewport() != NULL) {
        this->edicao->viewport()->setMouseTracking (true);
    }
    this->edicao->show();

    this->edwindow = new editautomato(this->edicao);
    Q_ASSERT (this->edwindow != NULL);
    this->edwindow->show();
    this->edicao->setWidget (this->edwindow);

    this->btnNovoEstado = new QPushButton (this);
    Q_ASSERT (this->btnNovoEstado != NULL);
    this->btnNovoEstado->setText ("Criar novo estado... [CTRL+N]");
    this->btnNovoEstado->show();
    this->btnNovoEstado->adjustSize();
    QObject::connect (this->btnNovoEstado, SIGNAL(clicked(bool)), this, SLOT(btnNovoEstado_clicked (bool)));

    this->btnNovaTransicao = new QPushButton (this);
    Q_ASSERT (this->btnNovaTransicao != NULL);
    this->btnNovaTransicao->setText ("Criar nova transicao... [CTRL+L]");
    this->btnNovaTransicao->show();
    this->btnNovaTransicao->adjustSize();
    QObject::connect (this->btnNovaTransicao, SIGNAL(clicked(bool)), this, SLOT(btnNovaTransicao_clicked (bool)));

    this->btnExcluir = new QPushButton (this);
    Q_ASSERT (this->btnExcluir != NULL);
    this->btnExcluir->setText ("Excluir estado/transicao... [DEL]");
    this->btnExcluir->show();
    this->btnExcluir->adjustSize();
    QObject::connect (this->btnExcluir, SIGNAL(clicked(bool)), this, SLOT(btnExcluir_clicked (bool)));

    this->btnMarcaInicial = new QPushButton (this);
    Q_ASSERT (this->btnMarcaInicial != NULL);
    this->btnMarcaInicial->setText ("Marcar estado inicial [CTRL+I]");
    this->btnMarcaInicial->show();
    this->btnMarcaInicial->adjustSize();
    QObject::connect (this->btnMarcaInicial, SIGNAL(clicked(bool)), this, SLOT(btnMarcaInicial_clicked (bool)));

    this->btnMarcaFinal = new QPushButton (this);
    Q_ASSERT (this->btnMarcaFinal != NULL);
    this->btnMarcaFinal->setText ("Marcar estado final [CTRL+F]");
    this->btnMarcaFinal->show();
    this->btnMarcaFinal->adjustSize();
    QObject::connect (this->btnMarcaFinal, SIGNAL(clicked(bool)), this, SLOT(btnMarcaFinal_clicked (bool)));

    this->lblMostraAlfabeto = new QLabel (this);
    Q_ASSERT (this->lblMostraAlfabeto != NULL);
    this->setTxtLabel ();
    this->lblMostraAlfabeto->setWordWrap (true);
    this->lblMostraAlfabeto->setAlignment (Qt::AlignCenter);
    this->lblMostraAlfabeto->show();

    this->btnEditarAlfabeto = new QPushButton (this);
    Q_ASSERT (this->btnEditarAlfabeto != NULL);
    this->btnEditarAlfabeto->setText ("Configurar alfabeto... [CTRL+E]");
    this->btnEditarAlfabeto->show();
    this->btnEditarAlfabeto->adjustSize();
    QObject::connect (this->btnEditarAlfabeto, SIGNAL(clicked(bool)), this, SLOT(btnEditarAlfabeto_clicked (bool)));

    this->btnSalvarAutomato = new QPushButton (this);
    Q_ASSERT (this->btnSalvarAutomato != NULL);
    this->btnSalvarAutomato->setText ("Salvar automato como... [CTRL+S]");
    this->btnSalvarAutomato->show();
    this->btnSalvarAutomato->adjustSize();
    QObject::connect (this->btnSalvarAutomato, SIGNAL(clicked(bool)), this, SLOT(btnSalvarAutomato_clicked (bool)));

    this->btnAbrirAutomato = new QPushButton (this);
    Q_ASSERT (this->btnAbrirAutomato != NULL);
    this->btnAbrirAutomato->setText ("Abrir automato... [CTRL+A]");
    this->btnAbrirAutomato->show();
    this->btnAbrirAutomato->adjustSize();
    QObject::connect (this->btnAbrirAutomato, SIGNAL(clicked(bool)), this, SLOT(btnAbrirAutomato_clicked (bool)));

    this->btnExecutarAutomato = new QPushButton (this);
    Q_ASSERT (this->btnExecutarAutomato != NULL);
    this->btnExecutarAutomato->setText ("Executar automato... [CTRL+R]");
    this->btnExecutarAutomato->show();
    this->btnExecutarAutomato->adjustSize();
    QObject::connect (this->btnExecutarAutomato, SIGNAL(clicked(bool)), this, SLOT(btnExecutarAutomato_clicked (bool)));

    this->btnLimparAutomato = new QPushButton (this);
    Q_ASSERT (this->btnLimparAutomato != NULL);
    this->btnLimparAutomato->setText ("Limpar automato... [CTRL+DEL]");
    this->btnLimparAutomato->show();
    this->btnLimparAutomato->adjustSize();
    QObject::connect (this->btnLimparAutomato, SIGNAL(clicked(bool)), this, SLOT(btnLimparAutomato_clicked (bool)));

    this->btnAjuda = new QPushButton (this);
    Q_ASSERT (this->btnAjuda != NULL);
    this->btnAjuda->setText ("Ajuda [F1]");
    this->btnAjuda->show();
    this->btnAjuda->adjustSize();
    QObject::connect (this->btnAjuda, SIGNAL(clicked(bool)), this, SLOT(btnAjuda_clicked (bool)));

    this->btnSair = new QPushButton (this);
    Q_ASSERT (this->btnSair != NULL);
    this->btnSair->setText ("Sair [ESC]");
    this->btnSair->show();
    this->btnSair->adjustSize();
    QObject::connect (this->btnSair, SIGNAL(clicked(bool)), this, SLOT(btnSair_clicked (bool)));

    this->setWindowTitle (TITULO);
    this->setMouseTracking (true);
    this->showMaximized();
}

void mainwindow::resizeEvent (QResizeEvent *ev) {
    int pw, ph, px, py, lblw;

    pw = this->maior (
             this->maior (
                 this->maior (
                     this->btnNovoEstado->width(),
                     this->btnNovaTransicao->width()
                 ),
                 this->btnExcluir->width(),
                 this->btnMarcaInicial->width(),
                 this->btnMarcaFinal->width(),
                 this->btnEditarAlfabeto->width()
             ),
             this->maior (
                 this->btnSalvarAutomato->width(),
                 this->btnAbrirAutomato->width(),
                 this->btnExecutarAutomato->width(),
                 this->btnAjuda->width(),
                 this->btnSair->width()
             )
         );
    ph = this->maior (
             this->maior (
                 this->maior (
                     this->btnNovoEstado->height(),
                     this->btnNovaTransicao->height()
                 ),
                 this->btnExcluir->height(),
                 this->btnMarcaInicial->height(),
                 this->btnMarcaFinal->height(),
                 this->btnEditarAlfabeto->height()
             ),
             this->maior (
                 this->btnSalvarAutomato->height(),
                 this->btnAbrirAutomato->height(),
                 this->btnExecutarAutomato->height(),
                 this->btnAjuda->height(),
                 this->btnSair->height()
             )
        );

    this->btnNovoEstado->resize (pw, ph);
    this->btnNovaTransicao->resize (pw, ph);
    this->btnExcluir->resize (pw, ph);
    this->btnMarcaInicial->resize (pw, ph);
    this->btnMarcaFinal->resize (pw, ph);
    this->lblMostraAlfabeto->setFixedWidth (pw);
    this->lblMostraAlfabeto->adjustSize ();
    lblw = this->lblMostraAlfabeto->height () + ESPACO;
    this->btnEditarAlfabeto->resize (pw, ph);
    this->btnSalvarAutomato->resize (pw, ph);
    this->btnAbrirAutomato->resize (pw, ph);
    this->btnExecutarAutomato->resize (pw, ph);
    this->btnLimparAutomato->resize (pw, ph);
    this->btnAjuda->resize (pw, ph);
    this->btnSair->resize (pw, ph);

    px = ev->size().width() - ESPACO - pw;
    py = 0;

    this->btnNovoEstado->move (px, ESPACO + (ESPACO + ph) * (py++));
    this->btnNovaTransicao->move (px, ESPACO + (ESPACO + ph) * (py++));
    this->btnExcluir->move (px, ESPACO + (ESPACO + ph) * (py++));
    this->btnMarcaInicial->move (px, ESPACO + (ESPACO + ph) * (py++));
    this->btnMarcaFinal->move (px, ESPACO + (ESPACO + ph) * (py++));
    py++;
    this->lblMostraAlfabeto->move (px, ESPACO + (ESPACO + ph) * py);
    this->btnEditarAlfabeto->move (px, lblw + ESPACO + (ESPACO + ph) * (py++));
    py++;
    this->btnAbrirAutomato->move (px, lblw + ESPACO + (ESPACO + ph) * (py++));
    this->btnSalvarAutomato->move (px, lblw + ESPACO + (ESPACO + ph) * (py++));
    this->btnExecutarAutomato->move (px, lblw + ESPACO + (ESPACO + ph) * (py++));
    this->btnLimparAutomato->move (px, lblw + ESPACO + (ESPACO + ph) * (py++));
    py++;
    this->btnAjuda->move (px, lblw + ESPACO + (ESPACO + ph) * (py++));
    this->btnSair->move (px, lblw + ESPACO + (ESPACO + ph) * (py++));

    this->edicao->resize (px - ESPACO - ESPACO, ev->size().height() - ESPACO - ESPACO);
    this->setMinimumSize (pw * 2 + ESPACO * 3, ph * 15 + ESPACO * 16 + lblw);
}

// Ja chegou a hora... 16/04/2006 02h16min
// FELIZ PASCOA PARA TODOS!!!

void mainwindow::keyPressEvent (QKeyEvent *ev) {
    if (ev->key() == Qt::Key_Delete && (ev->modifiers() & Qt::ControlModifier)) {
        // Limpar completamente o automato
        this->btnLimparAutomato_clicked (false);
        ev->accept();
    } else if (ev->key() == Qt::Key_Delete) {
        // Apagar um objeto selecionado
        this->btnExcluir_clicked (false);
        ev->accept();
    } else if (ev->key() == Qt::Key_N && (ev->modifiers() & Qt::ControlModifier)) {
        // Criar um novo estado
        this->btnNovoEstado_clicked (false);
        ev->accept();
    } else if (ev->key() == Qt::Key_L && (ev->modifiers() & Qt::ControlModifier)) {
        // Criar uma nova transicao
        this->btnNovaTransicao_clicked (false);
        ev->accept();
    } else if (ev->key() == Qt::Key_I && (ev->modifiers() & Qt::ControlModifier)) {
        // Marcar um estado como inicial
        this->btnMarcaInicial_clicked (false);
        ev->accept();
    } else if (ev->key() == Qt::Key_E && (ev->modifiers() & Qt::ControlModifier)) {
        // Editar o alfabeto utilizado no automato
        this->btnEditarAlfabeto_clicked (false);
        ev->accept();
    } else if (ev->key() == Qt::Key_F && (ev->modifiers() & Qt::ControlModifier)) {
        // Marcar um estado como final
        this->btnMarcaFinal_clicked (false);
        ev->accept();
    } else if (ev->key() == Qt::Key_S && (ev->modifiers() & Qt::ControlModifier)) {
        // Salvar o automato em um arquivo
        this->btnSalvarAutomato_clicked (false);
        ev->accept();
    } else if (ev->key() == Qt::Key_A && (ev->modifiers() & Qt::ControlModifier)) {
        // Carregar um automato de um arquivo
        this->btnAbrirAutomato_clicked (false);
        ev->accept();
    } else if (ev->key() == Qt::Key_R && (ev->modifiers() & Qt::ControlModifier)) {
        // Executar o automato
        this->btnExecutarAutomato_clicked (false);
        ev->accept();
    } else if (ev->key() == Qt::Key_F1) {
        // Box de ajuda
        this->btnAjuda_clicked (false);
        ev->accept();
    } else if (ev->key() == Qt::Key_Escape) {
        // Sair do programa
        this->close ();
        ev->accept();
    } else {
        ev->ignore();
    }
}

void mainwindow::btnNovoEstado_clicked (bool) {
    this->edwindow->processaCTRL_N();
}

void mainwindow::btnNovaTransicao_clicked (bool) {
    this->edwindow->processaCTRL_L();
}

void mainwindow::btnExcluir_clicked (bool) {
    this->edwindow->processaDEL();
}

void mainwindow::btnMarcaInicial_clicked (bool) {
    this->edwindow->processaCTRL_I();
}

void mainwindow::btnMarcaFinal_clicked (bool) {
    this->edwindow->processaCTRL_F();
}

void mainwindow::btnEditarAlfabeto_clicked (bool) {
    this->edwindow->processaCTRL_E();
    this->setTxtLabel ();
}

void mainwindow::btnSalvarAutomato_clicked (bool) {
    this->edwindow->processaCTRL_S();
}

void mainwindow::btnAbrirAutomato_clicked (bool) {
    this->edwindow->processaCTRL_A();
    this->setTxtLabel ();
}

void mainwindow::btnExecutarAutomato_clicked (bool) {
    execautomatos* e = new execautomatos (this, 0, this->edwindow->aut);
    e->resize (640, 480);
    e->exec ();
    delete (e);
}

void mainwindow::btnLimparAutomato_clicked (bool) {
    this->edwindow->processaCTRL_DEL();
    this->setTxtLabel ();
}

void mainwindow::btnAjuda_clicked (bool) {
    /* Vou ter que concordar... Fazer documentacao nao eh comigo!!! */
    QMessageBox::information ((QWidget*)0, QString(TITULO) + " - Ajuda", QString ("") +

    "<center>****    " + TITULO + "    ****    " + "<br>\n" +
    "Desenvolvido por Anderson Medeiros Gomes - amg1127-gmail-com" + "</center><br><br>\n\n" +
    "Este programa foi desenvolvido para permitir a criacao e o processamento de automatos finitos deterministicos, nao-deterministicos ou com movimentos vazios.<br><br>" +
    "Utilize as teclas de atalho ou os botoes que voce ve ao lado direito da tela para desenhar o automato. Apos, use o botao 'Executar automato' para abrir o 'Assistente de execucao de Automatos'.<br><br><hr><br>" +
    "Este programa foi feito com a biblioteca QT versao 4.x. Para saber mais, visite <a href='http://www.trolltech.com/' target='_blank'>http://www.trolltech.com/</a>."

    , QMessageBox::Ok);
}

void mainwindow::btnSair_clicked (bool) {
    this->close ();
}

void mainwindow::closeEvent (QCloseEvent *ev) {
    if (QMessageBox::question ((QWidget*)0, QString(TITULO) + " - Sair", "Tem certeza que deseja sair do programa?", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
        ev->accept();
    } else {
        ev->ignore();
    }
}

#include "main.moc"

int main (int argc, char *argv[]) {
    QApplication app (argc, argv);
    mainwindow w;
    return (app.exec());
}
