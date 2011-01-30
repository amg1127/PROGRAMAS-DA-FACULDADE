#include "messenger.h"

Messenger::Messenger (void) {
    DEBUGA ("Messenger::Messenger (void);");
    NOVO_OBJETO (this->lblTitulo, QLabel (this));
    NOVO_OBJETO (this->msgtela, mensagens_tela (this));
    NOVO_OBJETO (this->lblInfo, QLabel (this));
    NOVO_OBJETO (this->btnConectar, QPushButton (this));
    NOVO_OBJETO (this->btnServir, QPushButton (this));
    NOVO_OBJETO (this->txtMensEnviar, QTextEdit (this));
    NOVO_OBJETO (this->btnEnviar, QPushButton (this));
    NOVO_OBJETO (this->btnConfigurar, QPushButton (this));
    NOVO_OBJETO (this->comoVaoAsCoisas, QStatusBar (this));
    NOVO_OBJETO (this->tim, QTimer (this));
    this->comoVaoAsCoisas->setSizeGripEnabled (false);
    this->lblTitulo->setText (TITULO);
    this->lblTitulo->setFont (QFont (QString ("Verdana"), 24, QFont::Bold));
    this->lblTitulo->setAlignment (Qt::AlignCenter);
    this->lblTitulo->show ();
    this->txtMensEnviar->setReadOnly (false);
    this->txtMensEnviar->setTextFormat (Qt::PlainText);
    this->txtMensEnviar->resize (20, 60);
    this->txtMensEnviar->show ();
    this->lblInfo->show ();
    this->btnConectar->setText (QString ("&Conectar..."));
    this->btnConectar->show ();
    this->btnServir->setText (QString ("Serv&ir..."));
    this->btnServir->show ();
    this->btnEnviar->setText (QString ("&Enviar"));
    this->btnEnviar->show ();
    this->btnConfigurar->setText (QString ("C&onfigurar..."));
    this->btnConfigurar->show ();
    if (this->btnConfigurar->size().width() < this->btnEnviar->size().width())
        this->btnConfigurar->size().setWidth (this->btnEnviar->size().width());
    else
        this->btnEnviar->size().setWidth (this->btnConfigurar->size().width());
    if (this->btnConfigurar->size().height() < this->btnEnviar->size().height())
        this->btnConfigurar->size().setHeight (this->btnEnviar->size().height());
    else
        this->btnEnviar->size().setHeight (this->btnConfigurar->size().height());
    this->msgtela->defLarguraListaUsuarios (200);
    QObject::connect (this->btnConectar, SIGNAL(clicked()), this, SLOT (btnConectarClicked()));
    QObject::connect (this->btnServir, SIGNAL(clicked()), this, SLOT (btnServirClicked()));
    QObject::connect (this->btnEnviar, SIGNAL(clicked()), this, SLOT (btnEnviarClicked()));
    QObject::connect (this->btnConfigurar, SIGNAL(clicked()), this, SLOT (btnConfigurarClicked()));
    QObject::connect (this->txtMensEnviar, SIGNAL(returnPressed()), this, SLOT(txtMensEnviarReturnPressed()));
    QObject::connect (&(this->conex), SIGNAL(ComandoRecebido (QStringList)), this, SLOT (conexComandoRecebido (QStringList)));
    QObject::connect (&(this->conex), SIGNAL(ComandoExecutado (QStringList, QStringList)), this, SLOT (conexComandoExecutado (QStringList, QStringList)));
    QObject::connect (&(this->conex), SIGNAL(ErroComunicando (QString)), this, SLOT (conexErroComunicando (QString)));
    QObject::connect (&(this->conex), SIGNAL(hostFound()), this, SLOT (conexHostFound()));
    QObject::connect (&(this->conex), SIGNAL(connected()), this, SLOT (conexConnected()));
    QObject::connect (this->tim, SIGNAL(timeout()), this, SLOT (timerTimeout()));
    this->btnEnviar->setAccel (QKeySequence (Qt::CTRL | Qt::Key_Return));
    this->show ();
    this->setCaption (QString (TITULO));
    this->setFont (QFont (QString (""), 8));
    this->setMinimumSize (440, 250);
    this->btnEnviar->setEnabled (false);
    this->porta = PORTAPADRAO;
    this->servremoto = "";
    this->srv = NULL;
    this->pareceConectado = false;
    this->UsaCTRLEnter = true;
    this->btnConfigurarClicked();
    this->mensagemNaBarra (QString(TITULO), TEMPOPADRAO);
}

