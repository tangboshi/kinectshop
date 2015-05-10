#include "product.h"

product::product(){
}

int product::getAmount(){
    return amount;
}

int product::getPid(){
    return pid;
}

string product::getTitle(){
    return title;
}

int product::getPrice(){
    return price;
}

void product::setAmount(int newAmount){
    amount = newAmount;
}

void product::setPid(int newPid){
    pid = newPid;
}

void product::setTitle(string newTitle){
    title = newTitle;
}

void product::setPrice(int newPrice){
    price = newPrice;
}

//Zuweisungsoperator zum Kopieren von Produktinstanzen
void product::operator =(const product &theProduct){
    this->pid = theProduct.pid;
    this->amount = theProduct.amount;
    this->price = theProduct.price;
    this->title = theProduct.title;
}

//Operator um zwei Produkte zu vergleichen, liefert true falls gleich
bool product::operator == (const product& theProduct) const{
    return true;
}
