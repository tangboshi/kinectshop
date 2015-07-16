#include "sqlfunctions.h"

/**
 * @brief Constructor
 * Baut die Verbindung zur SQL-Datenbank her. Einige Membervariablen werden gesetzt.
 */
sqlfunctions::sqlfunctions(){
    // LfB-Datenbank
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("kinectsrv.lfb.rwth-aachen.de");
    db.setDatabaseName("kinectshop2015");
    db.setUserName("kinectshopClient");
    db.setPassword("lfb-student2015");
    db.setPort(3306);

    /* // lokale Datenbank
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("kinectshop2015");
    db.setUserName("root");
    db.setPassword("");
    db.setPort(3305);
    */

    if(!db.open()){
        QMessageBox msgBox;
        msgBox.setText("Es konnte leider keine Verbindung zur SQL-Datenbank hergestellt werden!");
        msgBox.exec();
    }

    /* // Ausgabe wenn Verbindung hergestellt wurde
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

    currentCartValue = 0;
}

// ----------------------------------------------------------------------------------
// --------------------------------------WAREZMGMT-----------------------------------
// ----------------------------------------------------------------------------------

// Zeige die im Shop verfügbaren Produkte an
/**
 * @brief Alle Produkte werden aufgelistet.
 * Ausgabe einer Tabelle aller vorhandener Produkte (ID, Name, Preis, Verfuegbarkeit(Menge), Menge im Einkaufswagen).
 * Die Daten werden aus der SQL Datenbank ausgelesen, in einen Stream geladen, dieser wird in einen String umgewandelt und dann in einem QString gespeichert und ausgegeben.
 * @return QString: Tabelle der Produkte
 */
QString sqlfunctions::listAllProducts(){
    QSqlQuery query;

    // Alles Abfragen
    query.prepare("SELECT id, title, price, stock FROM products ORDER BY products.id ASC");
    query.exec();

    int pid, stock;
    string title;
    double price;

    stringstream stream;

    stream <<   "<div class='input-prepend orange'>"
           <<   "<span class='fa fa-search'></span>"
           <<   "<input class='search' placeholder='Filter'>"
           <<   "</div>"
           <<   endl;

    // Alles darstellen
    stream  <<  "<table id='cartList' class='sortable'>"
            <<  "<thead>"
            <<  "<tr>"
            <<  "<th data-sort='number'     class='sortByPid ascending'>"      <<  "PID"    <<  "</th>"
            <<  "<th data-sort='name'       class='sortByName'>"     <<  "Produktname"   <<  "</th>"
            <<  "<th data-sort='number'     class='sortByPrice'>"    <<  "Preis"         <<  "</th>"
            <<  "<th data-sort='number'     class='sortByStock'>"    <<  "Verfügbar"     <<  "</th>"
            <<  "<th class='no-sort'>"   <<  "Menge"         <<  "</th>"
            <<  "</tr>"
            <<  "</thead>"
            <<  "<tbody class='list'>"
            <<  endl;

    while(query.next()){
        pid = query.value(0).toInt();
        title = query.value(1).toString().toStdString();
        price = query.value(2).toDouble();
        stock = query.value(3).toInt();
        stream  <<  "<tr>"
                <<  "<td class='sPid'>"     <<  pid       <<    "</td>"
                <<  "<td class='sTitle'>"   <<  title     <<    "</td>"
                <<  "<td class='sPrice'>"   <<  price     <<    "</td>"
                <<  "<td class='sStock'>"   <<  stock     <<    "</td>"
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


/**
 * @brief Alle Produkte werden aufgelistet.
 * Ausgabe einer Tabelle aller vorhandener Produkte (ID, Name, Preis, Verfuegbarkeit(Menge), Menge im Einkaufswagen).
 * Die Daten werden aus der SQL Datenbank ausgelesen, in einen Stream geladen, dieser wird in einen String umgewandelt und dann in einem QString gespeichert und ausgegeben.
 * @param mode Art der Auflistung ("  ", "checkboxes")
 * @return QString: Tabelle der Produkte
 */
QString sqlfunctions::listAllProducts(QString mode){
    QSqlQuery query;

    // Alles Abfragen
    query.prepare("SELECT id, title, price, stock, margin, revenue, profit FROM products ORDER BY products.id ASC");
    query.exec();

    int pid, stock;
    string title;
    double price, margin, revenue, profit;

    stringstream stream;

    stream <<   "<div class='input-prepend orange'>"
           <<   "<span class='fa fa-search'></span>"
           <<   "<input class='search' placeholder='Filter'>"
           <<   "</div>"
           <<   endl;

    // Alles darstellen
    stream  <<  "<table id='cartList' class='sortable'>"
            <<  "<thead>"
            <<  "<tr>"
            <<  endl;

    if(mode == "checkboxes"){
        stream  <<  "<th class='no-sort'></th>"
                <<  endl;
    }

    stream  <<  "<th data-sort='number'     class='sortByPid ascending'>"      <<  "PID"    <<  "</th>"
            <<  "<th data-sort='name'       class='sortByName'>"     <<  "Produktname"   <<  "</th>"
            <<  "<th data-sort='number'     class='sortByPrice'>"    <<  "Preis"         <<  "</th>"
            <<  "<th data-sort='number'     class='sortByStock'>"    <<  "Verfügbar"     <<  "</th>"
            <<  endl;

    if(mode == " "){
        stream  <<  "<th class='no-sort'>"   <<  "Menge"         <<  "</th>"
                <<  endl;
    }

    if(mode == "checkboxes"){
        stream  <<  "<th data-sort='number'     class='sortByMargin'>"      <<  "M"    <<  "</th>"
                <<  "<th data-sort='number'     class='sortByRevenue'>"     <<  "US"         <<  "</th>"
                <<  "<th data-sort='number'     class='sortByProfit'>"      <<  "PFT"     <<  "</th>"
                <<  endl;
    }

    stream  <<  "</tr>"
            <<  "</thead>"
            <<  "<tbody class='list'>"
            <<  endl;

    while(query.next()){
        pid = query.value(0).toInt();
        title = query.value(1).toString().toStdString();
        price = query.value(2).toDouble();
        stock = query.value(3).toInt();
        margin = query.value(4).toDouble();
        revenue = query.value(5).toDouble();
        profit = query.value(6).toDouble();

        stream  <<  "<tr>"
                <<  endl;
        if(mode == "checkboxes"){
            stream  <<  "<td>"      <<  "<input type='checkbox' name='wareSelect' id='user"
                    <<  pid         <<  "' value='"  <<     pid          <<  "'>"
                    <<  "</td>"
                    <<  endl;
        }
        stream  <<  "<td class='sPid'>"        <<  pid       <<    "</td>"
                <<  "<td class='sTitle'>"      <<  title     <<    "</td>"
                <<  "<td class='sPrice'>"      <<  price     <<    "</td>"
                <<  "<td class='sStock'>"      <<  stock     <<    "</td>"
                <<  endl;

        if(mode == " "){
            stream  <<  "<td>"      <<  "<input type='text' value='0' id='cartItemAmount"  <<   pid  <<  "'>"
                    <<  "<button class='orange-button' id='buyCartItem"                    <<   pid  <<  "'>"
                    <<  "in den <span class='fa fa-shopping-cart'></span></button>"        <<   "</td>"
                    <<  endl;
        }

        if(mode == "checkboxes"){
            stream  <<  "<td class='sMargin'>"       <<  margin       <<    "</td>"
                    <<  "<td class='sRevenue'>"      <<  revenue      <<    "</td>"
                    <<  "<td class='sProfit'>"       <<  profit       <<    "</td>"
                    <<  endl;
        }

        stream  <<  "</tr>"
                <<  endl;

        // Das funktioniert nur, weil alle ItemIds ab 1 vorkommen
        // und die SQL-Tabelle genau in dieser Reihenfolge sortiert ist.
    }

    stream  <<  "</tbody>"
            <<  "</table>"  <<  endl;

    if(mode == "checkboxes"){
        stream  <<  "<p>"
                <<  "<input type='checkbox' id='wareSelect' class='selectAll'>"
                <<  "Alle auswählen </p>"
                <<  endl;
    }


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
/**
 * @brief ein Produkt wird dem Warenkorb hinzugefuegt.
 * Ueberprueft, ob ein Produkt bereits im Warenkorb ist (Aufruf isAlreadyInCart(...)) und fuegt das Produkt dem Warenkorb hinzu.
 * @param pid Produkt ID
 * @param amount Menge
 * @param price Preis
 * @param title Name des Produkts
 * @return void
 * @see isAlreadyInCart(int pid, int amount, double price, QString title)
 */
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

    currentCartValue += amount*price;
    emit cartChanged();
}

// Überprüft, ob ein Element bereits im Warenkorb ist, falls ja erhöhe nur die Menge
// Gibt hinzuzufügendes Produkt an addToCart() zurück.
/**
 * @brief ueberprueft, ob ein Produkt bereits im Warenkorb ist.
 * Durchsucht den Warenkorb nach Produkten mit der angegeben Produkt ID. 
 * Wird ein entsprechendes Produkt gefunden, so wird die Menge um den Wert erhoeht, der schon im Einkaufswagen abgelegt war. Die restlichen Daten werden beibehalten.
 * Wird keines gefunden, so wird ein Produkt zurueckgegeben, das die uebergebenen Daten beinhaltet.
 * @param pid Produkt ID
 * @param amount Menge
 * @param price Preis
 * @param title Name des Produkts
 * @return product: Klasse, die die uebergebenen Daten (ggf. erhoehte Menge) als Member enthaelt.
 */
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

// Gibt den Inhalt des Einkaufswagens aus
/**
 * @brief Ausgabe des Inhaltes des Einkaufswagens
 * Ausgabe einer Tabelle aller Produkte (ID, Name, Preis, Menge im Einkaufswagen, Gesamtpreis).
 * Die Daten werden ueber einen Iterator aus dem Einkaufswagen(vector<product>) ausgelesen, in einen Stream geladen, dieser wird in einen String umgewandelt und dann in einem QString gespeichert und ausgegeben.
 * @return QString: Tabelle des Inhaltes des Einkaufswagens
 */
QString sqlfunctions::showCart(){

    stringstream stream;

    // Alles darstellen
    stream  <<  "<table id='productList' class='sortable'>"
            <<  "<thead>"
            <<  "<tr>"
            <<  "<th data-sort='number' class='sortByPid'>"         <<  "PID"            <<  "</th>"
            <<  "<th data-sort='name'   class='sortByName'>"       <<  "Produktname"    <<  "</th>"
            <<  "<th data-sort='number' class='sortByPrice'>"     <<  "Preis"          <<  "</th>"
            <<  "<th class='no-sort'>"                                  <<  "Bestellmenge"   <<  "</th>"
            <<  "<th data-sort='number' class='sortByTotal'>"     <<  "Gesamtpreis"    <<  "</th>"
            <<  "</tr>"
            <<  "</thead>"
            <<  "<tbody class='list'>"
            <<  endl;

    int pid, amount;
    string title;
    double price, subtotal;
    currentCartValue = 0;

    for(iter cursor = cart.begin();cursor!=cart.end();cursor++){
        pid = cursor->getPid();
        title = cursor->getTitle().toStdString();
        price = cursor->getPrice();
        amount = cursor->getAmount();
        subtotal = price*amount;
        currentCartValue += subtotal;

        stream  <<  "<tr>"
                <<  "<td class='sPid'>"         <<  pid       <<    "</td>"
                <<  "<td class='sTitle'>"       <<  title     <<    "</td>"
                <<  "<td class='sPrice'>"       <<  price     <<    "</td>"
                <<  "<td class='sAmount'>"      <<  amount
                <<  "<input type='text' value='0' id='itemAmount"  <<   pid  <<  "'>"
                <<  "<button class='orange-button' id='removeItem" <<   pid  <<  "'>"
                <<  "entfernen aus <span class='fa fa-shopping-cart'></span></button>"
                <<  "</td>"
                <<  "<td>"      <<  subtotal     <<    "</td>"
                <<  "</tr>"
                <<  endl;
    }

    stream  <<  "<tr id='total-preview' class='no-sort'>"
            <<  "<td colspan='4' style='text-align:right'>"       <<  "Gesamtpreis "      <<  "</td>"
            <<  "<td>"                                            <<  currentCartValue    <<  "</td>"
            <<  "</tr>"
            <<  "</tbody>"
            <<  "</table>"  <<  endl;

    string s = stream.str();

    // Testfunnktion
    // cout << s << endl;

    QString htmlOutput = QString::fromStdString(s);
    return htmlOutput;
}


// Leert den Einkaufswagen
/**
 * @brief Leert den Einkaufswagen
 * Der Einkaufswagen (vector<product>) wird mithilfe von pop_back() geleert.
 * @return void
 */
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
/**
 * @brief Das Produkt mit ID pid wird aus dem Warenkorb geloescht
 * Es wird das Signal cartChanged() ausgeloest.
 * @param pid Produkt ID
 * @param mode nur "clear" moeglich.
 * @return void
 * @see
 */
void sqlfunctions::changeAmount(int pid, QString mode){
        if(mode=="clear"){
            iter cursor = find(cart.begin(), cart.end(), pid);
            double value = (cursor->getAmount())*(cursor->getPrice());
            currentCartValue -= value;
            cart.erase(cursor);
        }
        emit cartChanged();
}

/**
 * @brief Das Produkt mit ID pid wird geaendert.
 * In Abhaengigkeit von mode wird eine Menge hinzugefuegt ("add") oder abgezogen ("sub").
 * Soll mehr abgezogen werden, als im Einkaufswagen ist, so wird eine Fehlermeldung ausgegeben.
 * Soll die Menge abgezogen werden, die im Einkaufswagen ist, so wird das Produkt geloescht (Aufruf changeAmount(...)). 
 *
 * Es wird das Signal cartChanged() ausgeloest.
 * @param pid Produkt ID
 * @param diff Menge, die hinzugefuegt oder abgezogen wird
 * @param modeQString "add" oder "sub"
 * @return void
 * @see changeAmount(int pid, QString mode)
 * @see 
 */
void sqlfunctions::changeAmount(int pid, int diff, QString modeQString){

    string mode = modeQString.toStdString();

    iter cursor = find(cart.begin(), cart.end(), pid);

    if(mode=="add"){
        cursor->setAmount(cursor->getAmount()+diff);
        currentCartValue += diff*(cursor->getAmount())*(cursor->getPrice());
    }
    else if(mode=="sub"){
        // guckt, ob der Kunde überhaupt soviel im Warenkorb hat
        if(cursor->getAmount() < diff){
            QMessageBox msgBox;
            msgBox.setText("Sie können maximal " + QString::number(cursor->getAmount()) + " aus dem Warenkorb entfernen.");
            msgBox.exec();
            return;
        }

        // guckt, ob nach der Entferung noch etwas von der Waren übrig ist
        // falls nein
        if(cursor->getAmount() == diff){
            changeAmount(pid, "clear");
            return;
        }
        // falls ja
        cursor->setAmount(cursor->getAmount()-diff);
        currentCartValue -= diff*(cursor->getAmount())*(cursor->getPrice());
    }
    emit cartChanged();
}

// Prüft für jede Ware im Warenkorb, ob noch genug Waren vorhanden sind
/**
 * @brief Prueft fuer jede Ware im Warenkorb, ob noch genug Waren im Shop vorhanden sind.
 * die Menge des Produkts im Einkaufswagen wird ermittelt und mit dem stock (SQL: stock) verglichen.
 * @return int: Differenz zwischen stock und Menge im Warenkorb
 */
int sqlfunctions::checkStock(){

    // Zu Offline-Testzwecken, mit uid: 0, un: off und pw:off
    if(uid == 0){
        return 0;
    }

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
/**
 * @brief Prueft, ob der Benutzer ausreichend Guthaben zum Kauf hat.
 * der Gesamtpreis des Einkaufswagens wird ermittelt und mit dem Guthaben (SQL: balance) des Benutzers verglichen.
 * @return double: Differenz zwischen balance und Gesamtpreis des Warenkorbes
 */
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

/**
 * @brief Prueft, ob der Einkaufswagen leer ist.
 * @return bool: true (der Einkaufswagen ist leer), false (es sind Produkte im Einkaufswagen)
 */
bool sqlfunctions::isCartEmpty(){
    if(cart.empty()){
        return true;
    }
    else{
        return false;
    }
}

// Die Bezahlfunktion
// zu checken: User eingeloggt, Einkaufswagen nicht leer, genug Waren vorhanden, genug Guthaben
/**
 * @brief Es wird ein Einkauf getaetigt.
 * Es wird ueberprueft, ob der Einkaufswagen leer ist (Fehlermeldung, return false) oder nicht (fortfahren).
 * Es wird ueberprueft, ob ein Benutzer angemeldet ist (fortfahren) oder nicht (Fehlermeldung, return false).
 * Es wird ueberprueft, ob der Benutzer genuegend Geld zur Verfuegung hat (fortfahren) oder nicht (Fehlermeldung (fehlende Geldeinheiten), return false). - checkBalance()
 * Es wird ueberprueft, ob genuegend Waren im Shop zur Verfuegung stehen (fortfahren) oder nicht (Fehlermeldung (zu verringerndes Produkt), return false). - checkStock()
 * 
 * Erstellt neue Buchungsnummer fuer jedes Produkt mit ID, UserID, ProductID, Menge, reduziert die Menge des Produkts im Shop um die Menge im Einkaufswagen, 
 * steigert Umsatz und Profit des jeweiligen Produktes und verringert schließlich das Guthaben des Nutzers um den Gesamtwert des Einkaufswagens. (SQL Datenbank)
 * 
 * Es werden die Signale purchaseDone(cart), balanceChanged(-(currentCartValue)), revenueMade(currentCartValue) und profitMade(currentCartValue) ausgeloest.
 * @return bool: false (Der Einkauf kann nicht getaetigt werden), true (Der Einkauf kann getaetigt werden)
 * @see checkBalance()
 * @see checkStock()
 */
bool sqlfunctions::purchase(){

    if(isCartEmpty()){
       QMessageBox msgBox;
        msgBox.setText("Der Einkaufswagen ist leer!");
        msgBox.exec();
        return false;
    }

    QSqlQuery query;
    // Überprüfe, ob User eingeloggt ist.
    if(isLogin){
        int hasEnoughMoney = checkBalance();
        // Überprüfe ob User genug Guthaben hat.
        if(hasEnoughMoney >= 0){
            // Überprüfe, ob genug waren vorhanden sind.
            int diff = checkStock();
            if(diff >= 0){

                int pid, amount, insertId, stock;
                double balance, margin, revenue, profit, price, oldRevenue, oldProfit;

                for(iter cursor = cart.begin(); cursor != cart.end(); ++cursor){

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

                    // Umsatz und Profit steigern
                    margin = cursor->getMargin();
                    price = cursor->getPrice();
                    revenue = price*amount;
                    profit = margin*amount;
                    query.prepare("SELECT revenue, profit FROM products WHERE id=:pid");
                    query.bindValue(":pid", pid);
                    query.exec();
                    query.next();
                    oldRevenue = query.value(0).toDouble();
                    oldProfit = query.value(1).toDouble();
                    query.prepare("UPDATE products SET revenue=:newRevenue, profit=:newProfit WHERE id =:pid");
                    query.bindValue(":newRevenue", revenue+oldRevenue);
                    query.bindValue(":newProfit", profit+oldProfit);
                    query.bindValue(":pid", pid);
                    query.exec();
                }

                // Geld von Useraccount abheben
                query.prepare("SELECT balance FROM users WHERE id = :uid");
                query.bindValue(":uid", uid);
                query.exec();
                query.next();
                balance = query.value(0).toDouble();
                balance -= currentCartValue;

                // Userguthaben abbuchen
                query.prepare("UPDATE users SET balance=:newBalance WHERE id =:uid");
                query.bindValue(":newBalance", balance);
                query.bindValue(":uid", uid);
                query.exec();

                // Signal: Einkauf abgeschlossen, Einkaufswagen leeren
                emit purchaseDone(cart);
                emit balanceChanged((-1)*currentCartValue);
                emit revenueMade(currentCartValue);
                emit profitMade(currentCartValue);

                QMessageBox msgBox;
                QString thankYou ="<p><b> Vielen Dank für Ihren Einkauf! Sie kauften: </b><br></p>";
                msgBox.setText(thankYou+showCart());
                msgBox.exec();

                currentCartValue = 0;
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

/**
 * @brief Ein Nutzer wird hinzugefuegt.
 * Es wird ueberprueft, ob die Angaben vollstaendig sind (fortfahren) oder nicht (Fehlermeldung return).
 * Es wird ueberprueft, ob die eingegebenen Passwoerter uebereinstimmen (fortfahren) oder nicht (Fehlermeldung return).
 * Es wird ueberprueft, ob es bereits einen Nutzer mit dem eingegebenen Namen in der Datenbank gibt (Fehlermeldung return) oder nicht (fortfahren).
 *
 * Es wird ein neuer Nutzer mit den eingegebenen Daten und einer neuen ID in der Datenbank angelegt.
 * @param username Nutzername
 * @param password gewaehltes Passwort
 * @param repeatedPassword wiederholte Eingabe des gewaehlten Passwortes
 * @return void
 */
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
        query.prepare("INSERT INTO users (id, username, password, balance, isAdmin, changePasswordMode) VALUES (:insertId, :username, :password, :balance, :isAdmin, :changePasswordMode)");
        query.bindValue(":id", insertId);
        query.bindValue(":username", username);
        query.bindValue(":password", password);
        query.bindValue(":balance", 0);
        query.bindValue(":isAdmin", 0);
        query.bindValue(":changePasswordMode", "n");
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

// gibt Tabelle mit allen Usern für Adminstration aus
/**
 * @brief Eine Tabelle mit allen Nutzern wird ausgegeben.
 * Ausgabe einer Tabelle aller Nutzer (ID, Name, Guthaben, Adminrechte?, Permanent Geblockt?).
 * Die Daten werden aus der SQL Datenbank ausgelesen, in einen Stream geladen, dieser wird in einen String umgewandelt und dann in einem QString gespeichert und ausgegeben.
 * @return QString: Tabelle aller Nutzer
 */
QString sqlfunctions::listAllUsers(){

    QSqlQuery query;
    stringstream stream;

    stream <<   "<div class='input-prepend orange'>"
           <<   "<span class='fa fa-search'></span>"
           <<   "<input class='search' placeholder='Filter'>"
           <<   "</div>"
           << endl;

    stream  <<  "<table id='users' class='sortable'>"
            <<  "<thead>"
            <<  "<tr>"
            <<  "<th class='no-sort'></th>"
            <<  "<th data-sort='number'>"          <<  "ID"                <<  "</th>"
            <<  "<th data-sort='name'>"            <<  "Benutzername"      <<  "</th>"
            //<<  "<th data-sort='name'>"            <<  "Passwort"          <<  "</th>"
            <<  "<th data-sort='number'>"          <<  "Guthaben"          <<  "</th>"
            <<  "<th data-sort='number'>"          <<  "Admin?"            <<  "</th>"
            <<  "<th data-sort='number'>"          <<  "PermBl?"            <<  "</th>"
            <<  "</tr>"
            <<  "</thead>"
            <<  "<tbody class='list'>"
            << endl;

    query.prepare("SELECT id,username,balance,isAdmin,isBlockedPermanently FROM users ORDER BY users.id ASC");
    query.exec();

    int id, isAdmin, isBlockedPermanently;
    double balance;
    string username;
    // string password;

    while(query.next()){
        id          =   query.value(0).toInt();
        username    =   query.value(1).toString().toStdString();
        //password    =   query.value(2).toString().toStdString();
        balance     =   query.value(2).toDouble();
        isAdmin     =   query.value(3).toInt();
        isBlockedPermanently = query.value(4).toInt();

        stream  <<  "<tr>"
                <<  "<td>"      <<  "<input type='checkbox' name='userSelect' id='user"
                <<  id          <<  "' value='"  <<     id          <<  "'>"
                <<  "</td>"
                <<  "<td class='sId'>"                      <<  id                          <<    "</td>"
                <<  "<td class='sUsername'>"                <<  username                    <<    "</td>"
                //<<  "<td class='sPassword'>"              <<  password                    <<    "</td>"
                <<  "<td  class='sBalance'>"                <<  balance                     <<    "</td>"
                <<  "<td  class='sIsAdmin'>"                <<  isAdmin                     <<    "</td>"
                <<  "<td  class='sIsBlockedPermanently'>"   <<  isBlockedPermanently        <<    "</td>"
                <<  "</tr>"
                <<  endl;
    }

    stream  <<   "</tbody>"
            <<   "</table>"
            <<   "<p>"
            <<   "<input type='checkbox' id='userSelect' class='selectAll'>"
            <<   "Alle auswählen </p>"
            <<   endl;

    string s = stream.str();

    // Testfunktion
    // cout    <<  s   <<  endl;

    QString htmlOutput = QString::fromStdString(s);
    return htmlOutput;
}

// Gibt User Admin-Privillegien
// Auswirkung: Zeige Admin-Menüpunkte und Optionen in App
/**
 * @brief Einem Nutzer werden Admin-Privilegien zugeteilt.
 * Der Nutzer kann nun Admin-Menuepunkte und Optionen der App nutzen.
 * @param id Nutzer ID
 * @return void
 */
void sqlfunctions::empowerUser(int id){
    QSqlQuery query;
    query.prepare("UPDATE users SET isAdmin=1 WHERE id=:uid");
    query.bindValue(":uid", id);
    query.exec();
}

// Nimmt User Admin-Privillegien
// Auswirkung: Verstecke Admin-Menüpunkte und Optionen in App
/**
 * @brief Einem Nutzer werden Admin-Privilegien genommen.
 * Es wird ueberprueft, ob der gewaehlte Nutzer "Superadmin" ist (Fehlermeldung, return) oder nicht (fortfahren).
 *
 * Der Nutzer kann Admin-Menuepunkte und Optionen der App nicht mehr nutzen.
 * @param id Nutzer ID
 * @return void
 */
void sqlfunctions::disempowerUser(int id){
    QSqlQuery query;
    query.prepare("SELECT isSuperAdmin FROM users WHERE id = :uid");
    query.bindValue(":uid", id);
    query.exec();
    query.next();
    bool isSuperAdmin = query.value(0).toBool();

    if(isSuperAdmin){
        QMessageBox msgBox;
        msgBox.setText("Dieser User ist ein Super-Admin. Diese sind immun.");
        return;
    }
    else{
        QMessageBox msgBox;
        msgBox.setText("Yolo, Biatch!");
    }

    query.prepare("UPDATE users SET isAdmin=0 WHERE id=:uid");
    query.bindValue(":uid", id);
    query.exec();
}

// blockt Account temporär
/**
 * @brief Ein Nutzer wird temporär blockiert.
 * Es wird das Datum berechnet, wann sich der Nutzer wieder anmelden kann.
 * Dieses Datum wird in der SQL Datenbank gespeichert.
 * @param id Nutzer ID
 * @param hours so viele Stunden soll der Nutzer gesperrt werden
 * @return void
 */
void sqlfunctions::blockAccount(int id, int hours){
    QDateTime blockedUntil = QDateTime::currentDateTime().addSecs(3600*hours);
    QSqlQuery query;
    query.prepare("UPDATE users SET blockedUntil = :blockedUntil WHERE id = :id");
    query.bindValue(":id", id);
    query.bindValue(":blockedUntil", blockedUntil);
    query.exec();

    QMessageBox msgBox;
    msgBox.setText("Der Account mit der id "+QString::number(id)+" wurde für "+ QString::number(hours) +" Stunden geblockt.");
    msgBox.exec();
}

// blockt Account dauerhaft
/**
 * @brief Ein Nutzer wird dauerhaft blockiert.
 * In der SQL Datenbank wird gespeichert, dass der Nutzer permanent blockiert ist.
 * @param id Nutzer ID
 * @return void
 */
void sqlfunctions::blockAccountPermanently(int id){
    QSqlQuery query;
    query.prepare("UPDATE users SET isBlockedPermanently = 1 WHERE id = :id");
    query.bindValue(":id", id);
    query.exec();

    QMessageBox msgBox;
    msgBox.setText("Der Account mit der id "+QString::number(id)+" wurde permanent geblockt.");
    msgBox.exec();
}

// entblockt Account
/**
 * @brief Ein blockierter Nutzer wird wieder freigeschaltet.
 * Das heutige Datum ueberschreibt den Eintrag in der SQL Datenbank, wann sich der Nutzer wieder anmelden darf.
 * Es wird zusaetzlich in der Datenbank gespeichert, dass der Nutzer nicht permanent blockiert ist.
 * @param id Nutzer ID
 * @return void
 */
void sqlfunctions::unblockAccount(int id){
    QDateTime blockedUntil = QDateTime::currentDateTime();
    // temporären Block entfernen
    QSqlQuery query;
    query.prepare("UPDATE users SET blockedUntil = :blockedUntil WHERE id = :id");
    query.bindValue(":id", id);
    query.bindValue(":blockedUntil", blockedUntil);
    query.exec();

    // dauerhaften Block entfernen
    query.prepare("UPDATE users SET isBlockedPermanently = 0 WHERE id = :id");
    query.bindValue(":id", id);
    query.exec();

    QMessageBox msgBox;
    msgBox.setText("Der Account mit der id "+QString::number(id)+" wurde wieder freigeschaltet.");
    msgBox.exec();
}

// löscht Account
/**
 * @brief Ein Nutzer wird geloescht.
 * Es wird abgefragt, ob der Account wirklich geloescht werden soll.
 * Der Button "Yes" wird gedrueckt: Der Nutzer wird aus der SQL Datenbank geloescht.
 * Der Button "No" wird gedrueckt: Es wird nichts ausgefuehrt.
 * @param id Nutzer ID
 * @return void
 */
void sqlfunctions::terminateAccount(int id){
    QMessageBox confirm;
    QMessageBox informative;
    confirm.setText("Achtung Account-Terminierung!");
    confirm.setInformativeText("Wollen Sie diese(n) Account(s) wirklick terminieren? Diese Aktion ist endgültig.");
    confirm.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    confirm.setDefaultButton(QMessageBox::No);

    int answer = confirm.exec();
    if(answer == QMessageBox::Yes){
        informative.setText("Account(s) wurde(n) terminiert!");
        informative.exec();
        QSqlQuery query;
        query.prepare("DELETE FROM users WHERE id=:uid");
        query.bindValue(":uid", id);
        query.exec();
    }
    else{
        informative.setText("Nichts ausgeführt.");
        informative.exec();
    }
}

// ändert Guthaben eines Accounts
/**
 * @brief Das Guthaben eines Nutzers wird aktualisiert.
 * Es wird abgefragt, ob ein Nutzer eingeloggt ist (fortfahren) oder nicht (Fehlermeldung, return)
 * 
 * Das Guthaben des Nutzers wird in Abhaengigkeit von mode ("add" erhoehen um, "set" setzen auf, "scale" skalieren um einen Wert number) veraendert und in der Datenbank aktualisiert.
 *
 * Das Signal balanceChanged(oldBalance - balance) wird ausgeloest.
 * @param id Nutzer ID
 * @param mode "add", "set" oder "scale" moeglich
 * @param number Wert um den das Guthaben veraendert wird
 * @return void
 */
void sqlfunctions::changeBalance(int id, QString mode, double number){

    if(!isLogin){
        QMessageBox msgBox;
        msgBox.setText("Es ist kein User eingeloggt!");
        msgBox.exec();
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT balance FROM users WHERE id=:id");
    query.bindValue(":id", id);
    query.exec();
    query.next();
    double balance = query.value(0).toDouble();
    int oldBalance = balance;

    QMessageBox msgBox;
    if(mode == "add"){
        balance += number;
        msgBox.setText("Ihr Guthaben wurde erfolgreich um "+QString::number(number)+" aufgelden. Es beträgt nun insgesamt "+QString::number(balance)+".");
        //msgBox.exec();
    }
    else if(mode == "set"){
        balance = number;
        msgBox.setText("Ihr Guthaben wurde erfolgreich auf"+QString::number(number)+" aufgeladen.");
        //msgBox.exec();
    }
    else if(mode == "scale"){
        balance *= number;
        msgBox.setText("Ihr Guthaben wurde erfolgreich um den Faktor"+QString::number(number)+" skaliert. Es beträgt nun insgesamt "+QString::number(balance)+".");
        //msgBox.exec();

    }
    query.prepare("UPDATE users SET balance=:balance WHERE id=:id");
    query.bindValue(":balance", balance);
    query.bindValue(":id", id);
    query.exec();

    emit balanceChanged(oldBalance - balance);
}

// Passwort ändern
/**
 * @brief Der changePasswordMode eines Nutzers wird geaendert.
 * der neue Modus wird in der Datenbank gespeichert ("r" fuer "request", "f" fue "force").
 * @param id Nutzer ID
 * @param mode "request" oder "force" moeglich
 * @return void
 */
void sqlfunctions::changePassword(int id, QString mode){
    QString newMode;
    QSqlQuery query;
    if(mode == "request"){
        newMode = "r";
    }
    if(mode == "force"){
        newMode = "f";
    }
    query.prepare("UPDATE users SET changePasswordMode=:mode WHERE id=:id");
    query.bindValue(":id", id);
    query.bindValue(":mode", newMode);
    query.exec();
}

/**
 * @brief Das Passwort eines Nutzers wird geaendert.
 * Das neue Passwort wird in der Datenbank gespeichert. 
 * der changePasswordMode wird zu "n" gesetzt.
 * @param id Nutzer ID
 * @param mode "set" moeglich
 * @param password neues Passwort
 * @return void
 */
void sqlfunctions::changePassword(int id, QString mode, QString password){
    QString newMode;
    QSqlQuery query;

    if(mode == "set"){
        query.prepare("UPDATE users SET password=:password WHERE id=:id");
        query.bindValue(":id", id);
        query.bindValue(":password", password);
        query.exec();
        newMode = "n";
    }

    query.prepare("UPDATE users SET changePasswordMode=:mode WHERE id=:id");
    query.bindValue(":id", id);
    query.bindValue(":mode", newMode);
    query.exec();
}

/**
 * @brief Ein Nutzer aendert sein eigenes Passwort.
 * Es wird ueberprueft, ob die eingegebenen Passwoerter uebereinstimmen (fortfahren) oder nicht (Fehlermeldung, return false)
 * 
 * Das neue Passwort wird in der Datenbank gespeichert.
 * @param id Nutzer ID
 * @param password neues Passwort
 * @param passwordRepeated Wiederholung des neuen Passworts
 * @return bool: false (die beiden Passwoerter stimmen nicht ueberein), true (das Passwort konnte geaendert werden)
 */
bool sqlfunctions::userChangesPassword(int id, QString password, QString passwordRepeated){
    if(password != passwordRepeated){
        QMessageBox msgBox;
        msgBox.setText("Die eingegebenen Passwörter stimmen nicht überein.");
        return false;
    }
    else{
        QSqlQuery query;
        query.prepare("UPDATE users SET password = :password WHERE id = :id");
        query.bindValue(":password", password);
        query.bindValue(":id", id);
        query.exec();
        return true;
    }
}

/**
 * @brief Ware wird aus dem Shop geloescht
 * Es wird abgefragt, ob die Waren wirklich geloescht werden sollen.
 * Der Button Yes wird gedrueckt: das Produkt wird aus der Datenbank geloescht.
 * Der Button No wird gedrueckt: das Produkt wird nicht geloescht.
 * @param id Produkt ID
 * @return void
 */
void sqlfunctions::deleteWareRecord(int id){
    QMessageBox confirm;
    QMessageBox informative;

    confirm.setText("Achtung Waren-Löschung!");
    confirm.setInformativeText("Wollen Sie diese Waren(n) wirklick löschen? Diese Aktion ist endgültig.");
    confirm.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    confirm.setDefaultButton(QMessageBox::No);

    int answer = confirm.exec();
    if(answer == QMessageBox::Yes){
        informative.setText("Produkt(e) wurden erfolgreich gelöscht.");
        informative.exec();
        QSqlQuery query;
        query.prepare("DELETE FROM products WHERE id=:id");
        query.bindValue(":id", id);
        query.exec();
    }
    else if(answer == QMessageBox::No){
        informative.setText("Aktion vom Benutzer abgebrochen.");
        informative.exec();
    }
}

void sqlfunctions::createWareRecord(){
}

/**
 * @brief die Menge einer Ware im Shop wird veraendert.
 * Die Menge einer Ware wird in Abhaengigkeit von mode ("add" erhoeht um, "set" gesetzt durch, "scale" skaliert um einen Wert number) veraendert.
 *
 * Der neue stock des Produkts wird in der Datenbank gespeichert.
 *
 * Das Signal stockChanged(oldStock - stock) wird ausgeloest.
 * @param id Produkt ID
 * @param mode moeglich "add", "set", "scale"
 * @param numer Wert um den der stock veraendert wird
 * @return void
 */
void sqlfunctions::changeStock(int id, QString mode, double number){
    QSqlQuery query;
    query.prepare("SELECT stock, title FROM products WHERE id=:id");
    query.bindValue(":id", id);
    query.exec();
    query.next();
    int stock = query.value(0).toInt();
    int oldStock = stock;
    QString title = query.value(1).toString();

    QMessageBox msgBox;
    if(mode == "add"){
        stock += number;
        msgBox.setText("Die Menge von "+ title +" wurde um "+QString::number(number)+" erhöht. Es beträgt nun insgesamt "+QString::number(stock)+".");
        msgBox.exec();
    }
    else if(mode == "set"){
        stock = number;
        msgBox.setText("Die Menge von "+ title +" wurde auf "+QString::number(number)+" gesetzt.");
        msgBox.exec();
    }
    else if(mode == "scale"){
        stock *= number;
        msgBox.setText("Die Menge von"+ title +" wurde um den Faktor "+ QString::number(number)+" skaliert. Er beträgt nun insgesamt "+QString::number(stock)+".");
        msgBox.exec();    
    }

    query.prepare("UPDATE products SET stock=:stock WHERE id=:pid");
    query.bindValue(":stock", stock);
    query.bindValue(":pid", id);
    query.exec();
    emit stockChanged(oldStock - stock);
}

/**
 * @brief der Preis einer Ware wird veraendert.
 * Der Preis einer Ware wird in Abhaengigkeit von mode ("add" erhoeht um, "set" gesetzt durch, "scale" skaliert um einen Wert number) veraendert.
 *
 * Der neue Preis des Produkts wird in der Datenbank gespeichert.
 *
 * Das Signal priceChanged(oldPrice - price) wird ausgeloest.
 * @param id Produkt ID
 * @param mode moeglich "add", "set", "scale"
 * @return void
 */
void sqlfunctions::changeWarePrice(int id, QString mode, double number){
    QSqlQuery query;
    query.prepare("SELECT price, title FROM products WHERE id=:id");
    query.bindValue(":id", id);
    query.exec();
    query.next();
    double price = query.value(0).toDouble();
    double oldPrice = price;
    QString title = query.value(1).toString();

    QMessageBox msgBox;
    if(mode == "add"){
        price += number;
        msgBox.setText("Der Preis von "+ title +" wurde um "+QString::number(number)+" erhöht. Er beträgt nun insgesamt "+QString::number(price)+".");
        msgBox.exec();
    }
    else if(mode == "set"){
        price= number;
        msgBox.setText("Der Preis von "+ title +" wurde auf "+QString::number(number)+" gesetzt.");
        msgBox.exec();
    }
    else if(mode == "scale"){
        price *= number;
        msgBox.setText("Der Preis von"+ title +" wurde um den Faktor "+ QString::number(number)+" skaliert. Er beträgt nun insgesamt "+QString::number(price)+".");
        msgBox.exec();
    }

    query.prepare("UPDATE products SET price=:price WHERE id=:pid");
    query.bindValue(":price", price);
    query.bindValue(":pid", id);
    query.exec();
    emit priceChanged(oldPrice - price);
}

/**
 * @brief die Gewinnspanne einer Ware wird veraendert.
 * Die Gewinnspanne einer Ware wird in Abhaengigkeit von mode ("add" erhoeht um, "set" gesetzt durch, "scale" skaliert um einen Wert number) veraendert.
 *
 * Die neue Gewinnspanne des Produkts wird in der Datenbank gespeichert.
 *
 * Das Signal marginChanged(oldmargin - margin) wird ausgeloest.
 * @param id Produkt ID
 * @param mode moeglich "add", "set", "scale"
 * @return void
 */
void sqlfunctions::changeMargin(int id, QString mode, double number){
    QSqlQuery query;
    query.prepare("SELECT margin, title FROM products WHERE id=:id");
    query.bindValue(":id", id);
    query.exec();
    query.next();
    double margin = query.value(0).toDouble();
    double oldmargin = margin;
    QString title = query.value(1).toString();

    QMessageBox msgBox;
    if(mode == "add"){
        margin += number;
        msgBox.setText("Die Gewinnspanne von "+ title +" wurde um "+QString::number(number)+" erhöht. Sie beträgt nun insgesamt "+QString::number(margin)+".");
        msgBox.exec();
    }
    else if(mode == "set"){
        margin= number;
        msgBox.setText("Die Gewinnspanne von "+ title +" wurde auf "+QString::number(number)+" gesetzt.");
        msgBox.exec();
    }
    else if(mode == "scale"){
        margin *= number;
        msgBox.setText("Die Gewinnspanne von"+ title +" wurde um den Faktor "+ QString::number(number)+" skaliert. Sie beträgt nun insgesamt "+QString::number(margin)+".");
        msgBox.exec();
    }

    query.prepare("UPDATE products SET margin=:margin WHERE id=:pid");
    query.bindValue(":margin", margin);
    query.bindValue(":pid", id);
    query.exec();
    emit marginChanged(oldmargin - margin);
}

/**
 * @brief getter
 * @return double Gesamtwert des Einkaufswagens
 */
double sqlfunctions::getCurrentCartValue(){
    return currentCartValue;
}

/**
 * @brief getter
 * @return bool true(ein Nutzer ist eingeloggt), false (es ist niemand eingeloggt)
 */
bool sqlfunctions::getLogin(){
    return isLogin;
}

/**
 * @brief getter
 * @return bool true(ein Admin ist eingeloggt), false (es ist kein Admin eingeloggt)
 */
bool sqlfunctions::getIsAdminLoggedIn(){
    return isAdminLoggedIn;
}

/**
 * @brief getter
 * @return int Nutzer ID
 */
int sqlfunctions::getUid(){
    return uid;
}

/**
 * @brief getter
 * @return QString Nutzer Name
 */
QString sqlfunctions::getUsername(){
    QSqlQuery query;
    query.prepare("SELECT username FROM users WHERE id = :uid");
    query.bindValue(":uid", uid);
    query.exec();
    query.next();
    return query.value(0).toString();
}

/**
 * @brief getter
 * @return double Kontostand des Nutzers
 */
double sqlfunctions::getBalance(){
    QSqlQuery query;
    query.prepare("SELECT balance FROM users WHERE id =:uid");
    query.bindValue(":uid", uid);
    query.exec();
    query.next();
    return query.value(0).toDouble();
}

/**
 * @brief Ein Nutzer wird angemeldet.
 * Es gibt einen geheimen Offline-Testaccount (falls username = "off" und password = "off")
 *
 * Falls zu oft (3x) ein falscher Nutzername / ein falsches Passwort eingegeben wurde, kann man sich in einer bestimmten Zeit erst wieder einloggen.
 *
 * Es wird ueberprueft, ob der Nutzer permanent oder temporär blockiert ist (Fehlermeldung, return false).
 * Es wird ueberprueft, ob es in der Datenbank einen Nutzer mit Namen username gibt (fortfahren) oder nicht (Aufruf timeout()).
 *
 * Es wird ueberprueft, ob es in der Datenbank einen Nutzer (username) mit dem Passwort (password) gibt und username nichtleer ist (fortfahren) oder nicht (Fehlermeldung, return false).
 * In Abhaengigkeit von changePasswordMode aus der Datenbank wird der Benutzer aufgerufen sein Passwort zu aendern("f", "r"), oder nicht("n").
 * Falls es einen Nutzer mit dem Nutzernamen gibt wird isLogin=true gesetzt und das Signal userLoggedIn() ausgeloest.
 * Falls der Nutzer ein Admin ist, wird isAdminLoggedIn=true gesetzt und das Signal adminLoggedIn() ausgeloest.
 * return = true.
 * @param username Nutzername
 * @param password Passwort
 * @return bool: true (Ein Nutzer konnte angemeldet werden), false (Nutzername und Passwort sind nicht in Datenbank gespeichert)
 * @see timeout()
 */
bool sqlfunctions::login(QString username, QString password){

    // Timeout bei mehrfach falscher Eingabe
    time_t timeNow = time(0);

    // Zu Offline-Testzwecken!!
    if(username.toStdString() == "off" && password.toStdString() == "off"){
        isLogin = true;
        isAdminLoggedIn = true;
        uid = 0;
        emit userLoggedIn();
        emit adminLoggedIn();
        testCpp();
        return true;
    }

    if(allowedAgain > timeNow && badTries > 2){
        time_t difference = allowedAgain - timeNow;
        QMessageBox msgBox;
        msgBox.setText("Zuviele ("+QString::number(badTries)+") Fehlversuche! Sie dürfen sich in "+QString::number(difference)+" Sekunden wieder einloggen.");
        msgBox.exec();
        return false;
    }

    QSqlQuery query;
    // Prüfen, ob Username-Password-Kombination existiert
    query.prepare("SELECT username, isBlockedPermanently, blockedUntil FROM users WHERE password = :password");
    query.bindValue(":password", password);
    bool accountExists = query.exec();
    query.next();
    QString receivedUsername = query.value(0).toString();
    bool isBlockedPermanently = query.value(1).toBool();
    QDateTime blockedUntil = query.value(2).toDateTime();
    if(isBlockedPermanently){
        QMessageBox msgBox;
        msgBox.setText("Ihr Account wurde wegen Verstoßes gegen die AGB dauerhaft geblockt. Falls Sie hier einen Fehler vermuten, kontaktieren Sie bitte den Support!");
        msgBox.exec();
        return false;
    }
    if(blockedUntil >= QDateTime::currentDateTime()){
        QMessageBox msgBox;
        msgBox.setText("Ihr Account ist temporär gebannt. Sie können sich zu folgender Zeit wieder einloggen: "+blockedUntil.toString()+" !");
        msgBox.exec();
        return false;
    }
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
        query.prepare("SELECT changePasswordMode FROM users WHERE username = :username");
        query.bindValue(":username", username);
        query.exec();
        query.next();
        QString changePasswordMode = query.value(0).toString();
        QMessageBox msgBox;

        // Testfunktion
        // msgBox.setText(changePasswordMode);
        // msgBox.exec();

        if(changePasswordMode.toStdString() != "n"){
            if(changePasswordMode.toStdString() == "f"){
                msgBox.setText("Ändern Sie umgehend Ihr Passwort!");
                msgBox.exec();
            }
            else if(changePasswordMode.toStdString() == "r"){
                msgBox.setText("Ändern Sie bitte Ihr Passwort!");
                msgBox.exec();
            }
        }

        // Member uid für Einkauf setzen
        query.prepare("SELECT id FROM users WHERE username =:username");
        query.bindValue(":username", username);
        if(query.exec()){         
            isLogin = true;
        }
        query.next();
        uid = query.value(0).toInt();

        if(isLogin){
            emit userLoggedIn();
        }

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
/**
 * @brief Login-Strafe ausrechnen
 * allowedAgain wird auf eine Zeit abhaengig von badTries gesetzt.
 * @return void
 */
void sqlfunctions::timeout(){
    allowedAgain = 30*(badTries-2) + time(0);
}

// User ausloggen
/**
 * @brief ein Nutzer wird abgemeldet.
 * Der Nutzer wird abgemeldet.
 * Der Einkaufswagen wird geloescht (Aufruf clearCart()) und das Signal userLoggedOut() wird ausgeloest.
 * @return bool: true (Logout erfolgreich)
 * @see clearCart()
 */
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

/**
 * @brief Testfunktion
 * Test zu listAllProducts()
 * @return void
 * @see listAllProducts()
 */
void sqlfunctions::testJs(){
    QMessageBox msgBox;
    msgBox.setText(listAllProducts());
    msgBox.exec();
}

/**
 * @brief Testfunktion
 * Ausgabe einer Messagebox
 * @return void
 */
void sqlfunctions::testCpp(){
    QMessageBox msgBox;
    msgBox.setText("Dieser Code wurde ausgeführt!");
    msgBox.exec();
}

/**
 * @brief Testfunktion
 * Ausgabe einer Messagebox.
 * @return void
 */
void sqlfunctions::testCpp2(){
    QMessageBox msgBox;
    msgBox.setText("Dieser andere Code wurde ausgeführt!");
    msgBox.exec();
}

/**
 * @brief Testfunktion
 * Ausgabe dreier Argumente in einer Messagebox.
 * @return void
 */
void sqlfunctions::testSql(QString a, QString b, QString c){
    QMessageBox msgBox;
    msgBox.setText("Erstes Argument: "+a +"\nZweites Argument: "+b + "\nDrittes Argument: "+c);
    msgBox.exec();
}
