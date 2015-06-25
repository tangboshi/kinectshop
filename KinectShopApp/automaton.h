#ifndef AUTOMATON_H
#define AUTOMATON_H

#include <QObject>
#include <QDebug>
#include <QMessageBox>

#include "state.h"
#include "transition.h"
#include "sqlfunctions.h"

class transition;
typedef vector<transition*>::iterator iterA;

class automaton : public QObject{
    Q_OBJECT
public:
    explicit automaton(QObject *parent = 0);

signals:
    void      stateChanged(int input);
    void      inputReceived(int input, state from);
    void      transitionToExecute(transition* toBeExecuted);

public slots:
    // Notfalls mit dieser Klasse nur einen einzigen, den benötigten Automaten realisieren
    void        transitions(int input);
    //void        receiveInput(int input);
    // Keine getter-Funktion, schlägt Transition im Vektor nach und emittet transitionToExecute(transition*)
    //bool      isCorrectTransition(transition* inputTransition);
    //void      getTransition(int input, state from);

public:
    void    setState(state newState);
    void    setAmount(unsigned int newAmount);
    void    setPid(unsigned int newPid);

    void    setObj(sqlfunctions* mySqlObj);
    //void    addTransition(transition* toBeAdded);

    state            getState();
    unsigned int     getAmount();
    unsigned int     getPid();

private:
    state                   currentState;
    unsigned int            pid;
    unsigned int            amount;
    sqlfunctions*           connectedObj;
    int                     receivedInput;

    /* Transitionen für mehr Automaten als nur diesen einen ermöglichen!
    vector<transition*>     transitions;
    int                     searchInput;
    state                   searchState;
    */

    // Plan: receivedInput wird erhalten, indem die Funktion receiveInput() ausgelöst wird. receiveInput emittet ein SIGNAL inputReceived(state from, int input)
    // executeTransition(transition myTransition) wird im Konstruktor der Transition mit SIGNAL inputReceived connectet.
};

#endif // AUTOMATON_H
