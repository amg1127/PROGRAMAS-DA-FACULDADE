// configuracao.cpp
#include "configuracao.h"

bool configuracao::cfgModoObtencaoConexao = true;
QString configuracao::cfgCaminhoExecutavel = "single/single";
QString configuracao::cfgIpDoServidor = "h3p.g3pd.ucpel.tche.br";
int configuracao::cfgPortaTCP = 1111;
int configuracao::cfgEscolheLargura = 640;
int configuracao::cfgEscolheAltura = 480;
bool configuracao::cfgEscolheDimAutomatico = true;
int configuracao::cfgNoIteracoes = 1000;
bool configuracao::cfgAtualizacaoAutomatica = false; // Isto aqui nao pode ser true por padrao. NAO ESQUECER!

void configuracao::mostra_paleta () {
    int i, l, pos, h, k, n;
    QPixmap *pixVelho = NULL;
    QColor cor;
    if (this->visualizar_paleta->width ()  != this->mostraPaletaDeCores->width () ||
        this->visualizar_paleta->height () != this->mostraPaletaDeCores->height ()) {
        pixVelho = this->visualizar_paleta;
        NOVO_OBJETO (this->visualizar_paleta, QPixmap (this->mostraPaletaDeCores->width (), this->mostraPaletaDeCores->height ()));
    }
    {
        QPainter pinta (this->visualizar_paleta);
        l = this->mostraPaletaDeCores->width ();
        h = this->visualizar_paleta->height ();
        n = this->paletaDeCoresReal->count ();
        k = l - 1;
        if (k == 0) k = 1;
        for (i = 0; i < l; i++) {
            pos = i * n / k;
            if (pos >= n) pos = n - 1;
            cor = this->paletaDeCoresReal->item(pos)->backgroundColor ();
            pinta.setBrush (cor);
            pinta.setPen (cor);
            pinta.drawLine (i, 0, i, h);
        }
    }
    this->mostraPaletaDeCores->setPixmap (*(this->visualizar_paleta));
    if (pixVelho != NULL)
        APAGA_OBJETO (pixVelho);
}

configuracao::~configuracao () {
    APAGA_OBJETO (this->mostraPaletaDeCores);
    // Destrutor de QWidget ira destruir os objetos-filhos que eu criei... Nao preciso fazer isso.
}

