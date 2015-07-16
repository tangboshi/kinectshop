#include "transition.h"

/**
 * @brief Constructor
 */
transition::transition()
{
}

/**
 * @brief Constructor
 * Erstellt eine Transition von einem Ausgangszustand initFrom mit Eingabe initInput in den Zustand initTo fuer den Automaten initConnectedAutomaton.
 *
 * Der Automat wird ueber das Signal transitionToExecute(transition*) mit dem Slot executeTransition(transition*) verbunten.
 * @param initFrom Zeiger auf einen Zustand (Ausgangszustand)
 * @param initTo Zeiger auf einen Zustand (Zustand nach der Transition)
 * @param initInput Eingabe im Ausgangszustand
 * @param initConnectedAutomaton Zugehoeriger Automat
 */
transition::transition(state* initFrom, state* initTo, int initInput, automaton* initConnectedAutomaton){
    from = *initFrom;
    to = *initTo;
    input = initInput;
    connectedAutomaton = initConnectedAutomaton;
    connect(connectedAutomaton, SIGNAL(transitionToExecute(transition*)), this, SLOT(executeTransition(transition*)));

    // Testfunktion
    qDebug() << "Eine Transition wurde erstellt!";
}

/**
 * @brief fuehrt eine Transition aus.
 * Ermittelt speichert den neuen Status des Automaten im Automaten. (Aufruf setState(state newState))
 *
 * Das Signal stateChanged(toBeExecuted->getTo()) wird ausgeloest.
 * @param toBeExecuted auszufuehrende Transition
 * @return void
 */
void transition::executeTransition(transition* toBeExecuted){
    toBeExecuted->connectedAutomaton->setState(toBeExecuted->getTo());
    emit connectedAutomaton->stateChanged(toBeExecuted->getInput());

    // Testfunktion
    qDebug() << "Eine Transition wurde ausgeführt!";
}

// Setter und getter

/**
 * @brief getter
 * @return state: Ausgangszustand
 */
state transition::getFrom(){
    return from;
}

/**
 * @brief setter
 * @param newState neuer Ausgangszustand
 * @return void
 */
void transition::setFrom(state newState){
    from = newState;
}

/**
 * @brief getter
 * @return state: Zustand nach der Transition
 */
state transition::getTo(){
    return to;
}

/**
 * @brief setter
 * @param newState neuer Zustand nach der Transition
 * @return void
 */
void transition::setTo(state newState){
    to = newState;
}

/**
 * @brief getter
 * @return int: Eingabe
 */
int transition::getInput(){
    return input;
}

/**
 * @brief setter
 * @param newInput neue Eingabe
 * @return void
 */
void transition::setInput(int newInput){
    input = newInput;
}
