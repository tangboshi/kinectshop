#ifndef KINECTIO_H
#define KINECTIO_H

#include <QObject>

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
    private:
};

#endif // KINECTIO_H
