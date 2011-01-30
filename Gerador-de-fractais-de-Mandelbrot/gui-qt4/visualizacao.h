// visualizacao.h
#ifndef __VISUALIZACAO_H
#define __VISUALIZACAO_H

#include <QString>
#include <QWidget>
#include <QScrollArea>
#include <QTextEdit>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QList>
#include <QRectF>
#include <QTcpSocket>
#include <QProcess>
#include <QByteArray>
#include <QResizeEvent>
#include <QRect>
#include <QScrollBar>
#include <QTime>
#include <QMenu>
#include <QAction>
#include <QListWidget>
#include <QRegExp>
#include <QTemporaryFile>
#include "common.h"

const int larguraGBox = 220;
const int alturaGBox = 120;
const int precisao = 3;
const int oTimeoutDoTimer = 200;
const char tipoN = 'e';
const int precisao_mais = 20;
const QString tituloDaJanela = "AMG Mandelbrot Viewer";

enum tipoLog {
    log_auto,
    log_info,
    log_warn,
    log_err,
    log_recv,
    log_send
};

enum fazendo {
    nada,
    recarregando,
    aumentandozoom,
    reduzindozoom,
};

class visualizacao : public QWidget {
    Q_OBJECT;
    public:
        visualizacao ();
    private:
        QScrollArea *imagemArea;
        paintbrush *imagemVisualizacao;
        QTextEdit *janelaLogs;
        QGroupBox *infoImagem;
        QGroupBox *infoSelecao;
        QLabel *lblInfoImagem;
        QLabel *lblInfoSelecao;
        QPushButton *novoFractal;
        QPushButton *diminuirZoom;
        QPushButton *aumentarZoom;
        QPushButton *salvarFractal;
        QPushButton *btnConfiguracao;
        QMenu *menuBotao;
        QList <QRectF> zooms;
        configuracao *janeConfiguracao;
        QTimer *tim, *tim2;
        QRectF ultimaSelecao;
        void registraNoLog (QString, tipoLog = log_auto);
        void setWindowModified (bool);
        QTcpSocket *o_sock;
        QProcess *o_proc;
        void obtemNovaImagem ();
        void cancelaObterNovaImagem ();
        void reabilitaBotoes ();
        void jaCarregouNovaImagem ();
        void o_sock_disconnected ();
        fazendo estaEmProcessoDeNovaImagem;
        QByteArray stderr_buffer;
        QString mkTextoInformativo (QRect, QRectF);
        QRectF proporcao (QRect, QRect, QRectF);
        QTime t1;
        double t1_d;
        void resizeEvent (QResizeEvent *);
        int passoSocket;
        int passoPaleta;
        int quantasCores;
        int paramINT;
        double paramDOUB;
        QColor paramQColor;
        bool recebeDado (QString, int&);
        bool recebeDado (QString, double&);
        void enviaDado (QString, int);
        void enviaDado (QString, double);
        void enviaDado (QString);
        QString makeEntradaMenu (QRectF);
        QTemporaryFile *tmpFile;
    private slots:
        void novoFractal_clicked ();
        void diminuirZoom_clicked ();
        void aumentarZoom_clicked ();
        void salvarFractal_clicked ();
        void btnConfiguracao_clicked ();
        void imagemVisualizacao_selecionouArea (QRect, bool);
        void o_proc_error (QProcess::ProcessError);
        void o_proc_finished (int);
        void o_proc_readyReadStandardError ();
        void o_proc_started ();
        void o_sock_connected ();
        void o_sock_error (QAbstractSocket::SocketError);
        void o_sock_hostFound ();
        void o_sock_readyRead ();
        void tim_timeout ();
        void tim2_timeout ();
        void menuBotao_triggered (QAction *);
};

#endif // __VISUALIZACAO_H
// visualizacao.h
