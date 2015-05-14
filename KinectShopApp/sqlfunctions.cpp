#include "sqlfunctions.h"

sqlfunctions::sqlfunctions(){
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("kinectsrv.lfb.rwth-aachen.de");
    db.setDatabaseName("kinectshop2015");
    db.setUserName("kinectshopClient");
    db.setPassword("lfb-student2015");
    db.setPort(3306);

    if(!db.open()){
        QMessageBox msgBox;
        msgBox.setText("Es konnte leider keine Verbindung zur SQL-Datenbank hergestellt werden!");
        msgBox.exec();
    }// Ausgeben wenn Verbindung hergestellt wurde
    /*
    else{
        QMessageBox msgBox;
        msgBox.setText("Die Verbindung zu " + db.hostName() + " wurde hergestellt.");
        msgBox.exec();
    }*/

    isLogin = false;
    isAdminLoggedIn = false;  
    uid = -1;

    badTries = 0;
    allowedAgain = 0;
}

// ----------------------------------------------------------------------------------
// --------------------------------------WAREZMGMT-----------------------------------
// ----------------------------------------------------------------------------------

// Zeige die im Shop verfügbaren Produkte an
QString sqlfunctions::listAllProducts(){
    QSqlQuery query;

    // Alles Abfragen
    query.prepare("SELECT * FROM products");
    query.exec();

    int pid, stock;
    string title;
    double price;

    stringstream stream;

    // Alles darstellen
    stream  <<  "<table id='cartList' class='sortable'>"
            <<  "<thead>"
            <<  "<tr>"
            <<  "<th data-sort='number'>"      <<  "Produkt-ID"    <<  "</th>"
            <<  "<th data-sort='name'>"        <<  "Produktname"   <<  "</th>"
            <<  "<th data-sort='number'>"      <<  "Preis"         <<  "</th>"
            <<  "<th data-sort='number'>"      <<  "Verfügbar"     <<  "</th>"
            <<  "<th data-sort='inputval'>"    <<  "Menge"         <<  "</th>"
            <<  "</tr>"
            <<  "</thead>"
            <<  "<tbody>"
            <<  endl;

    while(query.next()){
        pid = query.value(0).toInt();
        title = query.value(1).toString().toStdString();
        price = query.value(2).toDouble();
        stock = query.value(4).toInt();
        stream  <<  "<tr>"
                <<  "<td>"      <<  pid       <<    "</td>"
                <<  "<td>"      <<  title     <<    "</td>"
                <<  "<td>"      <<  price     <<    "</td>"
                <<  "<td>"      <<  stock     <<    "</td>"
                <<  "<td>"      <<  "<input type='text' value='0' id='cartItemAmount"  <<   pid  <<  "'>"
                <<  "<button class='orange-button' id='buyCartItem"                    <<   pid  <<  "'>"
                <<  "in den <span class='fa fa-shopping-cart'></span></button>"        <<   "</td>"
                <<  "</tr>"
                <<  endl;
        // Das funktioniert nur, weil alle ItemIds ab 1 vorkommen
        // und die SQL-Tabelle genau in dieser Reihenfolge sortiert ist.
    }

    stream  <<  "</tbody>"
            <<  "</table>"  <<  endl;

    string s = stream.str();

    // Testfunktion
    // cout    << s    << endl;

    QString htmlOutput = QString::fromStdString(s);

    /* // Test
    QMessageBox msgBox;
    msgBox.setText(htmlOutput);
    msgBox.exec();
    */

    return htmlOutput;
}

// Füge Element in den Warenkorb ein
void sqlfunctions::addToCart(int pid, int amount, double price, QString title){

    product myProduct;
    myProduct.setAmount(amount);
    myProduct.setPid(pid);
    myProduct.setPrice(price);
    myProduct.setTitle(title);

    product newProduct = isAlreadyInCart(pid, amount, price, title);
    cart.push_back(newProduct);

    /* Testfunktion ob JS-Übergabe funktioniert hat
    QMessageBox msgBox;
    msgBox.setText(QString::number(myProduct.getAmount()));
    msgBox.exec();
    */
    emit cartChanged();
}

