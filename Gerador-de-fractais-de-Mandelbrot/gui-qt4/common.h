// common.h
#ifndef __COMMON_H
#define __COMMON_H

#include <stdlib.h>

#define NOVO_OBJETO(var,type) {     do \
                                        var = new type; \
                                    while (var == NULL); \
                              }
#define APAGA_OBJETO(var) { delete (var); \
                            var = NULL; \
                          }

#define direita(o) ((o)->x() + (o)->width())
#define embaixo(o) ((o)->y() + (o)->height())

class paintbrush;
class configuracao;
class editor_paleta;
class visualizacao;

int maior2 (int, int);
int maior3 (int, int, int);
int maior4 (int, int, int, int);
int maior5 (int, int, int, int, int);
int main (int, char**);

#include "configuracao.h"
#include "editor_paleta.h"
#include "visualizacao.h"
#include "paintbrush.h"

#endif // __COMMON_H
// common.h
