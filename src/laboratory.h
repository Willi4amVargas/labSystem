#pragma once
#include <iostream>
#include <vector>
#include <sqlite3.h>
#include <list>
#include <limits>
#include <iomanip>

#include "include/Products.h"
#include "include/Patients.h"
#include "include/Inventory.h"
#include "include/LabTests.h"

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::left;
using std::setw;
using std::string;
using std::to_string;
using std::vector;

class Laboratory
{
private:
    string labName;
    string labRif;
    string labPlace;

public:
    Products *labProducts;
    Inventory *labInventoryOperations;
    Patients *labPatients;
    LabTests *labTests;

    sqlite3 *db;
    Laboratory(string labName = "", string labRif = "", string labPlace = "");
    ~Laboratory();
    void getLabData();

    // OPERACIONES CON LOS EXAMENES
    void getLabTests();
    void createLabTest();
    void updateLabTest();
    void deleteLabTest();
};