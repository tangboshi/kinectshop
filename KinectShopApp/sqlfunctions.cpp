#include "sqlfunctions.h"

sqlfunctions::sqlfunctions(){
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("kinectsrv.lfb.rwth-aachen.de");
    db.setDatabaseName("kinectshop2015");
    db.setUserName("kinectshopClient");
    db.setPassword("lfb-student2015");
    db.setPort(3306);
    qDebug()<<db.open();

    isLogin = 0;
}

// ----------------------------------------------------------------------------------
// --------------------------------------WAREZMGMT-----------------------------------
// ----------------------------------------------------------------------------------

// Füge Element in den Warenkorb ein
void sqlfunctions::addToCart(product myProduct){
    product newProduct = isAlreadyInCart(myProduct);
    cart.push_back(newProduct);
}

// Überprüft, ob ein Element bereits im Warenkorb ist, falls ja erhöhe nur die Menge
// Gibt hinzuzufügendes Produkt an addToCart() zurück.
product sqlfunctions::isAlreadyInCart(product myProduct){
    iter cursor = find(cart.begin(), cart.end(), myProduct.getPid());
    if(cursor!=cart.end()){

        product* newProduct = new product;

        int newPid = myProduct.getPid();
        newProduct->setPid(newPid);

        int newAmount = myProduct.getAmount() + cursor->getAmount();
        newProduct->setAmount(newAmount);

        string newTitle = myProduct.getTitle();
        newProduct->setTitle(newTitle);

        cart.erase(cursor);
        return *newProduct;
    }
    return myProduct;
}

// Löscht Produkt aus Einkaufswagen
void sqlfunctions::removeFromCart(product myProduct){
    iter cursor = find(cart.begin(), cart.end(), myProduct.getPid());
    cart.erase(cursor);
}

// Gibt den Inhalt des Einkaufswagens aus, bereits HTML
// Überdenken!
void sqlfunctions::showCart(){

    cout    <<  "<table id='cart'>"   << endl;
    for(iter cursor = cart.begin();cursor!=cart.end();cursor++){
        cout    <<  "<tr> "     << endl;
        cout    <<  "<td>"      <<  "Produkt-ID: "      << cursor->getPid()       << "</td>"      << endl;
        cout    <<  "<td>"      <<  "Produktname: "     << cursor->getTitle()     << "</td>"      << endl;
        cout    <<  "<td>"      <<  "Menge: "           << cursor->getAmount()    << "</td>"      << endl;
        cout    <<  "</tr> "    <<  endl;
    }
    cout    <<  "</table>"   << endl;
}

// Leert den Einkaufswagen
void sqlfunctions::clearCart(){
    int size = cart.size();
    while(size){
        cart.pop_back();
        --size;
    }
}

// Wenn man unzufrieden ist mit der Menge an eingekauften Waren, kann man diese ändern.
void sqlfunctions::changeAmount(product myProduct, int newAmount){
    myProduct.setAmount(newAmount);
}

// Prüft für jede Ware im Warenkorb, ob noch genug Waren vorhanden sind
bool sqlfunctions::checkStock(){
    QSqlQuery query;
    int diff, stock;
    for(iter cursor = cart.begin();cursor!=cart.end();cursor++){
        query.prepare("SELECT stock FROM products WHERE id = :input");
        query.bind(":input", cursor->getPid);
        query.exec();
        // Need to save result of query into variable stock
        stock = ??;
        diff = stock - cursor->getAmount;
        if(diff < 0){
            return false;
        }
    }
    return true;
}

// Prüft, ob der Benutzer ausreichend Guthaben zum Kauf hat.
// UNVOLLSTÄNDIG!!
int sqlfunctions::checkBalance(){
    // ermittelt Gesamtkosten des Warenkorbs
    int total = 0;
    for(iter cursor = cart.begin(); cursor!=cart.end(); cursor++){
        total += (cursor->getPrice() * cursor->getAmount());
    }
    // Vergleiche Guthaben mit Gesamtkosten
    int balance;
    return balance - total;
}

// Die Bezahlfunktion
void sqlfunctions::purchase(){
    // Überprüfe, ob User eingeloogt ist.
    if(isLogin){
        int hasEnoughMoney = checkBalance();
        // Überprüfe ob User genug Guthaben hat.
        if(hasEnoughMoney >= 0){
            if(checkStock()){
                // SQL-Befehle für den Bezahlvorgang
                // Buchungstabelle

                // Userguthaben abbuchen

                // Produktvorrat reduzieren

                // Signal: Einkauf abgeschlossen
                emit purchaseDone(cart);
            }
            else{
                // Füge noch die Ausgabe der (tlw.) nicht lieferbare Waren und deren vorhandene Menge
                QMessageBox msgBox;
                msgBox.setText("Es sind leider nicht genug Waren vorhanden!");
                msgBox.exec();
            }
        }
        else{
            // User hat nicht genug Geld, zeige Differenz an.
            QMessageBox msgBox;
            msgBox.setText("Sie haben nicht genug Guthaben. Ihnen fehlen "+QString::number(hasEnoughMoney*(-1))+" Geldeinheiten.");
            msgBox.exec();
        }
    }
    else{
        QMessageBox msgBox;
        msgBox.setText("Loggen Sie sich bitte ein, um zu bezahlen!");
        msgBox.exec();
    }
}

// ----------------------------------------------------------------------------------
// ---------------------------------------USERMGMT-----------------------------------
// ----------------------------------------------------------------------------------


void sqlfunctions::registerUser(string username, string password){
}

// Gibt User Admin-Privillegien
void sqlfunctions::empowerUser(){
}

// Nimmt User Admin-Privillegien
void sqlfunctions::disempowerUser(){
}

void sqlfunctions::login(string username, string password){
    // Prüfen, ob Username-Password-Kombination existiert
    // Eventuell einen Timeout bei mehrfacher falscher Eingabe einfügen!
    if(){
        // Prüfen ob ein Admin eingeloggt ist
        // Setze entsprechend isAdminLoggedIn auf true bzw. false
        // Wenn ja sende Signal adminLoggedIn aus
        // Gib Admin-Rechte.
        if(){
            emit adminLoggedIn();
            void empowerUser();
        }
    }
    else{
        QMessageBox msgBox;
        msgBox.setText("Benutzername oder Passwort inkorrekt!");
        msgBox.exec();
    }
}
