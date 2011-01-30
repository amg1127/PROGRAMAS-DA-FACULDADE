#include <QApplication>
#include <QCoreApplication>
#include <QFileInfo>
#include <QString>
#include <cstdlib>
#include <ctime>
#include "mainWindow.h"

int main (int argc, char** argv) {
    srand (time (NULL));
    QApplication prog (argc, argv);
    QFileInfo executavel(QCoreApplication::applicationFilePath ());
    QString base = executavel.completeBaseName();
    if (base.isEmpty()) {
        base = executavel.fileName();
    }
    QCoreApplication::addLibraryPath (executavel.dir().path());
    mainWindow mw;
    return (prog.exec ());
}
