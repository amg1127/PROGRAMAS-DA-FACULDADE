// visualizacao.cpp
#include "visualizacao.h"

visualizacao::visualizacao () {
    int ml, ma;

    this->tim = NULL;
    this->o_proc = NULL;
    this->o_sock = NULL;
    this->tmpFile = NULL;
    this->estaEmProcessoDeNovaImagem = nada;

    this->setWindowTitle (tituloDaJanela + " [*]");
    this->setWindowModified (true);

    NOVO_OBJETO (this->janeConfiguracao, configuracao(this));
    NOVO_OBJETO (this->tim, QTimer (this));
    NOVO_OBJETO (this->tim2, QTimer (this));
    this->tim->setSingleShot (true);
    this->tim2->setSingleShot (true);

    NOVO_OBJETO (this->imagemArea, QScrollArea(this));
    this->imagemArea->move (20, 20);
    this->imagemArea->show ();

    NOVO_OBJETO (this->imagemVisualizacao, paintbrush ());
    this->imagemArea->setWidget (this->imagemVisualizacao);
    this->imagemVisualizacao->move (0, 0);
    this->imagemArea->setWidgetResizable (this->janeConfiguracao->cfgEscolheDimAutomatico);
    if (! this->janeConfiguracao->cfgEscolheDimAutomatico) {
        this->imagemVisualizacao->resize (this->janeConfiguracao->cfgEscolheLargura, this->janeConfiguracao->cfgEscolheAltura);
    }

    NOVO_OBJETO (this->janelaLogs, QTextEdit(this));
    this->janelaLogs->setReadOnly (true);
    this->janelaLogs->setLineWrapMode (QTextEdit::NoWrap);
    this->janelaLogs->unsetCursor ();
    this->janelaLogs->viewport()->unsetCursor ();
    this->janelaLogs->show ();

    NOVO_OBJETO (this->infoImagem, QGroupBox(this));
    this->infoImagem->setTitle ("Imagem");
    this->infoImagem->show ();
    this->infoImagem->resize (larguraGBox, alturaGBox);

    NOVO_OBJETO (this->infoSelecao, QGroupBox(this));
    this->infoSelecao->setTitle ("Selecao");
    this->infoSelecao->show ();
    this->infoSelecao->resize (larguraGBox, alturaGBox);

    NOVO_OBJETO (this->lblInfoImagem, QLabel(this->infoImagem));
    this->lblInfoImagem->show ();
    this->lblInfoImagem->move (20, 20);
    this->lblInfoImagem->resize (larguraGBox - 40, alturaGBox - 40);

    NOVO_OBJETO (this->lblInfoSelecao, QLabel(this->infoSelecao));
    this->lblInfoSelecao->show ();
    this->lblInfoSelecao->move (20, 20);
    this->lblInfoSelecao->resize (larguraGBox - 40, alturaGBox - 40);

    NOVO_OBJETO (this->novoFractal, QPushButton(this));
    NOVO_OBJETO (this->diminuirZoom, QPushButton(this));
    NOVO_OBJETO (this->aumentarZoom, QPushButton(this));
    NOVO_OBJETO (this->salvarFractal, QPushButton(this));
    NOVO_OBJETO (this->btnConfiguracao, QPushButton(this));

    this->novoFractal->show ();
    this->diminuirZoom->show ();
    this->aumentarZoom->show ();
    this->salvarFractal->show ();
    this->btnConfiguracao->show ();

    NOVO_OBJETO (this->menuBotao, QMenu (this->diminuirZoom));

    this->zooms.append (QRectF (-2.0, -2.0, 4.0, 4.0));

    this->reabilitaBotoes ();

    this->novoFractal->adjustSize ();
    this->diminuirZoom->adjustSize ();
    this->aumentarZoom->adjustSize ();
    this->salvarFractal->adjustSize ();
    this->btnConfiguracao->adjustSize ();


    ml = maior5 (this->novoFractal->width (),
                 this->diminuirZoom->width (),
                 this->aumentarZoom->width (),
                 this->salvarFractal->width (),
                 this->btnConfiguracao->width ()
                );
    ma = maior5 (this->novoFractal->height (),
                 this->diminuirZoom->height (),
                 this->aumentarZoom->height (),
                 this->salvarFractal->height (),
                 this->btnConfiguracao->height ()
                );
    this->novoFractal->resize (ml, ma);
    this->diminuirZoom->resize (ml, ma);
    this->aumentarZoom->resize (ml, ma);
    this->salvarFractal->resize (ml, ma);
    this->btnConfiguracao->resize (ml, ma);
    this->janelaLogs->resize (100, ma * 5 + 40);

    this->setMinimumSize (ml + this->infoImagem->width() + 50, 60 + this->janelaLogs->height() + 2 * this->infoImagem->height ());
    this->resize (500, 400);

    QObject::connect (this->novoFractal, SIGNAL (clicked()), this, SLOT(novoFractal_clicked()));
    QObject::connect (this->diminuirZoom, SIGNAL (clicked()), this, SLOT(diminuirZoom_clicked()));
    QObject::connect (this->aumentarZoom, SIGNAL (clicked()), this, SLOT(aumentarZoom_clicked()));
    QObject::connect (this->salvarFractal, SIGNAL (clicked()), this, SLOT(salvarFractal_clicked()));
    QObject::connect (this->btnConfiguracao, SIGNAL (clicked()), this, SLOT(btnConfiguracao_clicked()));
    QObject::connect (this->imagemVisualizacao, SIGNAL (selecionouArea(QRect,bool)), this, SLOT(imagemVisualizacao_selecionouArea(QRect,bool)));
    QObject::connect (this->tim, SIGNAL(timeout()), this, SLOT(tim_timeout()));
    QObject::connect (this->tim2, SIGNAL(timeout()), this, SLOT(tim2_timeout()));
    QObject::connect (this->menuBotao, SIGNAL(triggered(QAction*)), this, SLOT(menuBotao_triggered(QAction*)));

    this->registraNoLog (tituloDaJanela + " iniciado.", log_info);

    QPixmap p (this->imagemVisualizacao->width(), this->imagemVisualizacao->height());
    p.fill (QColor (255, 255, 255));
    this->imagemVisualizacao->pixNaTelaOriginal = p;
    this->imagemVisualizacao->recarrega ();
}

