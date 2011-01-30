#ifndef __DEFINES_H__
#define __DEFINES_H__

// Largura e altura maxima permitida
const int MAXDIMENSION = 65536;

// Titulo da janela
const char TITULOPROG[] = "Simple Image Editor 1.0 {by AMG1127}";

// Tipos de imagem suportados.
const char FILTROS_IMAGEM[] = "Arquivos de imagem (*.bmp *.gif *.jpeg *.jpg *.jpe *.png *.pbm *.pgm *.ppm *.xbm *.xpm)";

// Intervalo do timer do spray
const int INTERVALO_SPRAY = 50;

// Quantos pontos devem ser impressos em cada intervalo de timer
const int NUMPONTOS_SPRAY = 150;

// Raio do spray
const double RAIO_SPRAY = 20;

// Valor de faixa de tolerancia para preenchimento
const double TOLERANCIA_PREENCHIMENTO = 5;

#endif // __DEFINES_H__
