TEMPLATE = app
TARGET = agente_g
DEPENDPATH += .
INCLUDEPATH += .
CONFIG += qt release warn_on create_prl link_prl
OBJECTS_DIR = .libs
QMAKE_CFLAGS_WARN_ON += -Werror
QMAKE_CXXFLAGS_WARN_ON += -Werror
QMAKE_CFLAGS_RELEASE += -O3
QMAKE_CXXFLAGS_RELEASE += -O3

# Input
FORMS += frmOpcoesSelecao.ui frmMainWindow.ui frmPensando.ui
HEADERS += asserts.h eventFilter.h mainWindow.h tabuleiroLabel.h templates.h opcoesSelecao.h acao.h agenteAnimado.h
SOURCES += main.cpp tabuleiroLabel.cpp eventFilter.cpp mainWindow.cpp opcoesSelecao.cpp acao.cpp agenteAnimado.cpp
RESOURCES += agente_g.qrc
