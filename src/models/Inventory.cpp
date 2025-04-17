#include <iostream>
#include "include/Inventory.h"

typedef std::string str;

Inventory::Inventory(str productCode, str productName, str operationType, int productStock)
{
    this->productCode = productCode;
    this->productName = productName;
    this->operationType = operationType;
    this->productStock = productStock;
}

str Inventory::getInventoryProductCode()
{
    return this->productCode;
}

str Inventory::getInventoryProductName()
{
    return this->productName;
}

str Inventory::getInventoryOperationType()
{
    return this->operationType;
}

int Inventory::getInventoryProductStock()
{
    return this->productStock;
}