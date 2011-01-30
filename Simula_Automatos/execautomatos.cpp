#include "execautomatos.h"

execautomatos::execautomatos (QWidget* parent, Qt::WFlags f, automato& au) : QDialog (parent, f) {
    bool ok;
    this->aut = &au;
    ok = aut->validaAutomato();
    this->passo = 0;
    this->querFechar = false;
    this->setWindowTitle (QString(TITULO) + " - Assistente de Execucao de Automatos");
    this->setMinimumSize (150, 150);
    this->opcao = 0;

    this->btnVoltar = new QPushButton(this);
    Q_ASSERT (this->btnVoltar != NULL);
    this->btnVoltar->setText ("Cancelar");
    this->btnVoltar->adjustSize();
    QObject::connect (this->btnVoltar, SIGNAL(clicked(bool)), this, SLOT(btnVoltar_clicked(bool)));
    this->btnVoltar->show();

    this->btnAvancar = new QPushButton(this);
    Q_ASSERT (this->btnAvancar != NULL);
    this->btnAvancar->setText ("Proximo >>");
    this->btnAvancar->adjustSize();
    this->btnAvancar->setEnabled (ok);
    QObject::connect (this->btnAvancar, SIGNAL(clicked(bool)), this, SLOT(btnAvancar_clicked(bool)));
    this->btnAvancar->show();

    this->lblTelaInicial = new QLabel(this);
    Q_ASSERT (this->lblTelaInicial != NULL);
    this->lblTelaInicial->setWordWrap (true);
    this->lblTelaInicial->setText (QString("<center><big><big>Bem vindo ao 'Assistente de Execucao de Automatos'.</big></big><br><br>\n\n") +
    "Este assistente lhe ajudara a aplicar o automato finito sobre um conjunto de palavras e dizer quais destas sao perfeitamente reconhecidas pelo mesmo.<br><br><hr><br>\n\n" +
    ((ok) ? "Para comecar, pressione o botao 'Proximo'." : "<font color='#ff0000'>Erro: o automato finito construido possui alguma inconsistencia que impede a sua execucao. Clique no botao 'Cancelar' e efetue as correcoes necessarias.</font>") + "</center>");
    this->lblTelaInicial->show();

    this->lblDadosEntrada = new QLabel(this);
    Q_ASSERT (this->lblDadosEntrada != NULL);
    this->lblDadosEntrada->setWordWrap (true);
    this->lblDadosEntrada->setText ("Digite as palavras que serao utilizadas como entrada para o automato (separe as palavras com caracteres nao-imprimiveis, como SPACE ou ENTER). Opcionalmente, voce podera carrega-las a partir de um arquivo texto, usando o botao 'Importar dados...'.");
    this->lblDadosEntrada->adjustSize();
    this->lblDadosEntrada->hide();

    this->btnDadosEntrada = new QPushButton(this);
    Q_ASSERT (this->btnDadosEntrada != NULL);
    this->btnDadosEntrada->setText ("Importar dados...");
    this->btnDadosEntrada->adjustSize();
    QObject::connect (this->btnDadosEntrada, SIGNAL(clicked(bool)), this, SLOT(btnDadosEntrada_clicked(bool)));
    this->btnDadosEntrada->hide();

    this->txtDadosEntrada = new QTextEdit(this);
    Q_ASSERT (this->txtDadosEntrada != NULL);
    this->txtDadosEntrada->setReadOnly (false);
    this->txtDadosEntrada->setWordWrapMode (QTextOption::NoWrap);
    this->txtDadosEntrada->setFont (QFont ("Courier New"));
    this->txtDadosEntrada->hide();

    this->lblOpts = new QLabel(this);
    Q_ASSERT (this->lblOpts != NULL);
    this->lblOpts->setText ("Como o resultado deve ser exibido na tela?");
    this->lblOpts->adjustSize();
    this->lblOpts->hide();

    this->bgrpOpts = new QButtonGroup(this);

    this->radOpt0 = new QRadioButton(this);
    this->radOpt0->setText("Apenas mostrar as palavras reconhecidas");
    this->radOpt0->adjustSize();
    this->radOpt0->hide();
    QObject::connect (this->radOpt0, SIGNAL(toggled(bool)), this, SLOT(radOpt0_toggled(bool)));
    this->bgrpOpts->addButton(this->radOpt0);

    this->radOpt1 = new QRadioButton(this);
    this->radOpt1->setText("Apenas mostrar as palavras nao-reconhecidas");
    this->radOpt1->adjustSize();
    this->radOpt1->hide();
    QObject::connect (this->radOpt1, SIGNAL(toggled(bool)), this, SLOT(radOpt1_toggled(bool)));
    this->bgrpOpts->addButton(this->radOpt1);

    this->radOpt2 = new QRadioButton(this);
    this->radOpt2->setText("Mostrar as palavras reconhecidas depois das nao-reconhecidas");
    this->radOpt2->adjustSize();
    this->radOpt2->hide();
    QObject::connect (this->radOpt2, SIGNAL(toggled(bool)), this, SLOT(radOpt2_toggled(bool)));
    this->bgrpOpts->addButton(this->radOpt2);

    this->radOpt3 = new QRadioButton(this);
    this->radOpt3->setText("Mostrar as palavras reconhecidas antes das nao-reconhecidas");
    this->radOpt3->adjustSize();
    this->radOpt3->hide();
    QObject::connect (this->radOpt3, SIGNAL(toggled(bool)), this, SLOT(radOpt3_toggled(bool)));
    this->bgrpOpts->addButton(this->radOpt3);

    this->radOpt4 = new QRadioButton(this);
    this->radOpt4->setText("Mostrar as palavras na ordem em que foram inseridas");
    this->radOpt4->adjustSize();
    this->radOpt4->hide();
    QObject::connect (this->radOpt4, SIGNAL(toggled(bool)), this, SLOT(radOpt4_toggled(bool)));
    this->bgrpOpts->addButton(this->radOpt4);

    this->bgrpOpts->setExclusive (true);
    this->radOpt4->setChecked (true);

    this->pbarProgresso = new QProgressBar(this);
    Q_ASSERT (this->pbarProgresso != NULL);
    this->pbarProgresso->resize (300, 20);
    this->pbarProgresso->hide();

    this->lblProgresso = new QLabel(this);
    Q_ASSERT (this->lblProgresso != NULL);
    this->lblProgresso->setText ("<big><big><big><big>Aguarde enquanto os dados de entrada sao processados...</big></big></big></big>");
    this->lblProgresso->adjustSize();
    this->lblProgresso->hide();

    this->lblDadosSaida = new QLabel(this);
    Q_ASSERT (this->lblDadosSaida != NULL);
    this->lblDadosSaida->setWordWrap (true);
    this->lblDadosSaida->setText ("Resultados do automato finito:");
    this->lblDadosSaida->adjustSize();
    this->lblDadosSaida->hide();

    this->btnDadosSaida = new QPushButton(this);
    Q_ASSERT (this->btnDadosSaida != NULL);
    this->btnDadosSaida->setText ("Exportar dados...");
    this->btnDadosSaida->adjustSize();
    QObject::connect (this->btnDadosSaida, SIGNAL(clicked(bool)), this, SLOT(btnDadosSaida_clicked(bool)));
    this->btnDadosSaida->hide();

    this->txtDadosSaida = new QTextEdit(this);
    Q_ASSERT (this->txtDadosSaida != NULL);
    this->txtDadosSaida->setReadOnly (true);
    this->txtDadosSaida->setWordWrapMode (QTextOption::NoWrap);
    this->txtDadosSaida->setFont (QFont ("Courier New"));
    this->txtDadosSaida->hide();

    this->thexaut = new threadexecutaautomato (this, this->txtDadosEntrada, this->txtDadosSaida, this->pbarProgresso, this->mutex, this->aut, this->opcao);
    QObject::connect (this->thexaut, SIGNAL(finished()), this, SLOT(threadexecutaautomato_finished()));
}