void Messenger::timerTimeout () {
    DEBUGA ("void Messenger::timerTimeout ();");
    if (this->conex.Estado() == QueEstado::Pronto)
        this->conex.Envia ("NADA");
}

void Messenger::mensagemNaBarra (QString s, int tempo) {
    DEBUGA ("void Messenger::mensagemNaBarra (QString, int);");
    this->comoVaoAsCoisas->message (s, tempo);
}

void Messenger::mensagemNaBarra (QString s) {
    DEBUGA ("void Messenger::mensagemNaBarra (QString);");
    this->comoVaoAsCoisas->message (s);
}

void Messenger::manda_conectar () {
    DEBUGA ("void Messenger::manda_conectar ();");
    this->pareceConectado = true;
    this->conex.connectToHost (this->servremoto, this->porta);
}

void Messenger::manda_desconectar () {
    QString n;
    QFont e;
    QColor c;
    DEBUGA ("void Messenger::manda_desconectar ();");
    this->conex.close ();
    this->tim->stop ();
    if (this->srv != NULL) {
        APAGA_OBJETO (this->srv);
    }
    if (this->pareceConectado) {
        QMessageBox::warning (this, QString (TITULO), QString("Erro de comunicacao desconhecido."), QMessageBox::Ok, QMessageBox::NoButton);
        this->pareceConectado = false;
        this->mensagemNaBarra (QString ("A conexao com o servidor foi perdida..."), TEMPOPADRAO);
    }
    while (this->msgtela->QuantosConversam ()) {
        this->msgtela->SaiDaConversa (0, n, e, c);
        this->msgtela->MensagemSaiDaConversa (n, e, c);
    }
    this->btnEnviar->setEnabled (false);
    this->btnConectar->setEnabled (true);
    this->btnConectar->setText (QString ("&Conectar..."));
    this->btnServir->setEnabled (true);
    this->btnServir->setText (QString ("Serv&ir..."));
    this->btnConfigurar->setEnabled (true);
    this->AtualizaInfo();
}

Messenger::~Messenger () {
    DEBUGA ("Messenger::~Messenger ();");
    if (srv != NULL)
        APAGA_OBJETO (srv);
    APAGA_OBJETO (this->comoVaoAsCoisas);
    APAGA_OBJETO (this->lblTitulo);
    APAGA_OBJETO (this->lblInfo);
    APAGA_OBJETO (this->msgtela);
    APAGA_OBJETO (this->txtMensEnviar);
    APAGA_OBJETO (this->btnConectar);
    APAGA_OBJETO (this->btnEnviar);
    APAGA_OBJETO (this->btnServir);
    APAGA_OBJETO (this->btnConfigurar);
    APAGA_OBJETO (this->tim);
}

