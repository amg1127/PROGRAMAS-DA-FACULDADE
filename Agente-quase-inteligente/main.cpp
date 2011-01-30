#include "asserts.h"
#include <QtGui>
#include "mainWindow.h"
#include <cstdlib>

int main (int argc, char* argv[]) {
    srand (time (NULL));
    (void) rand ();

    QApplication prog (argc, argv);

    QFileInfo executavel(QCoreApplication::applicationFilePath ());
    QString base = executavel.completeBaseName();
    if (base.isEmpty()) {
        base = executavel.fileName();
    }
    QCoreApplication::addLibraryPath (executavel.dir().path());

    MainWindow mainWindow;

    return (prog.exec ());
}
