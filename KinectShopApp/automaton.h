#ifndef AUTOMATON_H
#define AUTOMATON_H

#include <QObject>
#include <QDebug>
#include <QMessageBox>

#include "state.h"
#include "transition.h"
#include "sqlfunctions.h"
#include "kinectio.h"

class transition;
class kinectio;

typedef vector<transition*>::iterator iterA;

/**
 * @class automaton Class
 * @brief von QObject erbende Klasse
 * Diese Klasse stellt Funktionen zur Verfuegung um einen Automaten zu verwalten.
 * Sie besteht aus Funktionen, Slots, Signalen und privaten Membern.
 */
class automaton : public QObject{
    Q_OBJECT
public:
    explicit automaton(QObject *parent = 0);

signals:
    void      stateChanged(int input);
    void      idChanged(int id);
    void      amountChanged(int amount);
    void      inputReceived(int input, state from);
    void      transitionToExecute(transition* toBeExecuted);

public slots:
    // Notfalls mit dieser Klasse nur einen einzigen, den benötigten Automaten realisieren
    void       transitions(int input);

    //void        receiveInput(int input);
    // Keine getter-Funktion, schlägt Transition im Vektor nach und emittet transitionToExecute(transition*)
    //bool      isCorrectTransition(transition* inputTransition);
    //void      getTransition(int input, state from);

    // Aktualisiere Ansicht in Kinect-Tab
    QString     updateStatusViewer();
    QString     updateIdViewer();
    QString     updateAmountViewer();

public:
    void    setState(state newState);
    void    setAmount(unsigned int newAmount);
    void    setPid(unsigned int newPid);

    void    setObj(sqlfunctions* mySqlObj);
    void    setKinect(kinectio* kinect);
    //void    addTransition(transition* toBeAdded);

    state            getState();
    unsigned int     getAmount();
    unsigned int     getPid();

private:
    state                   currentState;		///< aktueller Zustand
    unsigned int            pid;				///< Produkt ID des ausgewaehlten Produktes (initialisiert mit 2)
    unsigned int            amount;				///< Menge des ausgewaehlten Produkts
    sqlfunctions*           connectedObj;		///< Zeiger auf das zugehoerige sqlfunctions Object
    kinectio*               connectedKinect;	///< Zeiger auf die verbundene Kinect
    int                     receivedInput;		///< Test

    /* Transitionen für mehr Automaten als nur diesen einen ermöglichen!
    vector<transition*>     transitions;
    int                     searchInput;
    state                   searchState;
    */
};

#endif // AUTOMATON_H
