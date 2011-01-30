#include <QDialog>
#include "defines.h"
#include "ui_frmDimensoesImagem.h"

class dimensoesImagem : public QDialog {
    Q_OBJECT;
    private:
        Ui_frmDimensoesImagem diui;
    public:
        inline void getResp (int&, int&);
        inline void setInitialResp (int, int);
        dimensoesImagem (QWidget* = 0, Qt::WindowFlags = 0);
};

inline void dimensoesImagem::getResp (int& w, int& h) {
    w = this->diui.spnbxLarg->value ();
    h = this->diui.spnbxAlt->value ();
}

inline void dimensoesImagem::setInitialResp (int w, int h) {
    this->diui.spnbxLarg->setValue (w);
    this->diui.spnbxAlt->setValue (h);
}
