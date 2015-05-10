#include "sqlfunctions.h"

sqlfunctions::sqlfunctions(){
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("kinectsrv.lfb.rwth-aachen.de");
    db.setDatabaseName("kinectshop2015");
    db.setUserName("kinectshopClient");
    db.setPassword("lfb-student2015");
    db.setPort(3306);
    qDebug()<<db.open();

    isLogin = false;
    isAdminLoggedIn = false;
}

// ----------------------------------------------------------------------------------
// --------------------------------------WAREZMGMT-----------------------------------
// ----------------------------------------------------------------------------------

// Zeige die im Shop verfügbaren Produkte an
void sqlfunctions::listAllProducts(){
    QSqlQuery query;

    // Alles Abfragen
    query.prepare("SELECT * FROM products");
    query.exec();

    // FEHLT Tabllen-Umgebung: nachträglich einfügen!!
    int pid, stock;
    string title;
    double price;

    // Alles darstellen
    while(query.next()){
        pid = query.value(0).toInt();
        title = query.value(1).toString().toStdString();
        price = query.value(2).toDouble();
        stock = query.value(4).toInt();
        cout    <<  "<tr>"
                <<  "<td>"      <<  pid       <<    "</td>"
                <<  "<td>"      <<  title     <<    "</td>"
                <<  "<td>"      <<  price     <<    "</td>"
                <<  "<td>"      <<  stock     <<    "</td>"
                <<  "</tr>"     <<  endl;
    }

}

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
        int newAmount = myProduct.getAmount() + cursor->getAmount();
        myProduct.setAmount(newAmount);
        cart.erase(cursor);
    }
    return myProduct;
}

// Gibt den Inhalt des Einkaufswagens aus, bereits HTML
// Überarbeiten! Verwende <thead><th><tbody> !!!
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

// Überladene Funktion, entspricht eigtl. removeFromCart(myProduct)
void sqlfunctions::changeAmount(product myProduct, string mode){
        if(mode=="clear"){
            iter cursor = find(cart.begin(), cart.end(), myProduct.getPid());
            cart.erase(cursor);
        }
}

// Wenn man unzufrieden ist mit der Menge an eingekauften Waren, kann man diese ändern.
void sqlfunctions::changeAmount(product myProduct, int diff, string mode){
    if(mode=="add"){
        myProduct.setAmount(myProduct.getAmount()+diff);
    }
    else if(mode=="sub"){
        myProduct.setAmount(myProduct.getAmount()-diff);
    }
}

// Prüft für jede Ware im Warenkorb, ob noch genug Waren vorhanden sind
int sqlfunctions::checkStock(){
    QSqlQuery query;
    int diff, stock;
    for(iter cursor = cart.begin();cursor!=cart.end();cursor++){
        query.prepare("SELECT stock FROM products WHERE id = :input");
        query.bindValue(":input", cursor->getPid());
        query.exec();      
        query.next();
        stock = query.value(0).toInt();
        diff = stock - cursor->getAmount();
    }
    return diff;
}

// Prüft, ob der Benutzer ausreichend Guthaben zum Kauf hat.
double sqlfunctions::checkBalance(){
    QSqlQuery query;
    // ermittelt Gesamtkosten des Warenkorbs
    double total = 0;
    for(iter cursor = cart.begin(); cursor!=cart.end(); cursor++){
        total += (cursor->getPrice() * cursor->getAmount());
    }
    // Vergleiche Guthaben mit Gesamtkosten
    query.prepare("SELECT balance FROM users WHERE id = :uid");
    query.bindValue(":uid", uid);
    query.next();
    double balance = query.value(0).toDouble();
    return balance - total;
}

