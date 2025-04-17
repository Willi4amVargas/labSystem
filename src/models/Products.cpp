#include "include/Products.h"
#include <iostream>

using std::cout;

int Products::cantProducts = 0;

Products::Products(/*int code,*/ str name, str description)
{
    // this->code = code;
    this->name = name;
    this->description = description;

    Products::cantProducts += 1;
}
// Products::~Products()
// {
//     cout << "\x1b[0mProducto " << this->name /*<< " \nCodigo: " << this->code */ << "\nEliminado\n";
// }

// int Products::getCode()
// {
//     return this->code;
// }

str Products::getName()
{
    return this->name;
}

str Products::getDescription()
{
    return this->description;
}

void Products::updateProduct(str name, str description)
{
    this->name = name;
    this->description = description;
}