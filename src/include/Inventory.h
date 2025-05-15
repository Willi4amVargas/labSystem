#pragma once
#include <iostream>
#include <sqlite3.h>
#include <vector>
#include "include/Products.h"
#include <iomanip>

using std::cin;
using std::cout;
using std::endl;
using std::left;
using std::right;
using std::setw;
using std::string;
using std::to_string;
using std::vector;

struct InventoryOperation
{
    int id;
    string product_code;
    string product_name;
    int operation_code;
    int product_stock;
};

class Inventory
{
private:
    sqlite3 *db;

    // Clase que contiene las operaciones con los productos
    // para poder actualizar el stock o obtenerlo desde la misma clase
    Products *productsClass;
    // OPERACIONES CON LA BASE DE DATOS
    vector<InventoryOperation> getInventoryOperations();
    string getOperationType(int code);
    int createInventoryOperation(int operationType, int productCode, int productStock);

public:
    Inventory(sqlite3 *db, Products *productsClass);

    // FUNCIONES DE MENU/VISTA
    void menu();
    void getInventoryOperationsMenu();
    void createInventoryOperationMenu();
};