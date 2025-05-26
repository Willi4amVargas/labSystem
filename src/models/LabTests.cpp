#include "include/LabTests.h"

int LabTests::cantLabTests = 0;

LabTests::LabTests(sqlite3 *db, Products *productsClass, Patients *patientsClass)
    : db(db),
      productsClass(productsClass),
      patientsClass(patientsClass)
{
    string createTestsTable = "CREATE TABLE IF NOT EXISTS lab_tests (id INTEGER PRIMARY KEY,name TEXT, description TEXT, value TEXT, price FLOAT);";

    string createTestsProductsUsageTable = "CREATE TABLE IF NOT EXISTS lt_products_usage (id INTEGER PRIMARY KEY,test_id INTEGER, product_id INTEGER, amount FLOAT, FOREIGN KEY (test_id) REFERENCES lab_tests(id) ON DELETE CASCADE, FOREIGN KEY (product_id) REFERENCES products(id) ON DELETE NO ACTION);";

    string createTestsRangesTable = "CREATE TABLE IF NOT EXISTS lt_ranges (id INTEGER PRIMARY KEY,test_id INTEGER, age INT,first_range FLOAT, second_range FLOAT, FOREIGN KEY (test_id) REFERENCES lab_tests(id) ON DELETE CASCADE);";

    string selectLabTests = "SELECT * FROM lab_tests;";
    sqlite3_exec(
        this->db,
        selectLabTests.c_str(),
        [](void *data, int argc, char **argv, char **colnames) -> int
        {
            LabTests *self = (LabTests *)data;
            self->cantLabTests++;
            return 0;
        },
        this,
        0);

    sqlite3_exec(this->db, createTestsTable.c_str(), 0, 0, 0);
    sqlite3_exec(this->db, createTestsProductsUsageTable.c_str(), 0, 0, 0);
    sqlite3_exec(this->db, createTestsRangesTable.c_str(), 0, 0, 0);
}

// DATABASE OPERATIONS

/**
 * Obtiene un examen del laboratorio por su ID
 * Si no se encuentra el examen, devuelve un LabTest con id = -1
 */
LabTest LabTests::getLabTest(int id)
{
    LabTest test =
        {
            .id = -1,
            .name = "",
            .description = "",
            .value = "",
            .price = 0.0,
            .productsUsage = {},
            .testsRanges = {}};

    string selectLabTest = "SELECT * FROM lab_tests WHERE id = " + to_string(id) + ";";

    sqlite3_exec(
        this->db,
        selectLabTest.c_str(),
        [](void *data, int argc, char **argv, char **colnames) -> int
        {
            LabTest *test = (LabTest *)data;

            test->id = atoi(argv[0]);
            test->name = argv[1];
            test->description = argv[2];
            test->value = argv[3];
            test->price = atof(argv[4]);

            return 0;
        },
        &test,
        0);

    if (test.id == -1)
    {
        return test;
    }

    string selectLabTestsRanges = "SELECT * FROM lt_ranges WHERE test_id = " + to_string(test.id) + ";";

    sqlite3_exec(
        this->db,
        selectLabTestsRanges.c_str(),
        [](void *data, int argc, char **argv, char **colnames) -> int
        {
            LabTest *test = (LabTest *)data;

            test_ranges testRange = {};
            testRange.age = atoi(argv[2]);
            testRange.firstRange = atof(argv[3]);
            testRange.secondRange = atof(argv[4]);

            test->testsRanges.emplace_back(testRange);

            return 0;
        },
        &test,
        0);

    pair<Products *, LabTest *> testData = {this->productsClass, &test};

    string selectLabTestsProductsUsage = "SELECT * FROM lt_products_usage WHERE test_id = " + to_string(test.id) + ";";

    sqlite3_exec(
        this->db,
        selectLabTestsProductsUsage.c_str(),
        [](void *data, int argc, char **argv, char **colnames) -> int
        {
            pair<Products *, LabTest *> *testData = (pair<Products *, LabTest *> *)data;

            Product product = testData->first->getProduct(atoi(argv[2]));

            if (product.id != -1)
            {
                testData->second->productsUsage.emplace_back(product, atof(argv[3]));
            }
            return 0;
        },
        &testData,
        0);

    return test;
};