configuracao::configuracao (visualizacao *v) : QDialog (v, Qt::Dialog) {
    QListWidgetItem *tmp;
    int tmp2, tmp3;
    QColor branco (255, 255, 255), preto (0, 0, 0);

    this->setAttribute (Qt::WA_ShowModal, true);
    this->setWindowTitle ("Configuracao do programa");

    NOVO_OBJETO (this->modoObtencao, QGroupBox (this));
    this->modoObtencao->setTitle ("Modo de obtencao de fractal");
    this->modoObtencao->show ();

    NOVO_OBJETO (this->modoObtencaoExecucao, QRadioButton (this->modoObtencao));
    this->modoObtencaoExecucao->setText ("Execucao de um programa");
    this->modoObtencaoExecucao->show ();
    this->modoObtencaoExecucao->adjustSize ();

    NOVO_OBJETO (this->caminhoExecutavel, QLabel (this->modoObtencao));
    this->caminhoExecutavel->setText ("Caminho do executavel: ");
    this->caminhoExecutavel->show ();
    this->caminhoExecutavel->adjustSize ();

    NOVO_OBJETO (this->txtCaminhoExecutavel, QLineEdit (this->modoObtencao));
    this->txtCaminhoExecutavel->show ();

    NOVO_OBJETO (this->procurarExecutavel, QPushButton (this->modoObtencao));
    this->procurarExecutavel->setText ("Procurar...");
    this->procurarExecutavel->show ();
    this->procurarExecutavel->adjustSize ();

    NOVO_OBJETO (this->modoObtencaoConexao, QRadioButton (this->modoObtencao));
    this->modoObtencaoConexao->setText ("Conexao a um servidor");
    this->modoObtencaoConexao->show ();
    this->modoObtencaoConexao->adjustSize ();

    NOVO_OBJETO (this->ipDoServidor, QLabel (this->modoObtencao));
    this->ipDoServidor->setText ("Nome ou endereco IP do servidor: ");
    this->ipDoServidor->show ();
    this->ipDoServidor->adjustSize ();

    NOVO_OBJETO (this->txtIpDoServidor, QLineEdit (this->modoObtencao));
    this->txtIpDoServidor->show ();

    NOVO_OBJETO (this->portaTCP, QLabel (this->modoObtencao));
    this->portaTCP->setText ("Porta: ");
    this->portaTCP->show ();
    this->portaTCP->adjustSize ();

    NOVO_OBJETO (this->txtPortaTCP, QSpinBox (this->modoObtencao));
    this->txtPortaTCP->setRange (1, 65535);
    this->txtPortaTCP->show ();

    NOVO_OBJETO (this->outrasconfigs, QGroupBox (this));
    this->outrasconfigs->setTitle ("Configuracoes do fractal");
    this->outrasconfigs->show ();

    NOVO_OBJETO (this->escolheLargura, QLabel (this->outrasconfigs));
    this->escolheLargura->setText ("Largura do fractal (px): ");
    this->escolheLargura->show ();
    this->escolheLargura->adjustSize ();

    NOVO_OBJETO (this->txtEscolheLargura, QSpinBox (this->outrasconfigs));
    this->txtEscolheLargura->setRange (1, 100000);
    this->txtEscolheLargura->show ();

    NOVO_OBJETO (this->escolheAltura, QLabel (this->outrasconfigs));
    this->escolheAltura->setText ("Altura do fractal (px): ");
    this->escolheAltura->show ();
    this->escolheAltura->adjustSize ();

    NOVO_OBJETO (this->escolheDimAutomatico, QCheckBox (this->outrasconfigs));
    this->escolheDimAutomatico->setText ("Ajustar tamanho da imagem conforme o tamanho da janela");
    this->escolheDimAutomatico->show ();
    this->escolheDimAutomatico->adjustSize ();

    NOVO_OBJETO (this->txtEscolheAltura, QSpinBox (this->outrasconfigs));
    this->txtEscolheAltura->setRange (1, 100000);
    this->txtEscolheAltura->show ();

    NOVO_OBJETO (this->noIteracoes, QLabel (this->outrasconfigs));
    this->noIteracoes->setText ("No. maximo de iteracoes: ");
    this->noIteracoes->show ();
    this->noIteracoes->adjustSize ();

    NOVO_OBJETO (this->txtNoIteracoes, QSpinBox (this->outrasconfigs));
    this->txtNoIteracoes->setRange (1, 100000);
    this->txtNoIteracoes->show ();

    NOVO_OBJETO (this->paletaDeCores, QLabel (this->outrasconfigs));
    this->paletaDeCores->setText ("Paleta de cores: ");
    this->paletaDeCores->show ();
    this->paletaDeCores->adjustSize ();

    NOVO_OBJETO (this->mostraPaletaDeCores, QLabel (this->outrasconfigs));
    this->mostraPaletaDeCores->show ();

    NOVO_OBJETO (this->modificaPaletaDeCores, QPushButton (this->outrasconfigs));
    this->modificaPaletaDeCores->setText ("Modificar...");
    this->modificaPaletaDeCores->show ();
    this->modificaPaletaDeCores->adjustSize ();

    NOVO_OBJETO (this->atualizacaoAutomatica, QCheckBox (this->outrasconfigs));
    this->atualizacaoAutomatica->setText ("Recarregar imagem automaticamente, quando necessario");
    this->atualizacaoAutomatica->show ();
    this->atualizacaoAutomatica->adjustSize ();

    NOVO_OBJETO (this->btnOk, QPushButton (this));
    this->btnOk->setText ("OK");
    this->btnOk->show ();
    this->btnOk->adjustSize ();

    NOVO_OBJETO (this->btnCancelar, QPushButton (this));
    this->btnCancelar->setText ("Cancelar");
    this->btnCancelar->show ();
    this->btnCancelar->adjustSize ();

    tmp2 = maior2 (this->btnOk->width(), this->btnCancelar->width());
    tmp3 = maior2 (this->btnOk->height(), this->btnCancelar->height());

    this->btnOk->resize (tmp2, tmp3);
    this->btnCancelar->resize (tmp2, tmp3);

    NOVO_OBJETO (this->paletaDeCoresReal, QListWidget (this));
    this->paletaDeCoresReal->hide ();

    NOVO_OBJETO (tmp, QListWidgetItem (this->paletaDeCoresReal));
    tmp->setBackgroundColor (QColor (255, 0, 0));
    tmp->setTextAlignment (Qt::AlignCenter);
    tmp->setTextColor (branco);
    NOVO_OBJETO (tmp, QListWidgetItem (this->paletaDeCoresReal));
    tmp->setBackgroundColor (QColor (0, 255, 0));
    tmp->setTextAlignment (Qt::AlignCenter);
    tmp->setTextColor (branco);
    NOVO_OBJETO (tmp, QListWidgetItem (this->paletaDeCoresReal));
    tmp->setBackgroundColor (QColor (0, 0, 255));
    tmp->setTextAlignment (Qt::AlignCenter);
    tmp->setTextColor (branco);
    NOVO_OBJETO (tmp, QListWidgetItem (this->paletaDeCoresReal));
    tmp->setBackgroundColor (QColor (255, 255, 0));
    tmp->setTextAlignment (Qt::AlignCenter);
    tmp->setTextColor (preto);
    NOVO_OBJETO (tmp, QListWidgetItem (this->paletaDeCoresReal));
    tmp->setBackgroundColor (QColor (0, 255, 255));
    tmp->setTextAlignment (Qt::AlignCenter);
    tmp->setTextColor (preto);
    NOVO_OBJETO (tmp, QListWidgetItem (this->paletaDeCoresReal));
    tmp->setBackgroundColor (QColor (255, 0, 255));
    tmp->setTextAlignment (Qt::AlignCenter);
    tmp->setTextColor (preto);
    NOVO_OBJETO (tmp, QListWidgetItem (this->paletaDeCoresReal));
    tmp->setBackgroundColor (QColor (255, 255, 255));
    tmp->setTextAlignment (Qt::AlignCenter);
    tmp->setTextColor (preto);
    NOVO_OBJETO (tmp, QListWidgetItem (this->paletaDeCoresReal));
    tmp->setBackgroundColor (QColor (0, 0, 0));
    tmp->setTextAlignment (Qt::AlignCenter);
    tmp->setTextColor (branco);

    this->txtCaminhoExecutavel->resize (largura_LineEdit, this->txtCaminhoExecutavel->height());
    this->txtIpDoServidor->resize (largura_LineEdit, this->txtIpDoServidor->height ());
    this->mostraPaletaDeCores->resize (largura_LineEdit, this->paletaDeCores->height ());
    this->txtEscolheLargura->resize (largura_SpinBox, this->txtEscolheLargura->height ());
    this->txtEscolheAltura->resize (largura_SpinBox, this->txtEscolheAltura->height ());
    this->txtPortaTCP->resize (largura_SpinBox, this->txtPortaTCP->height ());
    this->txtNoIteracoes->resize (largura_SpinBox, this->txtNoIteracoes->height ());

    this->modoObtencao->move (20, 20);
    this->modoObtencaoExecucao->move (20, 20);
    tmp2 = embaixo(this->modoObtencaoExecucao) + 4 + maior3 (this->caminhoExecutavel->height(), this->txtCaminhoExecutavel->height(), this->procurarExecutavel->height());
    this->caminhoExecutavel->move (40, tmp2 - this->caminhoExecutavel->height());
    this->txtCaminhoExecutavel->move (10 + direita(this->caminhoExecutavel), tmp2 - this->txtCaminhoExecutavel->height());
    this->procurarExecutavel->move (10 + direita(this->txtCaminhoExecutavel), tmp2 - this->procurarExecutavel->height());
    tmp2 += 20;
    this->modoObtencaoConexao->move (20, tmp2);
    tmp2 += 4 + this->modoObtencaoConexao->height () + maior2 (this->ipDoServidor->height(), this->txtIpDoServidor->height());
    this->ipDoServidor->move (40, tmp2 - this->ipDoServidor->height());
    this->txtIpDoServidor->move (direita(this->ipDoServidor) + 10, tmp2 - this->txtIpDoServidor->height());
    tmp2 += 4 + maior2 (this->portaTCP->height(), this->txtPortaTCP->height());
    this->portaTCP->move (40, tmp2 - this->portaTCP->height ());
    this->txtPortaTCP->move (10 + direita(this->portaTCP), tmp2 - this->txtPortaTCP->height());
    this->modoObtencao->resize (this->modoObtencao->width(), tmp2 + 20);

    tmp2 = embaixo (this->modoObtencao) + 20;
    this->outrasconfigs->move (20, tmp2);
    tmp2 = 20 + maior2 (this->escolheLargura->height(), this->txtEscolheLargura->height());
    this->escolheLargura->move (20, tmp2 - this->escolheLargura->height());
    this->txtEscolheLargura->move (10 + direita(this->escolheLargura), tmp2 - this->txtEscolheLargura->height());
    tmp2 += 4 + maior2 (this->escolheAltura->height(), this->txtEscolheAltura->height());
    this->escolheAltura->move (20, tmp2 - this->escolheAltura->height());
    this->txtEscolheAltura->move (10 + direita(this->escolheAltura), tmp2 - this->txtEscolheAltura->height());
    tmp2 += 4;
    this->escolheDimAutomatico->move (20, tmp2);
    tmp2 = 20 + embaixo(this->escolheDimAutomatico) + maior2 (this->noIteracoes->height (), this->txtNoIteracoes->height ());
    this->noIteracoes->move (20, tmp2 - this->noIteracoes->height ());
    this->txtNoIteracoes->move (10 + direita (this->noIteracoes), tmp2 - this->txtNoIteracoes->height ());
    tmp2 += 4 + maior3 (this->paletaDeCores->height (), this->mostraPaletaDeCores->height (), this->modificaPaletaDeCores->height ());
    this->paletaDeCores->move (20, tmp2 - this->paletaDeCores->height());
    this->mostraPaletaDeCores->move (10 + direita (this->paletaDeCores), tmp2 - this->mostraPaletaDeCores->height ());
    this->modificaPaletaDeCores->move (10 + direita (this->mostraPaletaDeCores), tmp2 - this->modificaPaletaDeCores->height());
    this->atualizacaoAutomatica->move (20, tmp2 + 20);

    tmp2 = maior5 (direita (this->modoObtencaoExecucao),
                   direita (this->procurarExecutavel),
                   direita (this->modoObtencaoConexao),
                   direita (this->txtIpDoServidor),
                   direita (this->txtPortaTCP));
    tmp2 = maior4 (tmp2,
                   direita (this->txtNoIteracoes),
                   direita (this->modificaPaletaDeCores),
                   direita (this->atualizacaoAutomatica));
    tmp2 += 20;
    this->modoObtencao->resize (tmp2, this->modoObtencao->height ());
    this->outrasconfigs->resize (tmp2,  20 + embaixo (this->atualizacaoAutomatica));

    tmp2 = (direita (this->outrasconfigs) >> 1) - this->btnOk->width ();
    tmp3 = embaixo (this->outrasconfigs) + 40;
    this->btnOk->move (tmp2, tmp3);
    this->btnCancelar->move (tmp2 + 40 + this->btnCancelar->width (), tmp3);

    this->setFixedSize (direita (this->outrasconfigs) + 20, embaixo (this->btnOk) + 20);

    NOVO_OBJETO (this->visualizar_paleta, QPixmap (this->mostraPaletaDeCores->width (), this->mostraPaletaDeCores->height ()));

    NOVO_OBJETO (this->edict, editor_paleta (this));

    this->loadConfig ();
    this->mostra_paleta ();

    QObject::connect (this->modificaPaletaDeCores, SIGNAL (clicked()), this, SLOT(modificaPaletaDeCores_clicked()));
    QObject::connect (this->modoObtencaoExecucao, SIGNAL(toggled(bool)), this, SLOT(modoObtencaoExecucao_toggled(bool)));
    QObject::connect (this->modoObtencaoConexao, SIGNAL(toggled(bool)), this, SLOT(modoObtencaoConexao_toggled(bool)));
    QObject::connect (this->procurarExecutavel, SIGNAL(clicked()), this, SLOT(procurarExecutavel_clicked()));
    QObject::connect (this->escolheDimAutomatico, SIGNAL(toggled(bool)), this, SLOT(escolheDimAutomatico_toggled(bool)));
    QObject::connect (this->btnOk, SIGNAL(clicked()), this, SLOT(btnOk_clicked()));
    QObject::connect (this->btnCancelar, SIGNAL(clicked()), this, SLOT(btnCancelar_clicked()));
}

