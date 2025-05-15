#include "include/Inventory.h"

/**
 * Constructor de la clase Inventory
 * Genera la tabla de inventory_operation y la tabla de operation_type
 * en caso de que no existan
 * @param db puntero a la base de datos
 */
Inventory::Inventory(sqlite3 *db, Products *productsClass) : db(db), productsClass(productsClass)
{
    string createOperationTypeTable = "CREATE TABLE IF NOT EXISTS operation_type (id INTEGER PRIMARY KEY,description TEXT);";
    sqlite3_exec(this->db, createOperationTypeTable.c_str(), 0, 0, 0);

    string selectOperationTypes = "SELECT * FROM operation_type;";
    bool operationTypesExist = false;

    sqlite3_exec(
        this->db,
        selectOperationTypes.c_str(),
        [](void *data, int argc, char **argv, char **colnames) -> int
        {
            bool *operationTypesExist = (bool *)data;
            *operationTypesExist = true;
            return 0;
        },
        &operationTypesExist,
        0);

    if (!operationTypesExist)
    {
        string operationTypesInserts[2] = {
            "INSERT INTO operation_type (id, description) VALUES (1,'LOAD');",
            "INSERT INTO operation_type (id, description) VALUES (2,'DOWNLOAD');"};
        for (int i = 0; i < 2; i++)
        {
            sqlite3_exec(this->db, operationTypesInserts[i].c_str(), 0, 0, 0);
        }
    }

    string createInventoryTable = "CREATE TABLE IF NOT EXISTS inventory_operation (id INTEGER PRIMARY KEY,product_code TEXT, product_name TEXT, operation_code INT, product_stock INT, FOREIGN KEY (product_code) REFERENCES products(id), FOREIGN KEY (operation_code) REFERENCES operation_type(id));";
    sqlite3_exec(this->db, createInventoryTable.c_str(), 0, 0, 0);
}

// OPERACIONES CON LA BASE DE DATOS
/**
 * Obtiene todas las operaciones de inventario
 * @return vector de operaciones de inventario
 */
vector<InventoryOperation> Inventory::getInventoryOperations()
{
    vector<InventoryOperation> inventoryOperations;
    string selectInventoryOperations = "SELECT * FROM inventory_operation;";
    sqlite3_exec(
        this->db,
        selectInventoryOperations.c_str(),
        [](void *data, int argc, char **argv, char **colnames) -> int
        {
            vector<InventoryOperation> *inventoryOperations = (vector<InventoryOperation> *)data;
            InventoryOperation operation;
            operation.id = atoi(argv[0]);
            operation.product_code = argv[1];
            operation.product_name = argv[2];
            operation.operation_code = atoi(argv[3]);
            operation.product_stock = atoi(argv[4]);
            inventoryOperations->push_back(operation);
            return 0;
        },
        &inventoryOperations,
        0);
    return inventoryOperations;
}

/**
 * Obtiene el nombre del tipo de operacion
 * 1 = LOAD
 * 2 = DOWNLOAD
 */
string Inventory::getOperationType(int code)
{
    string operationType = "";
    string selectOperationType = "SELECT description FROM operation_type WHERE id = " + to_string(code) + ";";
    sqlite3_exec(
        this->db,
        selectOperationType.c_str(),
        [](void *data, int argc, char **argv, char **colnames) -> int
        {
            *static_cast<string *>(data) = argv[0];
            return 0;
        },
        &operationType,
        0);
    return operationType;
}
/**
 * Crea una operacion de inventario
 * @return id de la operacion creada
 */
int Inventory::createInventoryOperation(int operationType, int productCode, int productStock)
{
    string insertInventoryOperation = "INSERT INTO inventory_operation (product_code, product_name, operation_code, product_stock) VALUES ('" + to_string(productCode) + "', '" + this->productsClass->getProduct(productCode).name + "', " + to_string(operationType) + ", " + to_string(productStock) + ");";

    sqlite3_exec(this->db, insertInventoryOperation.c_str(), 0, 0, 0);

    // Se obtiene el id del registro creado (deberia ser mejorado huele feo sjakjsk)
    string selectInventoryOperation = "SELECT MAX(id) FROM inventory_operations;";
    int id = -1;
    sqlite3_exec(
        this->db,
        selectInventoryOperation.c_str(),
        [](void *data, int argc, char **argv, char **colnames) -> int
        {
            int *id = (int *)data;
            *id = atoi(argv[0]);
            return 0;
        },
        &id,
        0);

    return id;
}

// OPERACION DE VISTA/MENU

/**
 * Menu principal de la seccion de inventario
 */
void Inventory::menu()
{
    bool exit = false;
    string exitText = "\n\x1b[38;5;136m0. Salir al menu principal\nOtro. Continuar al menu de inventario\n\x1b[38;5;52mopcion: \x1b[0m";
    string options[3] = {
        "Ver Operaciones de inventario del laboratorio",
        "Crear Operacion de inventario para el laboratorio",
        "Regresar"};
    int optionsLen = sizeof(options) / sizeof(string);
    int selectedOption;

    do
    {

        cout << "\x1b[2J\x1b[0;0H\x1b[48;5;20m\nOperaciones de Inventario\x1b[0m\n\x1b[s";

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
                this->getInventoryOperationsMenu();
                cout << exitText;
                cin >> goBack;
                if (goBack == 0)
                {
                    exit = true;
                }
                break;
            case 1:
                this->createInventoryOperationMenu();
                cout << exitText;
                cin >> goBack;
                if (goBack == 0)
                {
                    exit = true;
                }
                break;
            case 2:
                exit = true;
                break;
            }
        }
    } while (!exit);
}

