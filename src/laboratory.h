#pragma once
#include <iostream>
#include <vector>
#include "include/Products.h"
#include "include/Patients.h"
#include "include/Inventory.h"

typedef std::string str;
typedef std::vector<Products> arrayProducts;
typedef std::vector<Patients> arrayPatients;
typedef std::vector<Inventory> arrayInventory;

class Laboratory
{
private:
    str labName;
    str labRif;
    str labPlace;
    arrayProducts *labProducts;
    arrayPatients *labPatients;
    arrayInventory *labInventoryOperations;

public:
    Laboratory(str labName, str labRif, str labPlace);
    ~Laboratory();
    void getLabData();

    // OPERACIONES CON PRODUCTOS
    void getLabProducts();
    int getLabProductsSize();
    void createProduct();
    void updateProduct();
    void deleteProduct();

    // OPERACIONES CON PACIENTES
    void getLabPatients();
    void createPatient();
    void updatePatient();
    void deletePatient();

    // OPERACIONES DE INVENTARIO
    void getLabInventoryOperation();
    void createLabInventoryOperation();
};