void Messenger::resizeEvent (QResizeEvent *ev) {
    int maior1, maior2, maior3;
    DEBUGA ("void Messenger::resizeEvent (QResizeEvent *);");
    this->btnConectar->adjustSize();
    this->btnServir->adjustSize();
    this->btnConfigurar->adjustSize();
    this->btnEnviar->adjustSize();
    QPoint poiTitulo (this->lblTitulo->pos());
    QSize sizTitulo (this->lblTitulo->size());
    QPoint poiInfo (this->lblInfo->pos());
    QSize sizInfo (this->lblInfo->size());
    QPoint poiConectar (this->btnConectar->pos());
    QSize sizConectar (this->btnConectar->size());
    QPoint poiServir (this->btnServir->pos());
    QSize sizServir (this->btnServir->size());
    QPoint poiEnviar (this->btnEnviar->pos());
    QSize sizEnviar (this->btnEnviar->size());
    QPoint poiMensagens (this->msgtela->pos());
    QSize sizMensagens (this->msgtela->size());
    QPoint poiMensEnviar (this->txtMensEnviar->pos());
    QSize sizMensEnviar (this->txtMensEnviar->size());
    QPoint poiConfigurar (this->btnConfigurar->pos());
    QSize sizConfigurar (this->btnConfigurar->size());
    QPoint poiComoVaoAsCoisas (this->comoVaoAsCoisas->pos());
    QSize sizComoVaoAsCoisas (this->comoVaoAsCoisas->size());
    maior1 = max3 (sizInfo.height(), sizConectar.height(), sizServir.height());
    maior2 = max2 (sizEnviar.height() + 10 + sizConfigurar.height(), sizMensEnviar.height());
    maior3 = max2 (sizConfigurar.width(), sizEnviar.width());
    sizConfigurar.setWidth (maior3);
    sizEnviar.setWidth (maior3);
    poiTitulo.setX (10);
    poiTitulo.setY (10);
    sizTitulo.setWidth (ev->size().width() - 20);
    sizTitulo.setHeight (40);
    sizComoVaoAsCoisas.setWidth (sizTitulo.width());
    poiComoVaoAsCoisas.setX (10);
    poiComoVaoAsCoisas.setY (ev->size().height() - sizComoVaoAsCoisas.height());
    poiMensEnviar.setX (10);
    poiMensEnviar.setY (ev->size().height() - sizComoVaoAsCoisas.height() - 10 - maior2 / 2 - sizMensEnviar.height() / 2);
    sizMensEnviar.setWidth (ev->size().width() - 30 - maior3);
    poiEnviar.setX (sizMensEnviar.width() + 20);
    poiEnviar.setY (ev->size().height() - sizComoVaoAsCoisas.height() - 10 - (maior2 / 2 + sizEnviar.height() / 2 + sizConfigurar.height() / 2) - 5);
    poiInfo.setX (10);
    sizInfo.setWidth (ev->size().width() - 40 - sizConectar.width() - sizServir.width());
    poiConectar.setX (sizInfo.width() + 20);
    poiServir.setX (poiConectar.x() + sizConectar.width() + 10);
    poiInfo.setY (ev->size().height() - sizComoVaoAsCoisas.height() - 20 - maior2 - maior1 / 2 - sizInfo.height() / 2);
    poiConectar.setY (ev->size().height() - sizComoVaoAsCoisas.height() - 20 - maior2 - maior1 / 2 - sizConectar.height() / 2);
    poiServir.setY (ev->size().height() - sizComoVaoAsCoisas.height() - 20 - maior2 - maior1 / 2 - sizServir.height() / 2);
    poiMensagens.setX (10);
    sizMensagens.setWidth (sizTitulo.width());
    poiMensagens.setY (sizTitulo.height() + 20);
    sizMensagens.setHeight (ev->size().height() - sizComoVaoAsCoisas.height() - 30 - poiMensagens.y() - maior1 - maior2);
    poiConfigurar.setX (sizMensEnviar.width() + 20);
    poiConfigurar.setY (poiEnviar.y() + sizEnviar.height() + 10);
    this->lblTitulo->move (poiTitulo);
    this->lblTitulo->resize (sizTitulo);
    this->lblInfo->move (poiInfo);
    this->lblInfo->resize (sizInfo);
    this->msgtela->move (poiMensagens);
    this->msgtela->resize (sizMensagens);
    this->txtMensEnviar->move (poiMensEnviar);
    this->txtMensEnviar->resize (sizMensEnviar);
    this->btnConectar->move (poiConectar);
    this->btnConectar->resize (sizConectar);
    this->btnServir->move (poiServir);
    this->btnServir->resize (sizServir);
    this->btnEnviar->move (poiEnviar);
    this->btnEnviar->resize (sizEnviar);
    this->btnConfigurar->move (poiConfigurar);
    this->btnConfigurar->resize (sizConfigurar);
    this->comoVaoAsCoisas->move (poiComoVaoAsCoisas);
    this->comoVaoAsCoisas->resize (sizComoVaoAsCoisas);
}

void Messenger::btnEnviarClicked () {
    DEBUGA ("void Messenger::btnEnviarClicked ();");
    QString msg(this->txtMensEnviar->text().stripWhiteSpace());
    if (! msg.isEmpty()) {
        this->conex.AdicionaParametros (msg);
        this->conex.Envia ("ESCREVE");
        this->txtMensEnviar->setText ("");
        this->btnEnviar->setEnabled (false);
    }
    this->txtMensEnviar->setFocus ();
}