void execautomatos::radOpt0_toggled (bool ok) {
    if (ok) {
        this->opcao = 0;
    }
}
void execautomatos::radOpt1_toggled (bool ok) {
    if (ok) {
        this->opcao = 1;
    }
}

void execautomatos::radOpt2_toggled (bool ok) {
    if (ok) {
        this->opcao = 2;
    }
}

void execautomatos::radOpt3_toggled (bool ok) {
    if (ok) {
        this->opcao = 3;
    }
}

void execautomatos::radOpt4_toggled (bool ok) {
    if (ok) {
        this->opcao = 4;
    }
}

void execautomatos::resizeEvent (QResizeEvent*) {
    int w, h;
    int btns_w, btns_h;
    int aux, aux2, aux3;
    w = this->width();
    h = this->height();

    btns_w = this->maior (this->btnVoltar->width(), this->btnAvancar->width());
    btns_h = this->maior (this->btnVoltar->height(), this->btnAvancar->height());
    this->btnVoltar->resize (btns_w, btns_h);
    this->btnAvancar->resize (btns_w, btns_h);
    this->btnAvancar->move ((2 * w - btns_w) / 3, h - ESPACO - btns_h);
    this->btnVoltar->move ((w - 2 * btns_w) / 3, h - ESPACO - btns_h);

    if (this->passo == 0) {
        // Tela inicial
        this->lblTelaInicial->move (ESPACO, ESPACO);
        this->lblTelaInicial->setFixedWidth (w - 2 * ESPACO);
        this->lblTelaInicial->adjustSize();
        this->lblTelaInicial->resize (w - 2 * ESPACO, this->maior (h - 3 * ESPACO - btns_h, this->lblTelaInicial->height()));
    } else if (this->passo == 1) {
        // Tela para inserir os dados de entrada
        this->lblDadosEntrada->setFixedWidth (w - 3 * ESPACO - this->btnDadosEntrada->width());
        this->lblDadosEntrada->adjustSize ();
        aux = 2 * this->btnDadosEntrada->height();
        this->lblDadosEntrada->move (ESPACO, ESPACO + (aux - this->lblDadosEntrada->height()) / 2);
        this->btnDadosEntrada->move (w - ESPACO - this->btnDadosEntrada->width(), ESPACO + (aux - this->btnDadosEntrada->height()) / 2);
        this->txtDadosEntrada->move (ESPACO, 2 * ESPACO + aux);
        this->txtDadosEntrada->resize (w - 2 * ESPACO, h - 4 * ESPACO - btns_h - aux);
    } else if (this->passo == 2) {
        // Tela de opcao
        aux = (h - 3 * ESPACO - btns_h) / 2 + ESPACO;
        aux2 = (w - this->maior (
                        this->maior (
                            this->lblOpts->width(),
                            this->radOpt0->width()
                        ),
                        this->radOpt1->width(),
                        this->radOpt2->width(),
                        this->radOpt3->width(),
                        this->radOpt4->width()
                    )) / 2;
        aux3 = aux - (this->lblOpts->height() +
                      this->radOpt0->height() +
                      this->radOpt1->height() +
                      this->radOpt2->height() +
                      this->radOpt3->height() +
                      this->radOpt4->height() +
                      ESPACO) / 2;
        this->lblOpts->move (aux2, aux3);
        this->radOpt0->move (aux2, this->lblOpts->y() + this->lblOpts->height() + ESPACO);
        this->radOpt1->move (aux2, this->radOpt0->y() + this->radOpt0->height());
        this->radOpt2->move (aux2, this->radOpt1->y() + this->radOpt1->height());
        this->radOpt3->move (aux2, this->radOpt2->y() + this->radOpt2->height());
        this->radOpt4->move (aux2, this->radOpt3->y() + this->radOpt3->height());
    } else if (this->passo == 3 || this->passo == 4 || this->passo == 5) {
        // Tela de progresso
        aux = (h - 3 * ESPACO - btns_h) / 2 + ESPACO;
        this->mutex.lock();
        this->pbarProgresso->move ((w - this->pbarProgresso->width()) / 2, aux + ESPACO / 2);
        this->mutex.unlock();
        this->lblProgresso->move ((w - this->lblProgresso->width()) / 2, aux - ESPACO / 2 - this->lblProgresso->height());
    } else if (this->passo == 6) {
        // Tela para mostrar os dados de saida
        this->lblDadosSaida->setFixedWidth (w - 3 * ESPACO - this->btnDadosSaida->width());
        this->lblDadosSaida->adjustSize ();
        aux = 2 * this->btnDadosSaida->height();
        this->lblDadosSaida->move (ESPACO, ESPACO + (aux - this->lblDadosSaida->height()) / 2);
        this->btnDadosSaida->move (w - ESPACO - this->btnDadosSaida->width(), ESPACO + (aux - this->btnDadosSaida->height()) / 2);
        this->txtDadosSaida->move (ESPACO, 2 * ESPACO + aux);
        this->txtDadosSaida->resize (w - 2 * ESPACO, h - 4 * ESPACO - btns_h - aux);
    }
}

