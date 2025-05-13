#include <iostream>
#include <sqlite3.h>
#include "laboratory.h"
#include "include/Products.h"
#include "include/Patients.h"
#include "include/Inventory.h"
#include "include/LabTests.h"

#include <limits>
#include <iomanip>

typedef std::string str;
typedef std::vector<Products> arrayProducts;
typedef std::vector<Patients> arrayPatients;
typedef std::vector<Inventory> arrayInventory;
typedef std::vector<LabTest> arrayLabTest;

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::left;
using std::setw;
using std::to_string;

Laboratory::Laboratory(str labName, str labRif, str labPlace)
{
    sqlite3_open("laboratory.db", &(this->db));
    // CREO LA TABLA SI NO EXISTE
    string createTable = "CREATE TABLE IF NOT EXISTS configs (id INTEGER PRIMARY KEY, name TEXT, rif TEXT, place TEXT);";
    sqlite3_exec(this->db, createTable.c_str(), 0, 0, 0);

    // COMBROBAR QUE YA NO EXISTA INFORMACION DE CONFIGURACION ANTES
    string selectConfigData = "SELECT * FROM configs;";

    sqlite3_exec(
        this->db,
        selectConfigData.c_str(),
        [](void *data, int argc, char **argv, char **colnames) -> int
        {
            Laboratory *self = static_cast<Laboratory *>(data);
            self->labName = argv[1];
            self->labRif = argv[2];
            self->labPlace = argv[3];

            return 0;
        },
        this,
        0);

    if (this->labName == "")
    {
        cout << "\x1b[0J\x1b[38;5;20mNombre del laboratorio: \x1b[0m";
        getline(cin, this->labName);
        cout << "\x1b[0J\x1b[38;5;20mRIF del laboratorio: \x1b[0m";
        getline(cin, this->labRif);
        cout << "\x1b[0J\x1b[38;5;20mUbicacion del laboratorio: \x1b[0m";
        getline(cin, this->labPlace);

        string insertConfigData = "INSERT INTO configs (name, rif, place) VALUES ('" + this->labName + "', '" + this->labRif + "', '" + this->labPlace + "');";
        sqlite3_exec(this->db, insertConfigData.c_str(), 0, 0, 0);
    }

    this->labProducts = new arrayProducts();
    this->labPatients = new arrayPatients();
    this->labInventoryOperations = new arrayInventory();
    this->labTests = new arrayLabTest();
};

Laboratory::~Laboratory()
{
    sqlite3_close(this->db);
    delete labProducts;
}

/**
 * Muestra los datos del laboratorio
 */
void Laboratory::getLabData()
{
    string selectConfigData = "SELECT * FROM configs;";

    cout << left << "\x1b[u\x1b[0J" << endl
         << setw(20) << "NOMBRE" << setw(10) << "RIF" << setw(30) << "DIRECCION" << endl;

    sqlite3_exec(
        this->db,
        selectConfigData.c_str(),
        [](void *data, int argc, char **argv, char **colnames) -> int
        {
            Laboratory *self = static_cast<Laboratory *>(data);

            self->labName = argv[1];
            self->labRif = argv[2];
            self->labPlace = argv[3];

            cout << left << "\x1b[0J\x1b[38;5;20m" << setw(20) << self->labName << setw(10) << self->labRif << "\x1b[38;5;76m" << setw(30) << self->labPlace << "\x1b[0m" << endl;
            return 0;
        },
        this,
        0);
}

/**
 * Muestra los productos del laboratorio
 */
void Laboratory::getLabProducts()
{

    cout << "\x1b[u\x1b[0J\x1b[38;5;111m"
         << left << setw(20) << "CODIGO" << setw(30) << "NOMBRE" << setw(30) << "DESCRIPCION" << setw(10) << "STOCK" << setw(30) << "TOTAL DE PRODUCTOS: " << Products::cantProducts << "\x1b[0m" << endl;
    for (int i = 0; i < this->labProducts->size(); i++)
    {
        Products p = this->labProducts->at(i);
        cout << left << "\x1b[0J" << setw(20) << i << setw(30) << p.getName() << setw(30) << p.getDescription() << setw(10) << p.getStock() << endl;
    }
}

