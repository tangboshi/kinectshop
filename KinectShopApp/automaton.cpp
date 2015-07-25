#include "automaton.h"

/**
 * @brief Constructor
 * Setzt den Ausgangszustand zu "IDLE", id=0, stateInfo=0
 * @param parent Zeiger auf ein QObject (sqlfunctions)
 */
automaton::automaton(QObject *parent) :
    QObject(parent)
{
    //connect(this,SIGNAL(stateChanged(int)),this,SLOT(transitions(int)));

    state idle;
    idle.setId(0);
    idle.setStateInfo(0);
    currentState = idle;

    //connect(this, SIGNAL(inputReceived(int,state)), this, SLOT(getTransition(int,state)));

    // Zu Testzwecken
    pid = 2;

    // Testfunktion
    // qDebug() << currentState.getId();
}

// ///////////////////// Transitions /////////////////////

// Zustände
/**
 * @def IDLE 0
 * @brief untaetiger Zustand
 */

/**
 * @def SELECTED 1
 * @brief es wurde ein Produkt ausgewaehlt
 */

/**
 * @def AMOUNTSET 2
 * @brief es wurde eine Anzahl festgelegt
 */

/**
 * @def FINISHED 3
 * @brief der Prozess wurde abgeschlossen
 */

/**
 * @def ONE 1
 * @brief Anzahl Produkte = 1
 */

/**
 * @def TWO 2
 * @brief Anzahl Produkte = 2
 */

/**
 * @def SELECT 100
 * @brief ein Produkt wird ausgewaehlt. Der zugehoerige Status ist SELECTED.
 */

/**
 * @def PUTINCART 101
 * @brief das Produkt wird in den Einkaufswagen abgelegt
 */

/**
 * @def BACK 102
 * @brief der Prozess wird einen Schritt zurueckgesetzt
 */

/**
 * @def CANCEL 103
 * @brief der Prozess wird auf den Anfangszustand zurueckgesetzt
 */

/**
 * @def MORE 104
 * @brief der Prozess wurde abgeschlossen
 */
#define IDLE 0
#define SELECTED 1
#define AMOUNTSET 2
#define FINISHED 3

state idle, selected, amountset, finished;

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

/**
 * @brief eine Transition wird anhand der Eingabe durchgefuehrt
 * Die IDs der Zustaende idle(IDLE), selected(SELECTED), amountset(AMOUNTSET) und finished(FINISHED) werden gesetzt.
 * 
 * Anhand der ID des aktuellen Zustandes wird entschieden:
 * -IDLE: Anhand der Eingabe wird entschieden:
 *		 *SELECT: Der neue Zustand ist selected. Die Produkt ID wird (Aufruf getDetectedPid()) bestimmt.
 *		 *sonst: Fehlermeldung
 * -SELECTED: Anhand der Eingabe wird entschieden:
 *		 *ONE: Der neue Zustand ist amountset. Die Produktmenge ist 1.
 *		 *TWO: Der neue Zustand ist amountset. Die Produktmenge ist 2.
 *		 *BACK: Der neue Zustand ist idle.
 *		 *CANCEL: Der neue Zustand ist idle.
 *		 *sonst: Fehlermeldung
 * -AMOUNTSET: Anhand der Eingabe wird entschieden:
 *		 *PUTINCART: der neue Zustand ist finished. Das Produkt wird in den Warenkorb abgelegt.
 *		 *BACK: der neue Zustand ist selected. Die Produkt Menge wird zurueckgesetzt.
 *		 *CANCEL: der neue Zustand ist idle. Die Produkt Menge wird zurueckgesetzt.
 *		 *sonst: Fehlermeldung
 * -FINISHED: Anhand der Eingabe wird entschieden:
 *		 *MORE: der neue Zustand ist idle. Die Produkt Menge wird zurueckgesetzt.
 *		 *sonst: Fehlermeldung
 * @param input Eine Zahl, von der abhaengig ist, welche Aktion als naechstes ausgefuehrt wird.
 * @return void
 * @see IDLE 0
 * @see SELECTED 1
 * @see AMOUNTSET 2
 * @see FINISHED 3
 * @see ONE 1
 * @see TWO 2
 * @see SELECT 100
 * @see PUTINCART 101
 * @see BACK 102
 * @see CANCEL 103
 * @see MORE 104
 */
void automaton::transitions(int input){

    idle.setId(IDLE);
    selected.setId(SELECTED);
    amountset.setId(AMOUNTSET);
    finished.setId(FINISHED);

    QMessageBox msgBox;
    msgBox.setText("Die StateId ist: "+QString::number(currentState.getId())+"\nDer Input ist: "+QString::number(input));
    msgBox.exec();

    switch (currentState.getId()){

    case IDLE:
        switch (input) {
        case SELECT:
            // neue pid hier setzen
            setState(selected);
            this->setPid(connectedKinect->getDetectedPid());
            break;
        default:
            qDebug() << "FEHLER: case IDLE";
            break;
        }
    break;

    case SELECTED:
        switch (input) {
        case ONE:
            setState(amountset);
            setAmount(1);
            break;
        case TWO:
            setState(amountset);
            setAmount(2);
            break;
        case BACK:
            setState(idle);
            break;
        case CANCEL:
            setState(idle);
            break;
        default:
            qDebug() << "FEHLER: case SELECTED";
            break;
        }
    break;

    case AMOUNTSET:
        switch (input) {
        case PUTINCART:
        {
            setState(finished);
            QSqlQuery query;
            query.prepare("SELECT price, title FROM products WHERE id = :pid");
            query.bindValue(":pid", pid);
            query.exec();
            query.next();
            double price = query.value(0).toDouble();
            QString title = query.value(1).toString();
            connectedObj->addToCart(pid,amount,price,title);
            break;
        }
        case BACK:
            setState(selected);
            setAmount(0);
            break;
        case CANCEL:
            setState(idle);
            setAmount(0);
            break;
        default:
            qDebug() << "FEHLER: case AMOUNTSET";
            break;
        }
    break;

    case FINISHED:
        switch (input) {
        case MORE:
            setState(idle);
            setAmount(0);
            break;
        default:
            qDebug() << "FEHLER: case FINISHED";
            break;
        }
    break;

    default:
        qDebug() << "FEHLER: switch";
        break;
    }
}

