#ifndef MESSENGER_H
#define MESSENGER_H

class Messenger;

#include "common.h"
#include "config_usuario.h"
#include "servidor.h"
#include "mensagens_tela.h"
#include "comando.h"

class Messenger : public QWidget {
    Q_OBJECT;
    private:
        bool pareceConectado;
        void genResize (void);
        void AtualizaInfo();
        void manda_conectar ();
        void manda_desconectar ();
        bool UsaCTRLEnter;
    public:
        Messenger (void);
        ~Messenger ();
        QTimer *tim;
        QLabel *lblTitulo, *lblInfo;
        QTextEdit *txtMensEnviar;
        mensagens_tela *msgtela;
        QStatusBar *comoVaoAsCoisas;
        QPushButton *btnConectar, *btnServir, *btnEnviar, *btnConfigurar;
        void mensagemNaBarra (QString);
        void mensagemNaBarra (QString, int);
        Q_UINT16 porta;
        QString servremoto;
        QString EuMesmo_Nome;
        QFont EuMesmo_Estilo;
        QColor EuMesmo_Cor;
        servidor* srv;
        comando conex;
    public slots:
        void resizeEvent (QResizeEvent *);
        void btnEnviarClicked ();
        void btnConectarClicked ();
        void btnServirClicked ();
        void btnConfigurarClicked ();
        void conexComandoRecebido (QStringList);
        void conexComandoExecutado (QStringList, QStringList);
        void conexErroComunicando (QString);
        void conexHostFound ();
        void conexConnected ();
        void timerTimeout ();
        void txtMensEnviarReturnPressed ();
};

#endif // MESSENGER_H
