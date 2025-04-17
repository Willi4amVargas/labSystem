#pragma once
#include <iostream>

using std::string;
typedef string str;

class Products
{
private:
    // int code;
    str name;
    str description;
    int stock;

public:
    static int cantProducts;
    Products(str name, str description);
    // ~Products();
    // int getCode();
    str getName();
    str getDescription();
    int getStock();
    void updateProduct(str name, str description);
    void updateStock(int stock);
};