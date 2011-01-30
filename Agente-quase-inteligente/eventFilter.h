#ifndef __EVENTFILTER_H
#define __EVENTFILTER_H

#include <QtCore>

class EventFilter : public QObject {
    Q_OBJECT;
    private:
        QHash<QEvent::Type, QList<QObject*> > listaObjetosMonitor;
        QHash<QEvent::Type, QList<QObject*> > listaObjetosAlvos;
        QHash<QEvent::Type, QList<const char*> > listaObjetosSlots;
        static EventFilter* _self;
        static inline void ensureIExist ();
    protected:
        bool eventFilter (QObject*, QEvent*);
    public:
        static void IniciaMonitoramento (QObject*, QEvent::Type, QObject*, const char*);
        static void EncerraMonitoramento (QObject*, QEvent::Type);
        static void EncerraMonitoramento (QObject*);
};

#endif // __EVENTFILTER_H
