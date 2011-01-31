#ifndef __COMANDO_H
#define __COMANDO_H

class comando;

#include "common.h"

namespace QueEstado {
    enum __QueEstado {
        Pronto,
        RecebendoComando,
        EnviandoComando,
        ExecutandoComando,
        RecebendoResComando,
        EnviandoResComando
    };
};

class comando : public QSocket {
    Q_OBJECT;
    private:
        QRegExp novalinha;
        QStringList lstParams, lstRetorno, lstBackup, lstBackup2, lstBackup3, comandosNaFila;
        QueEstado::__QueEstado _Estado;
        QTimer *t;
        void f__eu (void);
        void f__eu (const char *);
        void f__eu (QString);
        void EnviaDado (const char *);
        void EnviaDado (QString);
        void funcaosemnome (void);
        bool _flag;
    public:
        comando ();
        ~comando ();
        void AdicionaParametros (QString);
        void AdicionaParametros (const char *);
        bool Envia (QString);
        bool Envia (const char *);
        QueEstado::__QueEstado Estado (void);
        void connectToHost (QString, Q_UINT16);
    signals:
        void RespostaEnviada (QStringList, QStringList);
        void ComandoRecebido (QStringList);
        void ComandoExecutado (QStringList, QStringList);
        void ErroComunicando (QString);
    private slots:
        void timerTimeout ();
        void socketReadyRead ();
        void socketConnectionClosed ();
        void socketError (int);
        void socketConnected ();
};

#endif // __COMANDO_H