/**
 * Vista donde se muestran todas las operaciones de inventario
 */
void Inventory::getInventoryOperationsMenu()
{
    vector<InventoryOperation> inventoryOperations = this->getInventoryOperations();

    cout << "\x1b[u\x1b[0J\x1b[48;5;111m" << setw(5) << left << "SER." << setw(25) << left << "PROD. CODE" << setw(25) << left << "PROD. NAME" << setw(25) << left << "OPE. TYPE" << setw(5) << left << "STOCK\x1b[0m" << endl;
    for (int i = 0; i < inventoryOperations.size(); i++)
    {
        string operationDescription = this->getOperationType(inventoryOperations[i].operation_code);
        string operationDescriptionColor =
            inventoryOperations[i].operation_code == 1 ? "\x1b[38;5;33m" : "\x1b[38;5;124m";

        cout << "\x1b[0J" << setw(5) << left << inventoryOperations[i].id
             << setw(25) << left << inventoryOperations[i].product_code
             << setw(25) << left << inventoryOperations[i].product_name
             << operationDescriptionColor << setw(25) << left << operationDescription << "\x1b[0m"
             << setw(5) << right << inventoryOperations[i].product_stock << endl;
    }
}
/**
 * Menu para crear una operacion de inventario
 * Puede ser 1=LOAD o 2=DOWNLOAD
 */
void Inventory::createInventoryOperationMenu()
{
    if (this->productsClass->cantProducts > 0)
    {
        int selectedProduct, selectedStock, operationType;

        cout << "\x1b[u\x1b[0J¿QUE OPERACION VA A REALIZAR?" << "\n\x1b[0J\x1b[38;5;33m1. CARGA DE INVENTARIO\x1b[0m" << "\n\x1b[0J\x1b[38;5;124m2. DESCARGA DE INVENTARIO\x1b[0m\n\x1b[s";
        do
        {
            cout << "\x1b[u\x1b[0J";
            cin >> operationType;
        } while (operationType < 1 || operationType > 2);

        cout << "\n";
        this->productsClass->getProductsMenu();

        cout << "Seleccione el codigo del producto: \x1b[s";

        Product operationProduct;

        do
        {
            cout << "\x1b[u\x1b[0J";
            cin >> selectedProduct;

            operationProduct = this->productsClass->getProduct(selectedProduct);

            if (operationProduct.id != -1)
            {
                break;
            }
        } while (true);

        if (operationType == 2 && operationProduct.stock <= 0)
        {
            cout << "\x1b[48;5;160mNo puedes realizar una descarga a un producto sin stock\x1b[0m";
            return;
        }

        cout << "Ingrese el stock que va a " << (operationType == 1 ? "\x1b[38;5;33mcargar\x1b[0m: " : "\x1b[38;5;124mdescargar\x1b[0m: ");

        do
        {
            cin >> selectedStock;
            if (selectedStock < 0)
            {
                continue;
            }
            else
            {
                break;
            }
        } while (true);

        if (operationType == 2 && selectedStock <= 0)
        {
            cout << "\x1b[48;5;160mNo puedes realizar una descarga de stock o igual a 0 negativo\x1b[0m";
            return;
        }
        if (operationType == 1 && selectedStock <= 0)
        {
            cout << "\x1b[48;5;160mNo puedes hacer una carga de stock negativa o igual a 0\x1b[0m";
            return;
        }

        if (operationType == 2 && operationProduct.stock - selectedStock < 0)
        {
            cout << "\x1b[48;5;160mNo puedes descargar mas del stock total\x1b[0m";
            return;
        }

        string headerText =
            operationType == 1 ? "\x1b[0J=====================\x1b[38;5;33mOperacion de carga\x1b[0m====================="
                               : "\x1b[0J====================\x1b[38;5;124mOperacion de descarga\x1b[0m===================";

        if (operationType == 2)
        {
            this->productsClass->updateStock(operationProduct.id, -selectedStock);

            this->createInventoryOperation(2, operationProduct.id, selectedStock);

            cout << setw(60) << left << headerText << endl;
            cout << setw(60) << left << "\x1b[0JProd. Code: " << selectedProduct << endl;
            cout << setw(60) << left << "\x1b[0JProd. Name: " << operationProduct.name << endl;
            cout << setw(60) << left << "\x1b[0JStock: " << selectedStock << endl;
            cout << setw(60) << left << "============================================================" << endl;
        }
        else if (operationType == 1)
        {
            this->productsClass->updateStock(operationProduct.id, selectedStock);

            this->createInventoryOperation(1, operationProduct.id, selectedStock);

            cout << setw(60) << left << headerText << endl;
            cout << setw(60) << left << "\x1b[0JProd. Code: " << selectedProduct << endl;
            cout << setw(60) << left << "\x1b[0JProd. Name: " << operationProduct.name << endl;
            cout << setw(60) << left << "\x1b[0JStock: " << selectedStock << endl;
            cout << setw(60) << left << "============================================================" << endl;
        }
    }
    else
    {
        cout << "\x1b[u\x1b[0J\x1b[48;5;124mDEBE EXISTIR ALMENOS 1 PRODUCTO ANTES\x1b[0m";
    }
}