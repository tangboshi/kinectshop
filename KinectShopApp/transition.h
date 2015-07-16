#ifndef TRANSITION_H
#define TRANSITION_H

#include "automaton.h"

class automaton;

/**
 * @class transition Class
 * @brief von QObject erbende Klasse
 * Diese Klasse stellt Funktionen zur Verfuegung um Transitionen (eines Automaten) zu verwalten.
 * Sie besteht aus Funktionen, Slots und privaten Membern.
 */
class transition : public QObject
{
public:
    transition();
    transition(state* initFrom, state* initTo, int initInput, automaton* initConnectedAutomaton);

    state       getFrom();
    void        setFrom(state newState);
    state       getTo();
    void        setTo(state newState);
    int         getInput();
    void        setInput(int newInput);

public slots:
    void        executeTransition(transition* toBeExecuted);

private:
    state       from;				///< Ausgangszustand
    state       to;					///< Zustand nach der Transition
    int         input;				///< Eingabe
    automaton*  connectedAutomaton;	///< Zugehoeriger Automat
};

#endif // TRANSITION_H