void execautomatos::btnVoltar_clicked (bool) {
    if (this->passo == 0) {
        this->close();
    } else if (this->passo == 1) {
        this->passo = 0;
        this->lblTelaInicial->show();
        this->txtDadosEntrada->hide();
        this->btnDadosEntrada->hide();
        this->lblDadosEntrada->hide();
        this->btnVoltar->setText ("Cancelar");
        this->btnVoltar->adjustSize();
        this->resizeEvent (NULL);
    } else if (this->passo == 2) {
        this->passo = 1;
        this->lblOpts->hide();
        this->radOpt0->hide();
        this->radOpt1->hide();
        this->radOpt2->hide();
        this->radOpt3->hide();
        this->radOpt4->hide();
        this->txtDadosEntrada->show();
        this->btnDadosEntrada->show();
        this->lblDadosEntrada->show();
        this->resizeEvent (NULL);
    } else if (this->passo == 3) {
        this->passo = 4;
        this->btnVoltar->setEnabled (false);
        this->btnAvancar->setEnabled (false);
        this->thexaut->paraProcessamento ();
    } else if (this->passo == 6) {
        this->passo = 0;
        this->txtDadosSaida->clear();
        this->txtDadosSaida->hide();
        this->btnDadosSaida->hide();
        this->lblDadosSaida->hide();
        this->lblTelaInicial->show();
        this->btnVoltar->setText ("Cancelar");
        this->btnVoltar->adjustSize();
        this->btnAvancar->setText ("Proximo >>");
        this->btnAvancar->adjustSize();
        this->resizeEvent (NULL);
    }
}

