#pragma once
#include <iostream>
#include <vector>
#include <sqlite3.h>
#include <iomanip>
#include <limits>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::left;
using std::setw;
using std::string;
using std::to_string;
using std::vector;

struct Product
{
    int id;
    string name;
    string description;
    int stock;
};

class Products
{
private:
    sqlite3 *db;

    // Database operations
    vector<Product> getProducts();
    int createProduct(string name, string description);
    void updateProduct(int id, string name, string description);
    void deleteProduct(int id);

public:
    static int cantProducts;
    Products(sqlite3 *db);
    void menu();
    // Unicas operaciones que manejan la base de datos publicas
    Product getProduct(int id);
    void updateStock(int id, int stock);

    // Menu operations
    void getProductsMenu();
    void getProductMenu();
    void createProductMenu();
    void updateProductMenu();
    void deleteProductMenu();
};