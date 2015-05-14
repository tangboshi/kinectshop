#include "product.h"

product::product(){
}

int product::getAmount(){
    return amount;
}

int product::getPid(){
    return pid;
}

QString product::getTitle(){
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

void product::setTitle(QString newTitle){
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

bool product::operator == (const int theProduct) const{
    return (pid == theProduct);
}