void Messenger::btnConectarClicked () {
    DEBUGA ("void Messenger::btnConectarClicked ();");
    Q_UINT16 p;
    QString s;
    bool ok;
    if (this->pareceConectado) {
        this->pareceConectado = false;
        this->manda_desconectar ();
    } else {
        s = QInputDialog::getText (QString(TITULO) + " - Configurar cliente de bate-papo", QString("Digite o nome ou o endereco IP do servidor para conectar:"), QLineEdit::Normal, this->servremoto, &ok);
        if (! ok) return;
        this->servremoto = s;
        p = QInputDialog::getInteger (QString(TITULO) + " - Configurar cliente de bate-papo", QString("Digite a porta TCP/IP para conectar:"), this->porta, 1, 65535, 1, &ok);
        if (! ok) return;
        this->porta = p;
        this->btnServir->setEnabled (false);
        this->btnConfigurar->setEnabled (false);
        this->btnConectar->setText (QString ("Des&conectar"));
        this->manda_conectar ();
        this->AtualizaInfo();
    }
}

void Messenger::conexComandoRecebido (QStringList comando) {
    DEBUGA("void Messenger::conexComandoRecebido (QStringList);");
    QString n, cmd;
    QFont e;
    QColor c;
    int pos;
    cmd = comando.first ();
    comando.remove (comando.begin ());
    this->AtualizaInfo ();
    if (cmd == "ENTRA") {
        if (comando.count () != 3) {
            DEBUGA ("Servidor enviou um comando 'ENTRA' com numero de parametros incorretos.");
            this->manda_desconectar ();
        } else {
            n = comando.first ();
            if (n.isEmpty ()) {
                DEBUGA ("Nome do usuario que entrou eh vazio.");
                this->manda_desconectar ();
            } else {
                comando.remove (comando.begin ());
                if (e.fromString (comando.first ())) {
                    comando.remove (comando.begin ());
                    c.setNamedColor (comando.first ());
                    if (c.isValid ()) {
                        if (this->msgtela->EntraNaConversa (n, e, c)) {
                            this->msgtela->MensagemEntraNaConversa (n, e, c);
                            this->mensagemNaBarra (QString ("'") + n + "' acabou de entrar.", TEMPOPADRAO);
                            this->setActiveWindow ();
                        } else {
                            DEBUGA ("Usuario enviado pelo servidor nao conseguiu entrar na sala.");
                            this->manda_desconectar ();
                        }
                    } else {
                        DEBUGA ("Servidor enviou uma cor desconhecida.");
                        this->manda_desconectar ();
                    }
                } else {
                    DEBUGA ("Servidor enviou uma fonte desconhecida.");
                    this->manda_desconectar ();
                }
            }
        }
    } else if (cmd == "ESCREVE") {
        if (comando.count () < 2) {
            DEBUGA ("Servidor enviou um comando 'ESCREVE' mas nao enviou o nome da figura.");
            this->manda_desconectar ();
        } else {
            n = comando.first ();
            if (n.isEmpty ()) {
                DEBUGA ("Nome do usuario que escreveu uma mensagem eh vazio.");
                this->manda_desconectar ();
            } else {
                comando.remove (comando.begin ());
                pos = this->msgtela->EstaNaConversa (n);
                if (pos < 0) {
                    DEBUGA ("Servidor enviou um nome de usuario que ainda nao esta registrado.");
                    this->manda_desconectar ();
                } else {
                    this->msgtela->EscreveMensagem (pos, comando.join ("\n"));
                    this->setActiveWindow ();
                }
            }
        }
    } else if (cmd == "SAI") {
        if (comando.count () != 1) {
            DEBUGA ("Servidor enviou um comando 'SAI' com numero incorreto de parametros.");
            this->manda_desconectar ();
        } else {
            n = comando.first ();
            if (n.isEmpty ()) {
                DEBUGA ("Nome do usuario que saiu eh vazio.");
                this->manda_desconectar ();
            } else {
                pos = this->msgtela->EstaNaConversa (n);
                if (pos < 0) {
                    DEBUGA ("Servidor enviou um nome de usuario que ainda nao esta registrado.");
                    this->manda_desconectar ();
                } else if (this->msgtela->SaiDaConversa (pos, n, e, c)) {
                    this->msgtela->MensagemSaiDaConversa (n, e, c);
                    this->mensagemNaBarra (QString ("'") + n + "' saiu da conversa.", TEMPOPADRAO);
                    this->setActiveWindow ();
                } else {
                    DEBUGA ("Usuario enviou um nome de usuario que ja nao existia.");
                    this->manda_desconectar ();
                }
            }
        }
    } else if (cmd == "NADA") {
        // Ignorar...
    } else {
        DEBUGA ("O servidor enviou um comando desconhecido.");
        this->manda_desconectar ();
    }
}

