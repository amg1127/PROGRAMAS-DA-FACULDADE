#include <QCloseEvent>
#include <QColorDialog>
#include <QDialog>
#include <QFileDialog>
#include <QFileInfo>
#include <QIcon>
#include <QIconEngine>
#include <QImage>
#include <QMainWindow>
#include <QMessageBox>
#include <QScrollArea>
#include <QVBoxLayout>
#include "defines.h"
#include "ui_frmMainWindow.h"
#include "dimensoesImagem.h"
#include "imageContainer.h"

class mainWindow : public QMainWindow {
    Q_OBJECT;
    private:
        Ui_frmMainWindow mwui;
        QScrollArea* scrlImage;
        QVBoxLayout* vlay;
        imageContainer* imgCont;
        bool chooseDimensions (int&, int&);
        void closeEvent (QCloseEvent*);
        bool wantContinue ();
        QString caminho;
        void imgContSetTool (imageTools, bool = true);
        void showActiveColor ();
    public:
        mainWindow (QWidget* = 0, Qt::WindowFlags = 0);
    public slots:
        void action_New_triggered (bool);
        void action_Open_triggered (bool);
        void action_Save_As_triggered (bool);
        void action_Quit_triggered (bool);
        void action_Pen_triggered (bool);
        void action_About_triggered (bool);
        void action_About_Qt_triggered (bool);
        void action_Line_triggered (bool);
        void action_Rectangle_triggered (bool);
        void action_Circle_triggered (bool);
        void action_Spray_triggered (bool);
        void action_Fill_triggered (bool);
        void action_Resize_Image_triggered (bool);
        void action_Choose_Active_Color_triggered (bool);
        void imgCont_altered ();
};
