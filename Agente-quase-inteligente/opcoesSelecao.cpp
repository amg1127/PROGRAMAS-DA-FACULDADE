#include "asserts.h"
#include "opcoesSelecao.h"

void OpcoesSelecao_agrupamentos::controlaSpinBoxes (QDoubleSpinBox* p0, QDoubleSpinBox* p1, QDoubleSpinBox* p2, QDoubleSpinBox* p3) {
    int i;
    this->spinBoxes[0] = p0;
    this->spinBoxes[1] = p1;
    this->spinBoxes[2] = p2;
    this->spinBoxes[3] = p3;
    this->conecta = true;
    QObject::connect (p0, SIGNAL(valueChanged(double)), this, SLOT(spinBoxNorte_valueChanged(double)), Qt::DirectConnection);
    QObject::connect (p1, SIGNAL(valueChanged(double)), this, SLOT(spinBoxOeste_valueChanged(double)), Qt::DirectConnection);
    QObject::connect (p2, SIGNAL(valueChanged(double)), this, SLOT(spinBoxLeste_valueChanged(double)), Qt::DirectConnection);
    QObject::connect (p3, SIGNAL(valueChanged(double)), this, SLOT(spinBoxSul_valueChanged(double)), Qt::DirectConnection);
    for (i = 0; i < 4; i++) {
        this->spinBoxes[i]->setValue (this->objetoAcao.probab (i));
    }
}

void OpcoesSelecao_agrupamentos::spinBoxNorte_valueChanged (double v) {
    this->spinBoxes_valueChanged (0, v);
}

void OpcoesSelecao_agrupamentos::spinBoxOeste_valueChanged (double v) {
    this->spinBoxes_valueChanged (1, v);
}

void OpcoesSelecao_agrupamentos::spinBoxLeste_valueChanged (double v) {
    this->spinBoxes_valueChanged (2, v);
}

void OpcoesSelecao_agrupamentos::spinBoxSul_valueChanged (double v) {
    this->spinBoxes_valueChanged (3, v);
}

void OpcoesSelecao_agrupamentos::spinBoxes_valueChanged (int pos, double v) {
    int i;
    if (this->conecta) {
        this->conecta = false;
        this->objetoAcao.setProbab (pos, v);
        for (i = 0; i < 4; i++) {
            if (i != pos) {
                this->spinBoxes[i]->setValue (this->objetoAcao.probab(i));
            }
        }
        this->conecta = true;
    }
}

OpcoesSelecao::OpcoesSelecao (int x, int y, TabuleiroLabel*** t, int dx, int dy, QWidget* parent, Qt::WindowFlags f) : QDialog (parent, f) {
    TabuleiroLabel* tsel;
    this->tabuleiroLabels = t;
    this->dx = dx;
    this->dy = dy;
    this->opswin.setupUi (this);
    QObject::connect (this->opswin.btnOk, SIGNAL(clicked()), this, SLOT(btnOk_clicked()));
    tsel = this->tabuleiroLabels[x][y];
    this->opswin.dspbRecompensa->setValue (tsel->recompensa());
    this->grpNorte.objetoAcao = tsel->prbNorte;
    this->grpOeste.objetoAcao = tsel->prbOeste;
    this->grpLeste.objetoAcao = tsel->prbLeste;
    this->grpSul.objetoAcao = tsel->prbSul;
    this->grpNorte.controlaSpinBoxes (this->opswin.dspbNorteNorte, this->opswin.dspbNorteOeste, this->opswin.dspbNorteLeste, this->opswin.dspbNorteSul);
    this->grpOeste.controlaSpinBoxes (this->opswin.dspbOesteNorte, this->opswin.dspbOesteOeste, this->opswin.dspbOesteLeste, this->opswin.dspbOesteSul);
    this->grpLeste.controlaSpinBoxes (this->opswin.dspbLesteNorte, this->opswin.dspbLesteOeste, this->opswin.dspbLesteLeste, this->opswin.dspbLesteSul);
    this->grpSul.controlaSpinBoxes (this->opswin.dspbSulNorte, this->opswin.dspbSulOeste, this->opswin.dspbSulLeste, this->opswin.dspbSulSul);
}

void OpcoesSelecao::btnOk_clicked () {
    int x, y;
    TabuleiroLabel* t;
    for (x = 0; x < this->dx; x++) {
        for (y = 0; y < this->dy; y++) {
            t = this->tabuleiroLabels[x][y];
            if (t->estaSelecionado ()) {
                t->prbNorte = this->grpNorte.objetoAcao;
                t->prbOeste = this->grpOeste.objetoAcao;
                t->prbLeste = this->grpLeste.objetoAcao;
                t->prbSul = this->grpSul.objetoAcao;
                t->setRecompensa (this->opswin.dspbRecompensa->value ());
            }
        }
    }
    this->close ();
}
