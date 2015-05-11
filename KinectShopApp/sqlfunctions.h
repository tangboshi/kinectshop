#ifndef SQLFUNCTIONS_H
#define SQLFUNCTIONS_H

#include <iostream>
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
        void        adminLoggedIn();

    public slots:
        // Warenmanagement
        product     isAlreadyInCart(product myProduct);
        void        listAllProducts();
        void        addToCart(product myProduct);
        void        showCart();
        void        clearCart();
        void        changeAmount(product myProduct, string mode);
        void        changeAmount(product myProduct, int diff, string mode);
        int         checkStock();
        double      checkBalance();
        void        purchase();

        // Usermanagement
        void        registerUser(QString username, QString password);
        void        login(QString username, QString password);
        void        empowerUser();
        void        disempowerUser();
        void        listAllUsers();
        void        refillBalance(int amount);

        // Testfunktion
        void        testJs();

    private:
        // Accountmanagement
        vector<product>     cart;
        bool                isLogin;
        bool                isAdminLoggedIn;
        int                 uid;
        QSqlDatabase        db;
};

#endif // SQLFUNCTIONS_H
