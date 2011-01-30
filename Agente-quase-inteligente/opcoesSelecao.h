#ifndef __OPCOESSELECAO_H
#define __OPCOESSELECAO_H

#include <QtGui>
#include "ui_frmOpcoesSelecao.h"
#include "acao.h"
#include "tabuleiroLabel.h"

class OpcoesSelecao_agrupamentos : public QObject {
    Q_OBJECT;
    public:
        Acao objetoAcao;
        QDoubleSpinBox* spinBoxes[4];
        void controlaSpinBoxes (QDoubleSpinBox*, QDoubleSpinBox*, QDoubleSpinBox*, QDoubleSpinBox*);
        void spinBoxes_valueChanged (int, double);
        bool conecta;
    public slots:
        void spinBoxNorte_valueChanged (double);
        void spinBoxOeste_valueChanged (double);
        void spinBoxLeste_valueChanged (double);
        void spinBoxSul_valueChanged (double);
};

class OpcoesSelecao : public QDialog {
    Q_OBJECT;
    private:
        int dx, dy;
        Ui_frmOpcoesSelecao opswin;
        TabuleiroLabel*** tabuleiroLabels;
        OpcoesSelecao_agrupamentos grpNorte;
        OpcoesSelecao_agrupamentos grpOeste;
        OpcoesSelecao_agrupamentos grpLeste;
        OpcoesSelecao_agrupamentos grpSul;
    public:
        OpcoesSelecao (int, int, TabuleiroLabel***, int, int, QWidget* = 0, Qt::WindowFlags = 0);
    public slots:
        void btnOk_clicked ();
};

#endif // __OPCOESSELECAO_H
