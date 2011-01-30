// configuracao.h
#ifndef __CONFIGURACAO_H
#define __CONFIGURACAO_H

#include <QDialog>
#include <QGroupBox>
#include <QRadioButton>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QCheckBox>
#include <QListWidget>
#include <QListWidgetItem>
#include <QResizeEvent>
#include <QPixmap>
#include <QPainter>
#include "common.h"

const int largura_LineEdit = 250;
const int largura_SpinBox = 100;

class configuracao : public QDialog {
    Q_OBJECT;
    private:
        QPixmap *visualizar_paleta;
    public:
        static bool cfgModoObtencaoConexao;
        static QString cfgCaminhoExecutavel;
        static QString cfgIpDoServidor;
        static int cfgPortaTCP;
        static int cfgEscolheLargura;
        static int cfgEscolheAltura;
        static bool cfgEscolheDimAutomatico;
        static int cfgNoIteracoes;
        static bool cfgAtualizacaoAutomatica;
        QGroupBox *modoObtencao;
        QRadioButton *modoObtencaoExecucao;
        QLabel *caminhoExecutavel;
        QLineEdit *txtCaminhoExecutavel;
        QPushButton *procurarExecutavel;
        QRadioButton *modoObtencaoConexao;
        QLabel *ipDoServidor;
        QLineEdit *txtIpDoServidor;
        QLabel *portaTCP;
        QSpinBox *txtPortaTCP;
        QGroupBox *outrasconfigs;
        QLabel *escolheLargura;
        QSpinBox *txtEscolheLargura;
        QLabel *escolheAltura;
        QSpinBox *txtEscolheAltura;
        QCheckBox *escolheDimAutomatico;
        QLabel *noIteracoes;
        QSpinBox *txtNoIteracoes;
        QLabel *paletaDeCores;
        QLabel *mostraPaletaDeCores;
        QPushButton *modificaPaletaDeCores;
        QCheckBox *atualizacaoAutomatica;
        QListWidget *paletaDeCoresReal;
        QPushButton *btnOk;
        QPushButton *btnCancelar;
        editor_paleta *edict;
        configuracao (visualizacao *);
        ~configuracao ();
        void loadConfig ();
        void mostra_paleta ();
        bool mexeuNaPaleta;
    private slots:
        void modificaPaletaDeCores_clicked ();
        void modoObtencaoExecucao_toggled (bool);
        void modoObtencaoConexao_toggled (bool);
        void procurarExecutavel_clicked ();
        void escolheDimAutomatico_toggled (bool);
        void btnOk_clicked ();
        void btnCancelar_clicked ();
};

#endif // __CONFIGURACAO_H
// configuracao.h
