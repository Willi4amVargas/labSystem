#pragma once
#include <iostream>
#include <vector>
#include <sqlite3.h>
#include "include/Products.h"
#include "include/Patients.h"
#include "include/Inventory.h"
#include "include/LabTests.h"
#include <list>

using std::vector;

typedef std::string str;

typedef vector<Patients> arrayPatients;
// typedef vector<Inventory> arrayInventory;
// typedef vector<LabTest> arrayLabTest;

class Laboratory
{
private:
    str labName;
    str labRif;
    str labPlace;
    arrayPatients *labPatients;
    // arrayInventory *labInventoryOperations;
    // arrayLabTest *labTests;

public:
    Products *labProducts;

    sqlite3 *db;
    Laboratory(str labName = "", str labRif = "", str labPlace = "");
    ~Laboratory();
    void getLabData();

    // OPERACIONES CON PACIENTES
    void getLabPatients();
    void createPatient();
    void updatePatient();
    void deletePatient();

    // OPERACIONES DE INVENTARIO
    void getLabInventoryOperation();
    void createLabInventoryOperation();

    // OPERACIONES CON LOS EXAMENES
    void getLabTests();
    void createLabTest();
    void updateLabTest();
    void deleteLabTest();
};