/**
 * Obtiene todos los examenes del laboratorio
 * (agradezco a dios que funciona, espero no me explote la RAM)
 */
vector<LabTest> LabTests::getLabTests()
{
    pair<LabTests *, vector<LabTest>> dataFirst = {this, {}};

    string selectLabTests = "SELECT * FROM lab_tests;";
    sqlite3_exec(
        this->db,
        selectLabTests.c_str(),
        [](void *data, int argc, char **argv, char **colnames) -> int
        {
            pair<LabTests *, vector<LabTest>> *dataSecond = (pair<LabTests *, vector<LabTest>> *)data;

            LabTest test = {};

            pair<Products *, LabTest *> dataThird = {dataSecond->first->productsClass, &test};

            test.id = atoi(argv[0]);
            test.name = argv[1];
            test.description = argv[2];
            test.value = argv[3];
            test.price = atof(argv[4]);

            test.testsRanges = {};
            test.productsUsage = {};

            string selectLabTestsRanges = "SELECT * FROM lt_ranges WHERE test_id = " + to_string(test.id) + ";";

            sqlite3_exec(
                dataSecond->first->db,
                selectLabTestsRanges.c_str(),
                [](void *data, int argc, char **argv, char **colnames) -> int
                {
                    LabTest *test = (LabTest *)data;

                    test_ranges testRange = {};
                    testRange.age = atoi(argv[2]);
                    testRange.firstRange = atof(argv[3]);
                    testRange.secondRange = atof(argv[4]);

                    test->testsRanges.emplace_back(testRange);

                    return 0;
                },
                &test,
                0);
            string selectLabTestsProductsUsage = "SELECT * FROM lt_products_usage WHERE test_id = " + to_string(test.id) + ";";

            sqlite3_exec(
                dataSecond->first->db,
                selectLabTestsProductsUsage.c_str(),
                [](void *data, int argc, char **argv, char **colnames) -> int
                {
                    pair<Products *, LabTest *> *test = (pair<Products *, LabTest *> *)data;

                    Product product = test->first->getProduct(atoi(argv[2]));

                    test->second->productsUsage.emplace_back(product, atoi(argv[3]));
                    return 0;
                },
                &dataThird, 0);

            dataSecond->second.emplace_back(test);

            return 0;
        },
        &dataFirst,
        0);

    return dataFirst.second;
}

/**
 * Crea un examen del laboratorio en la base de datos
 */
int LabTests::createLabTest(string name, string description, string value, double price, products_usage_simple productsUsage, vector<test_ranges> testRanges)
{
    string insertLabTest = "INSERT INTO lab_tests (name, description, value, price) VALUES ('" + name + "', '" + description + "', '" + value + "', " + to_string(price) + ");";

    string selectLastId = "SELECT MAX(id) FROM lab_tests;";

    sqlite3_exec(this->db, insertLabTest.c_str(), 0, 0, 0);

    int lastId = 0;
    sqlite3_exec(
        this->db,
        selectLastId.c_str(),
        [](void *data, int argc, char **argv, char **colnames) -> int
        {
            int *lastId = (int *)data;
            *lastId = atoi(argv[0]);
            return 0;
        },
        &lastId,
        0);

    for (int i = 0; i < productsUsage.size(); i++)
    {
        string insertLabTestProductsUsage = "INSERT INTO lt_products_usage (test_id, product_id, amount) VALUES (" + to_string(lastId) + ", " + to_string(productsUsage[i].first) + ", " + to_string(productsUsage[i].second) + ");";
        sqlite3_exec(this->db, insertLabTestProductsUsage.c_str(), 0, 0, 0);
    }

    for (int i = 0; i < testRanges.size(); i++)
    {
        string insertLabTestRanges = "INSERT INTO lt_ranges (test_id, age, first_range, second_range) VALUES (" + to_string(lastId) + ", " + to_string(testRanges[i].age) + ", " + to_string(testRanges[i].firstRange) + ", " + to_string(testRanges[i].secondRange) + ");";
        sqlite3_exec(this->db, insertLabTestRanges.c_str(), 0, 0, 0);
    }

    LabTests::cantLabTests++;
    return lastId;
}

