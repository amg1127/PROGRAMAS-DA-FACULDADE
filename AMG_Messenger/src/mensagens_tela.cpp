#include "mensagens_tela.h"

mensagens_tela::mensagens_tela (QWidget *pai) {
    DEBUGA ("mensagens_tela::mensagens_tela (QWidget *);");
    int i;
    for (i=0; i<MAX_ENTRADAS; i++)
        this->linhas_tela[i] = NULL;
    if (pai != NULL)
        this->reparent(pai, QPoint(0,0));
    this->indicador_linha = 0;
    this->tam_lista_usuarios = 0;
    this->ultimo_top = 10;
    this->comotava = true;
    NOVO_OBJETO (this->lblUsuarios, QLabel (this));
    NOVO_OBJETO (this->lstUsuarios, QListBox (this));
    NOVO_OBJETO (this->qsvMensagens, QScrollView (this));
    this->lblUsuarios->setText ("Usuarios na sala:");
    this->lblUsuarios->setAlignment (Qt::AlignHCenter);
    this->lblUsuarios->adjustSize();
    this->qsvMensagens->move (0, 0);
    this->lblUsuarios->move (0, 0);
    this->llu = 150;
    this->qsvMensagens->viewport()->setBackgroundMode(Qt::PaletteBase);
    this->qsvMensagens->setHScrollBarMode (QScrollView::AlwaysOff);
    this->qsvMensagens->enableClipper (true);
    QObject::connect ((this->qsvMensagens->verticalScrollBar()), SIGNAL(valueChanged(int)), this, SLOT (qsvMensagensValueChanged(int)));
    this->qsvMensagens->show ();
    this->lblUsuarios->show();
    this->lstUsuarios->show();
}

void mensagens_tela::qsvMensagensValueChanged (int valor) {
    DEBUGA ("void mensagens_tela::qsvMensagensValueChanged (int);");
    this->comotava = (this->qsvMensagens->verticalScrollBar()->maxValue () == valor || this->qsvMensagens->verticalScrollBar()->isHidden());
}

void mensagens_tela::rola_se_tava(void) {
    int issoai;
    DEBUGA ("void mensagens_tela::rola_se_tava(void);");
    this->comotava |= this->qsvMensagens->verticalScrollBar()->isHidden();
    this->ultimo_top = 10;
    for (issoai=0; issoai<MAX_ENTRADAS; this->prox_linha(), issoai++)
        if (this->linha_atual() != NULL) {
            this->linha_atual()->setFixedWidth (this->qsvMensagens->size().width()-20-this->qsvMensagens->verticalScrollBar()->size().width());
            this->linha_atual()->setFixedHeight (this->linha_atual()->contentsHeight());
            this->qsvMensagens->addChild(this->linha_atual(), 10, this->ultimo_top);
            this->ultimo_top += this->linha_atual()->size().height() + 10;
        }
    this->qsvMensagens->resizeContents (this->qsvMensagens->size().width(), this->ultimo_top);
    if (this->comotava)
        this->qsvMensagens->setContentsPos(0, this->ultimo_top);
}

mensagens_tela::~mensagens_tela () {
    DEBUGA ("mensagens_tela::~mensagens_tela ();");
    for (this->ante_linha(); this->linha_atual() != NULL; this->ante_linha()) {
        this->qsvMensagens->removeChild (this->linha_atual());
        APAGA_OBJETO_SEM_NULL (this->linha_atual());
    }
    APAGA_OBJETO (this->lblUsuarios);
    APAGA_OBJETO (this->lstUsuarios);
    APAGA_OBJETO (this->qsvMensagens);
}

int mensagens_tela::EstaNaConversa (QString usr) {
    DEBUGA ("bool mensagens_tela::ExisteNaSala (QString);");
    int i;
    for (i=0; i<this->tam_lista_usuarios; i++)
        if (this->lista_usuarios_Nome[i] == usr)
            return (i);
    return (-1);
}

void mensagens_tela::MensagemEntraNaConversa (QString Nome, QFont Estilo, QColor Cor) {
    DEBUGA ("void MensagemEntraNaConversa (QString, QFont, QColor);");
    this->PoeMensagem (Nome + " entra na conversa.", Estilo, Cor);
}

