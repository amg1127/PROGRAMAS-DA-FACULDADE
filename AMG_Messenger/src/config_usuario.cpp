#include "config_usuario.h"

void config_usuario::btnEditarFonteClicked () {
    DEBUGA ("void config_usuario::btnEditarFonteClicked ();");
    bool ok;
    QFont font = QFontDialog::getFont(&ok, this->novoEstilo, this);
    if (ok) {
        this->novoEstilo = font;
        this->arruma_as_coisas();
    }
}

void config_usuario::btnEditarCorClicked () {
    DEBUGA ("void config_usuario::btnEditarCorClicked ();");
    QColor c = QColorDialog::getColor (this->novaCor, this);
    if (c.isValid()) {
        this->novaCor = c;
        this->arruma_as_coisas();
    }
}

void config_usuario::btnOkClicked () {
    DEBUGA ("void config_usuario::btnOkClicked ();");
    if (this->txtID->text().stripWhiteSpace().isEmpty()) {
        QMessageBox::warning (this, QString (TITULO), QString("Voce precisa digitar um nome valido antes de comecar!"), QMessageBox::Ok, QMessageBox::NoButton);
        this->txtID->setFocus();
    } else {
        *(this->velhoNome) = this->txtID->text();
        *(this->velhoEstilo) = this->novoEstilo;
        *(this->velhaCor) = this->novaCor;
        *(this->velhoUsaCTRLEnter) = this->UsaCTRLEnter->isChecked ();
        this->btnCancelar->setEnabled (true);
        this->close();
    }
}

void config_usuario::btnCancelarClicked () {
    DEBUGA ("void config_usuario::btnCancelarClicked ();");
    this->close();
}

config_usuario::config_usuario (QString &oldNome, QFont &oldEstilo, QColor &oldCor, bool &UsaCTRLE) {
    DEBUGA ("config_usuario::config_usuario (QString &, QFont &, QColor &, bool &);");
    this->setCaption (QString(TITULO) + " - Configurar parametros de usuario");
    NOVO_OBJETO (this->lblID, QLabel (this));
    NOVO_OBJETO (this->lblEstilo, QLabel (this));
    NOVO_OBJETO (this->lblAmostraEstilo, QLabel (this));
    NOVO_OBJETO (this->txtID, QLineEdit (this));
    NOVO_OBJETO (this->btnEditarFonte, QPushButton (this));
    NOVO_OBJETO (this->btnEditarCor, QPushButton (this));
    NOVO_OBJETO (this->btnOk, QPushButton (this));
    NOVO_OBJETO (this->btnCancelar, QPushButton (this));
    NOVO_OBJETO (this->UsaCTRLEnter, QCheckBox (this));
    this->velhoNome = &oldNome;
    this->velhoEstilo = &oldEstilo;
    this->velhaCor = &oldCor;
    this->novoEstilo = oldEstilo;
    this->novaCor = oldCor;
    this->velhoUsaCTRLEnter = &UsaCTRLE;
    this->lblID->setText ("Nome:");
    this->lblID->show ();
    this->txtID->setText (oldNome);
    this->txtID->show ();
    this->lblEstilo->setText ("Fonte:");
    this->lblEstilo->show ();
    this->btnEditarFonte->setText ("Alterar &fonte...");
    this->btnEditarFonte->show ();
    this->btnEditarCor->setText ("Alterar &cor...");
    this->btnEditarCor->show ();
    this->btnOk->setText ("&OK");
    this->btnOk->setAccel (QKeySequence (Qt::Key_Return));
    this->btnCancelar->setAccel (QKeySequence (Qt::Key_Escape));
    this->btnOk->show ();
    this->btnCancelar->setText ("&Cancelar");
    this->btnCancelar->show ();
    this->UsaCTRLEnter->setText ("&Usar tambem 'ENTER' para enviar mensagens, ao inves de apenas 'CTRL+ENTER'");
    this->UsaCTRLEnter->show ();
    this->UsaCTRLEnter->setChecked (UsaCTRLE);
    this->btnOk->setFocus ();
    this->txtID->setFocus ();
    QObject::connect (this->txtID, SIGNAL(returnPressed()), this, SLOT (btnOkClicked()));
    QObject::connect (this->btnEditarCor, SIGNAL (clicked ()), this, SLOT (btnEditarCorClicked()));
    QObject::connect (this->btnEditarFonte, SIGNAL (clicked ()), this, SLOT (btnEditarFonteClicked()));
    QObject::connect (this->btnOk, SIGNAL (clicked ()), this, SLOT (btnOkClicked()));
    QObject::connect (this->btnCancelar, SIGNAL (clicked ()), this, SLOT (btnCancelarClicked()));
    this->arruma_as_coisas ();
}

config_usuario::~config_usuario () {
    DEBUGA ("config_usuario::~config_usuario();");
    APAGA_OBJETO (this->lblID);
    APAGA_OBJETO (this->lblEstilo);
    APAGA_OBJETO (this->lblAmostraEstilo);
    APAGA_OBJETO (this->txtID);
    APAGA_OBJETO (this->btnEditarFonte);
    APAGA_OBJETO (this->btnEditarCor);
    APAGA_OBJETO (this->btnOk);
    APAGA_OBJETO (this->btnCancelar);
    APAGA_OBJETO (this->UsaCTRLEnter);
}

