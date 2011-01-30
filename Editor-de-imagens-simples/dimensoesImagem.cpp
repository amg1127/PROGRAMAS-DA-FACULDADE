#include "dimensoesImagem.h"

dimensoesImagem::dimensoesImagem (QWidget* parent, Qt::WindowFlags f) : QDialog (parent, f) {
    this->diui.setupUi (this);
    this->diui.spnbxLarg->setFocus ();
    this->diui.spnbxLarg->setMaximum (MAXDIMENSION);
    this->diui.spnbxAlt->setMaximum (MAXDIMENSION);
    QObject::connect (this->diui.buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    QObject::connect (this->diui.buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    this->setWindowTitle (QString::fromUtf8 (TITULOPROG));
}
