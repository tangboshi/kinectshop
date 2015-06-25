#ifndef TRANSITION_H
#define TRANSITION_H

#include "automaton.h"

class automaton;

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
    state       from;
    state       to;
    int         input;
    automaton*  connectedAutomaton;
};

#endif // TRANSITION_H