/**
 * Elimina un examen del laboratorio de la base de datos
 */
int LabTests::deleteLabTest(int id)
{
    string deleteLabTest = "DELETE FROM lab_tests WHERE id = " + to_string(id) + ";";
    sqlite3_exec(this->db, deleteLabTest.c_str(), 0, 0, 0);

    string deleteLabTestProductsUsage = "DELETE FROM lt_products_usage WHERE test_id = " + to_string(id) + ";";
    sqlite3_exec(this->db, deleteLabTestProductsUsage.c_str(), 0, 0, 0);

    string deleteLabTestRanges = "DELETE FROM lt_ranges WHERE test_id = " + to_string(id) + ";";
    sqlite3_exec(this->db, deleteLabTestRanges.c_str(), 0, 0, 0);

    LabTests::cantLabTests--;
    return id;
}

/**
 * Actualiza un examen del laboratorio en la base de datos
 * (No implementado)
 */
int LabTests::updateLabTest()
{
    return 0;
};

// MENU/VISTA OPERATIONS
void LabTests::menu()
{
    bool exit = false;
    string exitText = "\n\x1b[38;5;136m0. Salir al menu principal\nOtro. Continuar al menu de examenes\n\x1b[38;5;52mopcion: \x1b[0m";
    string options[5] = {
        "Ver examenenes del laboratorio",
        "Crear examenenes del laboratorio",
        "Actualizar examenenes del laboratorio",
        "Eliminar examenenes del laboratorio",
        "Regresar"};
    int optionsLen = sizeof(options) / sizeof(string);
    int selectedOption;

    do
    {

        cout << "\x1b[2J\x1b[0;0H\x1b[48;5;20m\nOperaciones con los examenes del laboratorio\x1b[0m\n\x1b[s";

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
                this->getLabTestsMenu();
                cout << exitText;
                cin >> goBack;
                if (goBack == 0)
                {
                    exit = true;
                }
                break;
            case 1:
                this->createLabTestMenu();
                cout << exitText;
                cin >> goBack;
                if (goBack == 0)
                {
                    exit = true;
                }
                break;
            case 2:
                this->updateLabTestMenu();
                cout << exitText;
                cin >> goBack;
                if (goBack == 0)
                {
                    exit = true;
                }
                break;
            case 3:
                this->deleteLabTestMenu();
                cout << exitText;
                cin >> goBack;
                if (goBack == 0)
                {
                    exit = true;
                }
                break;
            case 4:
                exit = true;
                break;
            }
        }
    } while (!exit);
}

/**
 * Muestra el menu de los examenes del laboratorio
 * Muestra todos los examenes del laboratorio
 */