void visualizacao::setWindowModified (bool t) {
    QWidget::setWindowModified (t);
    if (t && this->tim != NULL) {
        this->tim->start (oTimeoutDoTimer);
    }
}

void visualizacao::resizeEvent (QResizeEvent *ev) {
    int nw, nh, tmp;
    QRectF univ;
    QString s;
    nw = ev->size().width();
    nh = ev->size().height();

    tmp = nw - 20 - this->infoImagem->width();
    this->infoImagem->move (tmp, 20);
    this->infoSelecao->move (tmp, 10 + embaixo (this->infoImagem));

    this->btnConfiguracao->move (20, nh - 20 - this->btnConfiguracao->height());
    this->salvarFractal->move (20, this->btnConfiguracao->y() - 10 - this->salvarFractal->height());
    this->diminuirZoom->move (20, this->salvarFractal->y() - 10 - this->diminuirZoom->height());
    this->aumentarZoom->move (20, this->diminuirZoom->y() - 10 - this->aumentarZoom->height());
    this->novoFractal->move (20, this->aumentarZoom->y() - 10 - this->novoFractal->height());
    this->janelaLogs->move (10 + direita(this->btnConfiguracao), this->novoFractal->y());
    this->janelaLogs->resize (nw - 50 - this->novoFractal->width (), this->janelaLogs->height ());

    tmp = maior2 (this->novoFractal->width (), this->infoImagem->width ());
    this->imagemArea->resize (nw - 50 - tmp, nh - 50 - this->janelaLogs->height());

    if (this->estaEmProcessoDeNovaImagem != nada) return;
    if (! this->zooms.isEmpty()) {
        this->lblInfoImagem->setText (mkTextoInformativo (this->imagemVisualizacao->rect(), this->zooms.last()));
    }
    if (this->janeConfiguracao->cfgEscolheDimAutomatico)
        this->setWindowModified (true);
}

QRectF visualizacao::proporcao (QRect universoINT, QRect retanguloINT, QRectF universoCPLX) {
    QRectF r;
    if (universoINT.isValid() && universoCPLX.isValid()) {
        r.setLeft ( (retanguloINT.left()  - universoINT.left()  ) *
                    (universoCPLX.right() - universoCPLX.left() ) /
                    (universoINT.right()  - universoINT.left()  ) +
                     universoCPLX.left()                        );
        r.setRight ( (retanguloINT.right() - universoINT.left()  ) *
                     (universoCPLX.right() - universoCPLX.left() ) /
                     (universoINT.right()  - universoINT.left()  ) +
                      universoCPLX.left()                        );
        r.setTop ( (retanguloINT.top()    - universoINT.top()  ) *
                   (universoCPLX.bottom() - universoCPLX.top() ) /
                   (universoINT.bottom()  - universoINT.top()  ) +
                    universoCPLX.top()                         );
        r.setBottom ( (retanguloINT.bottom() - universoINT.top()  ) *
                      (universoCPLX.bottom() - universoCPLX.top() ) /
                      (universoINT.bottom()  - universoINT.top()  ) +
                       universoCPLX.top()                         );
    }
    return (r);
}

QString visualizacao::mkTextoInformativo (QRect retanguloINT, QRectF retanguloCPLX) {
    QString s;
    if (retanguloINT.isValid()) {
        s = QString("Tamanho: ( ") + QString::number (retanguloINT.width()) + " x " + QString::number (retanguloINT.height()) + " )\n" +
        "Localizacao no plano complexo:\n" +
        "    ( " + QString::number (retanguloCPLX.left (),  'g', precisao) + " , " + QString::number (retanguloCPLX.top (),    'g', precisao) + " )\n" +
        "    ( " + QString::number (retanguloCPLX.right (), 'g', precisao) + " , " + QString::number (retanguloCPLX.bottom (), 'g', precisao) + " )";
    }
    return (s);
}

void visualizacao::salvarFractal_clicked () {
    static QString arquivo_salvar;
    QString novo;
    bool aviso;
    novo = QFileDialog::getSaveFileName (this, tituloDaJanela + " - Salvar imagem como...", arquivo_salvar, "Imagem PNG (*.png);;Todos os arquivos (*)", 0, 0);
    if (novo.isEmpty()) return;
    arquivo_salvar = novo;
    QFile arq (arquivo_salvar);
    aviso = arq.exists ();
    if (arq.open (QIODevice::WriteOnly)) {
        if (aviso) {
            this->registraNoLog ("Um arquivo ja existente esta sendo sobreescrito.", log_warn);
        }
        if (this->imagemVisualizacao->pixNaTelaOriginal.save (&arq, "PNG", 100)) {
            this->registraNoLog (QString("Arquivo '") + arquivo_salvar + "' gravado com sucesso.", log_info);
        } else {
            this->registraNoLog (QString("fwrite ('") + arquivo_salvar + "') falhou.", log_err);
            QMessageBox::warning (this, tituloDaJanela + " - Erro", QString("O arquivo nao pode ser gravado:\n\n\"") + arq.errorString() + "\"", QMessageBox::Ok, QMessageBox::NoButton);
        }
        arq.close ();
    } else {
        this->registraNoLog (QString("fopen ('") + arquivo_salvar + "') falhou.", log_err);
        QMessageBox::warning (this, tituloDaJanela + " - Erro", QString("O arquivo nao pode ser salvo:\n\n\"") + arq.errorString() + "\"", QMessageBox::Ok, QMessageBox::NoButton);
    }
}

