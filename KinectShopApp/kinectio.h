#ifndef KINECTIO_H
#define KINECTIO_H

#include <QObject>
#include <QString>

#include "automaton.h"

// #define gesture ...
// #define command ...

class automaton;

/**
 * @class kinectio Class
 * @brief von QObject erbende Klasse
 * Diese Klasse stellt Funktionen zur Verfuegung um die Erkennung eines Produkts (durch die Kinect) zu verwalten.
 * Sie besteht aus Slots, Signalen und privaten Membern.
 *
 * Anbindung an eine tatsächliche Kinect nicht vorhanden. Diese Klasse simuliert diese nur.
 */
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
        int         detectedPid;	///< erkannte Produkt ID
};

#endif // KINECTIO_H