void execautomatos::btnAvancar_clicked (bool) {
    if (this->passo == 0) {
        this->passo = 1;
        this->btnVoltar->setText ("<< Anterior");
        this->btnVoltar->adjustSize ();
        this->lblTelaInicial->hide();
        this->txtDadosEntrada->show();
        this->btnDadosEntrada->show();
        this->lblDadosEntrada->show();
        this->resizeEvent (NULL);
    } else if (this->passo == 1) {
        this->passo = 2;
        this->txtDadosEntrada->hide();
        this->btnDadosEntrada->hide();
        this->lblDadosEntrada->hide();
        this->lblOpts->show();
        this->radOpt0->show();
        this->radOpt1->show();
        this->radOpt2->show();
        this->radOpt3->show();
        this->radOpt4->show();
        this->resizeEvent (NULL);
    } else if (this->passo == 2) {
        this->passo = 3;
        this->txtDadosSaida->clear();
        this->lblOpts->hide();
        this->radOpt0->hide();
        this->radOpt1->hide();
        this->radOpt2->hide();
        this->radOpt3->hide();
        this->radOpt4->hide();
        this->pbarProgresso->show();
        this->lblProgresso->show();
        this->btnAvancar->setText ("Interromper");
        this->btnAvancar->adjustSize();
        this->pbarProgresso->setValue (this->pbarProgresso->minimum());
        this->resizeEvent (NULL);
        this->thexaut->start ();
    } else if (this->passo == 3) {
        this->passo = 5;
        this->btnVoltar->setEnabled (false);
        this->btnAvancar->setEnabled (false);
        this->thexaut->paraProcessamento ();
    } else if (this->passo == 6) {
        this->close();
    }
}

