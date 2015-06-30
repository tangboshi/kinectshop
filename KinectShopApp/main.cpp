#include <QApplication>

#include <QGraphicsWebView>
#include <QWebFrame>
#include <QtWebKit>

#include <QFuture>
#include <QtConcurrent/QtConcurrentRun>
#include <QCoreApplication>

#include "html5applicationviewer.h"

#include "sqlfunctions.h"
#include "automaton.h"
#include "kinectio.h"

void delay(int n){
    QTime dieTime= QTime::currentTime().addSecs(n);
    while( QTime::currentTime() < dieTime )
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void kinectInputSimulation(automaton* A, kinectio* K){
    int i = 0;
    while(1){
        i++;
        int simulatedDetectedPid = i%9+1;
        if(i>1000) i=0;
        delay(10);
        K->setDetectedPid(simulatedDetectedPid);
        emit K->kinectProductDetected(simulatedDetectedPid);
        qDebug() << K->getDetectedPid();
    }
}

int main(int argc, char *argv[]){
    QApplication app(argc, argv);

    sqlfunctions obj;
    kinectio kinect;

    // Automat für Sprachsteuerung //////////////////////////////////////////////////////////////////
    automaton automA;
    automA.setObj(&obj);
    automA.setKinect(&kinect);

    QFuture<void> counterThread = QtConcurrent::run(kinectInputSimulation, &automA, &kinect);

    /* Transitionen
    // Zustände
    #define START 0
    #define SELECTED 1
    #define AMOUNTSET 2
    #define END 3

    state start, selected, amountset, end;

    // Eingaben
    // Mengen
    #define ONE 1
    #define TWO 2
    //  andere Befehle
    #define SELECT 100
    #define PUTINCART 101
    #define BACK 102
    #define CANCEL 103
    #define MORE 104

    // Transitionen des Automaten
    // Trasition im "Normalfall"

    transition *startSelected           = new transition(&start, &selected, SELECT, &automA);
    transition *selectedAmountsetOne    = new transition(&selected, &amountset, ONE, &automA);
    transition *selectedAmountsetTwo    = new transition(&selected, &amountset, TWO, &automA);
    transition *amountsetEnd            = new transition(&amountset, &end, PUTINCART, &automA);

    // "Zurück-, Mehr- und Abbrechen-Transitionen"
    transition *selectedStartBack   = new transition(&selected, &start, BACK, &automA);
    transition *amountsetSelected   = new transition(&amountset, &selected, BACK, &automA);
    transition *endStart            = new transition(&end, &start, MORE, &automA);
    transition *selectedStartCancel = new transition(&selected, &start, CANCEL, &automA);
    transition *amountsetStart      = new transition(&amountset, &start, CANCEL, &automA);

    automA.addTransition(startSelected);
    automA.addTransition(selectedAmountsetOne);
    automA.addTransition(selectedAmountsetTwo);
    automA.addTransition(amountsetEnd);
    automA.addTransition(selectedStartBack);
    automA.addTransition(amountsetSelected);
    automA.addTransition(endStart);
    automA.addTransition(selectedStartCancel);
    automA.addTransition(amountsetStart);
    */

    // ---------------------------- //////////////////////////////////////////////////////////////////

    /*QObject::connect(&obj,SIGNAL(cartChanged()),&obj,SLOT(showCart())); // showCart() muss im js*/

    // Den Shop initialisieren //////////////////////////////////////////////////////////////////

    Html5ApplicationViewer viewer;
    viewer.setOrientation(Html5ApplicationViewer::ScreenOrientationAuto);
    viewer.showExpanded();
    viewer.loadFile(QLatin1String("src/index.html"));
    viewer.setFixedSize(1200, 900);

    QWebFrame *frame = viewer.webView()->page()->mainFrame();
    QString sqlObjName = "mySqlObj";
    QString automatonObjName = "automaton";
    QString kinectObjName = "kinect";
    frame->addToJavaScriptWindowObject(sqlObjName, &obj);
    frame->addToJavaScriptWindowObject(automatonObjName, &automA);
    frame->addToJavaScriptWindowObject(kinectObjName, &kinect);

    return app.exec();
}
