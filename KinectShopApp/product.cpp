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

double product::getPrice(){
    return price;
}

double product::getMargin(){
    QSqlQuery query;
    query.prepare("SELECT margin FROM products WHERE id=:pid");
    query.bindValue(":pid", pid);
    query.exec();
    query.next();
    return query.value(0).toDouble();
}

double product::getRevenue(){
    QSqlQuery query;
    query.prepare("SELECT revenue FROM products WHERE id=:pid");
    query.bindValue(":pid", this->getPid());
    query.exec();
    query.next();
    return query.value(0).toDouble();
}

double product::getProfit(){
    QSqlQuery query;
    query.prepare("SELECT profit FROM products WHERE id=:pid");
    query.bindValue(":pid", this->getPid());
    query.exec();
    query.next();
    return query.value(0).toDouble();
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

void product::setPrice(double newPrice){
    price = newPrice;
}

void product::setMargin(double newMargin){
    QSqlQuery query;
    query.prepare("UPDATE products SET margin=:margin WHERE id=:pid");
    query.bindValue(":pid", pid);
    query.bindValue(":margin", newMargin);
    query.exec();
    query.next();
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