void visualizacao::btnConfiguracao_clicked () {
    bool swinmod;
    int want, hant, maxiter;
    swinmod = false;
    want = this->imagemVisualizacao->width();
    hant = this->imagemVisualizacao->height();
    maxiter = this->janeConfiguracao->cfgNoIteracoes;
    this->registraNoLog ("Chamando janela de configuracao...", log_info);
    this->janeConfiguracao->loadConfig ();
    this->janeConfiguracao->exec ();
    this->registraNoLog ("Janela de configuracao foi fechada.", log_info);
    if (this->imagemArea->widgetResizable() != this->janeConfiguracao->cfgEscolheDimAutomatico) {
        this->imagemArea->setWidgetResizable (this->janeConfiguracao->cfgEscolheDimAutomatico);
    }
    if (! this->janeConfiguracao->cfgEscolheDimAutomatico) {
        this->imagemVisualizacao->resize (this->janeConfiguracao->cfgEscolheLargura, this->janeConfiguracao->cfgEscolheAltura);
    }
    if (this->imagemVisualizacao->width()  != want ||
        this->imagemVisualizacao->height() != hant) {
        swinmod = true;
    }
    if (! this->zooms.isEmpty()) {
        this->lblInfoImagem->setText (mkTextoInformativo (this->imagemVisualizacao->rect(), this->zooms.last()));
    }
    if (maxiter != this->janeConfiguracao->cfgNoIteracoes)
        swinmod = true;
    this->setWindowModified (this->isWindowModified() || swinmod || this->janeConfiguracao->mexeuNaPaleta);
}

void visualizacao::registraNoLog (QString msg, tipoLog t) {
    tipoLog t2 = t;
    QString prefixo;
    if (t == log_auto) {
        prefixo = msg.mid (0, 6).toUpper();
        if (prefixo == "SEND: ") {
            t2 = log_recv;
        } else if (prefixo == "RECV: ") {
            t2 = log_recv;
        } else if (prefixo == "AVISO:") {
            t2 = log_warn;
        } else if (prefixo == "ERRO: ") {
            t2 = log_err;
        } else if (prefixo == "INFO: ") {
            t2 = log_info;
        } else {
            prefixo = "INFO: ";
            msg = prefixo + msg;
            t2 = log_info;
        }
    } else {
        if (t == log_recv) {
            prefixo = "RECV: ";
        } else if (t == log_send) {
            prefixo = "SEND: ";
        } else if (t == log_info) {
            prefixo = "INFO: ";
        } else if (t == log_warn) {
            prefixo = "AVISO: ";
        } else {
            prefixo = "ERRO: ";
        }
    }
    if (t2 == log_recv) {
        this->janelaLogs->setTextColor (QColor (0, 128, 0));
    } else if (t2 == log_send) {
        this->janelaLogs->setTextColor (QColor (0, 0, 128));
    } else if (t2 == log_info) {
        this->janelaLogs->setTextColor (QColor (32, 32, 32));
    } else if (t2 == log_warn) {
        this->janelaLogs->setTextColor (QColor (160, 160, 0));
    } else {
        this->janelaLogs->setTextColor (QColor (255, 0, 0));
    }
    if (t == log_auto) {
        this->janelaLogs->append (prefixo + msg.mid(6));
    } else {
        this->janelaLogs->append (prefixo + msg);
    }
    QScrollBar *s = this->janelaLogs->verticalScrollBar();
    s->setValue (s->maximum());
}

void visualizacao::imagemVisualizacao_selecionouArea (QRect selecao, bool soltouMouse) {
    if (this->estaEmProcessoDeNovaImagem != nada || this->zooms.isEmpty ()) {
        return;
    }
    this->ultimaSelecao = this->proporcao (this->imagemVisualizacao->rect(),
                                           selecao,
                                           this->zooms.last());
    this->lblInfoSelecao->setText (mkTextoInformativo (selecao, ultimaSelecao));
    if (soltouMouse) {
        if (selecao.isValid()) {
            this->registraNoLog (QString("Foi selecionada a area: ( ") + QString::number (selecao.left()) + " x " + QString::number (selecao.top()) + " ) - ( " + QString::number (selecao.right()) + " x " + QString::number (selecao.bottom ()) + " )", log_info);
            this->tim2->start (oTimeoutDoTimer);
        } else {
            this->tim2->stop ();
        }
    }
}

void visualizacao::novoFractal_clicked () {
    this->tim->stop ();
    if (this->estaEmProcessoDeNovaImagem != nada) {
        this->cancelaObterNovaImagem ();
    } else {
        this->estaEmProcessoDeNovaImagem = recarregando;
        this->ultimaSelecao = this->zooms.last ();
        this->imagemVisualizacao->setEnabled (false);
        this->novoFractal->setText ("Cancelar");
        this->aumentarZoom->setEnabled (false);
        this->diminuirZoom->setEnabled (false);
        this->salvarFractal->setEnabled (false);
        this->btnConfiguracao->setEnabled (false);
        this->obtemNovaImagem ();
    }
}

