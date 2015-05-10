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

void product::setAmount(int amount){
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