/**
 * Obtiene la cantidad de productos existentes en la clase
 */
int Laboratory::getLabProductsSize()
{
    return this->labProducts->size();
}

/**
 * Crea un producto y lo inserta en la clase
 */
void Laboratory::createProduct()
{
    str name, description;
    int code = this->labProducts->size();

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    cout << "\x1b[u\x1b[0J" << endl
         << "\t\x1b[0mNOMBRE DEL PRODUCTO: \x1b[38;5;76m";
    getline(cin, name);

    cout << "\x1b[u\x1b[0J" << endl
         << "\tDESCRIPCION DEL PRODUCTO: \x1b[38;5;76m";
    getline(cin, description);

    this->labProducts->emplace_back(name, description);

    cout << endl
         << "\x1b[0J\x1b[0m" << setw(60) << "==============================Producto=============================="
         << endl
         << "\x1b[0J\x1b[38;5;33m" << setw(10) << "CODIGO: " << code << setw(20) << "NOMBRE: " << name << setw(30) << "DESCRIPCION: " << description << setw(10) << endl
         << "\x1b[0m" << setw(60) << left << "CREADO\x1b[0m";
}

/**
 * Actualiza un producto
 */
void Laboratory::updateProduct()
{
    if (this->labProducts->size() > 0)
    {
        int codeToUpdate;
        this->getLabProducts();
        cout << "\n\x1b[0J\x1b[38;5;166mIngrese el codigo del producto a cambiar: \x1b[s\x1b[0m";
        do
        {
            cout << "\x1b[u\x1b[0J";
            cin >> codeToUpdate;
        } while (codeToUpdate < 0 || codeToUpdate >= this->labProducts->size());

        // arrayProducts &allProducts = *this->labProducts;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        str newName, newDescription;
        cout << "\n\x1b[0m\x1b[0JIngrese el nuevo nombre del producto: \x1b[38;5;33m";
        getline(cin, newName);

        cout << "\n\x1b[0m\x1b[0JIngrese la nueva descripcion del producto: \x1b[38;5;33m";
        getline(cin, newDescription);

        (*this->labProducts)[codeToUpdate].updateProduct(newName, newDescription);
    }
    else
    {
        cout << "\n\x1b[38;5;52mNo hay productos registrados \x1b[0mε(´סּ︵סּ`)з\n";
    }
}

void Laboratory::deleteProduct()
{
    if (this->labProducts->size() > 0)
    {
        int codeToDelete;
        this->getLabProducts();
        cout << "\n\x1b[0J\x1b[38;5;166mIngrese el codigo del producto a eliminar: \x1b[s\x1b[0m";
        do
        {
            cout << "\x1b[u\x1b[0J";
            cin >> codeToDelete;
        } while (codeToDelete < 0 || codeToDelete >= this->labProducts->size());

        this->labProducts->erase(this->labProducts->begin() + codeToDelete);
    }
    else
    {
        cout << "\n\x1b[38;5;52mNo hay productos registrados \x1b[0mε(´סּ︵סּ`)з\n";
    }
}

void Laboratory::getLabPatients()
{
    int patientsSize = this->labPatients->size();
    arrayPatients &p = *(this->labPatients);
    cout << "\x1b[u\x1b[0J\x1b[48;5;111mCODIGO\tNOMBRE\tCEDULA\tEMAIL\tTELEFONO\t\tTOTAL DE PACIENTES: " << Patients::cantPatients << "\n\x1b[0m";
    for (int i = 0; i < patientsSize; i++)
    {
        cout << "\x1b[0J" << i << "\t" << p[i].getPatientFullName() << "\t" << p[i].getPatientCI() << "\t" << p[i].getPatientEmail() << "\t" << p[i].getPatientPhone() << "\n";
    }
}

