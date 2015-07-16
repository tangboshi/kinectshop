#ifndef SQLFUNCTIONS_H
#define SQLFUNCTIONS_H

#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <time.h>

#include <QSqlQuery>
#include <QSql>
#include <QSqlDatabase>
#include <QDebug>
#include <QMessageBox>
#include <QObject>
#include <QString>
#include <QDateTime>

#include "product.h"

class product;
using namespace std;

typedef vector<product>::iterator iter;

/**
 * @class sqlfunctions Class
 * @brief von QObject erbende Klasse
 * Diese Klasse stellt Funktionen zur Verfuegung um mit der SQL Datenbank zu kommunizieren.
 * Sie besteht aus Signalen, zb. purchaseDone(vector<product> cart) oder cartChanged(), 
 * Slots, zb. listAllProducts(), logout(), 
 * und privaten Membern.
 * 
 * Es gibt Funktionen um den Einkauf im Shop, die Accountverwaltung und den Login, die Nutzerverwaltung und die Warenverwaltung zu regeln.
 * @see purchaseDone(vector<product> cart)
 * @see cartChanged()
 * @see listAllProducts()
 * @see logout()
 */
class sqlfunctions:public QObject{
    Q_OBJECT

    public:
        sqlfunctions();

    signals:
        void        purchaseDone(vector<product> cart);

        void        balanceChanged(double amount);
        void        priceChanged(double price);
        void        marginChanged(double margin);
        void        revenueMade(double revenue);
        void        profitMade(double profit);
        void        stockChanged(double stock);
        void        cartChanged();

        void        adminLoggedIn();
        void        userLoggedIn();
        void        userLoggedOut();

    public slots:
        // SHOPFUNKTIONEN
        // Einkaufsfunktion und ihre Hilfsfunktionen
        // QString-Konzept: einfache Erweiterbarkeit der Funktionen
        product     isAlreadyInCart(int pid, int amount, double price, QString title);
        QString     listAllProducts();
        QString     listAllProducts(QString mode);
        void        addToCart(int pid, int amount, double price, QString title);
        QString     showCart();
        void        clearCart();
        void        changeAmount(int pid, QString mode);
        void        changeAmount(int pid, int diff, QString mode);
        int         checkStock();
        double      checkBalance();
        bool        purchase();
        bool        isCartEmpty();

        // Login und Accountverwaltung
        void        registerUser(QString username, QString password, QString repeatedPassword);
        bool        login(QString username, QString password);
        bool        logout();
        void        timeout();

        // ADMINISTRATORFUNKTIONEN
        // Userverwaltung
        QString     listAllUsers();
        // Accounts
        //void        createAccount();
        //void        editAccount(int id);*/
        void        empowerUser(int id);
        void        disempowerUser(int id);
        void        blockAccount(int id, int hours);
        void        blockAccountPermanently(int id);
        void        unblockAccount(int id);
        void        terminateAccount(int id);
        // Guthaben
        void        changeBalance(int id, QString mode, double number);
        // Passwort
        void        changePassword(int id, QString mode);
        void        changePassword(int id, QString mode, QString password);
        bool        userChangesPassword(int id, QString password, QString passwordRepeated);

        // Warenverwaltung
        void        deleteWareRecord(int id);
        void        createWareRecord();
        void        changeStock(int id, QString mode, double number);
        void        changeWarePrice(int id, QString mode, double number);
        void        changeMargin(int id, QString mode, double number);

        /*
        // Userstatistik
        void        showUserActivity();     // online Flag? Area User(Uhrzeit)
        void        showUserInformation();  // Geschlecht Pie, Alter Bar ...

        // Warenstatistik
        void        popularityByGender();   // Female: Bar, Male: Bar
        void        popularityByRevenue();  // Produkte: Bar
        void        popularityByProfit();   // Produkte: Bar
        */

        // SONSTIGE FUNKTIONEN
        // Getter-Funktionen
        double      getCurrentCartValue();
        bool        getLogin();
        bool        getIsAdminLoggedIn();
        int         getUid();
        QString     getUsername();
        double      getBalance();

        // Testfunktionen
        void        testJs();
        void        testCpp();
        void        testCpp2();
        void        testSql(QString a, QString b, QString c);

    private:
        // Accountmanagement --- des eingeloggten Users
        vector<product>     cart;				///< der Einkaufswagen - gespeichert in einem Vektor
        bool                isLogin;			///< true (Nutzer angemeldet), false (kein Nutzer angemeldet)
        bool                isAdminLoggedIn;	///< true (Nutzer ist Admin), false (Nutzer ist nicht Admin)
        int                 uid;				///< Nutzer ID
        QSqlDatabase        db;					///< SQL Datenbank (LfB)
        int                 badTries;			///< Anzahl Fehlversuche bei der Anmeldung
        time_t              lastTry;			///< Zeit des letzten Anmeldeversuchs
        time_t              allowedAgain;		///< Zeit des nächsten erlaubten Logins	
        double              currentCartValue;	///< Gesamtwert des Einkaufswagens
};

#endif // SQLFUNCTIONS_H
