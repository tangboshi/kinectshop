#include "product.h"

/**
 * @brief Constructor
 */
product::product(){
}

/**
 * @brief getter
 * @return int: Menge
 */
int product::getAmount(){
    return amount;
}

/**
 * @brief getter
 * @return int: Produkt ID
 */
int product::getPid(){
    return pid;
}

/**
 * @brief getter
 * @return QString: Name des Produkts
 */
QString product::getTitle(){
    return title;
}

/**
 * @brief getter
 * @return double: Preis
 */
double product::getPrice(){
    return price;
}

/**
 * @brief getter
 * Liest die Gewinnmarge aus dem SQL Server aus und gibt sie zurueck.
 * @return double Gewinnmarge
 */
double product::getMargin(){
    QSqlQuery query;
    query.prepare("SELECT margin FROM products WHERE id=:pid");
    query.bindValue(":pid", pid);
    query.exec();
    query.next();
    return query.value(0).toDouble();
}

/**
 * @brief getter
 * Liest den Umsatz aus dem SQL Server aus und gibt ihn zurueck.
 * @return double Umsatz
 */
double product::getRevenue(){
    QSqlQuery query;
    query.prepare("SELECT revenue FROM products WHERE id=:pid");
    query.bindValue(":pid", this->getPid());
    query.exec();
    query.next();
    return query.value(0).toDouble();
}

/**
 * @brief getter
 * Liest den Profit aus dem SQL Server aus und gibt ihn zurueck.
 * @return double Profit
 */
double product::getProfit(){
    QSqlQuery query;
    query.prepare("SELECT profit FROM products WHERE id=:pid");
    query.bindValue(":pid", this->getPid());
    query.exec();
    query.next();
    return query.value(0).toDouble();
}

/**
 * @brief setter
 * @param newAmount neue Menge
 * @return void
 */
void product::setAmount(int newAmount){
    amount = newAmount;
}

/**
 * @brief setter
 * @param newPid neue Produkt ID
 * @return void
 */
void product::setPid(int newPid){
    pid = newPid;
}

/**
 * @brief setter
 * @param newTitle neuer Produktname
 * @return void
 */
void product::setTitle(QString newTitle){
    title = newTitle;
}

/**
 * @brief setter
 * @param newPrice neue Preis
 * @return void
 */
void product::setPrice(double newPrice){
    price = newPrice;
}

/**
 * @brief setter
 * Ueberschreibt die Gewinnmarge im SQL Server.
 * @param newMargin neue Gewinnmarge
 * @return void
 */
void product::setMargin(double newMargin){
    QSqlQuery query;
    query.prepare("UPDATE products SET margin=:margin WHERE id=:pid");
    query.bindValue(":pid", pid);
    query.bindValue(":margin", newMargin);
    query.exec();
    query.next();
}

//Zuweisungsoperator zum Kopieren von Produktinstanzen
/**
 * @brief Zuweisungsoperator zum Kopieren von Produktinstanzen
 * Speichert "pid", "amount", "price", "title" von theProduct in dem aufrufenden Produkt.
 * 
 * product p = theProduct;
 * @param &theProduct ein Zeiger auf ein Produkt, welches kopiert werden soll
 * @return void
 */
void product::operator =(const product &theProduct){
    this->pid = theProduct.pid;
    this->amount = theProduct.amount;
    this->price = theProduct.price;
    this->title = theProduct.title;
}

/**
 * @brief Vergleichsoperator
 * Vergleicht die Produkt ID des aufrufenden Produkts mit einer eingegeben ID
 * @param the Product eine ID
 * @return bool: true (die IDs stimmen ueberein), false (sonst)
 */
bool product::operator == (const int theProduct) const{
    return (pid == theProduct);
}
