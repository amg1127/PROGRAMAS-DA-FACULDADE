#include "comando.h"

comando::comando () {
    DEBUGA("comando::comando ();");
    NOVO_OBJETO (this->t, QTimer (this));
    this->_Estado = QueEstado::Pronto;
    this->novalinha = QRegExp (QString("[\\r\\n]"));
    Q_ASSERT (this->novalinha.isValid());
    this->_flag = true;
    QObject::connect (this->t, SIGNAL(timeout()), this, SLOT (timerTimeout()));
    QObject::connect (this, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
    QObject::connect (this, SIGNAL(connected()), this, SLOT(socketConnected()));
    QObject::connect (this, SIGNAL(connectionClosed()), this, SLOT(socketConnectionClosed()));
    QObject::connect (this, SIGNAL(error(int)), this, SLOT(socketError(int)));
}

comando::~comando () {
    DEBUGA("comando::~comando ();");
    QObject::disconnect (this->t, 0, this, 0);
    QObject::disconnect (this, 0, this, 0);
    APAGA_OBJETO (this->t);
}

void comando::f__eu (const char *s) {
    DEBUGA("void comando::f__eu (const char *);");
    this->f__eu (QString (s));
}

void comando::f__eu (QString s) {
    DEBUGA("void comando::f__eu (QString);");
    this->lstParams.append (s);
    this->f__eu ();
}

void comando::f__eu (void) {
    DEBUGA("void comando::f__eu (void);");
    QString s("");
    this->close();
    this->t->stop();
    if (! this->lstParams.isEmpty()) {
        s = this->lstParams[this->lstParams.count() - 1];
        this->lstParams.clear();
    }
    emit ErroComunicando (QString(s));
    this->_Estado = QueEstado::Pronto;
}

void comando::AdicionaParametros (const char *str) {
    DEBUGA("void comando::AdicionaParametros (const char *);");
    this->AdicionaParametros (QString (str));
}

void comando::AdicionaParametros (QString str) {
    DEBUGA("void comando::AdicionaParametros (QString);");
    QStringList s;
    QValueList<QString>::size_type i;
    s = QStringList::split (this->novalinha, str, true);
    for (i=0; i<s.count(); i++)
        if (this->_flag && this->_Estado == QueEstado::Pronto)
            this->lstParams.append (s[i]);
        else if (this->_Estado == QueEstado::ExecutandoComando) {
            this->lstRetorno.append (s[i]);
            this->lstBackup3.append (s[i]);
        } else {
            if (this->comandosNaFila.isEmpty())
                this->comandosNaFila.append (s[i] + "\n");
            else
                this->comandosNaFila.last() += s[i] + "\n";
        }
}

void comando::EnviaDado (const char *msg) {
    DEBUGA("void comando::EnviaDado (const char *);");
    int i = 0;
    while (msg[i]) i++;
    this->writeBlock (msg, i);
    this->writeBlock ("\n", 1);
    this->flush ();
}

void comando::EnviaDado (QString msg) {
    DEBUGA("void comando::EnviaDado (QString);");
    this->EnviaDado ((const char*) msg.ascii());
}

QueEstado::__QueEstado comando::Estado (void) {
    DEBUGA("QueEstado::__QueEstado comando::Estado (void);");
    return (this->_Estado);
}

bool comando::Envia (const char *cmd) {
    DEBUGA("bool comando::Envia (const char *);");
    return (this->Envia (QString (cmd)));
}

bool comando::Envia (QString cmd) {
    DEBUGA("bool comando::Envia (QString);");
    QStringList s;
    int i;
    s = QStringList::split (this->novalinha, cmd, true);
    if (s.isEmpty()) return (false);
    if (this->_flag && this->_Estado == QueEstado::Pronto) {
        if (this->state() != QSocket::Connected) return (false);
        this->_Estado = QueEstado::EnviandoComando;
        for (i=s.count() - 1; i>=0; i--)
            this->lstParams.prepend (s[i]);
        this->EnviaDado (QString("CO ") + this->lstParams[0]);
        this->lstBackup.clear ();
        this->lstBackup.append (this->lstParams[0]);
        this->t->start (TEMPONEGOCIACAO, true);
        this->lstParams.remove (this->lstParams.begin());
    } else {
        for (i=s.count() - 1; i>=0; i--) {
            if (this->comandosNaFila.isEmpty ()) {
                this->comandosNaFila.append (s[i]);
            } else {
                this->comandosNaFila.last() = s[i] + "\n" + this->comandosNaFila.last();
            }
        }
        this->comandosNaFila.append ("");
    }
    return (true);
}

void comando::timerTimeout () {
    DEBUGA("void comando::timerTimeout ();");
    this->f__eu ("ERRO Tempo limite estourou.");
}

void comando::socketConnectionClosed () {
    DEBUGA("void comando::socketConnectionClosed ();");
    this->f__eu ("ERRO Conexao fechada inesperadamente pelo host remoto.");
}

void comando::socketError (int e) {
    DEBUGA("void comando::socketError (int);");
    e++;
    e--;
    switch (e) {
        case QSocket::ErrConnectionRefused:
            this->f__eu ("ERRO Conexao recusada pelo host remoto.");
            break;
        case QSocket::ErrHostNotFound:
            this->f__eu ("ERRO Servidor nao encontrado.");
            break;
        case QSocket::ErrSocketRead:
            this->f__eu ("ERRO Leitura do socket falhou.");
            break;
        default:
            this->f__eu ("ERRO (nao-especificado)");
    }
}

void comando::socketReadyRead () {
    DEBUGA("void comando::socketReadyRead ();");
    QString linha, inicio, fim;
    QStringList s;
    int i, l;
    while (this->canReadLine()) {
        linha = this->readLine().stripWhiteSpace();
        if (linha.length() < 2) continue;
        inicio = linha.left(2).stripWhiteSpace();
        fim = linha.mid(2).stripWhiteSpace();
        if (inicio.isEmpty()) {
            this->f__eu ("ERRO Comunicacao perdeu sincronismo.");
            return;
        }
        if (inicio == "CO") {
            if (fim.isEmpty()) {
                this->f__eu ("ERRO Comunicacao perdeu sincronismo.");
                return;
            }
        }
        if (this->_Estado == QueEstado::Pronto) {
            if (inicio != "CO") {
                this->f__eu ("ERRO Comunicacao perdeu sincronismo.");
                return;
            }
            this->lstParams.append (fim);
            this->lstBackup2.clear ();
            this->lstBackup2.append (fim);
            this->_Estado = QueEstado::RecebendoComando;
            this->t->start (TEMPONEGOCIACAO, true);
            this->EnviaDado ("OK");
        } else if (this->_Estado == QueEstado::RecebendoComando) {
            if (inicio == "PA") {
                this->lstParams.append (fim);
                this->lstBackup2.append (fim);
                this->EnviaDado ("OK");
            } else if (inicio == "EX") {
                this->_Estado = QueEstado::ExecutandoComando;
                this->lstRetorno.clear ();
                this->lstBackup3.clear ();
                emit ComandoRecebido (QStringList (this->lstParams));
                this->lstParams.clear();
                if (this->lstRetorno.isEmpty ()) {
                    this->funcaosemnome();
                } else {
                    this->_Estado = QueEstado::EnviandoResComando;
                    this->EnviaDado (QString("PA ") + this->lstRetorno.first());
                    this->lstRetorno.remove (this->lstRetorno.begin());
                }
            } else {
                this->f__eu ("ERRO Comunicacao perdeu sincronismo.");
                return;
            }
        } else if (this->_Estado == QueEstado::EnviandoComando) {
            if (inicio != "OK") {
                this->f__eu ("ERRO Comunicacao perdeu sincronismo.");
                return;
            }
            if (this->lstParams.isEmpty()) {
                this->EnviaDado ("EX");
                this->_Estado = QueEstado::RecebendoResComando;
            } else {
                this->EnviaDado (QString("PA ") + this->lstParams.first());
                this->lstBackup.append (this->lstParams.first());
                this->lstParams.remove (this->lstParams.begin());
            }
        } else if (this->_Estado == QueEstado::RecebendoResComando) {
            if (inicio == "PA") {
                this->lstRetorno.append (fim);
                this->EnviaDado ("OK");
            } else if (inicio == "EX") {
                this->_Estado = QueEstado::Pronto;
                this->t->stop ();
                this->_flag = false;
                emit ComandoExecutado (QStringList (this->lstBackup), QStringList (this->lstRetorno));
                this->lstRetorno.clear ();
                this->_flag = true;
                if (! this->comandosNaFila.isEmpty()) {
                    s = QStringList::split (this->novalinha, this->comandosNaFila.first(), true);
                    this->comandosNaFila.remove (this->comandosNaFila.begin());
                    l = s.count();
                    if (l > 0) {
                        for (i=1; i<l; i++) {
                            this->AdicionaParametros (s[i]);
                        }
                        this->Envia (s.first());
                    }
                }
            } else if (inicio == "CO") {
                this->_Estado = QueEstado::Pronto;
                this->t->stop ();
                this->_flag = false;
                emit ComandoExecutado (QStringList (this->lstBackup), QStringList (this->lstRetorno));
                this->lstRetorno.clear ();
                this->_flag = true;
                this->lstParams.append (fim);
                this->lstBackup2.clear ();
                this->lstBackup2.append (fim);
                this->_Estado = QueEstado::RecebendoComando;
                this->t->start (TEMPONEGOCIACAO, true);
            } else {
                this->f__eu ("ERRO Comunicacao perdeu sincronismo.");
                return;
            }
        } else if (this->_Estado == QueEstado::EnviandoResComando) {
            if (inicio != "OK") {
                this->f__eu ("ERRO Comunicacao perdeu sincronismo.");
                return;
            }
            if (this->lstRetorno.isEmpty ()) {
                this->funcaosemnome();
            } else {
                this->EnviaDado (QString("PA ") + this->lstRetorno.first());
                this->lstRetorno.remove (this->lstRetorno.begin());
            }
        } else {
            this->f__eu ("ERRO Comunicacao perdeu sincronismo.");
            return;
        }
    }
}

void comando::connectToHost (QString host, Q_UINT16 port) {
    DEBUGA ("void comando::connectToHost (QString, Q_UINT16);");
    QSocket::connectToHost (host, port);
    this->t->start (TEMPONEGOCIACAO, true);
}

void comando::socketConnected () {
    DEBUGA ("void comando::socketConnected ();");
    if (this->_Estado == QueEstado::Pronto)
        this->t->stop ();
}

void comando::funcaosemnome (void) {
    DEBUGA("void comando::funcaosemnome (void);");
    QStringList s;
    int i, l;
    this->t->stop ();
    this->_Estado = QueEstado::Pronto;
    this->_flag = false;
    emit RespostaEnviada (QStringList (this->lstBackup2), QStringList (this->lstBackup3));
    this->lstBackup2.clear();
    this->lstBackup3.clear();
    this->_flag = true;
    if (this->comandosNaFila.isEmpty()) {
        this->EnviaDado ("EX");
    } else {
        s = QStringList::split (this->novalinha, this->comandosNaFila.first(), true);
        this->comandosNaFila.remove (this->comandosNaFila.begin());
        l = s.count();
        if (l > 0) {
            for (i=1; i<l; i++) {
                this->AdicionaParametros (s[i]);
            }
            this->Envia (s.first());
        }
    }
}