void Laboratory::createPatient()
{
    str fullName, ci, email, phone;
    int code = this->labPatients->size();

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    cout << "\x1b[u\x1b[0m\x1b[0JNOMBRE COMPLETO DEL PACIENTE: \x1b[38;5;76m";
    getline(cin, fullName);

    cout << "\x1b[0m\x1b[0JCEDULA DEL PACIENTE: \x1b[38;5;76m";
    getline(cin, ci);

    cout << "\x1b[0m\x1b[0JEMAIL DEL PACIENTE: \x1b[38;5;76m";
    getline(cin, email);

    cout << "\x1b[0m\x1b[0JTELEFONO DEL PACIENTE: \x1b[38;5;76m";
    getline(cin, phone);

    this->labPatients->emplace_back(fullName, ci, email, phone);

    cout << "\n\x1b[0J---Paciente--- \n\x1b[0J\x1b[38;5;33mCODIGO: " << code
         << "\n\x1b[0JNOMBRE: " << fullName << "\t\t\t\x1b[48;5;76m\x1b[38;5;255mCREADO\x1b[0m\x1b[38;5;33m"
         << "\n\x1b[0JCEDULA: " << ci << "\t\x1b[0m\n";
}

void Laboratory::updatePatient()
{
    if (this->labPatients->size() > 0)
    {
        int codeToUpdate;
        this->getLabPatients();
        cout << "\n\x1b[0J\x1b[38;5;166mIngrese el codigo del paciente a cambiar: \x1b[s\x1b[0m";
        do
        {
            cout << "\x1b[u\x1b[0J";
            cin >> codeToUpdate;
        } while (codeToUpdate < 0 || codeToUpdate >= this->labPatients->size());

        arrayPatients &allPatients = *this->labPatients;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        str newFullName, newCi, newEmail, newPhone;
        cout << "\n\x1b[0m\x1b[0JIngrese el nuevo nombre del paciente: \x1b[38;5;33m";
        getline(cin, newFullName);

        cout << "\n\x1b[0m\x1b[0JIngrese la nueva cedula del paciente: \x1b[38;5;33m";
        getline(cin, newCi);

        cout << "\n\x1b[0m\x1b[0JIngrese el nuevo email del paciente: \x1b[38;5;33m";
        getline(cin, newEmail);

        cout << "\n\x1b[0m\x1b[0JIngrese el nuevo telefono del paciente: \x1b[38;5;33m";
        getline(cin, newPhone);

        allPatients[codeToUpdate].updatePatient(newFullName, newCi, newEmail, newPhone);
    }
    else
    {
        cout << "\n\x1b[38;5;52mNo hay pacientes registrados \x1b[0mε(´סּ︵סּ`)з\n";
    }
}

void Laboratory::deletePatient()
{
    if (this->labPatients->size() > 0)
    {
        int codeToDelete;
        this->getLabPatients();
        cout << "\n\x1b[0J\x1b[38;5;166mIngrese el codigo del paciente a eliminar: \x1b[s\x1b[0m";
        do
        {
            cout << "\x1b[u\x1b[0J";
            cin >> codeToDelete;
        } while (codeToDelete < 0 || codeToDelete >= this->labPatients->size());

        this->labPatients->erase(this->labPatients->begin() + codeToDelete);
    }
    else
    {
        cout << "\n\x1b[38;5;52mNo hay pacientes registrados \x1b[0mε(´סּ︵סּ`)з\n";
    }
}

void Laboratory::getLabInventoryOperation()
{
    int inventorySize = this->labInventoryOperations->size();
    arrayInventory &i = *(this->labInventoryOperations);

    cout << "\x1b[u\x1b[0J\x1b[48;5;111mCODIGO\tNOMBRE\tTIPO DE OPERACION\tSTOCK\n\x1b[0m";
    for (int a = 0; a < inventorySize; a++)
    {
        cout << "\x1b[0J" << i[a].getInventoryProductCode() << "\t" << i[a].getInventoryProductName() << "\t" << i[a].getInventoryOperationType() << "\t" << i[a].getInventoryProductStock() << "\n";
    }
}