void mensagens_tela::MensagemEntraNaConversa (int usr) {
    DEBUGA ("void MensagemEntraNaConversa (int);");
    if (usr >= 0 && usr < this->tam_lista_usuarios)
        this->MensagemEntraNaConversa (this->lista_usuarios_Nome[usr], this->lista_usuarios_Estilo[usr], this->lista_usuarios_Cor[usr]);
    else
        DEBUGA ("Aviso ao cara que pensa que eh programador: variavel 'usr' tem valor incorreto.");
}

bool mensagens_tela::EntraNaConversa (QString Nome, QFont Estilo, QColor Cor) {
    DEBUGA ("bool mensagens_tela::EntraNaConversa (QString, QFont, QColor);");
    QListBoxText *novoitem;
    if (this->tam_lista_usuarios == MAX_USUARIOS)
        return (false);
    else if (this->EstaNaConversa (Nome) >= 0)
        return (false);
    this->lista_usuarios_Nome[this->tam_lista_usuarios] = Nome;
    this->lista_usuarios_Estilo[this->tam_lista_usuarios] = Estilo;
    this->lista_usuarios_Cor[this->tam_lista_usuarios] = Cor;
    this->tam_lista_usuarios++;
    novoitem = new QListBoxText (this->lstUsuarios, Nome);
    return (true);
}

void mensagens_tela::MensagemSaiDaConversa (QString Nome, QFont Estilo, QColor Cor) {
    DEBUGA ("void MensagemSaiDaConversa (QString, QFont, QColor);");
    this->PoeMensagem (Nome + " sai da conversa.", Estilo, Cor);
}

void mensagens_tela::MensagemSaiDaConversa (int usr) {
    DEBUGA ("void MensagemSaiDaConversa (int);");
    if (usr >= 0 && usr < this->tam_lista_usuarios)
        this->MensagemSaiDaConversa (this->lista_usuarios_Nome[usr], this->lista_usuarios_Estilo[usr], this->lista_usuarios_Cor[usr]);
    else
        DEBUGA ("Aviso ao cara que pensa que eh programador: variavel 'usr' tem valor incorreto.");
}

bool mensagens_tela::SaiDaConversa (int pos, QString& Nome, QFont& Estilo, QColor& Cor) {
    DEBUGA ("bool mensagens_tela::SaiDaConversa (int, QString&, QFont&, QColor&);");
    if (pos < 0 || pos >= this->tam_lista_usuarios) {
        DEBUGA ("Aviso ao cara que pensa que eh programador: variavel 'pos' tem valor incorreto.");
        return (false);
    }
    Nome = this->lista_usuarios_Nome[pos];
    Estilo = this->lista_usuarios_Estilo[pos];
    Cor = this->lista_usuarios_Cor[pos];
    return (this->SaiDaConversa (Nome));
}

bool mensagens_tela::SaiDaConversa (QString usr) {
    DEBUGA ("bool mensagens_tela::SaiDaConversa (QString);");
    int i, i2;
    for (i=0; i<this->tam_lista_usuarios; i++)
        if (this->lista_usuarios_Nome[i] == usr) {
            for (i2 = i+1; i2 < this->tam_lista_usuarios; i2++) {
                this->lista_usuarios_Nome[i2-1] = this->lista_usuarios_Nome[i2];
                this->lista_usuarios_Estilo[i2-1] = this->lista_usuarios_Estilo[i2];
                this->lista_usuarios_Cor[i2-1] = this->lista_usuarios_Cor[i2];
            }
            this->tam_lista_usuarios--;
            this->lstUsuarios->removeItem (i);
            return (true);
        }
    return (false);
}

void mensagens_tela::EscreveMensagem (QString Nome, QFont Estilo, QColor Cor, QString msg) {
    DEBUGA ("void EscreveMensagem (QString, QFont, QColor, QString);");
    this->PoeMensagem (Nome + " diz:\n" + msg, Estilo, Cor);
}

void mensagens_tela::EscreveMensagem (int usr, QString msg) {
    DEBUGA ("void EscreveMensagem (int, QString);");
    if (usr >= 0 && usr < this->tam_lista_usuarios)
        this->EscreveMensagem (this->lista_usuarios_Nome[usr], this->lista_usuarios_Estilo[usr], this->lista_usuarios_Cor[usr], msg);
    else
        DEBUGA ("Aviso ao cara que pensa que eh programador: variavel 'usr' tem valor incorreto.");
}

