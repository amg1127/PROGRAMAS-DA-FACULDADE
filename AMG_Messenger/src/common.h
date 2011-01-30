#ifndef COMMON_H
#define COMMON_H

// Nao ignorar chamadas de Q_ASSERT
#define QT_CHECK_STATE
// Abortar o programa se a assertiva falhou
#define QT_FATAL_ASSERT

// Um monte de includes de biblioteca QT...
#include <qapplication.h>
#include <qcheckbox.h>
#include <qcolordialog.h>
#include <qdatetime.h>
#include <qfontdialog.h>
#include <qinputdialog.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlistbox.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qregexp.h>
#include <qserversocket.h>
#include <qsocket.h>
#include <qstatusbar.h>
#include <qstringlist.h>
#include <qtextedit.h>
#include <qtimer.h>
#include <qwidget.h>

// Define quantas linhas de conversa podem aparecer
#define MAX_ENTRADAS 4096
// Define quantos usuarios podem entrar na sala
#define MAX_USUARIOS 50
// Titulo da janela
#define TITULO "AMG Messenger 1.0 beta"
// Tempo das mensagens na barra de status (ms)
#define TEMPOPADRAO 8000
// Tempo para um cliente negociar a entrada em uma sala no servidor (ms)
#define TEMPONEGOCIACAO 5*60*1000
// Tempo que o servidor usa para checar se um cliente ainda esta conectado (ms)
#define OHVIVENTE 15*60*1000
// Porta padrao do servidor de bate-papo
#define PORTAPADRAO 6228

// Em algum lugar nos includes do Linux tem isso, mas eu nao sei porque nao tem no Windows
#ifndef __STRING
    #define __STRING(x) "x"
#endif

#define DEBUGA(msg) qDebug ("%s:%d - %s", __FILE__, __LINE__, msg);
#define NOVO_OBJETO(alvo,tipo) { qDebug ("%s:%d - %s = new %s;", __FILE__, __LINE__, __STRING (alvo), __STRING (tipo)); \
                                 do \
                                 alvo = new tipo; \
                                 while (alvo == NULL); }
#define APAGA_OBJETO(alvo) { qDebug ("%s:%d - delete %s;", __FILE__, __LINE__, __STRING (alvo)); \
                             delete (alvo); \
                             alvo = NULL; }
#define APAGA_OBJETO_SEM_NULL(alvo) { qDebug ("%s:%d - delete %s;", __FILE__, __LINE__,  __STRING (alvo)); \
                                      delete (alvo); }

#define max2(a,b) ((a>b)?(a):(b))
#define max3(a,b,c) ((a>max2(b,c))?a:max2(b,c))
#define max4(a,b,c,d) ((a>max3(b,c,d))?a:max3(b,c,d))

// Teste de sanidade...
#if MAX_ENTRADAS < 1
    #error MAX_ENTRADAS esta incorreto... Favor arrumar antes de compilar!
#elif MAX_ENTRADAS < 100
    #warning MAX_ENTRADAS esta OK... Mas o valor eh pequeno demais! Melhor aumentar antes de entregar para o professor!
#endif
#if MAX_USUARIOS < 1
    #error MAX_USUARIOS esta incorreto. Favor arrumar antes de compilar!
#endif
#endif // COMMON_H