// Die Bezahlfunktion
void sqlfunctions::purchase(){
    QSqlQuery query;
    // Überprüfe, ob User eingeloogt ist.
    if(isLogin){
        int hasEnoughMoney = checkBalance();
        // Überprüfe ob User genug Guthaben hat.
        if(hasEnoughMoney >= 0){
            // Überprüfe, ob genug waren vorhanden sind.
            int diff = checkStock();
            if(diff >= 0){
                for(iter cursor = cart.begin(); cursor != cart.end(); ++cursor){

                    int pid = cursor->getPid();
                    int amount = cursor->getAmount();

                    // SQL-Befehle für den Bezahlvorgang
                    // Buchungstabelle
                    // größte Buchungsnummer ermitteln
                    query.prepare("SELECT MAX(id) FROM bookings");
                    query.exec();
                    query.next();
                    int insertId = query.value(0).toInt();
                    // Buchungsnummer um 1 erhöhen
                    insertId++;
                    query.prepare("INSERT INTO bookings (id,uid,pid,amount) VALUES (:id, :uid, :pid, :amount)");
                    query.bindValue(":id", insertId);
                    query.bindValue(":uid", uid);
                    query.bindValue(":pid", pid);
                    query.bindValue(":amount", amount);
                    query.exec();

                    // Kosten ausrechnen
                    query.prepare("SELECT price FROM products WHERE id =:pid");
                    query.bindValue(":pid", pid);
                    query.exec();

                    query.next();
                    double price = query.value(0).toDouble();
                    double cost = (double)amount*price;

                    query.prepare("SELECT balance FROM users WHERE id = :uid");
                    query.bindValue(":uid", uid);
                    query.exec();
                    query.next();
                    double balance = query.value(0).toDouble();
                    balance -= cost;

                    // Userguthaben abbuchen
                    query.prepare("UPDATE users SET balance=:newBalance WHERE id =:uid");
                    query.bindValue(":newBalance", balance);
                    query.bindValue(":uid", uid);
                    query.exec();

                    // Produktvorrat reduzieren
                    query.prepare("SELECT stock FROM products WHERE id = :pid");
                    query.bindValue(":pid", pid);
                    query.exec();
                    query.next();
                    int stock = query.value(0).toInt();
                    stock -= amount;

                    query.prepare("UPDATE products SET stock=:newStock WHERE id=:pid");
                    query.bindValue(":newStock", stock);
                    query.bindValue(":pid", pid);
                    query.exec();
                }
                // Signal: Einkauf abgeschlossen, Einkaufswagen leeren
                emit purchaseDone(cart);
                clearCart();
            }
            else{
                // Ausgabe falls nicht genug Waren zur Verfügung stehen.
                // Füge noch die Ausgabe der (tlw.) nicht lieferbare Waren und deren vorhandene Menge
                QMessageBox msgBox;
                msgBox.setText("Es sind nicht genug Waren vorhanden. Verringern Sie ihre Bestellung um "+QString::number(diff*(-1))+" Einheiten.");
                msgBox.exec();
            }
        }
        else{
            // Ausgabe falls der User nicht genug Geld hat.
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


void sqlfunctions::registerUser(QString username, QString password){
    // Überprüfen, ob User in DB vorhanden ist
    QSqlQuery query;
    query.prepare("SELECT :username FROM users");
    query.bindValue(":username", username);
    query.exec();
    query.next();
    QString newUsername = query.value(0).toString();
    // Wenn ja, trage in DB ein und gebe Erfolgsmeldung aus.
    if(username.toStdString() == newUsername.toStdString()){
        // Maximale Usernummer herausfinden
        query.prepare("SELECT MAX(id) FROM users");
        query.exec();
        query.next();
        int insertId = query.value(0).toInt();
        // Usernummer um 1 erhöhen
        insertId++;
        query.prepare("INSERT INTO users(id, username, password, balance, isAdmin) VALUES(:id :username :pasword :balance :isAdmin)");
        query.bindValue(":id", insertId);
        query.bindValue(":username", username);
        query.bindValue(":username", password);
        query.bindValue(":balance", 0);
        query.bindValue(":isAdmin", 0);
        query.exec();
        QMessageBox msgBox;
        msgBox.setText("Herzlichen Dank für Ihre Registrierung.");
    }

    // Wenn nicht, gib Fehlermeldung aus.
    else{
        QMessageBox msgBox;
        msgBox.setText("Das hat leider nicht funktioniert. Es gibt bereits einen Benutzer mit diesem Namen.");
    }
}

// Geld auf User-Account laden
void sqlfunctions::refillBalance(int amount){
    QSqlQuery query;
    query.prepare("SELECT balance FROM users WHERE id = :uid");
    query.bindValue(":uid", uid);
    query.exec();
    query.next();
    double balance = query.value(0).toDouble();
    balance += amount;
    query.prepare("UPDATE users SET balance=:newBalance WHERE id=:uid");
    query.bindValue(":newBalance", balance);
    query.bindValue(":uid", uid);
    query.exec();
}

// GIBT TABELLE MIT ALLEN USERN AUS
void sqlfunctions::listAllUsers(){
}

// GIBT USER ADMIN-PRIVILLEGIEN
// Auswirkung: Zeige Admin-Menüpunkte und Optionen in App
void sqlfunctions::empowerUser(){
}

// NIMMT USER ADMIN-PRIVILLEGIEN
// Auswirkung: Verstecke Admin-Menüpunkte und Optionen in App
void sqlfunctions::disempowerUser(){
}

void sqlfunctions::login(QString username, QString password){
    // Prüfen, ob Username-Password-Kombination existiert
    // TIMEOUT BEI MEHRFACH FALSCHER EINGABE EINFÜGEN
    QSqlQuery query;
    bool accountExists = query.prepare("SELECT username FROM users WHERE password = :password");
    query.bindValue(":password", password);
    query.exec();
    QString receivedUsername = query.value(0).toString();
    bool credentialsMatch = false;
    if(receivedUsername.toStdString() == username.toStdString()){
        credentialsMatch = true;
    }
    if(accountExists&&credentialsMatch){
        // Member uid für Einkauf setzen
        query.prepare("SELECT id FROM users WHERE username =:username");
        query.bindValue(":username", username);
        query.exec();
        query.next();
        uid = query.value(0).toInt();

        // Prüfen ob ein Admin eingeloggt ist
        // Setze entsprechend isAdminLoggedIn auf true bzw. false
        // Wenn ja sende Signal adminLoggedIn aus
        // Gib Admin-Rechte.
        QSqlQuery query;
        query.prepare("SELECT isAdmin FROM users WHERE id = :uid");
        query.bindValue(":uid", uid);
        query.exec();
        query.next();
        bool userIsAdmin = query.value(0).toBool();
        if(userIsAdmin){
            emit adminLoggedIn();
        }
    }
    else{
        QMessageBox msgBox;
        msgBox.setText("Benutzername oder Passwort inkorrekt!");
        msgBox.exec();
    }
}