void LabTests::getLabTestsMenu()
{
    cout << "\x1b[u\x1b[0J\x1b[48;5;20m\nExamenes del laboratorio\x1b[0m\n";
    vector<LabTest> tests = this->getLabTests();

    cout << "\x1b[0J\x1b[38;5;232m\x1b[48;5;111m" << setw(5) << left << "ID" << setw(25) << left << "Nombre" << setw(25) << left << "Descripcion" << setw(10) << left << "Precio" << setw(20) << "CANT. Examenes: " << this->cantLabTests << "\x1b[0m" << endl;

    for (int i = 0; i < tests.size(); i++)
    {
        cout << "\x1b[0J" << setw(5) << left << tests[i].id << setw(25) << left << tests[i].name << setw(25) << left << tests[i].description << setw(10) << left << tests[i].price << "\n\x1b[0m";

        cout << "\x1b[0J\t\t\x1b[48;5;20m" << setw(40) << right << "Rangos Normales del examen de " << right << tests[i].name << ": \n\x1b[0m";
        for (int j = 0; j < tests[i].testsRanges.size(); j++)
        {
            cout << "\x1b[0J" << setw(30) << right << "EDAD: " << tests[i].testsRanges[j].age << "\t" << tests[i].testsRanges[j].firstRange << " -> " << tests[i].testsRanges[j].secondRange << " " << tests[i].value << "\n";
        }
        cout << "\x1b[0J\t\t\x1b[48;5;20m" << setw(40) << "Productos usados en el examen de " << right << tests[i].name << ": \n\x1b[0m";

        for (int j = 0; j < tests[i].productsUsage.size(); j++)
        {
            cout << "\x1b[0J" << setw(30) << left << "\t" << tests[i].productsUsage[j].first.name << " -> " << tests[i].productsUsage[j].second << "\n";
        }
    }
}

/**
 * Crea un examen del laboratorio
 * El usuario debera ingresar los datos del examen
 * RANGOS NORMALES DEL EXAMEN
 * PRODUCTOS USADOS EN EL EXAMEN
 */