void config_usuario::arruma_as_coisas (void) {
    int maior, aux;
    DEBUGA ("void config_usuario::arruma_as_coisas (void);");
    this->lblAmostraEstilo->setFont (this->novoEstilo);
    this->lblAmostraEstilo->setText (this->novoEstilo.toString());
    this->lblAmostraEstilo->setPaletteForegroundColor (this->novaCor);
    this->lblID->adjustSize();
    this->lblEstilo->adjustSize();
    this->lblAmostraEstilo->adjustSize();
    QPoint poilblID (this->lblID->pos());
    QSize sizlblID (this->lblID->size());
    QPoint poitxtID (this->txtID->pos());
    QSize siztxtID (this->txtID->size());
    QPoint poilblEstilo (this->lblEstilo->pos());
    QSize sizlblEstilo (this->lblEstilo->size());
    QPoint poilblAmostraEstilo (this->lblAmostraEstilo->pos());
    QSize sizlblAmostraEstilo (this->lblAmostraEstilo->size());
    QPoint poibtnEditarFonte (this->btnEditarFonte->pos());
    QSize sizbtnEditarFonte (this->btnEditarFonte->size());
    QPoint poibtnEditarCor (this->btnEditarCor->pos());
    QSize sizbtnEditarCor (this->btnEditarCor->size());
    QPoint poibtnOk (this->btnOk->pos());
    QSize sizbtnOk (this->btnOk->size());
    QPoint poibtnCancelar (this->btnCancelar->pos());
    QSize sizbtnCancelar (this->btnCancelar->size());
    QSize dialogo (this->size());
    QPoint poiUsaCTRLEnter (this->UsaCTRLEnter->pos());
    QSize sizUsaCTRLEnter (this->UsaCTRLEnter->size());
    poilblID.setX(10);
    poitxtID.setX(sizlblID.width() + 20);
    poilblEstilo.setX(10);
    poilblAmostraEstilo.setX(sizlblEstilo.width() + 20);
    poibtnEditarFonte.setX(poilblAmostraEstilo.x() + sizlblAmostraEstilo.width() + 10);
    poibtnEditarCor.setX(poibtnEditarFonte.x() + sizbtnEditarFonte.width() + 10);
    poiUsaCTRLEnter.setX (10);
    dialogo.setWidth(poibtnEditarCor.x() + sizbtnEditarCor.width() + 10);
    sizUsaCTRLEnter.setWidth (dialogo.width ());
    this->UsaCTRLEnter->move (poiUsaCTRLEnter);
    this->UsaCTRLEnter->resize (sizUsaCTRLEnter);
    sizUsaCTRLEnter = this->UsaCTRLEnter->size ();
    siztxtID.setWidth (dialogo.width() - 30 - sizlblID.width());
    poibtnOk.setX (dialogo.width() / 4 - sizbtnOk.width() / 2);
    poibtnCancelar.setX (3 * dialogo.width() / 4 - sizbtnCancelar.width() / 2);
    maior = max2 (sizlblID.height(), siztxtID.height());
    aux = 10;
    poilblID.setY (aux + maior / 2 - sizlblID.height() / 2);
    poitxtID.setY (aux + maior / 2 - siztxtID.height() / 2);
    aux += maior + 10;
    maior = max4 (sizlblEstilo.height(), sizlblAmostraEstilo.height(), sizbtnEditarFonte.height(), sizbtnEditarCor.height());
    poilblEstilo.setY (aux + maior / 2 - sizlblEstilo.height() / 2);
    poilblAmostraEstilo.setY (aux + maior / 2 - sizlblAmostraEstilo.height() / 2);
    poibtnEditarFonte.setY (aux + maior / 2 - sizbtnEditarFonte.height() / 2);
    poibtnEditarCor.setY (aux + maior / 2 - sizbtnEditarCor.height() / 2);
    aux += maior + 10;
    poiUsaCTRLEnter.setY (aux);
    aux += sizUsaCTRLEnter.height() + 10;
    maior = max2 (sizbtnOk.height(), sizbtnCancelar.height());
    poibtnOk.setY (aux + maior / 2 - sizbtnOk.height() / 2);
    poibtnCancelar.setY (aux + maior / 2 - sizbtnCancelar.height() / 2);
    aux += maior + 10;
    dialogo.setHeight (aux);
    this->lblID->move (poilblID);
    this->lblID->resize (sizlblID);
    this->txtID->move (poitxtID);
    this->txtID->resize (siztxtID);
    this->lblEstilo->move (poilblEstilo);
    this->lblEstilo->resize (sizlblEstilo);
    this->lblAmostraEstilo->move (poilblAmostraEstilo);
    this->lblAmostraEstilo->resize (sizlblAmostraEstilo);
    this->btnEditarFonte->move (poibtnEditarFonte);
    this->btnEditarFonte->resize (sizbtnEditarFonte);
    this->btnEditarCor->move (poibtnEditarCor);
    this->btnEditarCor->resize (sizbtnEditarCor);
    this->btnOk->move (poibtnOk);
    this->btnOk->resize (sizbtnOk);
    this->btnCancelar->move (poibtnCancelar);
    this->btnCancelar->resize (sizbtnCancelar);
    this->UsaCTRLEnter->move (poiUsaCTRLEnter);
    this->resize (dialogo);
    this->setMaximumSize (dialogo);
    this->setMinimumSize (dialogo);
}
