#pragma once
#include <iostream>
#include <vector>
#include <utility>
#include "include/Products.h"

using std::cin;
using std::cout;
using std::pair;
using std::string;
using std::vector;


class LabTest
{
private:
    string testName;
    string testDescription;
    string testValue;
    float testPrice;
    /**Productos usados en el examen
    Posicion 0 -> posicion en el arreglo de productos
    Posicion 1 -> cantidad usada
    */
    vector<pair<Products*, int>> usageProducts;
    /**Rangos Normales del examen
    Posicion 0 -> Rango inicial
    Posicion 1 -> Rango final
    */
    vector<pair<int, int>> testRanges;

public:
    static int cantLabTests;
    LabTest(string testName, string testDescription, string testValue, float testPrice, vector<pair<Products*, int>> usageProducts, vector<pair<int, int>> testRanges);
    void getLabTests();
    void updateLabTest();
    void deleteLabTest();
};