void configuracao::modificaPaletaDeCores_clicked () {
    edict->loadConfig ();
    edict->exec ();
    this->mostra_paleta ();
}

void configuracao::modoObtencaoExecucao_toggled (bool) {
    this->modoObtencaoConexao_toggled (false);
}

void configuracao::modoObtencaoConexao_toggled (bool valor) {
    this->ipDoServidor->setEnabled (valor);
    this->txtIpDoServidor->setEnabled (valor);
    this->portaTCP->setEnabled (valor);
    this->txtPortaTCP->setEnabled (valor);
    valor = !valor;
    this->caminhoExecutavel->setEnabled (valor);
    this->txtCaminhoExecutavel->setEnabled (valor);
    this->procurarExecutavel->setEnabled (valor);
}

void configuracao::procurarExecutavel_clicked () {
    QString ab = QFileDialog::getOpenFileName (this, this->windowTitle() + " - Abrir...", this->txtCaminhoExecutavel->text(), QString(), 0, 0);
    if (ab.isEmpty()) return;
    this->txtCaminhoExecutavel->setText(ab);
}

void configuracao::escolheDimAutomatico_toggled (bool valor) {
    valor = !valor;
    this->escolheLargura->setEnabled (valor);
    this->escolheAltura->setEnabled (valor);
    this->txtEscolheLargura->setEnabled (valor);
    this->txtEscolheAltura->setEnabled (valor);
}

