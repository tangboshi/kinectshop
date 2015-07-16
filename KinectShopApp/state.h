#ifndef STATE_H
#define STATE_H

/**
 * @class state Class
 * @brief Nicht erbende Klasse
 * Diese Klasse stellt Funktionen zur Verfuegung um Zustaende (eines Automaten) zu verwalten.
 * Sie besteht aus Funktionen und privaten Membern.
 */
class state{
public:
    state();
    void            setStateInfo(int info);
    int             getStateInfo();
    void            setId(unsigned int newId);
    int             getId();
private:
    int             stateInfo;	///< Info	
    unsigned int    id;			///< Zustands ID
};

#endif // STATE_H