void Laboratory::createLabInventoryOperation()
{
    int productsInLab = this->labProducts->size();
    if (productsInLab > 0)
    {
        int selectedProduct, selectedStock, operationType;

        cout << "\x1b[u\x1b[0J¿QUE OPERACION VA A REALIZAR?" << "\n\x1b[0J\x1b[38;5;33m0. CARGA DE INVENTARIO\x1b[0m" << "\n\x1b[0J\x1b[38;5;124m1. DESCARGA DE INVENTARIO\x1b[0m\n\x1b[s";
        do
        {
            cout << "\x1b[u\x1b[0J";
            cin >> operationType;
        } while (operationType < 0 || operationType > 1);

        cout << "\n";
        this->getLabProducts();

        cout << "Seleccione el codigo del producto: \x1b[s";
        do
        {
            cout << "\x1b[u\x1b[0J";
            cin >> selectedProduct;
        } while (selectedProduct < 0 || selectedProduct >= productsInLab);

        Products &operationProduct = (*this->labProducts)[selectedProduct];

        if (operationType == 1 && operationProduct.getStock() <= 0)
        {
            cout << "\x1b[48;5;160mNo puedes realizar una descarga a un producto sin stock\x1b[0m";
            return;
        }

        cout << "Ingrese el stock que va a " << (operationType == 0 ? "\x1b[38;5;33mcargar\x1b[0m: " : "\x1b[38;5;124mdescargar\x1b[0m: ");
        cin >> selectedStock;

        if (operationType == 1 && selectedStock < 0)
        {
            cout << "\x1b[48;5;160mNo puedes realizar una descarga de stock negativo\x1b[0m";
            return;
        }

        if (operationType == 1 && operationProduct.getStock() - selectedStock < 0)
        {
            cout << "\x1b[48;5;160mNo puedes descargar mas del stock total\x1b[0m";
            return;
        }

        str headerText = operationType == 0 ? "\n\x1b[0J\x1b[38;5;33m---OPERACION DE CARGA---\x1b[0m" : "\n\x1b[0J\x1b[38;5;124m---OPERACION DE DESCARGA---\x1b[0m";

        if (operationType == 1)
        {
            operationProduct.updateStock(-selectedStock);
            this->labInventoryOperations->emplace_back(to_string(selectedProduct), operationProduct.getName(), "DOWNLOAD", selectedStock);

            cout << headerText << "\n\x1b[0J\x1b[38;5;33mPRODUCTO CODIGO: " << selectedProduct
                 << "\n\x1b[0JNOMBRE: " << operationProduct.getName() << "\t\t\t\x1b[48;5;76m\x1b[38;5;255mCREADO\x1b[0m\x1b[38;5;33m"
                 << "\n\x1b[0JSTOCK: " << selectedStock << "\t\x1b[0m\n";
        }
        else if (operationType == 0)
        {
            operationProduct.updateStock(selectedStock);
            this->labInventoryOperations->emplace_back(to_string(selectedProduct), operationProduct.getName(), "LOAD", selectedStock);

            cout << headerText << "\n\x1b[0J\x1b[38;5;33mPRODUCTO CODIGO: " << selectedProduct
                 << "\n\x1b[0JNOMBRE: " << operationProduct.getName() << "\t\t\t\x1b[48;5;76m\x1b[38;5;255mCREADO\x1b[0m\x1b[38;5;33m"
                 << "\n\x1b[0JSTOCK: " << selectedStock << "\t\x1b[0m\n";
        }
    }
    else
    {
        cout << "\x1b[u\x1b[0J\x1b[48;5;124mDEBE EXISTIR ALMENOS 1 PRODUCTO ANTES\x1b[0m";
    }
}

void Laboratory::getLabTests()
{
    int testsSize = this->labTests->size();
    arrayLabTest &t = *(this->labTests);
    cout << "\x1b[u\x1b[0J\x1b[48;5;111mCODIGO\tNOMBRE\tDESCRIPCION\tVALOR\tTOTAL DE TESTS: " << LabTest::cantLabTests << "\n\x1b[0m";
    for (int i = 0; i < testsSize; i++)
    {
        t[i].getLabTests();
    }
}

