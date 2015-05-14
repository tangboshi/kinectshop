#ifndef SQLFUNCTIONS_H
#define SQLFUNCTIONS_H

#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

#include <QSqlQuery>
#include <QSql>
#include <QSqlDatabase>
#include <QDebug>
#include <QMessageBox>
#include <QObject>
#include <QString>

#include "product.h"

class product;
using namespace std;

typedef vector<product>::iterator iter;

class sqlfunctions:public QObject{
    Q_OBJECT

    public:
        sqlfunctions();

    signals:
        void        purchaseDone(vector<product> cart);

        // Nimmt insbesondere auch negative Werte an!
        double      balanceChanged(double amount);

        void        adminLoggedIn();
        void        userLoggedIn();
        void        userLoggedOut();

    public slots:
        // Warenmanagement
        product     isAlreadyInCart(int pid, int amount, double price, QString title);
        QString     listAllProducts();
        void        addToCart(int pid, int amount, double price, QString title);
        //QString     showCart();
        void        clearCart();
        void        changeAmount(product myProduct, string mode);
        void        changeAmount(product myProduct, int diff, string mode);
        int         checkStock();
        double      checkBalance();
        void        purchase();

        // Usermanagement
        void        registerUser(QString username, QString password, QString repeatedPassword);
        bool        login(QString username, QString password);
        bool        logout();
        void        empowerUser();
        void        disempowerUser();
        void        listAllUsers();
        void        refillBalance(double amount);

        bool        getLogin();
        bool        getIsAdminLoggedIn();
        int         getUid();
        QString     getUsername();
        double      getBalance();

        // Testfunktionen
        void        testJs();
        void        testCpp();
        void        testSql(QString a, QString b, QString c);

    private:
        // Accountmanagement
        vector<product>     cart;
        bool                isLogin;
        bool                isAdminLoggedIn;
        int                 uid;
        QSqlDatabase        db;
};

#endif // SQLFUNCTIONS_H
