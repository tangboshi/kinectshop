#ifndef KINECTIO_H
#define KINECTIO_H

#include <QObject>

// #define gesture ...
// #define command ...

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

    private:
        int         detectedPid;
};

#endif // KINECTIO_H
