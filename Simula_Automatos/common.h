#ifndef __COMMON_H
#define __COMMON_H

// Nao ignorar chamadas de Q_ASSERT
#define QT_CHECK_STATE
// Abortar o programa se a assertiva falhou
#define QT_FATAL_ASSERT

#include <cmath>
#include <QApplication>
#include <QButtonGroup>
#include <QChar>
#include <QDialog>
#include <QFile>
#include <QFileDialog>
#include <QFont>
#include <QFontMetrics>
#include <QKeyEvent>
#include <QInputDialog>
#include <QLabel>
#include <QLinkedList>
#include <QMessageBox>
#include <QMouseEvent>
#include <QMutex>
#include <QPainter>
#include <QPoint>
#include <QProgressBar>
#include <QPushButton>
#include <QRadioButton>
#include <QRect>
#include <QScrollArea>
#include <QSemaphore>
#include <QSettings>
#include <QSize>
#include <QString>
#include <QTextEdit>
#include <QThread>
#include <QVector>
#include <QWidget>

const char TITULO[] = "Simulador de Automatos Finitos 1.0"; // Definir o nome do programa
const int COMPRIMENTO_SETA = 16; // Comprimento das setas
const int LARGURA_SETA = 8; // Largura das setas
const int DISTANCIA_ESTADO = 8; // Distancia entre as setas e os circulos
const int ESTADO_DIAMETRO = 60; // Diametro normal do circulo que contorna um estado
const int ESTADO_DIAMETRO_OFFSET_FINAL = 4; // Parametro para uso ao desenhar circulos para estados finais.
const int TAM_FONTE = 16; // Tamanho da fonte utilizada nos desenhos [em pixels]
const int LARG_MEIO_CIRC = 20; // Largura do meio-circulo que eh desenhado em arcos que possuem o mesmo no como origem e extremidade
const int ALT_MEIO_CIRC = 50; // Altura do meio-circulo que eh desenhado em arcos que possuem o mesmo no como origem e extremidade
const int ESPACO = 10; // Espaco minimo entre 1 controle e outro qualquer (mas nao tem nada a ver com os estados do automato)
const QColor COR_FUNDO (255, 255, 255); // Cor de fundo
const QColor COR_NORMAL (0, 0, 0); // Cor para utilizar em estados
const QColor COR_ESTADO_INICIAL (255, 0, 0); // Cor para utilizar no estado inicial
const QColor COR_ESTADO_FINAL (0, 0, 255); // Cor para utilizar nos estados finais
const QColor COR_ESTADO_INICIAL_FINAL (0, 0, 255); // Cor para utilizar se o estado eh inicial e final ao mesmo tempo
const QColor COR_SELECIONADO (0, 170, 0); // Cor para utilizar em um estado que foi selecionado com o mouse

#endif // __COMMON_H
