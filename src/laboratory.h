#pragma once
#include <iostream>
#include <vector>
#include "include/Products.h"

typedef std::string str;
typedef std::vector<Products> arrayProducts;

class Laboratory
{
private:
    str labName;
    str labRif;
    str labPlace;
    arrayProducts *labProducts;

public:
    Laboratory(str labName, str labRif, str labPlace);
    ~Laboratory();
    void getLabData();
    void getLabProducts();
    void createProduct();
    void updateProduct();
    void deleteProduct();
};