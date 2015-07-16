#ifndef PRODUCT_H
#define PRODUCT_H

#include <QString>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

using namespace std;

/**
 * @class product Class
 * @brief Nicht erbende Klasse
 * Diese Klasse stellt Funktionen zur Verfuegung um Produkte zu verwalten.
 * Sie besteht aus Funktionen und privaten Membern.
 */
class product{
    public:
        product();

        int getPid();
        int getAmount();
        double getPrice();
        QString getTitle();
        double getMargin();
        double getRevenue();
        double getProfit();

        void setPid(int newPid);
        void setAmount(int newAmount);
        void setTitle(QString newTitle);
        void setPrice(double newPrice);
        void setMargin(double newMargin);
        // setRevenue() und setProfit() werden nicht benötigt

        bool operator == (const int theProduct) const;
        void operator = (const product &theProduct);

    private:
        int pid;		///< Produkt ID
        int amount;		///< Menge 
        double price;	///< Preis	
        QString title;	///< Name
};

#endif // PRODUCT_H
