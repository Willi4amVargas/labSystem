#include "include/Products.h"

int Products::cantProducts = 0;

/**
 * Constructor de la clase Products
 * Accede a el puntero de la base de datos y crea la tabla de productos si no existe
 * Tambien inicializa la variable de la clase
 * @param db referencia a la base de datos
 */
Products::Products(sqlite3 *db) : db(db)
{
    // Se crea la tabla de los productos si no existe
    string createTable = "CREATE TABLE IF NOT EXISTS products (id INTEGER PRIMARY KEY, name TEXT, description TEXT, stock INT);";
    sqlite3_exec(db, createTable.c_str(), 0, 0, 0);

    string selectProducts = "SELECT * FROM products;";

    sqlite3_exec(
        this->db,
        selectProducts.c_str(),
        [](void *data, int argc, char **argv, char **colnames) -> int
        {
            Products::cantProducts++;
            return 0;
        },
        0,
        0);
}

// OPERACIONES CON LA BASE DE DATOS
/**
 * Obtiene todos los productos de la base de datos
 * @return vector de la estructura productos
 */
vector<Product> Products::getProducts()
{
    vector<Product> products = {};

    string selectProducts = "SELECT * FROM products;";

    sqlite3_exec(
        this->db,
        selectProducts.c_str(),
        [](void *data, int argc, char **argv, char **colnames) -> int
        {
            vector<Product> *products = (vector<Product> *)data;

            Product product;
            product.id = atoi(argv[0]);
            product.name = argv[1];
            product.description = argv[2];
            product.stock = atoi(argv[3]);

            products->push_back(product);
            return 0;
        },
        &products,
        0);
    return products;
}

/**
 * Obtiene un producto de la base de datos
 * @param id id del producto a obtener
 * @return el producto con el id especificado en la estructura Product
 */
Product Products::getProduct(int id)
{
    Product product = {-1, "", "", 0};
    string selectProduct = "SELECT * FROM products WHERE id = " + to_string(id) + ";";

    sqlite3_exec(
        this->db,
        selectProduct.c_str(),
        [](void *data, int argc, char **argv, char **colnames) -> int
        {
            Product *product = (Product *)data;

            product->id = atoi(argv[0]);
            product->name = argv[1];
            product->description = argv[2];
            product->stock = atoi(argv[3]);
            return 0;
        },
        &product,
        0);

    return product;
}

/**
 * Crea un producto y lo inserta en la base de datos
 * @param name nombre del producto
 * @param description descripcion del producto
 * @return el id del producto creado
 */
int Products::createProduct(string name, string description)
{
    string insertProduct = "INSERT INTO products (name, description, stock) VALUES ('" + name + "', '" + description + "', 0);";
    sqlite3_exec(this->db, insertProduct.c_str(), 0, 0, 0);

    // Se obtiene el id del producto creado (deberia ser mejorado huele feo sjakjsk)
    string selectProduct = "SELECT MAX(id) FROM products;";
    int id = -1;
    sqlite3_exec(
        this->db,
        selectProduct.c_str(),
        [](void *data, int argc, char **argv, char **colnames) -> int
        {
            int *id = (int *)data;
            *id = atoi(argv[0]);
            return 0;
        },
        &id,
        0);

    Products::cantProducts++;
    return id;
}

/**
 * Actualiza un producto de la base de datos
 * @param id id del producto a actualizar
 * @param name nuevo nombre del producto
 * @param description nueva descripcion del producto
 */
void Products::updateProduct(int id, string name, string description)
{
    string updateProduct = "UPDATE products SET name = '" + name + "', description = '" + description + "' WHERE id = " + to_string(id) + ";";
    sqlite3_exec(this->db, updateProduct.c_str(), 0, 0, 0);
}

/**
 * Elimina un producto de la base de datos
 * @param id id del producto a eliminar
 */
void Products::deleteProduct(int id)
{
    string deleteProduct = "DELETE FROM products WHERE id = " + to_string(id) + ";";
    sqlite3_exec(this->db, deleteProduct.c_str(), 0, 0, 0);
    Products::cantProducts--;
}

/**
 * Actualiza el stock de un producto de la base de datos
 * Siempre le va a sumar el parametro stock al stock actual del producto
 * @param id id del producto a actualizar
 * @param stock cantidad de stock a agregar o quitar
 */
void Products::updateStock(int id, int stock)
{
    string updateStock = "UPDATE products SET stock = stock + " + to_string(stock) + " WHERE id = " + to_string(id) + ";";
    sqlite3_exec(this->db, updateStock.c_str(), 0, 0, 0);
}

