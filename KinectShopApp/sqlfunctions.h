#ifndef SQLFUNCTIONS_H
#define SQLFUNCTIONS_H

#include <QObject>
#include "product.h"

class sqlfunctions{
    public:
        sqlfunctions();

    public slots:
        int login(char username[], char password[]);    // return: pid, -1:Fehler
        void purchase(product products[]);
};

#endif // SQLFUNCTIONS_H
