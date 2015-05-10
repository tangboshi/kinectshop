#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>

using namespace std;

class product{
    public:
        product();

        int getPid();
        int getAmount();
        int getPrice();
        string getTitle();

        void setPid(int newPid);
        void setAmount(int newAmount);
        void setTitle(string newTitle);
        void setPrice(int newPrice);

    private:
        int pid;
        int amount;
        int price;
        string title;
};

#endif // PRODUCT_H