void visualizacao::diminuirZoom_clicked () {
    if (this->estaEmProcessoDeNovaImagem != nada) {
        this->cancelaObterNovaImagem ();
    }
}

void visualizacao::aumentarZoom_clicked () {
    if (this->estaEmProcessoDeNovaImagem != nada) {
        this->cancelaObterNovaImagem ();
    } else if (this->ultimaSelecao.isValid()) {
        this->estaEmProcessoDeNovaImagem = aumentandozoom;
        this->imagemVisualizacao->setEnabled (false);
        this->novoFractal->setEnabled (false);
        this->aumentarZoom->setText ("Cancelar");
        this->diminuirZoom->setEnabled (false);
        this->salvarFractal->setEnabled (false);
        this->btnConfiguracao->setEnabled (false);
        this->obtemNovaImagem ();
    }
}

void visualizacao::menuBotao_triggered (QAction *ac) {
    int l, y;
    if (this->estaEmProcessoDeNovaImagem != nada) return;
    l = this->zooms.count() - 2;
    if (l < 0) return;
    y = 0;
    while (this->menuBotao->actions()[y] != ac) {
        l--;
        y++;
        // Isso nao deveria acontecer...
        if (l < 0) return;
    }
    this->estaEmProcessoDeNovaImagem = reduzindozoom;
    this->imagemVisualizacao->setEnabled (false);
    this->novoFractal->setEnabled (false);
    this->aumentarZoom->setEnabled (false);
    this->diminuirZoom->setText ("Cancelar");
    this->diminuirZoom->setMenu (NULL);
    this->salvarFractal->setEnabled (false);
    this->btnConfiguracao->setEnabled (false);
    this->ultimaSelecao = this->zooms[l];
    this->obtemNovaImagem ();
}

void visualizacao::o_proc_error (QProcess::ProcessError e) {
    if (this->estaEmProcessoDeNovaImagem == nada) return;
    if (e == QProcess::FailedToStart) {
        this->registraNoLog(QString ("O programa '") + this->janeConfiguracao->cfgCaminhoExecutavel + "' nao pode ser iniciado. Abortando...", log_err);
    } else if (e == QProcess::Crashed) {
        this->registraNoLog(QString ("O programa '") + this->janeConfiguracao->cfgCaminhoExecutavel + "' terminou inesperadamente. Abortando...", log_err);
    } else if (e == QProcess::Timedout) {
        this->registraNoLog(QString ("Ocorreu um erro de temporizacao durante a execucao do programa '") + this->janeConfiguracao->cfgCaminhoExecutavel + "'. Abortando...", log_err);
    } else if (e == QProcess::WriteError || e == QProcess::ReadError) {
        this->registraNoLog(QString ("Ocorreu um erro de comunicacao com o programa '") + this->janeConfiguracao->cfgCaminhoExecutavel + "'. Abortando...", log_err);
    } else {
        this->registraNoLog(QString ("Ocorreu um erro desconhecido durante a execucao do programa '") + this->janeConfiguracao->cfgCaminhoExecutavel + "'. Abortando...", log_err);
    }
    this->cancelaObterNovaImagem ();
}

void visualizacao::o_proc_finished (int codigo) {
    int u;
    if (this->estaEmProcessoDeNovaImagem == nada) return;
    if (codigo) {
        this->registraNoLog(QString ("O programa '") + this->janeConfiguracao->cfgCaminhoExecutavel + "' nao finalizou com sucesso.", log_warn);
    } else {
        this->registraNoLog(QString ("O programa '") + this->janeConfiguracao->cfgCaminhoExecutavel + "' finalizou com sucesso.", log_info);
    }
    if (this->imagemVisualizacao->pixNaTelaOriginal.load (this->tmpFile->fileName (), "PNG")) {
        u = this->t1.elapsed ();
        QTime t2 = QTime (0, 0, 0, 0).addMSecs (u);
        this->registraNoLog (QString ("Imagem gerada pelo programa foi reconhecida. Tempo para a geracao foi de ") + QString::number (u) + " ms [ " +
                             QString::number (t2.minute()) + "min ; " +
                             QString::number (t2.second()) + "s ; " +
                             QString::number (t2.msec()) + "ms ].", log_info);
        this->jaCarregouNovaImagem ();
        this->setWindowModified (false);
    } else {
        this->registraNoLog(QString ("A imagem gerada pelo programa '") + this->janeConfiguracao->cfgCaminhoExecutavel + "' nao foi reconhecida.", log_err);
    }
    if (this->tmpFile != NULL) {
        this->tmpFile->remove ();
        APAGA_OBJETO (this->tmpFile);
    }
    this->reabilitaBotoes ();
}

void visualizacao::o_proc_readyReadStandardError () {
    int i, l, n;
    char u;
    bool x;
    QString tmp;
    if (this->estaEmProcessoDeNovaImagem == nada) return;
    this->stderr_buffer += this->o_proc->readAllStandardError ();
    do {
        x = false;
        tmp = "";
        l = this->stderr_buffer.length ();
        for (i = 0; i < l; i++) {
            u = this->stderr_buffer[i];
            if (u == '\n' || u == '\r') {
                x = true;
                n = tmp.length ();
                if (n != 0) {
                    this->registraNoLog (tmp);
                }
                this->stderr_buffer.remove (0, n + 1);
                break;
            } else {
                tmp += u;
            }
        }
    } while (x);
}