void configuracao::btnOk_clicked () {
    if (this->modoObtencaoConexao->isChecked ()) {
        if (this->txtIpDoServidor->text().trimmed().isEmpty ()) {
            this->txtIpDoServidor->setFocus ();
            QMessageBox::warning (this, this->windowTitle () + " - Erro", "Voce deve especificar o nome ou o endereco IP do servidor!", QMessageBox::Ok, QMessageBox::NoButton);
            return;
        }
        this->cfgModoObtencaoConexao = true;
    } else {
        if (this->txtCaminhoExecutavel->text().trimmed().isEmpty ()) {
            this->txtCaminhoExecutavel->setFocus ();
            QMessageBox::warning (this, this->windowTitle () + " - Erro", "Voce deve especificar o caminho do arquivo executavel!", QMessageBox::Ok, QMessageBox::NoButton);
            return;
        }
        this->cfgModoObtencaoConexao = false;
    }
    this->cfgCaminhoExecutavel = this->txtCaminhoExecutavel->text();
    this->cfgIpDoServidor = this->txtIpDoServidor->text();
    this->cfgPortaTCP = this->txtPortaTCP->value ();
    this->cfgEscolheLargura = this->txtEscolheLargura->value ();
    this->cfgEscolheAltura = this->txtEscolheAltura->value ();
    this->cfgEscolheDimAutomatico = this->escolheDimAutomatico->isChecked ();
    this->cfgNoIteracoes = this->txtNoIteracoes->value ();
    this->cfgAtualizacaoAutomatica = this->atualizacaoAutomatica->isChecked ();
    this->close ();
}

void configuracao::btnCancelar_clicked () {
    this->close ();
}

void configuracao::loadConfig () {
    this->mexeuNaPaleta = false;
    if (this->cfgModoObtencaoConexao) {
        this->modoObtencaoConexao->setChecked (true);
        this->modoObtencaoConexao_toggled (true);
    } else {
        this->modoObtencaoExecucao->setChecked (true);
        this->modoObtencaoExecucao_toggled (true);
    }
    this->txtCaminhoExecutavel->setText (this->cfgCaminhoExecutavel);
    this->txtIpDoServidor->setText (this->cfgIpDoServidor);
    this->txtPortaTCP->setValue (this->cfgPortaTCP);
    this->txtEscolheLargura->setValue (this->cfgEscolheLargura);
    this->txtEscolheAltura->setValue (this->cfgEscolheAltura);
    this->escolheDimAutomatico->setChecked (this->cfgEscolheDimAutomatico);
    this->escolheDimAutomatico_toggled (this->cfgEscolheDimAutomatico);
    this->txtNoIteracoes->setValue (this->cfgNoIteracoes);
    this->atualizacaoAutomatica->setChecked (this->cfgAtualizacaoAutomatica);
}
// configuracao.cpp
