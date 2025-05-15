#include "laboratory.h"

/**
    Constructor de la clase Laboratory
    Abre la base de datos y crea la tabla de configuracion si no existe
    Si ya existe, obtiene los datos de configuracion
*/
Laboratory::Laboratory(string labName, string labRif, string labPlace)
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

    this->labProducts = new Products(this->db);
    this->labPatients = new Patients(this->db);
    this->labInventoryOperations = new Inventory(this->db, this->labProducts);
    // this->labTests = new arrayLabTest();
};

/**
 * Destructor de la clase Laboratory
 * Cierra la base de datos y libera la memoria
 */
Laboratory::~Laboratory()
{
    delete this->labProducts;
    delete this->labPatients;
    delete this->labInventoryOperations;
    sqlite3_close(this->db);
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

// void Laboratory::getLabTests()
// {
//     int testsSize = this->labTests->size();
//     arrayLabTest &t = *(this->labTests);
//     cout << "\x1b[u\x1b[0J\x1b[48;5;111mCODIGO\tNOMBRE\tDESCRIPCION\tVALOR\tTOTAL DE TESTS: " << LabTest::cantLabTests << "\n\x1b[0m";
//     for (int i = 0; i < testsSize; i++)
//     {
//         t[i].getLabTests();
//     }
// }

// void Laboratory::createLabTest()
// {

//     if (this->labProducts->size() == 0)
//     {
//         cout << "\x1b[u\x1b[0J\x1b[38;5;124mDEBE EXISTIR ALMENOS 1 PRODUCTO ANTES\x1b[0m";
//         return;
//     }

//     string testName, testDescription, testValue;
//     float testPrice;
//     int rangeStart, rangeEnd, selectedProduct, productUsageAmount;
//     vector<pair<Products *, int>> usageProducts;
//     vector<pair<int, int>> testRanges;

//     cout << "\x1b[u\x1b[0J\x1b[48;5;20m\nCreacion del Examen\x1b[0m\n";

//     std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

//     cout << "\x1b[0JIngrese el nombre del examen: \x1b[38;5;33m";
//     getline(cin, testName);
//     cout << "\x1b[0m\x1b[0JIngrese la descripcion del examen: \x1b[38;5;33m";
//     getline(cin, testDescription);
//     cout << "\x1b[0m\x1b[0JIngrese el valor del examen (ejemplo\x1b[38;5;52m examen de sangre->Hgb\x1b[0m): \x1b[38;5;33m";
//     getline(cin, testValue);
//     cout << "\x1b[0m\x1b[0JIngrese el precio del examen: \x1b[38;5;33m";
//     cin >> testPrice;

//     cout << "\x1b[0m\x1b[48;5;20m\x1b[0JRangos Normales del examen\n\x1b[0m\x1b[s";
//     do
//     {
//         int addAnotherRange;

//         cout << "\x1b[u\x1b[0m\x1b[0JInicial de " << testValue << ": \x1b[38;5;33m";
//         cin >> rangeStart;
//         cout << "\x1b[0m\x1b[0JFinal de " << testValue << ": \x1b[38;5;33m";
//         cin >> rangeEnd;
//         testRanges.emplace_back(rangeStart, rangeEnd);

//         cout << "\x1b[0m\x1b[0J¿Desea agregar otro rango? (\x1b[38;5;52m0\x1b[0m/\x1b[38;5;40m1\x1b[0m):";
//         cin >> addAnotherRange;
//         if (addAnotherRange == 0)
//         {
//             break;
//         }
//     } while (true);

//     cout << "\n\x1b[0m\x1b[s";

//     this->getLabProducts();

//     cout << "\n\x1b[48;5;20m\x1b[0JProductos Usados en el examen de " << testValue << "\n\x1b[0m\x1b[s";
//     do
//     {
//         do
//         {
//             cout << "\x1b[u\x1b[0m\x1b[0JSeleccione el producto usado en el examen: \x1b[38;5;33m";
//             cin >> selectedProduct;
//         } while (selectedProduct < 0 || selectedProduct >= this->labProducts->size());

//         do
//         {
//             cout << "\x1b[u\x1b[0m\x1b[0JIngrese la cantidad de producto usado: \x1b[38;5;33m";
//             cin >> productUsageAmount;
//         } while (productUsageAmount < 0);

//         Products &operationProduct = (*this->labProducts)[selectedProduct];

//         usageProducts.emplace_back(&operationProduct, productUsageAmount);

//         if (usageProducts.size() == this->labProducts->size())
//         {
//             break;
//         }

//         int addProduct;

//         cout << "\x1b[0m\x1b[0J¿Desea agregar otro producto? (\x1b[38;5;52m0/\x1b[38;5;40m1\x1b[0m):";
//         cin >> addProduct;
//         if (addProduct == 0)
//         {
//             break;
//         }
//     } while (true);

//     this->labTests->emplace_back(testName, testDescription, testValue, testPrice, usageProducts, testRanges);
//     cout << "\n\x1b[0J---Examen--- \n\x1b[0J\x1b[38;5;33mNOMBRE: " << testName
//          << "\t\t\t\x1b[48;5;76mCREADO\x1b[0m";
// };

// void Laboratory::updateLabTest()
// {
//     cout << "AUN NO IMPLEMENTADO";
// };

// void Laboratory::deleteLabTest()
// {
//     cout << "AUN NO IMPLEMENTADO";
// };