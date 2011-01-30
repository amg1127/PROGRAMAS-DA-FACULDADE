#include "servidor.h"

servidor::servidor (Q_UINT16 port, Messenger * parent) : QServerSocket (port, 1, parent, 0) {
    DEBUGA ("servidor::servidor (Q_UINT16, Messenger*);");
    int i;
    NOVO_OBJETO (this->tim, QTimer (this));
    QObject::connect (this->tim, SIGNAL (timeout()), this, SLOT (timerTimeout()));
    janela = parent;
    this->tim->start (OHVIVENTE, true);
    for (i=0; i<MAX_USUARIOS; i++)
        this->dentro_da_sala[i] = NULL;
    this->quantos_usuarios = 0;
    this->entrando_na_sala = NULL;
    this->saindo_da_sala = NULL;
}

void servidor::marca_destroi (usuarios_sala *u) {
    if (this->saindo_da_sala != NULL)
        APAGA_OBJETO (this->saindo_da_sala);
    this->saindo_da_sala = u;
    if (u != NULL) {
        QObject::disconnect (u, 0, this, 0);
    }
}

servidor::~servidor () {
    DEBUGA ("servidor::~servidor ();");
    int i;
    APAGA_OBJETO (this->tim);
    if (this->entrando_na_sala != NULL)
        APAGA_OBJETO (this->entrando_na_sala);
    if (this->saindo_da_sala != NULL)
        APAGA_OBJETO (this->saindo_da_sala);
    for (i=0; i<this->quantos_usuarios; i++)
        APAGA_OBJETO (this->dentro_da_sala[i]);
}

void servidor::newConnection (int socket) {
    DEBUGA ("void servidor::newConnection (int);");
    QSocket sock(this);
    QString str ("AGUARDE Outro usuario esta em fase de autenticacao nesta sala.\n");
    if (this->entrando_na_sala != NULL) {
        sock.setSocket (socket);
        sock.writeBlock (str.ascii(), str.length());
        sock.flush();
        sock.close();
        return;
    }
    this->janela->mensagemNaBarra (QString ("Um novo usuario se conectou a sala. Negociando entrada..."), TEMPOPADRAO);
    NOVO_OBJETO (this->entrando_na_sala, usuarios_sala (socket));
    QObject::connect (this->entrando_na_sala, SIGNAL(RespostaEnviada (usuarios_sala*, QStringList, QStringList)), this, SLOT(RespostaEnviada (usuarios_sala*, QStringList, QStringList)));
    QObject::connect (this->entrando_na_sala, SIGNAL(ComandoRecebido (usuarios_sala*, QStringList)), this, SLOT(ComandoRecebido (usuarios_sala*, QStringList)));
    QObject::connect (this->entrando_na_sala, SIGNAL(ErroComunicando (usuarios_sala*, QString)), this, SLOT(ErroComunicando (usuarios_sala*, QString)));
}

void servidor::RespostaEnviada (usuarios_sala* uai, QStringList comandorecebido, QStringList valorenviado) {
    DEBUGA ("void servidor::RespostaEnviada (usuarios_sala*, QStringList, QStringList);");
    if (uai == this->entrando_na_sala) {
        // Evitar mensagens de 'unused variable...' do compilador
        this->janela->mensagemNaBarra (QString ("A entrada do novo usuario nao foi realizada com sucesso."), TEMPOPADRAO);
        comandorecebido.isEmpty ();
        valorenviado.isEmpty ();
        marca_destroi (this->entrando_na_sala);
        this->entrando_na_sala = NULL;
    }
}