void visualizacao::o_proc_started () {
    if (this->estaEmProcessoDeNovaImagem == nada) return;
    this->registraNoLog(QString ("O programa '") + this->janeConfiguracao->cfgCaminhoExecutavel + "' foi iniciado com sucesso.", log_info);
    this->t1.start ();
}

void visualizacao::o_sock_connected () {
    if (this->estaEmProcessoDeNovaImagem == nada) return;
    this->passoSocket = 0;
    this->registraNoLog (QString ("Conectado com sucesso ao host '") + this->janeConfiguracao->cfgIpDoServidor + "'.", log_info);
    this->enviaDado ("LARG", this->imagemVisualizacao->width());
}

void visualizacao::o_sock_disconnected () {
    int u, c;
    if (this->passoSocket == 13) {
        QByteArray tudo = this->o_sock->readAll ();
        c = tudo.count ();
        if (c == 0) {
            this->registraNoLog ("Foi recebida uma resposta de tamanho nulo.", log_err);
        } else {
            u = this->t1.elapsed();
            QTime t2 = QTime (0, 0, 0, 0).addMSecs (u);
            this->registraNoLog (QString ("Download da imagem foi efetuado com sucesso. O tempo de transferencia foi de ") + QString::number (u) + " ms [ " +
                                 QString::number (t2.minute()) + "min ; " +
                                 QString::number (t2.second()) + "s ; " +
                                 QString::number (t2.msec()) + "ms ].", log_info);
            if (u != 0)
                this->registraNoLog (QString ("Taxa de transferencia media: ") + QString::number (c / u) + " KB/s.", log_info);
            if (this->imagemVisualizacao->pixNaTelaOriginal.loadFromData (tudo, "PNG")) {
                this->registraNoLog ("Imagem gerada pelo servidor foi carregada com sucesso.", log_info);
                this->jaCarregouNovaImagem ();
                this->setWindowModified (false);
            } else {
                this->registraNoLog ("Imagem gerada pelo servidor nao foi carregada com sucesso.", log_err);
            }
        }
    } else {
        this->registraNoLog ("A conexao com o servidor foi encerrada inesperadamente.", log_err);
    }
    this->reabilitaBotoes ();
}

