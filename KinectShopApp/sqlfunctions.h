#ifndef SQLFUNCTIONS_H
#define SQLFUNCTIONS_H

#include <iostream>
#include <vector>
#include <algorithm>

#include <QSql>
#include <QSqlDatabase>
#include <QDebug>
#include <QMessageBox>
#include <QObject>
#include <string>

#include "product.h"

using namespace std;

typedef vector<product>::iterator iter;

class sqlfunctions{ 
    Q_OBJECT

    public:
        sqlfunctions();

    signals:
        void        purchaseDone(vector<product> cart);
        void        adminLoggedIn();

    public slots:
        product     isAlreadyInCart(product myProduct);
        void        addToCart(product myProduct);
        void        removeFromCart(product myProduct);
        void        showCart();
        void        clearCart();
        void        changeAmount(product myProduct, int newAmount);
        bool        checkStock();
        int         checkBalance();
        void        purchase();
        void        registerUser(string username, string password);
        void        login(string username, string password);
        void        empowerUser();

    private:
        vector<product>     cart;
        bool                isLogin;
        bool                isAdminLoggedIn;
        int                 uid;
};

#endif // SQLFUNCTIONS_H