void servidor::ComandoRecebido (usuarios_sala* uai, QStringList lsd) {
    DEBUGA ("void servidor::ComandoRecebido (usuarios_sala*, QStringList);");
    int l, i, j, x;
    l = lsd.count ();
    if (uai == this->entrando_na_sala) {
        if (l < 4) {
            uai->Conexao.AdicionaParametros ("ERRO");
            uai->Conexao.AdicionaParametros ("Numero de parametros incorreto.");
        } else if (lsd[0] != "ENTRA") {
            uai->Conexao.AdicionaParametros ("ERRO");
            uai->Conexao.AdicionaParametros ("Processo de autenticacao nao foi iniciado.");
        } else {
            uai->Nome = lsd[1];
            if (uai->Nome.isEmpty ()) {
                uai->Conexao.AdicionaParametros ("ERRO");
                uai->Conexao.AdicionaParametros ("Nome nao pode ser vazio.");
            }
            for (i=0; i<this->quantos_usuarios; i++) {
                if (this->dentro_da_sala[i]->Nome == uai->Nome)
                    break;
            }
            if (i != this->quantos_usuarios) {
                uai->Conexao.AdicionaParametros ("ERRO");
                uai->Conexao.AdicionaParametros ("Ja existe um usuario com esse nome na sala de bate-papo.");
            } else if (this->quantos_usuarios == MAX_USUARIOS) {
                uai->Conexao.AdicionaParametros ("ERRO");
                uai->Conexao.AdicionaParametros ("Sala de bate-papo esta lotada.");
            } else if (uai->Estilo.fromString (lsd[2])) {
                uai->Cor.setNamedColor (lsd[3]);
                if (uai->Cor.isValid()) {
                    this->entrando_na_sala = NULL;
                    this->dentro_da_sala[this->quantos_usuarios] = uai;
                    uai->Conexao.AdicionaParametros ("OK");
                    for (i=0; i<this->quantos_usuarios; i++) {
                        uai->Conexao.AdicionaParametros (this->dentro_da_sala[i]->Nome);
                        uai->Conexao.AdicionaParametros (this->dentro_da_sala[i]->Estilo.toString ());
                        uai->Conexao.AdicionaParametros (this->dentro_da_sala[i]->Cor.name ());
                        this->dentro_da_sala[i]->Conexao.AdicionaParametros (uai->Nome);
                        this->dentro_da_sala[i]->Conexao.AdicionaParametros (uai->Estilo.toString());
                        this->dentro_da_sala[i]->Conexao.AdicionaParametros (uai->Cor.name());
                        this->dentro_da_sala[i]->Conexao.Envia ("ENTRA");
                    }
                    this->quantos_usuarios++;
                    this->janela->mensagemNaBarra (QString ("O novo usuario agora eh conhecido como '") + uai->Nome + "'.", TEMPOPADRAO);
                } else {
                    uai->Conexao.AdicionaParametros ("ERRO");
                    uai->Conexao.AdicionaParametros ("Especificacao de cor nao foi reconhecida.");
                }
            } else {
                uai->Conexao.AdicionaParametros ("ERRO");
                uai->Conexao.AdicionaParametros ("Especificacao de fonte nao foi reconhecida.");
            }
        }
    } else {
        for (i=0; i<this->quantos_usuarios; i++) {
            if (uai == this->dentro_da_sala[i]) {
                if (lsd[0] == "ESCREVE") {
                    for (x=0; x<i; x++) {
                        this->dentro_da_sala[x]->Conexao.AdicionaParametros (uai->Nome);
                    }
                    for (x++; x<this->quantos_usuarios; x++) {
                        this->dentro_da_sala[x]->Conexao.AdicionaParametros (uai->Nome);
                    }
                    for (j=1; j<l; j++) {
                        for (x=0; x<i; x++) {
                            this->dentro_da_sala[x]->Conexao.AdicionaParametros (lsd[j]);
                        }
                        for (x++; x<this->quantos_usuarios; x++) {
                            this->dentro_da_sala[x]->Conexao.AdicionaParametros (lsd[j]);
                        }
                    }
                    for (x=0; x<i; x++) {
                        this->dentro_da_sala[x]->Conexao.Envia ("ESCREVE");
                    }
                    for (x++; x<this->quantos_usuarios; x++) {
                        this->dentro_da_sala[x]->Conexao.Envia ("ESCREVE");
                    }
                } else if (lsd[0] == "NADA") {
                    // Ignorar...
                } else {
                    uai->Conexao.AdicionaParametros ("ERRO");
                    uai->Conexao.AdicionaParametros ("Comando invalido.");
                }
                break;
            }
        }
        Q_ASSERT (i != this->quantos_usuarios);
    }
}

