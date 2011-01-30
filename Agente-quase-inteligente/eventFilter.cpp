#include "asserts.h"
#include "eventFilter.h"

EventFilter* EventFilter::_self = NULL;

inline void EventFilter::ensureIExist () {
    if (EventFilter::_self == NULL) {
        EventFilter::_self = new EventFilter ();
    }
}

void EventFilter::IniciaMonitoramento (QObject* obj, QEvent::Type tipo, QObject* alvo, const char* slot) {
    EventFilter::ensureIExist ();
    EventFilter::_self->listaObjetosMonitor[tipo].append (obj);
    EventFilter::_self->listaObjetosAlvos[tipo].append (alvo);
    EventFilter::_self->listaObjetosSlots[tipo].append (slot);
    obj->installEventFilter (EventFilter::_self);
}

void EventFilter::EncerraMonitoramento (QObject* obj) {
    EventFilter::ensureIExist ();
    QList<QEvent::Type> lst = EventFilter::_self->listaObjetosMonitor.keys();
    int i, tam = lst.count ();
    for (i = 0; i < tam; i++) {
        EventFilter::EncerraMonitoramento (obj, lst[i]);
    }
}

void EventFilter::EncerraMonitoramento (QObject* obj, QEvent::Type tipo) {
    int pos, tam;
    EventFilter::ensureIExist ();
    tam = EventFilter::_self->listaObjetosMonitor[tipo].count();
    for (pos = 0; pos < tam; pos++) {
        if (EventFilter::_self->listaObjetosMonitor[tipo][pos] == obj) {
            EventFilter::_self->listaObjetosAlvos[tipo].removeAt (pos);
            EventFilter::_self->listaObjetosSlots[tipo].removeAt (pos);
            EventFilter::_self->listaObjetosMonitor[tipo].removeAt (pos);
            pos--;
            tam--;
        }
    }
}

bool EventFilter::eventFilter (QObject *obj, QEvent *event) {
    QEvent::Type evt = event->type ();
    int i, tam;
    tam = EventFilter::_self->listaObjetosMonitor[evt].count();
    for (i = 0; i < tam; i++) {
        if (EventFilter::_self->listaObjetosMonitor[evt][i] == obj) {
            if (! QMetaObject::invokeMethod (EventFilter::_self->listaObjetosAlvos[evt][i], EventFilter::_self->listaObjetosSlots[evt][i], Qt::DirectConnection, Q_ARG (QObject*, obj), Q_ARG (QEvent*, event))) {
                qDebug ("Aviso: metodo %s nao pode ser executado!", EventFilter::_self->listaObjetosSlots[evt][i]);
            }
        }
    }
    return QObject::eventFilter (obj, event);
}