QString automaton::updateStatusViewer(){
    stringstream stream;

    int currentId = currentState.getId();
    string currentStateName;

    switch(currentId){
        case 0:
            currentStateName = "Idle";
            break;
        case 1:
            currentStateName = "Selected";
            break;
        case 2:
            currentStateName = "Amountset";
            break;
        case 3:
            currentStateName = "Finished";
            break;
        default:
            currentStateName = "Fehler!";
    }

    stream << "<p>Sie befinden sich in folgenden Zustand: " << currentStateName << " </p>" << endl;

    string s = stream.str();
    QString htmlOutput = QString::fromStdString(s);

    return htmlOutput;
}

/**
 * @brief Ausgabe der vom Automaten gespeicherten Produktmenge
 * Eine Nachricht mit der Produktmenge wird in einen Stream geladen, dieser wird in einen String umgewandelt und dann in einem QString gespeichert und ausgegeben.
 * @return QString: Nachricht mit Produktmenge
 */
QString automaton::updateAmountViewer(){
    stringstream stream;

    stream << "<p>Die Anzahl der im Automaten festgelegten Einheiten: " << amount << " </p>" << endl;

    string s = stream.str();
    QString htmlOutput = QString::fromStdString(s);

    return htmlOutput;
}

/**
 * @brief Ausgabe der vom Automaten gespeicherten Produkt ID
 * Eine Nachricht mit der Produkt ID wird in einen Stream geladen, dieser wird in einen String umgewandelt und dann in einem QString gespeichert und ausgegeben.
 * @return QString: Nachricht mit Produkt ID
 */
QString automaton::updateIdViewer(){
    stringstream stream;

    stream << "<p>Die Produkt-ID, die im Automaten gespeichert ist: " << pid << " </p>" << endl;

    string s = stream.str();
    QString htmlOutput = QString::fromStdString(s);

    return htmlOutput;
}

/*
void automaton::receiveInput(int input){
    receivedInput = input;
    transition* toBeExecuted= new transition();
    emit transitionToExecute(toBeExecuted);
}

void automaton::addTransition(transition* toBeAdded){
    transitions.push_back(toBeAdded);
}

int inputValue = 0;

bool automaton::isCorrectTransition(transition* inputTransition){
    if((inputTransition->getFrom().getId() == searchState.getId()) && (inputTransition->getInput() == searchInput)) return true;
    return false;
}

struct isCorrectTransition : std::unary_function<transition*, bool> {
    int inputValue;
    state inputFrom;
    isCorrectTransition(int inputValue):inputValue(inputValue){ }
    bool operator()(transition* const& inputTransition) const {
        return ((inputTransition->getInput() == inputValue) && (inputTransition->getFrom().getId() == inputFrom.getId()));
    }
};

void automaton::getTransition(int inputValue, state inputFrom){
    searchInput = inputValue;
    searchState = inputFrom;
    iterA cursor = find_if(transitions.begin(), transitions.end(), isCorrectTransition(inputValue));
    if(cursor!=transitions.end()){
        emit transitionToExecute(*cursor);
        return;
    }
}
*/

// ///////////////////////////////////////////////////////

/**
 * @brief setter
 * Der aktuelle Zustand wird geaendert. Das Signal stateChanged(newState.getID()) wird ausgeloest.
 * @param newState neuer Zustand 
 * @return void
 */
void automaton::setState(state newState){
    currentState = newState;
    emit stateChanged(newState.getId());

    // Testfunktion
    QMessageBox msgBox;
    msgBox.setText("Die Id des States auf den zu setzen ist, ist: "+ QString::number(newState.getId()));
    msgBox.exec();

    msgBox.setText("Die Id des neuen States ist: "+ QString::number(currentState.getId()));
    msgBox.exec();
}

/**
 * @brief setter
 * Die Produktmenge wird geaendert. Das Signal amountChanged(newAmount) wird ausgeloest.
 * @param newAmount neue Produktmenge
 * @return void
 */
void automaton::setAmount(unsigned int newAmount){
    amount = newAmount;
    emit amountChanged(newAmount);
}

/**
 * @brief setter
 * Die Produkt ID wird geaendert. Das Signal idChanged(newPid) wird ausgeloest.
 * @param newPid neue Produkt ID
 * @return void
 */
void automaton::setPid(unsigned int newPid){
    pid = newPid;
    emit idChanged(newPid);
}

/**
 * @brief setter
 * @param obj Zeiger auf ein sqlfunctions Objekt
 * @return void
 */
void automaton::setObj(sqlfunctions *obj){
    connectedObj = obj;
}

/**
 * @brief setter
 * @param kinect Zeiger auf ein kinectio Objekt
 * @return void
 */
void automaton::setKinect(kinectio *kinect){
    connectedKinect = kinect;
}

/**
 * @brief getter
 * @return state: der aktuelle Zustand
 */
state automaton::getState(){
    return currentState;
}

/**
 * @brief getter
 * @return unsigned int: Produktmenge
 */
unsigned int automaton::getAmount(){
    return amount;
}

/**
 * @brief getter
 * @return unsigned int: Produkt ID
 */
unsigned int automaton::getPid(){
    return pid;
}
