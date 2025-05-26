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
            Laboratory *self = (Laboratory *)data;
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
    this->labTests = new LabTests(this->db, this->labProducts, this->labPatients);
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
    delete this->labTests;
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
};