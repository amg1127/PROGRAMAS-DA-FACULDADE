#include "mainWindow.h"

bool mainWindow::chooseDimensions (int& w, int& h) {
    dimensoesImagem di;
    di.setInitialResp (w, h);
    if (di.exec () == QDialog::Accepted) {
        di.getResp (w, h);
        return (true);
    } else {
        return (false);
    }
}

mainWindow::mainWindow (QWidget* parent, Qt::WindowFlags f) : QMainWindow (parent, f) {
    this->mwui.setupUi (this);
    this->caminho = QDir::homePath ();
    this->vlay = new QVBoxLayout (this->centralWidget ());
    this->vlay->setSpacing (0);
    this->scrlImage = new QScrollArea (this->centralWidget());
    this->vlay->addWidget (this->scrlImage);
    this->imgCont = new imageContainer (this->scrlImage);
    this->scrlImage->setWidget (this->imgCont);
    this->statusBar()->showMessage (QString::fromUtf8 ("Ready."));
    this->setWindowTitle (QString::fromUtf8 (TITULOPROG) + " [*]");
    this->showActiveColor ();
    QObject::connect (this->mwui.action_New, SIGNAL(triggered(bool)), this, SLOT(action_New_triggered(bool)));
    QObject::connect (this->mwui.action_Open, SIGNAL(triggered(bool)), this, SLOT(action_Open_triggered(bool)));
    QObject::connect (this->mwui.action_Save_As, SIGNAL(triggered(bool)), this, SLOT(action_Save_As_triggered(bool)));
    QObject::connect (this->mwui.action_Quit, SIGNAL(triggered(bool)), this, SLOT(action_Quit_triggered(bool)));
    QObject::connect (this->mwui.action_Pen, SIGNAL(triggered(bool)), this, SLOT(action_Pen_triggered(bool)));
    QObject::connect (this->mwui.action_About, SIGNAL(triggered(bool)), this, SLOT(action_About_triggered(bool)));
    QObject::connect (this->mwui.action_About_Qt, SIGNAL(triggered(bool)), this, SLOT(action_About_Qt_triggered(bool)));
    QObject::connect (this->mwui.action_Line, SIGNAL(triggered(bool)), this, SLOT(action_Line_triggered(bool)));
    QObject::connect (this->mwui.action_Rectangle, SIGNAL(triggered(bool)), this, SLOT(action_Rectangle_triggered(bool)));
    QObject::connect (this->mwui.action_Circle, SIGNAL(triggered(bool)), this, SLOT(action_Circle_triggered(bool)));
    QObject::connect (this->mwui.action_Spray, SIGNAL(triggered(bool)), this, SLOT(action_Spray_triggered(bool)));
    QObject::connect (this->mwui.action_Fill, SIGNAL(triggered(bool)), this, SLOT(action_Fill_triggered(bool)));
    QObject::connect (this->mwui.action_Resize_Image, SIGNAL(triggered(bool)), this, SLOT(action_Resize_Image_triggered(bool)));
    QObject::connect (this->mwui.action_Choose_Active_Color, SIGNAL(triggered(bool)), this, SLOT(action_Choose_Active_Color_triggered(bool)));
    QObject::connect (this->imgCont, SIGNAL(altered()), this, SLOT(imgCont_altered()));
    this->showMaximized ();
}

void mainWindow::closeEvent (QCloseEvent* ev) {
    if (this->isWindowModified ()) {
        if (this->wantContinue ()) {
            ev->accept ();
        } else {
            ev->ignore ();
        }
    } else {
        ev->accept ();
    }
}

bool mainWindow::wantContinue () {
    return (QMessageBox::question (
        (QWidget*)0,
        QString::fromUtf8 (TITULOPROG), QString::fromUtf8 ("There is unsaved data on image!\n\nDo you want to continue?"),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No) == QMessageBox::Yes);
}

void mainWindow::action_New_triggered (bool) {
    int w, h;
    w = this->imgCont->width ();
    h = this->imgCont->height ();
    if (this->chooseDimensions (w, h)) {
        if (this->isWindowModified ()) {
            if (! this->wantContinue ()) {
                return;
            }
        }
        this->imgCont->resize (w, h);
        this->imgCont->eraseAll ();
        this->setWindowModified (false);
        this->imgContSetTool (no_tool, false);
        this->statusBar()->showMessage (QString::fromUtf8 ("A new image was created."));
    }
}

void mainWindow::action_Open_triggered (bool) {
    QString resp;
    resp = QFileDialog::getOpenFileName (
        (QWidget*)0,
        QString::fromUtf8 ("Open image file..."),
        this->caminho, QString::fromUtf8 (FILTROS_IMAGEM));
    if (! resp.isNull ()) {
        if (this->isWindowModified ()) {
            if (! this->wantContinue ()) {
                return;
            }
        }
        QFileInfo f (resp);
        this->caminho = f.dir().absolutePath ();
        if (this->imgCont->readFromFile (resp)) {
            this->setWindowModified (false);
            this->imgContSetTool (no_tool, false);
            this->statusBar()->showMessage (QString::fromUtf8 ("Loaded file '") + resp + "'.");
        } else {
            QMessageBox::warning ((QWidget*)0, QString::fromUtf8(TITULOPROG), QString::fromUtf8("Unable to read the image!\n\nMaybe it is too big or the file can not be opened."));
        }
    }
}

