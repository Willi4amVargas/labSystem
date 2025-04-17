#pragma once
#include <iostream>

typedef std::string str;

class Inventory
{
private:
    str productCode;
    str productName;
    str operationType;
    int productStock;

public:
    Inventory(str productCode, str productName, str operationType, int productStock);
    str getInventoryProductCode();
    str getInventoryProductName();
    str getInventoryOperationType();
    int getInventoryProductStock();
};