void visualizacao::o_sock_readyRead () {
    int param2INT, rr, gg, bb, pos, u;
    double param2DOUB;
    QListWidget *obj;
    bool ok;
    obj = this->janeConfiguracao->paletaDeCoresReal;
    if (this->passoSocket == 13) return;
    while (this->o_sock->canReadLine ()) {
        QString linha (this->o_sock->readLine ());
        linha = linha.trimmed ();
        if (this->passoSocket != 9) {
            this->registraNoLog (linha, log_recv);
        }
        if (this->passoSocket < 5) {
            if (this->recebeDado (linha, param2INT)) {
                if (this->paramINT != param2INT) {
                    switch (this->passoSocket) {
                        case 1:
                            this->registraNoLog ("O servidor nao aceitou a largura de imagem especificada na configuracao.", log_warn);
                            break;
                        case 2:
                            this->registraNoLog ("O servidor nao aceitou a altura de imagem especificada na configuracao.", log_warn);
                            break;
                        case 3:
                            this->registraNoLog ("O servidor nao aceitou a quantidade de cores especificada na configuracao.", log_warn);
                            this->quantasCores = param2INT;
                            break;
                        case 4:
                            this->registraNoLog ("O servidor nao aceitou o numero maximo de iteracoes especificado na configuracao.", log_warn);
                            break;
                    }
                }
                switch (this->passoSocket) {
                    case 1:
                        this->enviaDado ("ALT", this->imagemVisualizacao->height());
                        break;
                    case 2:
                        this->passoPaleta = 0;
                        this->quantasCores = obj->count();
                        this->enviaDado ("PALLEN", this->quantasCores);
                        break;
                    case 3:
                        this->enviaDado ("MAXITER", this->janeConfiguracao->cfgNoIteracoes);
                        break;
                    case 4:
                        this->enviaDado ("MINX", this->ultimaSelecao.left());
                        break;
                }
            } else {
                this->registraNoLog ("Resposta do servidor nao foi reconhecida. Abortando...", log_err);
                this->cancelaObterNovaImagem();
                return;
            }
        } else if (this->passoSocket < 9) {
            if (this->recebeDado (linha, param2DOUB)) {
                if (this->paramDOUB != param2DOUB) {
                    switch (this->passoSocket) {
                        case 5:
                            this->registraNoLog ("O servidor nao aceitou a abcissa menor especificada na imagem.", log_warn);
                            break;
                        case 6:
                            this->registraNoLog ("O servidor nao aceitou a abcissa maior especificada na imagem.", log_warn);
                            break;
                        case 7:
                            this->registraNoLog ("O servidor nao aceitou a ordenada menor especificada na imagem.", log_warn);
                            break;
                        case 8:
                            this->registraNoLog ("O servidor nao aceitou a ordenada maior especificada na imagem.", log_warn);
                            break;
                    }
                }
                switch (this->passoSocket) {
                    case 5:
                        this->enviaDado ("MAXX", this->ultimaSelecao.right());
                        break;
                    case 6:
                        this->enviaDado ("MINY", this->ultimaSelecao.top());
                        break;
                    case 7:
                        this->enviaDado ("MAXY", this->ultimaSelecao.bottom());
                        break;
                    case 8:
                        this->paramQColor = obj->item(0)->backgroundColor();
                        this->registraNoLog ("Enviando paleta de cores para o servidor...", log_info);
                        this->enviaDado (QString ("PAL 0 ") + QString::number (this->paramQColor.red()) + " " + QString::number (this->paramQColor.green()) + " " + QString::number (this->paramQColor.blue()));
                        break;
                }
            } else {
                this->registraNoLog ("Resposta do servidor nao foi reconhecida. Abortando...", log_err);
                this->cancelaObterNovaImagem();
                return;
            }
        } else if (this->passoSocket == 9) {
            QStringList partes = linha.split (QRegExp ("\\s+"), QString::SkipEmptyParts);
            if (partes.count() != 5) {
                this->registraNoLog ("Resposta do servidor nao foi reconhecida. Abortando...", log_err);
                this->cancelaObterNovaImagem();
                return;
            }
            if (partes[0].toUpper() != "OK") {
                this->registraNoLog ("Resposta do servidor nao foi reconhecida. Abortando...", log_err);
                this->cancelaObterNovaImagem();
                return;
            }
            pos = partes[1].toInt (&ok);
            if ((!ok) || pos != this->passoPaleta) {
                this->registraNoLog ("Resposta do servidor nao foi reconhecida. Abortando...", log_err);
                this->cancelaObterNovaImagem();
                return;
            }
            rr = partes[2].toInt (&ok);
            if (!ok) {
                this->registraNoLog ("Resposta do servidor nao foi reconhecida. Abortando...", log_err);
                this->cancelaObterNovaImagem();
                return;
            }
            gg = partes[3].toInt (&ok);
            if (!ok) {
                this->registraNoLog ("Resposta do servidor nao foi reconhecida. Abortando...", log_err);
                this->cancelaObterNovaImagem();
                return;
            }
            bb = partes[4].toInt (&ok);
            if (!ok) {
                this->registraNoLog ("Resposta do servidor nao foi reconhecida. Abortando...", log_err);
                this->cancelaObterNovaImagem();
                return;
            }
            if (QColor (rr, gg, bb) != this->paramQColor) {
                this->registraNoLog ("O servidor nao aceitou como valida a cor especificada na configuracao.", log_warn);
            }
            if (++pos == this->quantasCores) {
                this->registraNoLog ("Paleta de cores enviada com sucesso.", log_info);
                this->enviaDado ("AGORA");
            } else {
                this->paramQColor = obj->item(pos)->backgroundColor();
                this->passoSocket = 8;
                this->enviaDado (QString ("PAL ") + QString::number (pos) + " " + QString::number (this->paramQColor.red()) + " " + QString::number (this->paramQColor.green()) + " " + QString::number (this->paramQColor.blue()));
            }
            this->passoPaleta = pos;
        } else if (this->passoSocket == 10) {
            if (this->recebeDado (linha, param2DOUB)) {
                this->t1_d = param2DOUB;
                this->enviaDado ("MAKE");
            } else {
                this->registraNoLog ("Resposta do servidor nao foi reconhecida. Abortando...", log_err);
                this->cancelaObterNovaImagem();
                return;
            }
        } else if (this->passoSocket == 11) {
            if (linha.toUpper() == "OK") {
                this->enviaDado ("AGORA");
            } else {
                this->registraNoLog ("Resposta do servidor nao foi reconhecida. Abortando...", log_err);
                this->cancelaObterNovaImagem();
                return;
            }
        } else if (this->passoSocket == 12) {
            if (this->recebeDado (linha, param2DOUB)) {
                u = (int) (1000.0 * (param2DOUB - this->t1_d));
                QTime t2 = QTime (0, 0, 0, 0).addMSecs (u);
                this->registraNoLog (QString ("Imagem foi criada pelo servidor com sucesso. Tempo para a geracao foi de ") + QString::number (u) + " ms [ " +
                                     QString::number (t2.minute()) + "min ; " +
                                     QString::number (t2.second()) + "s ; " +
                                     QString::number (t2.msec()) + "ms ].", log_info);
                this->t1.start ();
                this->registraNoLog ("Solicitando download da imagem agora...", log_info);
                this->enviaDado ("DUMP");
            } else {
                this->registraNoLog ("Resposta do servidor nao foi reconhecida. Abortando...", log_err);
                this->cancelaObterNovaImagem();
                return;
            }
        }
        if (this->passoSocket == 13) break;
    }
}

bool visualizacao::recebeDado (QString s, int& saida) {
    int n;
    bool ok;
    if (s.length () < 4)
        return (false);
    QString ps = s.left (3).toUpper ();
    if (ps != "OK ")
        return (false);
    n = s.mid (3).trimmed().toInt (&ok);
    if (ok) {
        saida = n;
    }
    return (ok);
}

bool visualizacao::recebeDado (QString s, double& saida) {
    double n;
    bool ok;
    if (s.length () < 4)
        return (false);
    QString ps = s.left (3).toUpper ();
    if (ps != "OK ")
        return (false);
    n = s.mid (3).toDouble (&ok);
    if (ok) {
        saida = n;
    }
    return (ok);
}

void visualizacao::enviaDado (QString s) {
    this->passoSocket++;
    if (this->passoSocket != 9) {
        this->registraNoLog (s, log_send);
    }
    this->o_sock->write (s.append("\n").toAscii());
    this->o_sock->flush ();
}

void visualizacao::enviaDado (QString s, int n) {
    this->paramINT = n;
    this->enviaDado (s + " " + QString::number (n));
}

