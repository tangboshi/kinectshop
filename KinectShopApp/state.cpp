#include "state.h"

/**
 * @brief Constructor
 */
state::state()
{
}

/**
 * @brief setter
 * @param info
 * @return void
 */
void state::setStateInfo(int info){
    stateInfo = info;
}

/**
 * @brief getter
 * @return int: Info
 */
int state::getStateInfo(){
    return stateInfo;
}

/**
 * @brief setter
 * @param newID Zustands ID
 * @return void
 */
void state::setId(unsigned int newId){
    id = newId;
}

/**
 * @brief getter
 * @return int: Zustands ID
 */
int state::getId(){
    return id;
}