int mensagens_tela::QuantosConversam (void) {
    DEBUGA ("int mensagens_tela::QuantosConversam (void);");
    return (this->tam_lista_usuarios);
}

bool mensagens_tela::UsuarioNaConversa (int pos, QString& Nome, QFont& Estilo, QColor& Cor) {
    DEBUGA ("bool mensagens_tela::UsuarioNaConversa (int, QString&, QFont&, QColor&);");
    if (pos < 0 || pos >= this->tam_lista_usuarios)
        return (false);
    Nome = this->lista_usuarios_Nome[pos];
    Estilo = this->lista_usuarios_Estilo[pos];
    Cor = this->lista_usuarios_Cor[pos];
    return (true);
}

void mensagens_tela::PoeMensagem (QString msg, QFont Estilo, QColor Cor) {
    DEBUGA ("void mensagens_tela::PoeMensagem (QString, QFont, QColor);");
    QWidget *dentro;
    QPoint pon;
    int reducao, issoai;
    if (this->linha_atual() == NULL) {
        dentro = this->qsvMensagens->viewport ();
        NOVO_OBJETO (this->linhas_tela[this->indicador_linha], QTextEdit(dentro));
        this->linha_atual()->show();
        this->linha_atual()->setTextFormat (Qt::PlainText);
        this->linha_atual()->setHScrollBarMode (QScrollView::AlwaysOff);
        this->linha_atual()->setVScrollBarMode (QScrollView::AlwaysOff);
        this->linha_atual()->setMargin (0);
        this->linha_atual()->setFocusProxy (this->qsvMensagens);
        this->linha_atual()->setFrameShape (QFrame::NoFrame);
    } else {
        reducao = 10 + this->linha_atual()->size().height();
        this->ultimo_top -= reducao;
        this->prox_linha();
        for (issoai=1; issoai<MAX_ENTRADAS; issoai++, this->prox_linha()) {
            pon = this->linha_atual()->pos();
            pon.setY(pon.y() - reducao);
            this->linha_atual()->move (pon);
        }
    }
    this->linha_atual()->setFont (Estilo);
    this->linha_atual()->setPaletteForegroundColor (Cor);
    this->linha_atual()->setText (QString ("[") + QTime::currentTime().toString(QString("hh:mm:ss")) + "] " + msg);
    this->prox_linha();
    this->rola_se_tava();
}

void mensagens_tela::resizeEvent (QResizeEvent *ev) {
    int larg;
    QPoint poilblUsuarios, poilstUsuarios, poiqsvMensagens;
    QSize sizlblUsuarios, sizlstUsuarios, sizqsvMensagens;
    DEBUGA ("void resizeEvent (QResizeEvent *);");
    poilblUsuarios = this->lblUsuarios->pos();
    sizlblUsuarios = this->lblUsuarios->size();
    poilstUsuarios = this->lstUsuarios->pos();
    sizlstUsuarios = this->lstUsuarios->size();
    poiqsvMensagens = this->qsvMensagens->pos();
    sizqsvMensagens = this->qsvMensagens->size();
    larg = ev->size().width();
    if (larg > this->llu)
        larg = this->llu;
    sizlblUsuarios.setWidth (larg);
    sizlstUsuarios.setWidth (larg);
    this->lblUsuarios->resize (sizlblUsuarios);
    sizlblUsuarios = this->lblUsuarios->size();
    poilblUsuarios.setX (ev->size().width() - sizlblUsuarios.width());
    poilstUsuarios.setX (poilblUsuarios.x());
    sizqsvMensagens.setWidth (poilblUsuarios.x() - 10);
    sizqsvMensagens.setHeight (ev->size().height());
    poilstUsuarios.setY (sizlblUsuarios.height());
    sizlstUsuarios.setHeight(sizqsvMensagens.height() - sizlblUsuarios.height());
    this->lblUsuarios->move (poilblUsuarios);
    this->lblUsuarios->resize (sizlblUsuarios);
    this->lstUsuarios->move (poilstUsuarios);
    this->lstUsuarios->resize (sizlstUsuarios);
    this->qsvMensagens->move (poiqsvMensagens);
    this->qsvMensagens->resize (sizqsvMensagens);
    this->rola_se_tava ();
}

void mensagens_tela::defLarguraListaUsuarios (int l) {
    DEBUGA ("void mensagens_tela::defLarguraListaUsuarios (QSize);");
    this->llu = l;
    this->resize (this->size());
}