// FUNCIONES DE VISTA/MENU

/**
 * Menu Principal de los productos
 * Muestra las opciones de los productos y llama a las funciones correspondientes
 */
void Products::menu()
{
    bool exit = false;
    string exitText = "\n\x1b[38;5;136m0. Salir al menu principal\nOtro. Continuar al menu de productos\n\x1b[38;5;52mopcion: \x1b[0m";
    string options[6] = {
        "Ver Productos del laboratorio",
        "Buscar Productos del laboratorio",
        "Crear Producto para el laboratorio",
        "Actualizar Producto del laboratorio",
        "Eliminar un Producto del laboratorio",
        "Regresar"};
    int optionsLen = sizeof(options) / sizeof(string);
    int selectedOption;

    do
    {

        cout << "\x1b[2J\x1b[0;0H\x1b[48;5;20m\nOperaciones de Productos\x1b[0m\n\x1b[s";

        cout << "\x1b[u";

        for (int i = 0; i < optionsLen; i++)
        {
            cout << i << ". " << options[i] << "\x1b[0J\n";
        }
        cout << "\x1b[38;5;33mopcion: \x1b[0m";
        cin >> selectedOption;

        if (selectedOption <= optionsLen - 1)
        {
            int goBack;
            switch (selectedOption)
            {
            case 0:
                this->getProductsMenu();
                cout << exitText;
                cin >> goBack;
                if (goBack == 0)
                {
                    exit = true;
                }
                break;
            case 1:
                this->getProductMenu();
                cout << exitText;
                cin >> goBack;
                if (goBack == 0)
                {
                    exit = true;
                }
                break;
            case 2:
                this->createProductMenu();
                cout << exitText;
                cin >> goBack;
                if (goBack == 0)
                {
                    exit = true;
                }
                break;
            case 3:
                this->updateProductMenu();
                cout << exitText;
                cin >> goBack;
                if (goBack == 0)
                {
                    exit = true;
                }
                break;
            case 4:
                this->deleteProductMenu();
                cout << exitText;
                cin >> goBack;
                if (goBack == 0)
                {
                    exit = true;
                }
                break;
            case 5:
                exit = true;
                break;
            }
        }
    } while (!exit);
}

/**
 * Muestra todos los productos del laboratorio
 * Si no hay productos registrados, muestra un mensaje diferente
 */
void Products::getProductsMenu()
{
    cout << "\x1b[u\x1b[0J\x1b[38;5;111m"
         << left << setw(20) << "CODIGO" << setw(30) << "NOMBRE" << setw(30) << "DESCRIPCION" << setw(10) << "STOCK" << setw(30) << "TOTAL DE PRODUCTOS: " << Products::cantProducts << "\x1b[0m" << endl;
    vector<Product> products = this->getProducts();
    int productsSize = products.size();
    if (productsSize > 0)
    {
        for (int i = 0; i < productsSize; i++)
        {
            cout << "\x1b[0J" << left << setw(20) << products[i].id << setw(30) << products[i].name << setw(30) << products[i].description << setw(10) << products[i].stock << endl;
        }
    }
    else
    {
        cout << "\x1b[38;5;52mNo hay productos registrados \x1b[0mε(´סּ︵סּ`)з\n";
    }
}

/**
 * Muestra un producto en especifico el usuario debera pasar el id del producto
 * Si no existe el producto, muestra un mensaje de error
 */
void Products::getProductMenu()
{
    int productId;
    this->getProductsMenu();
    cout << "\n\x1b[0J\x1b[38;5;166mIngrese el codigo del producto a ver: \x1b[s\x1b[0m";
    cin >> productId;

    Product product = this->getProduct(productId);

    if (product.id == -1)
    {
        cout << "\x1b[0J\x1b[38;5;124mNo existe el producto con el id: " << productId << "\x1b[0m" << endl;
    }
    else
    {
        cout << "\x1b[0J\x1b[38;5;33m" << setw(60) << left << "CODIGO: \x1b[0m" << product.id << endl
             << "\x1b[38;5;33m" << setw(60) << left << "NOMBRE: \x1b[0m" << product.name << endl
             << "\x1b[38;5;33m" << setw(60) << left << "DESCRIPCION: \x1b[0m" << product.description << endl
             << "\x1b[38;5;33m" << setw(60) << left << "STOCK: " << product.stock;
    }
}

