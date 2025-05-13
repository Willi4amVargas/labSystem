#pragma once
#include <iostream>
#include <vector>
#include <utility>
#include "Products.h"

using std::pair;
using std::string;
using std::vector;
typedef string str;

class LabTest
{
private:
    str testName;
    str testDescription;
    str testValue;
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
    LabTest(str testName, str testDescription, str testValue, float testPrice, vector<pair<Products*, int>> usageProducts, vector<pair<int, int>> testRanges);
    void getLabTests();
    void updateLabTest();
    void deleteLabTest();
};