void Messenger::conexComandoExecutado (QStringList comando, QStringList resposta) {
    DEBUGA("void Messenger::conexComandoExecutado (QStringList, QStringList);");
    QString n;
    QFont e;
    QColor c;
    if (comando.first() == "ENTRA") {
        if (resposta.first() == "OK") {
            resposta.remove (resposta.begin ());
            this->msgtela->EntraNaConversa (this->EuMesmo_Nome, this->EuMesmo_Estilo, this->EuMesmo_Cor);
            this->msgtela->MensagemEntraNaConversa (this->EuMesmo_Nome, this->EuMesmo_Estilo, this->EuMesmo_Cor);
            while (! resposta.isEmpty ()) {
                n = resposta.first ();
                resposta.remove (resposta.begin ());
                if (resposta.isEmpty ()) {
                    DEBUGA ("Ei! A lista esvaziou depois que eu tirei o nome do usuario!");
                    this->manda_desconectar ();
                    return;
                }
                if (n.isEmpty ()) {
                    DEBUGA ("O servidor enviou um nome vazio...");
                    this->manda_desconectar ();
                    return;
                }
                if (! e.fromString (resposta.first ())) {
                    DEBUGA ("Ei! O servidor mandou uma fonte que eu nao conheco!");
                    this->manda_desconectar ();
                    return;
                }
                resposta.remove (resposta.begin ());
                if (resposta.isEmpty ()) {
                    DEBUGA ("Ei! A lista esvaziou depois que eu tirei a fonte do usuario!");
                    this->manda_desconectar ();
                    return;
                }
                c.setNamedColor (resposta.first ());
                resposta.remove (resposta.begin ());
                if (! c.isValid ()) {
                    DEBUGA ("Ei! O servidor mandou uma cor que eu nao conheco!");
                    this->manda_desconectar ();
                    return;
                }
                if (this->msgtela->EntraNaConversa (n, e, c)) {
                    this->msgtela->MensagemEntraNaConversa (n, e, c);
                    this->mensagemNaBarra (QString ("Conexao em '") + this->servremoto + ":" + QString::number (this->porta) + "' foi efetuada com sucesso.", TEMPOPADRAO);
                } else {
                    DEBUGA ("Legal... A sala parece ter lotado, mas o servidor continua enviando usuarios.");
                    this->manda_desconectar ();
                    return;
                }
            }
            this->btnEnviar->setEnabled (true);
        } else {
            resposta.remove (resposta.begin ());
            this->mensagemNaBarra (QString ("A conexao com o servidor foi perdida..."), TEMPOPADRAO);
            QMessageBox::warning (this, QString (TITULO), QString("Erro de comunicacao. A mensagem de erro retornada pelo subsistema foi:\n\n") + resposta.first (), QMessageBox::Ok, QMessageBox::NoButton);
            this->pareceConectado = false;
            this->manda_desconectar ();
        }
    } else if (comando.first() == "ESCREVE") {
        if (resposta.isEmpty()) {
            comando.remove (comando.begin ());
            this->msgtela->EscreveMensagem (this->EuMesmo_Nome, this->EuMesmo_Estilo, this->EuMesmo_Cor, comando.join (QString ("\n")));
            this->btnEnviar->setEnabled (true);
        } else {
            DEBUGA ("O comando 'ESCREVE' nao deveria retornar valor...");
            this->mensagemNaBarra (QString ("A conexao com o servidor foi perdida..."), TEMPOPADRAO);
            QMessageBox::warning (this, QString (TITULO), QString("Erro de comunicacao. A mensagem de erro retornada pelo subsistema foi:\n\n") + resposta.join ("\n"), QMessageBox::Ok, QMessageBox::NoButton);
            this->pareceConectado = false;
            this->manda_desconectar ();
        }
    } else if (comando.first() == "NADA") {
        // Ignorar...
    } else {
        DEBUGA ("Isso quer dizer que eu mesmo enviei um comando desconhecido...");
        this->mensagemNaBarra (QString ("A conexao com o servidor foi perdida..."), TEMPOPADRAO);
        QMessageBox::warning (this, QString (TITULO), QString("Erro de comunicacao desconhecido."), QMessageBox::Ok, QMessageBox::NoButton);
        this->pareceConectado = false;
        this->manda_desconectar ();
    }
}

