#ifndef __MAINWINDOW_H
#define __MAINWINDOW_H

#include <QtGui>
#include "templates.h"
#include "eventFilter.h"
#include "tabuleiroLabel.h"
#include "opcoesSelecao.h"
#include "ui_frmMainWindow.h"
#include "ui_frmPensando.h"
#include "agenteAnimado.h"
#include <cstdlib>

class MainWindow : public QWidget {
    Q_OBJECT;
    private:
        TabuleiroLabel*** tabuleiroLabels;
        int dx, dy;
        QGridLayout* grlay;
        QWidget* spcTopLeft;
        QWidget* spcBottomRight;
        QWidget* frmTabuleiro;
        QLabel* lblQuadroDeSelecao;
        QScrollArea* scrlTrabuleiro;
        QDialog* dial;
        bool segurouShift, segurouCTRL;
        QRect movimento;
        Ui_frmMainWindow mw;
        Ui_frmPensando brain;
        int ultSelecX, ultSelecY;
        bool wantPopup;
        bool mostrandoPoliticaOtima;
        bool querContinuar;
        AgenteAnimado* boneco;
        int xAgente, yAgente;
        TabuleiroLabel* navAgente;
        void destroiTabuleiro ();
        void desselecionaTudo ();
        TabuleiroLabel* quadroSobOPonteiro (QPoint);
        TabuleiroLabel* quadroSobOPonteiro (int, int);
        bool gravaEstadoNoArquivo (QString);
        bool LeEstadoDoArquivo (QString);
        void parseiaProbabs (QString, double&, double&, double&, double&);
        void parseiaPrioridades (QString, int&, int&, int&, int&);
        void determinaIntervRecompensa ();
        bool bateEVolta (int, int, int, int&, int&);
        void calculaPoliticaOtima ();
        void menuAcaoMarcacaoFinal (bool);
        void habilitaOuDesabilita ();
        int calcProxEstado (Acao*);
        void avancaAgenteAnimado ();
    public:
        MainWindow (QWidget* = 0, Qt::WindowFlags = 0);
        ~MainWindow ();
    public slots:
        void iniciaAnimacao ();
        void criaTabuleiro ();
        void janelas_close (QObject*, QEvent*);
        void tabuleiroLabels_dragEnter (QObject*, QEvent*);
        void tabuleiroLabels_dragLeave (QObject*, QEvent*);
        void tabuleiroLabels_drop (QObject*, QEvent*);
        void lblsAgenteMuro_mousePress (QObject*, QEvent*);
        void frmTabuleiro_mousePress (QObject*, QEvent*);
        void frmTabuleiro_mouseMove (QObject*, QEvent*);
        void frmTabuleiro_mouseRelease (QObject*, QEvent*);
        void btnAlterarDimsTab_clicked ();
        void btnEditarPropriedades_clicked ();
        void menuAcaoPoeAgente_triggered ();
        void menuAcaoPoeMuro_triggered ();
        void menuAcaoRemoverElemento_triggered ();
        void menuAcaoMarcaNaoFinal_triggered ();
        void menuAcaoMarcaFinal_triggered ();
        void btnPoliticaOtima_clicked ();
        void btnCarregar_clicked ();
        void btnSalvar_clicked ();
        void brainTimer_timeout ();
        void btnSelectTudo_clicked ();
        void boneco_endedAnimation ();
};

#endif // __MAINWINDOW_H