void visualizacao::enviaDado (QString s, double n) {
    this->paramDOUB = n;
    this->enviaDado (s + " " + QString::number (n, tipoN, precisao_mais));
}

void visualizacao::o_sock_error (QAbstractSocket::SocketError e) {
    if (this->estaEmProcessoDeNovaImagem == nada) return;
    switch (e) {
        case QAbstractSocket::ConnectionRefusedError:
            this->registraNoLog (QString ("Host '") + this->janeConfiguracao->cfgIpDoServidor + "' recusou a conexao pela porta " + QString::number (this->janeConfiguracao->cfgPortaTCP) + ". Abortando...", log_err);
            break;
        case QAbstractSocket::RemoteHostClosedError:
            this->o_sock_disconnected ();
            return;
        case QAbstractSocket::HostNotFoundError:
            this->registraNoLog (QString ("Host '") + this->janeConfiguracao->cfgIpDoServidor + "' nao foi localizado. Abortando...", log_err);
            break;
        case QAbstractSocket::SocketTimeoutError:
            this->registraNoLog (QString ("Estouro de tempo limite durante a conexao ao host'") + this->janeConfiguracao->cfgIpDoServidor + "' na porta " + QString::number (this->janeConfiguracao->cfgPortaTCP) + ". Abortando...", log_err);
            break;
        case QAbstractSocket::SocketAccessError:
        case QAbstractSocket::SocketResourceError:
        case QAbstractSocket::NetworkError:
            this->registraNoLog (QString ("Erro de sistema durante a conexao ao host'") + this->janeConfiguracao->cfgIpDoServidor + "' na porta " + QString::number (this->janeConfiguracao->cfgPortaTCP) + ". Abortando...", log_err);
            break;
        default:
            this->registraNoLog (QString ("Erro desconhecido durante a conexao ao host'") + this->janeConfiguracao->cfgIpDoServidor + "' na porta " + QString::number (this->janeConfiguracao->cfgPortaTCP) + ". Abortando...", log_err);
    }
    this->cancelaObterNovaImagem ();
}

void visualizacao::o_sock_hostFound () {
    if (this->estaEmProcessoDeNovaImagem == nada) return;
    this->registraNoLog (QString ("Host '") + this->janeConfiguracao->cfgIpDoServidor + "' foi encontrado. Estabelecendo conexao...", log_info);
}

void visualizacao::obtemNovaImagem () {
    if (this->estaEmProcessoDeNovaImagem == nada) {
        return;
    }
    if (this->o_sock != NULL) {
        APAGA_OBJETO (this->o_sock);
    }
    if (this->o_proc != NULL) {
        APAGA_OBJETO (this->o_proc);
    }
    NOVO_OBJETO (this->o_proc, QProcess (this));
    NOVO_OBJETO (this->o_sock, QTcpSocket (this));
    QObject::connect (this->o_proc, SIGNAL(error(QProcess::ProcessError)), this, SLOT(o_proc_error(QProcess::ProcessError)));
    QObject::connect (this->o_proc, SIGNAL(finished(int)), this, SLOT(o_proc_finished(int)));
    QObject::connect (this->o_proc, SIGNAL(readyReadStandardError()), this, SLOT(o_proc_readyReadStandardError()));
    QObject::connect (this->o_proc, SIGNAL(started()), this, SLOT(o_proc_started()));
    QObject::connect (this->o_sock, SIGNAL(connected()), this, SLOT(o_sock_connected()));
    QObject::connect (this->o_sock, SIGNAL(error (QAbstractSocket::SocketError)), this, SLOT(o_sock_error (QAbstractSocket::SocketError)));
    QObject::connect (this->o_sock, SIGNAL(hostFound()), this, SLOT(o_sock_hostFound()));
    QObject::connect (this->o_sock, SIGNAL(readyRead()), this, SLOT(o_sock_readyRead()));
    this->registraNoLog ("Iniciando processo de obtencao de um fractal...", log_info);
    this->setCursor (Qt::WaitCursor);
    if (this->janeConfiguracao->cfgModoObtencaoConexao) {
        this->registraNoLog (QString ("Localizando host '") + this->janeConfiguracao->cfgIpDoServidor + "'...", log_info);
        this->o_sock->connectToHost (this->janeConfiguracao->cfgIpDoServidor, this->janeConfiguracao->cfgPortaTCP);
    } else {
        QStringList args;
        QListWidget *li;
        QColor bg;
        int lM, i;
        NOVO_OBJETO (this->tmpFile, QTemporaryFile (this));
        this->tmpFile->setAutoRemove (false);
        if (! this->tmpFile->open ()) {
            this->registraNoLog ("Erro: arquivo para armazenamento temporario de imagem nao pode ser criado.", log_err);
            this->cancelaObterNovaImagem ();
        } else {
            args.append ("-o");
            args.append (this->tmpFile->fileName ());
            args.append ("-m");
            args.append (QString::number (this->janeConfiguracao->cfgNoIteracoes));
            args.append ("-l");
            args.append (QString::number (this->imagemVisualizacao->width()));
            args.append ("-a");
            args.append (QString::number (this->imagemVisualizacao->height()));
            args.append ("-x");
            args.append (QString::number (this->ultimaSelecao.left(), tipoN, precisao_mais));
            args.append ("-X");
            args.append (QString::number (this->ultimaSelecao.right(), tipoN, precisao_mais));
            args.append ("-y");
            args.append (QString::number (this->ultimaSelecao.top(), tipoN, precisao_mais));
            args.append ("-Y");
            args.append (QString::number (this->ultimaSelecao.bottom(), tipoN, precisao_mais));
            li = this->janeConfiguracao->paletaDeCoresReal;
            lM = li->count();
            for (i = 0; i < lM; i++) {
                bg = li->item(i)->backgroundColor();
                args.append (QString::number (bg.red()));
                args.append (QString::number (bg.green()));
                args.append (QString::number (bg.blue()));
            }
            this->registraNoLog(QString ("Executando programa '") + this->janeConfiguracao->cfgCaminhoExecutavel + "'...");
            this->o_proc->setReadChannel (QProcess::StandardError);
            this->o_proc->setReadChannelMode (QProcess::SeparateChannels);
            this->stderr_buffer.clear ();
            this->o_proc->start (this->janeConfiguracao->cfgCaminhoExecutavel, args);
        }
    }
}

