#pragma once
#include <iostream>
#include <vector>
#include <utility>
#include <iomanip>
#include <sqlite3.h>
#include "include/Products.h"
#include "include/Patients.h"

using std::cin;
using std::cout;
using std::left;
using std::pair;
using std::right;
using std::setw;
using std::string;
using std::vector;

typedef vector<pair<Product, double>> products_usage;
typedef vector<pair<int, double>> products_usage_simple;

struct test_ranges
{
    int age;
    float firstRange;
    float secondRange;
};

struct LabTest
{
    int id;
    string name;
    string description;
    string value;
    double price;
    products_usage productsUsage;
    vector<test_ranges> testsRanges;
};

class LabTests
{
private:
    sqlite3 *db;
    Products *productsClass;
    Patients *patientsClass;

    int lastId = 0;

    // Database Operations
    vector<LabTest> getLabTests();
    LabTest getLabTest(int id);
    int createLabTest(string name, string description, string value, double price, products_usage_simple productsUsage, vector<test_ranges> testRanges);
    int updateLabTest();
    int deleteLabTest(int id);

public:
    static int cantLabTests;
    LabTests(sqlite3 *db, Products *productsClass, Patients *patientsClass);

    // Menu/Vista
    void menu();
    void getLabTestsMenu();
    void getLabTestMenu();
    void createLabTestMenu();
    void updateLabTestMenu();
    void deleteLabTestMenu();
};