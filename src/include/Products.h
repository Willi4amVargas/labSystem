#pragma once
#include <iostream>
#include <vector>
#include <sqlite3.h>

using std::string;
using std::vector;
typedef string str;

struct Product
{
    int id;
    str name;
    str description;
    int stock;
};

class Products
{
private:
    sqlite3 *db;

    // Database operations
    vector<Product> getProducts();
    int createProduct(str name, str description);
    void updateProduct(int id, str name, str description);
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