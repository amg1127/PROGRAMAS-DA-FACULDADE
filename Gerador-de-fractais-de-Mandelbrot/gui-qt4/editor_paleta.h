// editor_paleta.h
#ifndef __EDITOR_PALETA_H
#define __EDITOR_PALETA_H

#include <QDialog>
#include <QColorDialog>
#include <QPushButton>
#include <QListWidget>
#include <QButtonGroup>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QFile>
#include "common.h"

const QString defotexto("--<##>--");
const int maxint = 0x7FFFFFFF;

class editor_paleta : public QDialog {
    Q_OBJECT;
    private:
        configuracao *p;
        bool lop;
        int oCaraSelecionado (QListWidgetItem *&);
    public:
        QPushButton *btnAdicionar;
        QPushButton *btnEditar;
        QPushButton *btnRemover;
        QPushButton *btnLimpar;
        QPushButton *btnParaBaixo;
        QPushButton *btnParaCima;
        QPushButton *btnEscolhaAleatoria;
        QPushButton *btnAdicionarGradiente;
        QPushButton *btnOk;
        QPushButton *btnCancelar;
        QPushButton *btnAbreArquivo;
        QPushButton *btnSalvaArquivo;
        QListWidget *mostrador;
        QButtonGroup *grupo;
        void loadConfig ();
        editor_paleta (configuracao *);
        ~editor_paleta ();
    private slots:
        void mostrador_aconteceuAlgumaCoisa (QListWidgetItem *);
        void btnAdicionar_clicked ();
        void btnEditar_clicked ();
        void btnRemover_clicked ();
        void btnLimpar_clicked ();
        void btnParaBaixo_clicked ();
        void btnParaCima_clicked ();
        void btnEscolhaAleatoria_clicked ();
        void btnAdicionarGradiente_clicked ();
        void btnOk_clicked ();
        void btnCancelar_clicked ();
        void btnAbreArquivo_clicked ();
        void btnSalvaArquivo_clicked ();
};

#endif // __EDITOR_PALETA_H
// editor_paleta.h
