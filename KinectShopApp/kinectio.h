#ifndef KINECTIO_H
#define KINECTIO_H

#include <QObject>
#include <QString>

#include "automaton.h"

// #define gesture ...
// #define command ...

class automaton;

class kinectio :public QObject
{
    Q_OBJECT

    public:
        kinectio();

    signals:
        void        kinectInitialized();
        void        kinectProductDetected(int pid);
        //void      kinectSpeechDetected(command cmd);
        //void      kinectGestureDetected(gesture gst);

    public slots:
        void        setDetectedPid(int pid);
        int         getDetectedPid();
        QString     updateDetectedPidViewer();

    private:
        int         detectedPid;
};

#endif // KINECTIO_H
