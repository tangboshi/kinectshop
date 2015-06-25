#include "transition.h"

transition::transition()
{
}

transition::transition(state* initFrom, state* initTo, int initInput, automaton* initConnectedAutomaton){
    from = *initFrom;
    to = *initTo;
    input = initInput;
    connectedAutomaton = initConnectedAutomaton;
    connect(connectedAutomaton, SIGNAL(transitionToExecute(transition*)), this, SLOT(executeTransition(transition*)));

    // Testfunktion
    qDebug() << "Eine Transition wurde erstellt!";
}

void transition::executeTransition(transition* toBeExecuted){
    toBeExecuted->connectedAutomaton->setState(toBeExecuted->getTo());
    emit connectedAutomaton->stateChanged(toBeExecuted->getInput());

    // Testfunktion
    qDebug() << "Eine Transition wurde ausgeführt!";
}

// Setter und getter

state transition::getFrom(){
    return from;
}

void transition::setFrom(state newState){
    from = newState;
}

state transition::getTo(){
    return to;
}

void transition::setTo(state newState){
    to = newState;
}

int transition::getInput(){
    return input;
}

void transition::setInput(int newInput){
    input = newInput;
}