/**
 * Crea un producto y lo inserta en la base de datos
 * el usuario debera ingresar el nombre y la descripcion del producto
 * Si el producto se crea correctamente, se muestra un mensaje de exito
 */
void Products::createProductMenu()
{
    string name, description;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    cout << "\x1b[u\x1b[0J" << endl
         << "\t\x1b[0mNOMBRE DEL PRODUCTO: \x1b[38;5;76m";
    getline(cin, name);

    cout << "\x1b[u\x1b[0J" << endl
         << "\tDESCRIPCION DEL PRODUCTO: \x1b[38;5;76m";
    getline(cin, description);

    int code = this->createProduct(name, description);

    cout << endl
         << "\x1b[0J\x1b[0m" << setw(60) << "==============================Producto=============================="
         << endl
         << "\x1b[0J\x1b[38;5;33m" << setw(60) << left << "CODIGO: \x1b[0m" << code << endl
         << "\x1b[38;5;33m" << setw(60) << left << "NOMBRE: \x1b[0m" << name << endl
         << "\x1b[38;5;33m" << setw(60) << left << "DESCRIPCION: \x1b[0m" << description << endl
         << endl
         << setw(60) << "===============================\x1b[38;5;40mCreado\x1b[0m===============================" << endl;
}

/**
 * Actualiza un producto de la base de datos
 * el usuario debera ingresar el id del producto a actualizar
 * asi como su nuevo nombre y descripcion
 * Si el producto se actualiza correctamente, se muestra un mensaje de exito
 * Si el producto no existe, se muestra un mensaje de error
 */
void Products::updateProductMenu()
{
    int productId;
    this->getProductsMenu();
    cout << "\n\x1b[0J\x1b[38;5;166mIngrese el codigo del producto a cambiar: \x1b[s\x1b[0m";
    cin >> productId;

    Product p = this->getProduct(productId);

    if (p.id == -1)
    {
        cout << "\x1b[0J\x1b[38;5;124mNo existe el producto con el id: " << productId << "\x1b[0m" << endl;
        return;
    }

    string name, description;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    cout << "\x1b[u\x1b[0J" << endl
         << "\t\x1b[0mNOMBRE DEL PRODUCTO: \x1b[38;5;76m";
    getline(cin, name);

    cout << "\x1b[u\x1b[0J" << endl
         << "\t\x1b[0mDESCRIPCION DEL PRODUCTO: \x1b[38;5;76m";
    getline(cin, description);

    this->updateProduct(productId, name, description);

    cout << endl
         << "\x1b[0J\x1b[0m" << setw(60) << "==========================Producto=========================="
         << endl
         << "\x1b[0J\x1b[38;5;33m" << setw(60) << left << "CODIGO: \x1b[0m" << productId << endl
         << "\x1b[38;5;33m" << setw(60) << left << "NOMBRE: \x1b[0m" << name << endl
         << "\x1b[38;5;33m" << setw(60) << left << "DESCRIPCION: \x1b[0m" << description << endl
         << endl
         << "\x1b[0m" << setw(60) << "=========================\x1b[38;5;20mActualizado\x1b[0m========================" << endl;
}

/**
 * Elimina un producto de la base de datos
 * el usuario debera ingresar el id del producto a eliminar
 * Si el producto se elimina correctamente, se muestra un mensaje de exito
 * Si el producto no existe, se muestra un mensaje de error
 */
void Products::deleteProductMenu()
{
    int productId;
    this->getProductsMenu();
    cout << "\n\x1b[0J\x1b[38;5;166mIngrese el codigo del producto a eliminar: \x1b[s\x1b[0m";
    cin >> productId;

    Product p = this->getProduct(productId);

    if (p.id == -1)
    {
        cout << "\x1b[0J\x1b[38;5;124mNo existe el producto con el id: " << productId << "\x1b[0m" << endl;
        return;
    }

    this->deleteProduct(productId);

    cout << "\x1b[0J\x1b[0m" << setw(60) << "==========================Producto=========================="
         << endl
         << "\x1b[0J\x1b[38;5;33m" << setw(60) << left << "CODIGO: \x1b[0m" << productId << endl
         << "\x1b[38;5;33m" << setw(60) << left << "NOMBRE: \x1b[0m" << p.name << endl
         << "\x1b[38;5;33m" << setw(60) << left << "DESCRIPCION: \x1b[0m" << p.description << endl
         << endl
         << setw(60) << "===========================\x1b[38;5;124mEliminado\x1b[0m==========================" << endl;
}