void execautomatos::btnDadosEntrada_clicked (bool) {
    static QString arqname;
    QString newarqname;
    QFile f;
    qint64 dread;
    char dados[1025];
    newarqname = QFileDialog::getOpenFileName ((QWidget*)0, QString (TITULO) + " - Abrir arquivo de texto", arqname);
    if (newarqname != "") {
        arqname = newarqname;
        f.setFileName (arqname);
        if (f.open (QIODevice::ReadOnly)) {
            while ((dread = f.read (dados, 1024)) != 0) {
                if (dread > 0) {
                    dados[dread] = 0;
                    this->txtDadosEntrada->insertPlainText (dados);
                } else if (QMessageBox::critical ((QWidget*)0, QString(TITULO) + " - Abrir arquivo de texto", QString("Ocorreu um erro durante a leitura do arquivo '") + arqname + "'.\n\nPossivel causa do erro: '" + f.errorString() + "'.", QMessageBox::Retry, QMessageBox::Cancel) != QMessageBox::Retry) {
                    break;
                }
            }
            this->txtDadosEntrada->ensureCursorVisible ();
        } else {
            QMessageBox::critical ((QWidget*)0, QString(TITULO) + " - Abrir arquivo de texto", QString("Erro:\n\nNao foi possivel carregar o arquivo '") + arqname + "'.\n\nPossivel causa do erro: '" + f.errorString() + "'.");
        }
    }
}

void execautomatos::btnDadosSaida_clicked (bool) {
    static QString arqname;
    QString newarqname;
    QFile f;
    qint64 dwritten;
    newarqname = QFileDialog::getSaveFileName ((QWidget*)0, QString (TITULO) + " - Salvar arquivo de texto", arqname);
    if (newarqname != "") {
        arqname = newarqname;
        f.setFileName (arqname);
        if (f.open (QIODevice::WriteOnly)) {
            do {
                dwritten = f.write (this->txtDadosSaida->toPlainText().toAscii());
                if (dwritten < 0) {
                    if (QMessageBox::critical ((QWidget*)0, QString(TITULO) + " - Salvar arquivo de texto", QString("Ocorreu um erro durante a gravacao do arquivo '") + arqname + "'.\n\nPossivel causa do erro: '" + f.errorString() + "'.", QMessageBox::Retry, QMessageBox::Cancel) != QMessageBox::Retry) {
                        break;
                    }
                    f.close();
                    if (! f.open (QIODevice::WriteOnly)) {
                        QMessageBox::critical ((QWidget*)0, QString(TITULO) + " - Salvar arquivo de texto", QString("Erro:\n\nNao foi possivel gravar o arquivo '") + arqname + "'.\n\nPossivel causa do erro: '" + f.errorString() + "'.");
                        break;
                    }
                }
            } while (dwritten < 0);
        } else {
            QMessageBox::critical ((QWidget*)0, QString(TITULO) + " - Salvar arquivo de texto", QString("Erro:\n\nNao foi possivel gravar o arquivo '") + arqname + "'.\n\nPossivel causa do erro: '" + f.errorString() + "'.");
        }
    }
}

void execautomatos::closeEvent (QCloseEvent *ev) {
    if (this->passo == 3 || this->passo == 4 || this->passo == 5) {
        ev->ignore();
        this->querFechar = true;
        this->btnAvancar_clicked (false);
    } else {
        ev->accept();
    }
}

void execautomatos::threadexecutaautomato_finished () {
    if (this->passo == 5) {
        QMessageBox::warning ((QWidget*)0, QString(TITULO) + " - Aviso", "A operacao foi cancelada pelo usuario!");
    }
    this->lblProgresso->hide();
    this->pbarProgresso->hide();
    this->btnVoltar->setEnabled(true);
    this->btnAvancar->setEnabled(true);
    if (this->passo == 3 || this->passo == 5) {
        this->passo = 6;
        this->btnAvancar->setText ("Concluir");
        this->btnAvancar->adjustSize ();
        this->btnVoltar->setText ("Reiniciar");
        this->btnVoltar->adjustSize ();
        this->txtDadosSaida->show();
        this->txtDadosSaida->setPlainText (this->thexaut->txtSaida);
        this->thexaut->txtSaida = "";
        this->btnDadosSaida->show();
        this->lblDadosSaida->show();
    } else {
        this->passo = 2;
        this->btnAvancar->setText ("Proximo >>");
        this->btnAvancar->adjustSize ();
        this->lblOpts->show();
        this->radOpt0->show();
        this->radOpt1->show();
        this->radOpt2->show();
        this->radOpt3->show();
        this->radOpt4->show();
    }
    this->resizeEvent (NULL);
    if (this->querFechar) {
        this->close();
    }
}

int threadexecutaautomato::contagem = 0;

