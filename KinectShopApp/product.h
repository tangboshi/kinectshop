#ifndef PRODUCT_H
#define PRODUCT_H

#include <QString>

using namespace std;

class product{
    public:
        product();

        int getPid();
        int getAmount();
        int getPrice();
        QString getTitle();

        void setPid(int newPid);
        void setAmount(int newAmount);
        void setTitle(QString newTitle);
        void setPrice(int newPrice);

        bool operator == (const int theProduct) const;
        void operator = (const product &theProduct);

    private:
        int pid;
        int amount;
        int price;
        QString title;
};

#endif // PRODUCT_H