void mainWindow::action_Save_As_triggered (bool) {
    QString resp;
    resp = QFileDialog::getSaveFileName (
        (QWidget*)0,
        QString::fromUtf8 ("Save image file..."),
        this->caminho, QString::fromUtf8 (FILTROS_IMAGEM));
    if (! resp.isNull ()) {
        QFileInfo f (resp);
        this->caminho = f.dir().absolutePath ();
        if (this->imgCont->saveToFile (resp)) {
            this->setWindowModified (false);
            this->statusBar()->showMessage (QString::fromUtf8 ("Saved file '") + resp + "'.");
        } else {
            QMessageBox::warning ((QWidget*)0, QString::fromUtf8(TITULOPROG), QString::fromUtf8("Unable to write the image!"));
        }
    }
}

void mainWindow::imgCont_altered () {
    this->setWindowModified (true);
}

void mainWindow::action_Quit_triggered (bool) {
    this->close ();
}

void mainWindow::imgContSetTool (imageTools tool, bool doStatus) {
    imageTools actual_tool;
    QString str_actual_tool;
    this->mwui.action_Pen->setChecked (false);
    this->mwui.action_Line->setChecked (false);
    this->mwui.action_Rectangle->setChecked (false);
    this->mwui.action_Circle->setChecked (false);
    this->mwui.action_Spray->setChecked (false);
    this->mwui.action_Fill->setChecked (false);
    actual_tool = this->imgCont->setTool (tool);
    switch (actual_tool) {
        case no_tool:
            str_actual_tool = "No"; break;
        case pen_tool:
            str_actual_tool = "Pen"; this->mwui.action_Pen->setChecked (true); break;
        case line_tool:
            str_actual_tool = "Line"; this->mwui.action_Line->setChecked (true); break;
        case rectangle_tool:
            str_actual_tool = "Rectangle"; this->mwui.action_Rectangle->setChecked (true); break;
        case circle_tool:
            str_actual_tool = "Circle"; this->mwui.action_Circle->setChecked (true); break;
        case spray_tool:
            str_actual_tool = "Spray"; this->mwui.action_Spray->setChecked (true); break;
        case fill_tool:
            str_actual_tool = "Fill"; this->mwui.action_Fill->setChecked (true); break;
    }
    if (doStatus && (! str_actual_tool.isNull())) {
        this->statusBar()->showMessage (str_actual_tool + " tool was activated.");
    }
}

void mainWindow::action_Pen_triggered (bool) {
    this->imgContSetTool (pen_tool);
}

void mainWindow::action_About_triggered (bool) {
    QMessageBox::information ((QWidget*)0, QString::fromUtf8(TITULOPROG), QString::fromUtf8(TITULOPROG) + QString::fromUtf8 ("\n\nDeveloped by: Anderson Medeiros Gomes <amg1127@gmail.com>\nVersion 1.0"));
}

void mainWindow::action_About_Qt_triggered (bool) {
    QMessageBox::aboutQt ((QWidget*)0);
}

void mainWindow::action_Line_triggered (bool) {
    this->imgContSetTool (line_tool);
}

void mainWindow::action_Rectangle_triggered (bool) {
    this->imgContSetTool (rectangle_tool);
}

void mainWindow::action_Circle_triggered (bool) {
    this->imgContSetTool (circle_tool);
}

void mainWindow::action_Spray_triggered (bool) {
    this->imgContSetTool (spray_tool);
}

void mainWindow::action_Fill_triggered (bool) {
    this->imgContSetTool (fill_tool);
}

void mainWindow::action_Resize_Image_triggered (bool) {
    int w, h;
    w = this->imgCont->width ();
    h = this->imgCont->height ();
    if (this->chooseDimensions (w, h)) {
        this->imgCont->resize (w, h);
        this->imgContSetTool (no_tool, false);
        this->imgCont_altered ();
    }
}

void mainWindow::action_Choose_Active_Color_triggered (bool) {
    QColor resp = QColorDialog::getColor (this->imgCont->corCaneta);
    if (resp.isValid ()) {
        this->imgCont->corCaneta = resp;
        this->showActiveColor ();
    }
}

void mainWindow::showActiveColor () {
    QPixmap pix (100, 100);
    pix.fill (this->imgCont->corCaneta);
    QIcon i (pix);
    i.addPixmap (pix, QIcon::Disabled, QIcon::Off);
    this->mwui.action_Show_Color->setIcon (i);
    this->mwui.action_Show_Color->setToolTip (QString("Active color is ") + this->imgCont->corCaneta.name().toUpper() + ".");
    this->mwui.action_Show_Color->setText (this->mwui.action_Show_Color->toolTip ());
}