threadexecutaautomato::threadexecutaautomato (QObject* parent, QTextEdit* caixaEntrada, QTextEdit* caixaSaida, QProgressBar* barra, QMutex& mutex, automato* aut, int& opcao) : QThread (parent) {
    this->caixaEntrada = caixaEntrada;
    this->caixaSaida = caixaSaida;
    this->barra = barra;
    this->mutex = &mutex;
    this->aut = aut;
    this->opcao = &opcao;
}

void threadexecutaautomato::paraProcessamento () {
    this->continuar = false;
}

void threadexecutaautomato::run () {
    int i, len, pos, parm, ii, reco, noreco, palvaz, tot;
    QByteArray txt;
    QString palavra, sapar1, sapar2, pal2;
    QChar carac;
    char ch;
    Q_ASSERT (this->contagem++ < 1);
    Q_ASSERT (this->barra != NULL);
    Q_ASSERT (this->caixaEntrada != NULL);
    Q_ASSERT (this->caixaSaida != NULL);
    Q_ASSERT (this->mutex != NULL);
    Q_ASSERT (this->aut != NULL);
    this->continuar = true;
    this->mutex->lock();
    this->barra->setMinimum (0);
    txt = this->caixaEntrada->toPlainText().toAscii();
    len = txt.count();
    this->barra->setMaximum (len - 1);
    this->mutex->unlock();
    pos = 0;
    this->txtSaida = "";
    sapar1 = "";
    sapar2 = "";
    parm = len / 100;
    reco = 0;
    noreco = 0;
    palvaz = 0;
    for (i = ii = 0; i < len && this->continuar; i++, ii++) {
        if (ii >= parm) {
            this->mutex->lock();
            this->barra->setValue(i);
            this->mutex->unlock();
            ii = 0;
        }
        ch = txt[i];
        carac = ch;
        if (carac.isSpace()) {
            if (palavra == "") {
                pal2 = "(palavra vazia)";
                palvaz++;
            } else {
                pal2 = "\"" + palavra + "\"";
            }
            if (this->aut->reconhece (palavra)) {
                reco++;
                if (this->opcao[0] < 4) {
                    sapar1 += pal2 + "\n";
                } else {
                    this->txtSaida += pal2 + " => ACEITA\n";
                }
            } else {
                noreco++;
                if (this->opcao[0] < 4) {
                    sapar2 += pal2 + "\n";
                } else {
                    this->txtSaida += pal2 + " => REJEITA\n";
                }
            }
            palavra = "";
        } else {
            palavra += carac;
        }
    }
    tot = reco + noreco;
    if (this->opcao[0] == 0) {
        this->txtSaida = "Palavras que foram aceitas pelo automato:\n\n" + sapar1;
    } else if (this->opcao[0] == 1) {
        this->txtSaida = "Palavras que foram rejeitadas pelo automato:\n\n" + sapar2;
    } else if (this->opcao[0] == 2) {
        this->txtSaida = "Palavras que foram rejeitadas pelo automato:\n\n" + sapar2;
        this->txtSaida += "\n**************************************************************\n\n";
        this->txtSaida += "Palavras que foram aceitas pelo automato:\n\n" + sapar1;
    } else if (this->opcao[0] == 3) {
        this->txtSaida = "Palavras que foram aceitas pelo automato:\n\n" + sapar1;
        this->txtSaida += "\n**************************************************************\n\n";
        this->txtSaida += "Palavras que foram rejeitadas pelo automato:\n\n" + sapar2;
    } else {
        this->txtSaida = "Resultado da execucao:\n\n" + this->txtSaida;
    }
    if (tot > 0) {
        this->txtSaida += "\n**************************************************************\n";
        this->txtSaida += "\n" + QString::number (tot) + " palavras foram verificadas, sendo que " + QString::number (palvaz) + " eram vazias.\n";
        this->txtSaida += QString::number (reco) + " palavras [" + QString::number (reco * 100 / tot) + "%] foram aceitas pelo automato.\n";
        this->txtSaida += QString::number (noreco) + " palavras [" + QString::number (100 - (reco * 100 / tot)) + "%] foram rejeitadas pelo automato.\n";
    }
    this->mutex->lock();
    this->barra->setValue(i);
    this->mutex->unlock();
    this->contagem--;
}