void LabTests::createLabTestMenu()
{
    string name, description, value;
    double price;

    products_usage_simple productsUsage = {};
    vector<test_ranges> testRanges = {};

    if (this->productsClass->cantProducts == 0)
    {
        cout << "\x1b[0J\x1b[38;5;124mNo hay productos en el inventario, no se puede crear un examen.\n\x1b[0m";
        return;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    cout << "\x1b[u\x1b[0J\x1b[48;5;50m\x1b[38;5;232mDatos del examen\x1b[0m" << endl;

    cout << "\x1b[0J\t\x1b[38;5;20mNombre del examen: \x1b[0m";
    getline(cin, name);
    cout << "\x1b[0J\t\x1b[38;5;20mDescripcion del examen: \x1b[0m";
    getline(cin, description);
    cout << "\x1b[0J\t\x1b[38;5;20mNotacion que tendra el examen: \x1b[0m";
    getline(cin, value);
    cout << "\x1b[0J\t\x1b[38;5;20mPrecio del examen: \x1b[0m";
    cin >> price;

    cout << "\x1b[48;5;50m\x1b[38;5;232mRangos normales segun la edad\x1b[0m\n\x1b[s";
    do
    {
        int age;
        float firtsRange, secondRange;

        cout << "\x1b[u";
        cout << "\x1b[0J\tEdad: ";
        cin >> age;

        if (age <= 0)
        {
            continue;
        }

        for (int i = 0; i < testRanges.size(); i++)
        {
            if (testRanges[i].age == age)
            {
                continue;
            }
        }

        cout << "\x1b[0J\t\x1b[38;5;20mRango inicial de la edad " << age << ": \x1b[0m";
        cin >> firtsRange;
        if (firtsRange <= 0)
        {
            continue;
        }
        cout << "\x1b[0J\t\x1b[38;5;20mRango final de la edad " << age << ": \x1b[0m";
        cin >> secondRange;

        if (secondRange <= 0)
        {
            continue;
        }

        test_ranges testRange = {age, firtsRange, secondRange};
        testRanges.emplace_back(testRange);

        cout << "\x1b[0JAgregar otro rango? (0.Si / 1.No): ";
        int continueLoop;
        cin >> continueLoop;
        if (continueLoop == 1)
        {
            break;
        }

    } while (true);

    cout << "\x1b[s\n\n";
    this->productsClass->getProductsMenu();
    cout << "\x1b[48;5;50m\x1b[38;5;232mProductos usados en el examen\x1b[0m\n\x1b[s";

    do
    {

        int productId;
        double amount;
        cout << "\x1b[u\x1b[0J\t\x1b[38;5;20mIngrese el CODIGO del producto que se usara en el examen: \x1b[0m";
        cin >> productId;

        Product product = this->productsClass->getProduct(productId);
        if (product.id == -1)
        {
            continue;
        }

        for (int i = 0; i < productsUsage.size(); i++)
        {
            if (productsUsage[i].first == productId)
            {
                continue;
            }
        }

        cout << "\x1b[0J\t\x1b[38;5;20mIngrese la cantidad de " << product.name << " que se usara en el examen: \x1b[0m";
        cin >> amount;

        if (amount <= 0)
        {
            continue;
        }
        productsUsage.emplace_back(productId, amount);

        cout << "\x1b[0JAgregar otro producto? (0.Si / 1.No): ";
        int continueLoop;
        cin >> continueLoop;
        if (continueLoop == 1)
        {
            break;
        }
    } while (true);

    this->createLabTest(name, description, value, price, productsUsage, testRanges);

    cout << "\x1b[0J\x1b[38;5;34mExamen creado correctamente.\n\n\x1b[0m";

    cout << left << "\x1b[48;5;81m\x1b[38;5;16m" << setw(30) << "NOMBRE" << setw(30) << "DESCRIPCION" << setw(30) << "PRECIO" << "\n\x1b[0m";
    cout << left << setw(30) << name << setw(30) << description << setw(30) << price << "\n\n";
    cout << "\x1b[48;5;20m\x1b[38;5;16mProductos Usados\x1b[0m\n";
    cout << left << setw(20) << "Nombre" << setw(20) << " ---> " << setw(20) << "Cantidad" << "\n";
    for (int i = 0; i < productsUsage.size(); i++)
    {
        Product product = this->productsClass->getProduct(productsUsage[i].first);

        cout << left << setw(20) << product.name << setw(20) << " ---> " << setw(20) << productsUsage[i].second << "\n";
    }
    cout << "\n\x1b[48;5;20m\x1b[38;5;16mRangos Normales del examen\x1b[0m\n";
    cout << left << setw(20) << "Edad" << setw(20) << "Rango Inicial" << setw(6) << " ---> " << setw(20) << "Rango Final\n";
    for (int i = 0; i < testRanges.size(); i++)
    {
        cout << left << setw(20) << testRanges[i].age << setw(20) << testRanges[i].firstRange << setw(6) << " ---> " << setw(20) << testRanges[i].secondRange << "\n";
    }
}

/**
 * Elimina un examen del laboratorio
 * El usuario debera ingresar el ID del examen a eliminar
 */
void LabTests::deleteLabTestMenu()
{
    cout << "\x1b[u\x1b[0J\x1b[48;5;50m\x1b[38;5;232mEliminar examen del laboratorio\x1b[0m" << endl;

    if (this->cantLabTests == 0)
    {
        cout << "\x1b[0J\x1b[38;5;124mNo hay examenes en el laboratorio, no se puede eliminar un examen.\n\x1b[0m";
        return;
    }

    this->getLabTestsMenu();

    int id;
    do
    {
        cout << "\x1b[0J\t\x1b[38;5;20mIngrese el ID del examen que desea eliminar: \x1b[0m";
        cin >> id;

        LabTest labTestFind = this->getLabTest(id);
        if (labTestFind.id == -1)
        {
            continue;
        }
        else
        {
            break;
        }
    } while (true);

    this->deleteLabTest(id);
    cout << "\x1b[0J\x1b[38;5;34mExamen eliminado correctamente.\n\x1b[0m";
}

/**
 * Actualiza un examen del laboratorio
 * El usuario debera ingresar el ID del examen a actualizar
 * Luego, debera ingresar los nuevos datos del examen
 */
void LabTests::updateLabTestMenu()
{
    cout << "WASAA";
};