void Messenger::conexErroComunicando (QString msgerro) {
    DEBUGA("void Messenger::conexErroComunicando (QString);");
    if (this->pareceConectado) {
        this->mensagemNaBarra (QString ("A conexao com o servidor foi perdida..."), TEMPOPADRAO);
        QMessageBox::warning (this, QString (TITULO), QString("Erro de comunicacao. A mensagem de erro retornada pelo subsistema foi:\n\n") + msgerro, QMessageBox::Ok, QMessageBox::NoButton);
        this->pareceConectado = false;
    }
    this->manda_desconectar ();
}

void Messenger::conexHostFound () {
    DEBUGA("void Messenger::conexHostFound ();");
    if (this->srv == NULL)
        this->mensagemNaBarra (QString ("Host '") + this->servremoto + "' localizado. Estabelecendo conexao...", TEMPOPADRAO);
}

void Messenger::conexConnected () {
    DEBUGA("void Messenger::conexConnected ();");
    if (this->srv == NULL)
        this->mensagemNaBarra (QString ("Host '") + this->servremoto + "' conectado. Negociando entrada na sala...", TEMPOPADRAO);
    this->conex.AdicionaParametros (this->EuMesmo_Nome);
    this->conex.AdicionaParametros (this->EuMesmo_Estilo.toString());
    this->conex.AdicionaParametros (this->EuMesmo_Cor.name());
    this->conex.Envia ("ENTRA");
    this->tim->start (OHVIVENTE, true);
}

void Messenger::btnServirClicked () {
    Q_UINT16 p;
    bool ok;
    DEBUGA ("void Messenger::btnServirClicked ();");
    if (this->srv != NULL) {
        this->pareceConectado = false;
        this->manda_desconectar ();
    } else {
        p = QInputDialog::getInteger (QString(TITULO) + " - Configurar servidor de bate-papo", QString("Digite a porta TCP/IP para servir:"), this->porta, 1, 65535, 1, &ok);
        if (! ok) return;
        this->porta = p;
        if (this->porta == 0) {
            QMessageBox::warning (this, QString (TITULO), QString("Numero de porta TCP/IP invalido!"), QMessageBox::Ok, QMessageBox::NoButton);
            return;
        }
        NOVO_OBJETO (this->srv, servidor (this->porta, this));
        if (! this->srv->ok()) {
            QMessageBox::warning (this, QString (TITULO), QString("Erro inesperado abrindo porta TCP/IP para escuta!"), QMessageBox::Ok, QMessageBox::NoButton);
            this->btnServirClicked ();
            return;
        }
        this->servremoto = "127.0.0.1";
        this->btnServir->setText (QString("&Interromper servidor"));
        this->btnConfigurar->setEnabled (false);
        this->btnConectar->setEnabled (false);
        this->manda_conectar ();
        this->AtualizaInfo ();
    }
}

void Messenger::genResize (void) {
    DEBUGA ("void Messenger::genResize (void);");
    QResizeEvent ev (this->size(), this->size());
    this->resizeEvent (&ev);
}

void Messenger::AtualizaInfo () {
    DEBUGA ("void Messenger::AtualizaInfo ();");
    QString texto("");
    if (this->srv != NULL)
        texto += "Servindo bate-papo na porta " + QString().setNum (this->porta) + "\n";
    else if (this->conex.state() == QSocket::Connected)
        texto += "Conectado ao bate-papo no host " + this->servremoto + ":" + QString().setNum (this->porta) + "\n";
    else if (this->pareceConectado)
        texto += "Conectando ao bate-papo no host " + this->servremoto + ":" + QString().setNum (this->porta) + "\n";
    else
        texto += "Desconectado.\n";
    texto += "Nome: " + this->EuMesmo_Nome;
    this->lblInfo->setText (texto);
    this->lblInfo->adjustSize();
    this->genResize ();
}

void Messenger::btnConfigurarClicked () {
    DEBUGA ("void Messenger::btnConfigurarClicked ();");
    config_usuario(this->EuMesmo_Nome, this->EuMesmo_Estilo, this->EuMesmo_Cor, this->UsaCTRLEnter).exec();
    this->txtMensEnviar->setFont (this->EuMesmo_Estilo);
    this->txtMensEnviar->setPaletteForegroundColor (this->EuMesmo_Cor);
    this->AtualizaInfo();
    this->txtMensEnviar->setFocus ();
}

void Messenger::txtMensEnviarReturnPressed () {
    DEBUGA ("void Messenger::txtMensEnviarReturnPressed ();");
    if (this->UsaCTRLEnter && this->btnEnviar->isEnabled ())
        this->btnEnviarClicked ();
}
