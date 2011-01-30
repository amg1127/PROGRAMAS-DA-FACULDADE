#ifndef CONFIG_USUARIO_H
#define CONFIG_USUARIO_H

class config_usuario;

#include "common.h"

class config_usuario : public QDialog {
    Q_OBJECT;
    private:
        void arruma_as_coisas (void);
        QString *velhoNome;
        QFont novoEstilo, *velhoEstilo;
        QColor novaCor, *velhaCor;
        bool *velhoUsaCTRLEnter;
    public:
        QCheckBox *UsaCTRLEnter;
        QLabel *lblID, *lblEstilo, *lblAmostraEstilo;
        QLineEdit *txtID;
        QPushButton *btnEditarFonte, *btnEditarCor, *btnOk, *btnCancelar;
        config_usuario (QString&, QFont&, QColor&, bool &);
        ~config_usuario ();
    public slots:
        void btnEditarFonteClicked ();
        void btnEditarCorClicked ();
        void btnOkClicked ();
        void btnCancelarClicked ();
};

#endif // CONFIG_USUARIO_H
