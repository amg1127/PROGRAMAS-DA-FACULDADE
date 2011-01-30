#include "main.h"

int main (int argc, char **argv) {
    QApplication prog (argc, argv);
    Messenger mess;
    prog.setMainWidget (&mess);
    if (mess.EuMesmo_Nome.isEmpty())
        return (1);
    return (prog.exec ());
}