void Laboratory::createLabTest()
{

    if (this->labProducts->size() == 0)
    {
        cout << "\x1b[u\x1b[0J\x1b[38;5;124mDEBE EXISTIR ALMENOS 1 PRODUCTO ANTES\x1b[0m";
        return;
    }

    str testName, testDescription, testValue;
    float testPrice;
    int rangeStart, rangeEnd, selectedProduct, productUsageAmount;
    vector<pair<Products *, int>> usageProducts;
    vector<pair<int, int>> testRanges;

    cout << "\x1b[u\x1b[0J\x1b[48;5;20m\nCreacion del Examen\x1b[0m\n";

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    cout << "\x1b[0JIngrese el nombre del examen: \x1b[38;5;33m";
    getline(cin, testName);
    cout << "\x1b[0m\x1b[0JIngrese la descripcion del examen: \x1b[38;5;33m";
    getline(cin, testDescription);
    cout << "\x1b[0m\x1b[0JIngrese el valor del examen (ejemplo\x1b[38;5;52m examen de sangre->Hgb\x1b[0m): \x1b[38;5;33m";
    getline(cin, testValue);
    cout << "\x1b[0m\x1b[0JIngrese el precio del examen: \x1b[38;5;33m";
    cin >> testPrice;

    cout << "\x1b[0m\x1b[48;5;20m\x1b[0JRangos Normales del examen\n\x1b[0m\x1b[s";
    do
    {
        int addAnotherRange;

        cout << "\x1b[u\x1b[0m\x1b[0JInicial de " << testValue << ": \x1b[38;5;33m";
        cin >> rangeStart;
        cout << "\x1b[0m\x1b[0JFinal de " << testValue << ": \x1b[38;5;33m";
        cin >> rangeEnd;
        testRanges.emplace_back(rangeStart, rangeEnd);

        cout << "\x1b[0m\x1b[0J¿Desea agregar otro rango? (\x1b[38;5;52m0\x1b[0m/\x1b[38;5;40m1\x1b[0m):";
        cin >> addAnotherRange;
        if (addAnotherRange == 0)
        {
            break;
        }
    } while (true);

    cout << "\n\x1b[0m\x1b[s";

    this->getLabProducts();

    cout << "\n\x1b[48;5;20m\x1b[0JProductos Usados en el examen de " << testValue << "\n\x1b[0m\x1b[s";
    do
    {
        do
        {
            cout << "\x1b[u\x1b[0m\x1b[0JSeleccione el producto usado en el examen: \x1b[38;5;33m";
            cin >> selectedProduct;
        } while (selectedProduct < 0 || selectedProduct >= this->labProducts->size());

        do
        {
            cout << "\x1b[u\x1b[0m\x1b[0JIngrese la cantidad de producto usado: \x1b[38;5;33m";
            cin >> productUsageAmount;
        } while (productUsageAmount < 0);

        Products &operationProduct = (*this->labProducts)[selectedProduct];

        usageProducts.emplace_back(&operationProduct, productUsageAmount);

        if (usageProducts.size() == this->labProducts->size())
        {
            break;
        }

        int addProduct;

        cout << "\x1b[0m\x1b[0J¿Desea agregar otro producto? (\x1b[38;5;52m0/\x1b[38;5;40m1\x1b[0m):";
        cin >> addProduct;
        if (addProduct == 0)
        {
            break;
        }
    } while (true);

    this->labTests->emplace_back(testName, testDescription, testValue, testPrice, usageProducts, testRanges);
    cout << "\n\x1b[0J---Examen--- \n\x1b[0J\x1b[38;5;33mNOMBRE: " << testName
         << "\t\t\t\x1b[48;5;76mCREADO\x1b[0m";
};

void Laboratory::updateLabTest()
{
    cout << "AUN NO IMPLEMENTADO";
};

void Laboratory::deleteLabTest()
{
    cout << "AUN NO IMPLEMENTADO";
};