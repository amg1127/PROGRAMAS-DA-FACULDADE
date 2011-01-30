#ifndef SERVIDOR_H
#define SERVIDOR_H

class servidor;
class usuarios_sala;

#include "common.h"
#include "messenger.h"
#include "comando.h"

class servidor : public QServerSocket {
    Q_OBJECT;
    private:
        int quantos_usuarios;
        usuarios_sala *dentro_da_sala [MAX_USUARIOS];
        usuarios_sala *entrando_na_sala;
        usuarios_sala *saindo_da_sala;
        void marca_destroi (usuarios_sala*);
        Messenger *janela;
    public:
        QTimer *tim;
        ~servidor ();
        servidor (Q_UINT16, Messenger *);
        void newConnection (int);
    private slots:
        void RespostaEnviada (usuarios_sala*, QStringList, QStringList);
        void ComandoRecebido (usuarios_sala*, QStringList);
        void ErroComunicando (usuarios_sala*, QString);
        void timerTimeout ();
};

class usuarios_sala : public QObject {
    Q_OBJECT;
    public:
        QString Nome;
        QFont Estilo;
        QColor Cor;
        comando Conexao;
        usuarios_sala (int);
    signals:
        void RespostaEnviada (usuarios_sala*, QStringList, QStringList);
        void ComandoRecebido (usuarios_sala*, QStringList);
        void ComandoExecutado (usuarios_sala*, QStringList, QStringList);
        void ErroComunicando (usuarios_sala*, QString);
    private slots:
        void RespostaEnviada (QStringList, QStringList);
        void ComandoRecebido (QStringList);
        void ComandoExecutado (QStringList, QStringList);
        void ErroComunicando (QString);
};
#endif // SERVIDOR_H