void visualizacao::cancelaObterNovaImagem () {
    if (this->estaEmProcessoDeNovaImagem == nada) return;
    if (this->janeConfiguracao->cfgModoObtencaoConexao) {
        this->o_sock->close ();
        this->o_sock->disconnectFromHost ();
        this->o_sock->abort ();
    } else {
        this->o_proc->kill ();
    }
    if (this->tmpFile != NULL) {
        this->tmpFile->remove ();
        APAGA_OBJETO (this->tmpFile);
    }
    this->reabilitaBotoes ();
    if (this->janeConfiguracao->cfgModoObtencaoConexao) {
        this->registraNoLog (QString ("A conexao com o host '") + this->janeConfiguracao->cfgIpDoServidor + "' foi interrompida.", log_err);
    } else {
        this->registraNoLog(QString ("O programa '") + this->janeConfiguracao->cfgCaminhoExecutavel + "' foi finalizado.", log_err);
    }
}

void visualizacao::jaCarregouNovaImagem () {
    QRectF ult;
    QAction *a;
    if (this->estaEmProcessoDeNovaImagem == aumentandozoom) {
        NOVO_OBJETO (a, QAction (this->makeEntradaMenu (this->zooms.last()), this->menuBotao));
        this->menuBotao->insertAction (this->menuBotao->actions().first(), a);
        this->zooms.append (this->ultimaSelecao);
    } else if (this->estaEmProcessoDeNovaImagem == reduzindozoom) {
        while (! this->zooms.isEmpty ()) {
            ult = this->zooms.last();
            if (this->ultimaSelecao.width() > ult.width() ||
                this->ultimaSelecao.height() > ult.height()) {
                this->zooms.removeLast();
                a = this->menuBotao->actions().takeFirst();
                APAGA_OBJETO (a);
            } else {
                break;
            }
        }
        if (this->zooms.isEmpty ()) {
            // Realmente, isso NAO deveria acontecer... Mas, vai saber...
            this->zooms.append (QRectF (-2.0, -2.0, 4.0, 4.0));
        }
    }
}

void visualizacao::reabilitaBotoes () {
    QString s;
    this->ultimaSelecao.setWidth (0);
    this->ultimaSelecao.setHeight (0);
    this->unsetCursor ();
    this->estaEmProcessoDeNovaImagem = nada;
    this->lblInfoSelecao->setText ("");
    this->novoFractal->setText ("Recarregar fractal");
    this->diminuirZoom->setText ("Reduzir zoom...");
    this->diminuirZoom->setMenu (this->menuBotao);
    this->aumentarZoom->setText ("Zoom na selecao");
    this->salvarFractal->setText ("Salvar imagem como...");
    this->btnConfiguracao->setText ("Configurar...");
    this->imagemVisualizacao->setEnabled (true);
    this->novoFractal->setEnabled (true);
    this->aumentarZoom->setEnabled (true);
    this->diminuirZoom->setEnabled (this->zooms.count() > 1);
    this->salvarFractal->setEnabled (true);
    this->btnConfiguracao->setEnabled (true);
    if (! this->zooms.isEmpty()) {
        s = this->mkTextoInformativo (this->imagemVisualizacao->rect(), this->zooms.last ());
    }
    this->lblInfoImagem->setText (s);
    this->imagemVisualizacao->recarrega ();
    this->imagemVisualizacao->update ();
}

void visualizacao::tim_timeout () {
    if (this->estaEmProcessoDeNovaImagem != nada) return;
    this->tim2->stop ();
    if (this->janeConfiguracao->cfgAtualizacaoAutomatica) {
        this->registraNoLog ("A imagem necessita ser recarregada.", log_info);
        this->novoFractal_clicked ();
    } else {
        this->registraNoLog ("A imagem necessita ser recarregada, mas o modo de recarga automatica esta desativado.", log_warn);
    }
}

void visualizacao::tim2_timeout () {
    if (this->estaEmProcessoDeNovaImagem != nada) return;
    this->tim->stop ();
    if (this->janeConfiguracao->cfgAtualizacaoAutomatica && this->ultimaSelecao.isValid()) {
        this->registraNoLog ("Efetuando zoom na area selecionada...", log_info);
        this->aumentarZoom_clicked ();
    }
}

QString visualizacao::makeEntradaMenu (QRectF area) {
    return (QString ("") +
            "( " + QString::number (area.left (),  'g', precisao) + " , " + QString::number (area.top (),    'g', precisao) + " )" +
            " a " +
            "( " + QString::number (area.right (), 'g', precisao) + " , " + QString::number (area.bottom (), 'g', precisao) + " )"
           );
}
// visualizacao.cpp