// Überprüft, ob ein Element bereits im Warenkorb ist, falls ja erhöhe nur die Menge
// Gibt hinzuzufügendes Produkt an addToCart() zurück.
product sqlfunctions::isAlreadyInCart(int pid, int amount, double price, QString title){

    product myProduct;
    myProduct.setAmount(amount);
    myProduct.setPid(pid);
    myProduct.setPrice(price);
    myProduct.setTitle(title);

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

QString sqlfunctions::showCart(){

    stringstream stream;

    // Alles darstellen
    stream  <<  "<table id='productList' class='sortable'>"
            <<  "<thead>"
            <<  "<tr>"
            <<  "<th data-sort='number'>"      <<  "Produkt-ID"     <<  "</th>"
            <<  "<th data-sort='name'>"        <<  "Produktname"    <<  "</th>"
            <<  "<th data-sort='number'>"      <<  "Preis"          <<  "</th>"
            <<  "<th data-sort='number'>"      <<  "Bestellmenge"   <<  "</th>"
            <<  "<th data-sort='number'>"      <<  "Gesamtpeis"     <<  "</th>"
            <<  "</tr>"
            <<  "</thead>"
            <<  "<tbody>"
            <<  endl;

    int pid, amount;
    string title;
    double price, total;

    for(iter cursor = cart.begin();cursor!=cart.end();cursor++){
        pid = cursor->getPid();
        title = cursor->getTitle().toStdString();
        price = cursor->getPrice();
        amount = cursor->getAmount();
        total = price*amount;

        stream  <<  "<tr>"
                <<  "<td>"      <<  pid       <<    "</td>"
                <<  "<td>"      <<  title     <<    "</td>"
                <<  "<td>"      <<  price     <<    "</td>"
                <<  "<td>"      <<  amount
                <<  "<input type='text' value='0' id='itemAmount"  <<   pid  <<  "'>"
                <<  "<button class='orange-button' id='removeItem" <<   pid  <<  "'>"
                <<  "entfernen aus <span class='fa fa-shopping-cart'></span></button>"
                <<  "</td>"
                <<  "<td>"      <<  total     <<    "</td>"
                <<  "</tr>"
                <<  endl;
    }

    stream  <<  "</tbody>"
            <<  "</table>"  <<  endl;

    string s = stream.str();

    // Testfunnktion
    // cout << s << endl;

    QString htmlOutput = QString::fromStdString(s);
    return htmlOutput;
}


// Leert den Einkaufswagen
void sqlfunctions::clearCart(){
    int size = cart.size();
    while(size){
        cart.pop_back();
        --size;
    }
    emit cartChanged();
}

// Wenn man unzufrieden ist mit der Menge an eingekauften Waren, kann man diese ändern.
// Überladene Funktion
void sqlfunctions::changeAmount(int pid, QString mode){
        if(mode=="clear"){
            iter cursor = find(cart.begin(), cart.end(), pid);
            cart.erase(cursor);
        }
        emit cartChanged();
}

void sqlfunctions::changeAmount(int pid, int diff, QString modeQString){
    /*iter cursor = cart.begin();
    while((cursor!= cart.end())&& (cursor->getPid() != pid)){
        ++cursor;
    }*/

    string mode = modeQString.toStdString();

    testCpp();

    iter cursor = find(cart.begin(), cart.end(), pid);

    if(mode=="add"){
        cursor->setAmount(cursor->getAmount()+diff);
    }
    else if(mode=="sub"){
        cursor->setAmount(cursor->getAmount()-diff);
    }
    emit cartChanged();
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
    query.exec();
    query.next();
    double balance = query.value(0).toDouble();

    /* // Testfunktion
    QMessageBox msgBox;
    msgBox.setText(QString::number(balance)+" "+QString::number(uid));
    msgBox.exec(); */

    return balance - total;
}

// Die Bezahlfunktion
// BUG: nach abgeschlossenem Kauf funktioniert diese Funktion nicht mehr !!!
bool sqlfunctions::purchase(){

    QSqlQuery query;
    // Überprüfe, ob User eingeloogt ist.
    if(isLogin){
        int hasEnoughMoney = checkBalance();
        // Überprüfe ob User genug Guthaben hat.
        if(hasEnoughMoney >= 0){
            // Überprüfe, ob genug waren vorhanden sind.
            int diff = checkStock();
            if(diff >= 0){

                int pid, amount, insertId, stock;
                double price, cost, balance, total = 0;

                for(iter cursor = cart.begin(); cursor != cart.end(); ++cursor){
                    // Variablen cost usw. brauchen nicht ressetet zu werden, da sie jedes mal neu ausgerechnet werden.

                    pid = cursor->getPid();
                    amount = cursor->getAmount();

                    // SQL-Befehle für den Bezahlvorgang
                    // Buchungstabelle
                    // größte Buchungsnummer ermitteln
                    query.prepare("SELECT MAX(id) FROM bookings");
                    query.exec();
                    query.next();
                    insertId = query.value(0).toInt();
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
                    price = query.value(0).toDouble();
                    cost = (double)amount*price;

                    query.prepare("SELECT balance FROM users WHERE id = :uid");
                    query.bindValue(":uid", uid);
                    query.exec();
                    query.next();
                    balance = query.value(0).toDouble();
                    balance -= cost;
                    total += cost;

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
                    stock = query.value(0).toInt();
                    stock -= amount;

                    query.prepare("UPDATE products SET stock=:newStock WHERE id=:pid");
                    query.bindValue(":newStock", stock);
                    query.bindValue(":pid", pid);
                    query.exec();
                }
                // Signal: Einkauf abgeschlossen, Einkaufswagen leeren
                emit purchaseDone(cart);
                emit balanceChanged((-1)*total);

                QMessageBox msgBox;
                QString thankYou ="<p><b> Vielen Dank für Ihren Einkauf! Sie kauften: </b><br></p>";
                msgBox.setText(thankYou+showCart());
                msgBox.exec();

                clearCart();
                return true;
            }
            else{
                // Ausgabe falls nicht genug Waren zur Verfügung stehen.
                // Füge noch die Ausgabe der (tlw.) nicht lieferbare Waren und deren vorhandene Menge
                QMessageBox msgBox;
                msgBox.setText("Es sind nicht genug Waren vorhanden. Verringern Sie ihre Bestellung um "+QString::number(diff*(-1))+" Einheiten.");
                msgBox.exec();
                return false;
            }
        }
        else{
            // Ausgabe falls der User nicht genug Geld hat.
            // User hat nicht genug Geld, zeige Differenz an.
            QMessageBox msgBox;
            msgBox.setText("Sie haben nicht genug Guthaben. Ihnen fehlen "+QString::number(hasEnoughMoney*(-1))+" Geldeinheiten.");
            msgBox.exec();
            return false;
        }
    }
    else{
        QMessageBox msgBox;
        msgBox.setText("Loggen Sie sich bitte ein, um zu bezahlen!");
        msgBox.exec();
        return false;
    }
}

// ----------------------------------------------------------------------------------
// ---------------------------------------USERMGMT-----------------------------------
// ----------------------------------------------------------------------------------

// erfordert auf Javascript-Seite noch einiges an Arbeit!
void sqlfunctions::registerUser(QString username, QString password, QString repeatedPassword){

    if(password.toStdString() == "" || username.toStdString() == "" || repeatedPassword.toStdString() == ""){
        QMessageBox msgBox;
        msgBox.setText("Ihre Angaben sind unvollständig!");
        msgBox.exec();
        return;
    }

    if(password.toStdString() != repeatedPassword.toStdString()){
        QMessageBox msgBox;
        msgBox.setText("Die Passwörter stimmen nicht überein!");
        msgBox.exec();
        return;
    }

    // Check erfolgreich
    // Überprüfen, ob User in DB vorhanden ist
    QSqlQuery query;
    query.prepare("SELECT username FROM users WHERE username = :username");
    query.bindValue(":username", username);
    query.exec();
    query.next();
    QString dbUsername = query.value(0).toString();
    // Wenn ja, trage in DB ein und gebe Erfolgsmeldung aus.
    if(username.toStdString() != dbUsername.toStdString()){
        // Maximale Usernummer herausfinden
        query.prepare("SELECT MAX(id) FROM users");
        query.exec();
        query.next();
        int insertId = query.value(0).toInt();
        // Usernummer um 1 erhöhen
        insertId++;
        query.prepare("INSERT INTO users (id, username, password, balance, isAdmin) VALUES (:insertId, :username, :password, :balance, :isAdmin)");
        query.bindValue(":id", insertId);
        query.bindValue(":username", username);
        query.bindValue(":password", password);
        query.bindValue(":balance", 0);
        query.bindValue(":isAdmin", 0);
        query.exec();
        QMessageBox msgBox;
        msgBox.setText("Herzlichen Dank für Ihre Registrierung.");
        msgBox.exec();
    }

    // Wenn nicht, gib Fehlermeldung aus.
    else{
        QMessageBox msgBox;
        msgBox.setText("Das hat leider nicht funktioniert. Es gibt bereits einen Benutzer mit diesem Namen.");
        msgBox.exec();
    }
}

// Geld auf User-Account laden
void sqlfunctions::refillBalance(double amount){
    if(!isLogin){
        QMessageBox msgBox;
        msgBox.setText("Es ist kein User eingeloggt!");
        msgBox.exec();
        return;
    }
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

    QMessageBox msgBox;
    msgBox.setText("Ihr Guthaben wurde erfolgreich um "+QString::number(amount)+" aufgelden. Es beträgt nun insgesamt "+QString::number(balance)+".");
    msgBox.exec();

    emit balanceChanged(amount);

    return;
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

bool sqlfunctions::getLogin(){
    return isLogin;
}

bool sqlfunctions::getIsAdminLoggedIn(){
    return isAdminLoggedIn;
}

int sqlfunctions::getUid(){
    return uid;
}

QString sqlfunctions::getUsername(){
    QSqlQuery query;
    query.prepare("SELECT username FROM users WHERE id = :uid");
    query.bindValue(":uid", uid);
    query.exec();
    query.next();
    return query.value(0).toString();
}

double sqlfunctions::getBalance(){
    QSqlQuery query;
    query.prepare("SELECT balance FROM users WHERE id =:uid");
    query.bindValue(":uid", uid);
    query.exec();
    query.next();
    return query.value(0).toDouble();
}

bool sqlfunctions::login(QString username, QString password){
    // Timeout bei mehrfach falscher Eingabe
    time_t timeNow = time(0);
    if(allowedAgain > timeNow && badTries > 2){
        time_t difference = allowedAgain - timeNow;
        QMessageBox msgBox;
        msgBox.setText("Zuviele ("+QString::number(badTries)+") Fehlversuche! Sie dürfen sich in "+QString::number(difference)+" Sekunden wieder einloggen.");
        msgBox.exec();
        return false;
    }

    QSqlQuery query;
    // Prüfen, ob Username-Password-Kombination existiert
    query.prepare("SELECT username FROM users WHERE password = :password");
    query.bindValue(":password", password);
    bool accountExists = query.exec();
    query.next();
    QString receivedUsername = query.value(0).toString();
    bool credentialsMatch = false;
    if(receivedUsername.toStdString() == username.toStdString()){
        credentialsMatch = true;
    }
    else{
       badTries++;
       timeout();
    }
    // Überprüft ob Account existiert, Passwort/Benutzername-Kombination stimmt
    // und ob ein nicht leerer Benutzername eingegeben wurde.
    if(accountExists && credentialsMatch && username.toStdString()!=""){
        // Member uid für Einkauf setzen
        query.prepare("SELECT id FROM users WHERE username =:username");
        query.bindValue(":username", username);
        if(query.exec()){
            emit userLoggedIn();
            isLogin = true;
        }
        query.next();
        uid = query.value(0).toInt();

        /*// Testfunktion
        QMessageBox msgBox;
        msgBox.setText("Der eingeloggte User hat die ID "+QString::number(uid)+".");
        msgBox.exec();*/

        // Prüfen ob ein Admin eingeloggt ist
        // Setze entsprechend isAdminLoggedIn auf true bzw. false
        // Wenn ja sende Signal adminLoggedIn aus
        // Gib Admin-Rechte.

        query.prepare("SELECT isAdmin FROM users WHERE id = :uid");
        query.bindValue(":uid", uid);
        query.exec();
        query.next();
        bool userIsAdmin = query.value(0).toBool();
        if(userIsAdmin){
            emit adminLoggedIn();
            isAdminLoggedIn = true;
            /* // Testfunktion
            QMessageBox msgBox;
            msgBox.setText("User als Admin erkannt. Das Admin-Flag ist auf "+QString::number(isAdminLoggedIn)+".");
            msgBox.exec();*/
        }
        else{
            /* // Testfunktion
            QMessageBox msgBox;
            msgBox.setText("User als Kunde erkannt. Das Admin-Flag ist auf "+QString::number(isAdminLoggedIn)+".");
            msgBox.exec();
            */
        }

        return true;
    }
    else{
        QMessageBox msgBox;
        msgBox.setText("Benutzername oder Passwort inkorrekt!");
        msgBox.exec();
        return false;
    }
}

// Login-Strafe ausrechnen
void sqlfunctions::timeout(){
    allowedAgain = 30*(badTries-2) + time(0);
}

// User ausloggen
bool sqlfunctions::logout(){
    /* USER FRAGEN, OB ER SICH WIRKLICH AUSLOGGEN WILL, DA SEIN WARENKORB GELÖSCHT WIRD!
    bool areYouSure = false;
    QMessageBox msgBox;
    QAbstractButton button;
    msgBox.addButton(button, );
    msgBox.setText("Logout vom User abgebrochen.");
    msgBox.exec();

    if(!areYouSure){
        msgBox.setText("Logout vom User abgebrochen.");
        msgBox.exec();
        return false;
    }
    */

    // Auslog-Funktionalität
    isLogin = false;
    isAdminLoggedIn = false;
    uid = -1;
    clearCart();
    emit userLoggedOut();

    QMessageBox msgBox;
    msgBox.setText("Logout erfolgreich!");
    msgBox.exec();
    return true;
}

// ----------------------------------------------------------------------------------
// -----------------------------------------TEST-------------------------------------
// ----------------------------------------------------------------------------------

void sqlfunctions::testJs(){
    QMessageBox msgBox;
    msgBox.setText(listAllProducts());
    msgBox.exec();
}

void sqlfunctions::testCpp(){
    QMessageBox msgBox;
    msgBox.setText("Dieser Code wurde ausgeführt!");
    msgBox.exec();
}

void sqlfunctions::testSql(QString a, QString b, QString c){
    QMessageBox msgBox;
    msgBox.setText("Erstes Argument: "+a +"\nZweites Argument: "+b + "\nDrittes Argument: "+c);
    msgBox.exec();
}

// ----------------------------------------------------------------------------------
// ---------------------------------------SIGNALE------------------------------------
// ----------------------------------------------------------------------------------
/*
void sqlfunctions::cartChanged(){
    return;
}
*/