void servidor::ErroComunicando (usuarios_sala* uai, QString err) {
    DEBUGA ("void servidor::ErroComunicando (usuarios_sala*, QString);");
    int i, x;
    if (uai == this->entrando_na_sala) {
        marca_destroi (this->entrando_na_sala);
        this->janela->mensagemNaBarra (QString ("A entrada do novo usuario nao foi realizada com sucesso."), TEMPOPADRAO);
        this->entrando_na_sala = NULL;
    } else {
        for (i=0; i<this->quantos_usuarios; i++) {
            if (uai == this->dentro_da_sala[i]) {
                for (x=0; x<i; x++) {
                    this->dentro_da_sala[x]->Conexao.AdicionaParametros (uai->Nome);
                    this->dentro_da_sala[x]->Conexao.Envia ("SAI");
                }
                for (x++; x<this->quantos_usuarios; x++) {
                    this->dentro_da_sala[x]->Conexao.AdicionaParametros (uai->Nome);
                    this->dentro_da_sala[x]->Conexao.Envia ("SAI");
                    this->dentro_da_sala[x-1] = this->dentro_da_sala[x];
                }
                this->dentro_da_sala[--this->quantos_usuarios] = NULL;
                marca_destroi (uai);
                return;
            }
        }
        i = err.length ();
        // Se a funcao chegou nesse ponto aqui, entao, eu deixei algum furo...
        Q_ASSERT (false);
    }
}

void servidor::timerTimeout () {
    DEBUGA ("void servidor::timerTimeout ();");
    int i;
    for (i=0; i<this->quantos_usuarios; i++) {
        if (this->dentro_da_sala[i]->Conexao.Estado() == QueEstado::Pronto) {
            this->dentro_da_sala[i]->Conexao.Envia ("NADA");
        }
    }
}

usuarios_sala::usuarios_sala (int sock) {
    DEBUGA ("usuarios_sala::usuarios_sala (int);");
    this->Conexao.setSocket (sock);
    QObject::connect (&(this->Conexao), SIGNAL(RespostaEnviada(QStringList, QStringList)), this, SLOT(RespostaEnviada (QStringList, QStringList)));
    QObject::connect (&(this->Conexao), SIGNAL(ComandoRecebido (QStringList)), this, SLOT(ComandoRecebido (QStringList)));
    QObject::connect (&(this->Conexao), SIGNAL(ComandoExecutado (QStringList, QStringList)), this, SLOT(ComandoExecutado (QStringList, QStringList)));
    QObject::connect (&(this->Conexao), SIGNAL(ErroComunicando (QString)), this, SLOT(ErroComunicando (QString)));
}

void usuarios_sala::RespostaEnviada (QStringList s1, QStringList s2) {
    DEBUGA ("void usuarios_sala::RespostaEnviada (QStringList, QStringList);");
    emit RespostaEnviada (this, s1, s2);
}

void usuarios_sala::ComandoRecebido (QStringList s) {
    DEBUGA ("void usuarios_sala::ComandoRecebido (QStringList);");
    emit ComandoRecebido (this, s);
}

void usuarios_sala::ComandoExecutado (QStringList s1, QStringList s2) {
    DEBUGA ("void usuarios_sala::ComandoExecutado (QStringList, QStringList);");
    emit ComandoExecutado (this, s1, s2);
}

void usuarios_sala::ErroComunicando (QString s) {
    DEBUGA ("void usuarios_sala::ErroComunicando (QString);");
    emit ErroComunicando (this, s);
}
