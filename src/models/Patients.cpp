#include "include/Patients.h"

int Patients::cantPatients = 0;

Patients::Patients(sqlite3 *db)
{
    this->db = db;
    string createTable = "CREATE TABLE IF NOT EXISTS patients (id INTEGER PRIMARY KEY, full_name TEXT, ci TEXT, email TEXT, phone TEXT);";

    sqlite3_exec(this->db, createTable.c_str(), 0, 0, 0);

    string selectProducts = "SELECT * FROM patients;";

    sqlite3_exec(
        this->db,
        selectProducts.c_str(),
        [](void *data, int argc, char **argv, char **colnames) -> int
        {
            Patients::cantPatients++;
            return 0;
        },
        0,
        0);
}

// DATABASE OPERATIONS

/**
 * Obtiene todos los pacientes de la base de datos
 */
vector<Patient> Patients::getPatients()
{
    vector<Patient> patients = {};

    string getPatientsQuery = "SELECT * FROM patients;";

    sqlite3_exec(this->db, getPatientsQuery.c_str(), [](void *data, int argc, char **argv, char **colnames) -> int
                 {
            vector<Patient> *patients = (vector<Patient> *)data;
            
            patients->push_back({
                atoi(argv[0]),
                argv[1],
                argv[2],
                argv[3],
                argv[4],
            });

            return 0; }, &patients, 0);
    return patients;
};

/**
 * Obtiene y retorna los datos de un solo paciente
 */
Patient Patients::getPatient(int code)
{
    Patient patient = {-1, "", "", "", ""};

    string getPatientQuery = "SELECT * FROM patients WHERE id=" + to_string(code) + ";";

    sqlite3_exec(this->db, getPatientQuery.c_str(), [](void *data, int argc, char **argv, char **colnames) -> int
                 {
            Patient *patient = (Patient *)data;
            
            patient->id = atoi(argv[0]);
            patient->fullName = argv[1];
            patient->ci = argv[2];
            patient->email = argv[3];
            patient->phone = argv[4];

            return 0; }, &patient, 0);
    return patient;
};

/**
 * Crea un nuevo paciente en la base de datos
 */
int Patients::createPatient(string fullName, string ci, string email, string phone)
{
    int createCode = -1;
    string createPatient = "INSERT INTO patients(full_name, ci, email, phone) VALUES('" + fullName + "','" + ci + "','" + email + "','" + phone + "' );";

    sqlite3_exec(this->db, createPatient.c_str(), 0, 0, 0);

    string selectPatient = "SELECT MAX(id) FROM patients;";
    sqlite3_exec(this->db, selectPatient.c_str(), [](void *data, int argc, char **argv, char **colnames) -> int
                 {
                    int *createCode = (int *)data;
                    *createCode = atoi(argv[0]);
                    
                     return 0; }, &createCode, 0);
    this->cantPatients++;
    return createCode;
};

/**
 * Actualiza un paciente de la base de datos
 */
int Patients::updatePatient(int code, string fullName, string ci, string email, string phone)
{
    string updatePatientQuery = "UPDATE patients SET full_name='" + fullName + "', ci='" + ci + "', email='" + email + "', phone='" + phone + "' WHERE id=" + to_string(code) + ";";

    sqlite3_exec(this->db, updatePatientQuery.c_str(), 0, 0, 0);
    return code;
};

/**
 * Eliminar un paciente de la base de datos
 */
int Patients::deletePatient(int code)
{
    string deletePatientQuery = "DELETE FROM patients WHERE id = " + to_string(code);

    sqlite3_exec(this->db, deletePatientQuery.c_str(), 0, 0, 0);
    this->cantPatients--;
    return code;
};

// FUNCIONES DE MENU/VISTAS

/**
 * Menu principal de las operaciones de los pacientes
 */
void Patients::menu()
{
    bool exit = false;
    string exitText = "\n\x1b[38;5;136m0. Salir al menu principal\nOtro. Continuar al menu de pacientes\n\x1b[38;5;52mopcion: \x1b[0m";
    string options[6] = {
        "Ver Pacientes del laboratorio",
        "Buscar un paciente",
        "Crear Pacientes para el laboratorio",
        "Actualizar Pacientes del laboratorio",
        "Eliminar un Pacientes del laboratorio",
        "Regresar"};
    int optionsLen = sizeof(options) / sizeof(string);
    int selectedOption;

    do
    {

        cout << "\x1b[2J\x1b[0;0H\x1b[48;5;20m\nOperaciones de Pacientes\x1b[0m\n\x1b[s";

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
                this->getPatientsMenu();
                cout << exitText;
                cin >> goBack;
                if (goBack == 0)
                {
                    exit = true;
                }
                break;
            case 1:
                this->getPatientMenu();
                cout << exitText;
                cin >> goBack;
                if (goBack == 0)
                {
                    exit = true;
                }
                break;
            case 2:
                this->createPatientMenu();
                cout << exitText;
                cin >> goBack;
                if (goBack == 0)
                {
                    exit = true;
                }
                break;
            case 3:
                this->updatePatientMenu();
                cout << exitText;
                cin >> goBack;
                if (goBack == 0)
                {
                    exit = true;
                }
                break;
            case 4:
                this->deletePatientMenu();
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
};

/**
 * Menu/Vista para obtener todos los pacientes de la base de datos
 */
void Patients::getPatientsMenu()
{
    vector<Patient> patients = this->getPatients();

    cout << "\x1b[u\x1b[0J\x1b[48;5;111m" << setw(5) << left << "ID" << setw(25) << left << "Nombre" << setw(25) << left << "CI" << setw(25) << left << "Email" << setw(25) << left << "Telefono" << setw(25) << left << "Total de pacientes: " << this->cantPatients << "\x1b[0m" << endl;

    for (int i = 0; i < patients.size(); i++)
    {
        cout << "\x1b[0J" << setw(5) << left << patients[i].id << setw(25) << left << patients[i].fullName << setw(25) << left << patients[i].ci << setw(25) << left << patients[i].email << setw(25) << left << patients[i].phone << endl;
    }
};

/**
 * Menu/Vista para obtener un paciente
 */
void Patients::getPatientMenu()
{
    int patientCode;
    Patient patientData;
    this->getPatientsMenu();

    cout << endl
         << "Ingrese el codigo del paciente a buscar: ";

    do
    {
        cin >> patientCode;
        patientData = this->getPatient(patientCode);

        if (patientData.id != -1)
        {
            break;
        }
    } while (true);
    cout << setw(50) << "=====================\x1b[38;5;20mPaciente\x1b[0m=====================" << endl;
    cout << setw(50) << left << "Nombre: " << patientData.fullName << endl;
    cout << setw(50) << left << "CI: " << patientData.ci << endl;
    cout << setw(50) << left << "Email: " << patientData.email << endl;
    cout << setw(50) << left << "Phone: " << patientData.phone << endl;
    cout << setw(50) << "==================================================";
};

/**
 * Menu/Vista para crear un paciente
 */
void Patients::createPatientMenu()
{
    string fullName, ci, email, phone;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    cout << "\x1b[u\x1b[0m\x1b[0JNombre completo del paciente: \x1b[38;5;76m";
    getline(cin, fullName);

    cout << "\x1b[0m\x1b[0JCedula del paciente: \x1b[38;5;76m";
    getline(cin, ci);

    cout << "\x1b[0m\x1b[0JEmail del paciente: \x1b[38;5;76m";
    getline(cin, email);

    cout << "\x1b[0m\x1b[0JTelefono del paciente: \x1b[38;5;76m";
    getline(cin, phone);

    int patientNewCode = this->createPatient(fullName, ci, email, phone);

    cout << endl
         << "\x1b[0J\x1b[0m" << setw(60) << "==========================Paciente=========================="
         << endl
         << "\x1b[0J\x1b[38;5;33m" << setw(60) << left << "CODIGO: \x1b[0m" << patientNewCode << endl
         << "\x1b[0J\x1b[38;5;33m" << setw(60) << left << "NOMBRE: \x1b[0m" << fullName << endl
         << "\x1b[38;5;33m" << setw(60) << left << "CI: \x1b[0m" << ci << endl
         << "\x1b[38;5;33m" << setw(60) << left << "EMAIL: \x1b[0m" << phone << endl
         << endl
         << setw(60) << "===========================\x1b[38;5;40mCreado\x1b[0m===========================" << endl;
};

/**
 * Menu/Vista para actualizar un paciente
 */
void Patients::updatePatientMenu()
{
    if (this->cantPatients > 0)
    {
        int codeToUpdate;
        Patient patientFind;
        this->getPatientsMenu();
        cout << "\n\x1b[0J\x1b[38;5;166mIngrese el codigo del paciente a cambiar: \x1b[s\x1b[0m";
        do
        {
            cout << "\x1b[u\x1b[0J";
            cin >> codeToUpdate;

            patientFind = this->getPatient(codeToUpdate);

            if (patientFind.id != -1)
            {
                break;
            }
        } while (true);

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        string newFullName, newCi, newEmail, newPhone;
        cout << "\n\x1b[0m\x1b[0JIngrese el nuevo nombre del paciente: \x1b[38;5;33m";
        getline(cin, newFullName);

        cout << "\n\x1b[0m\x1b[0JIngrese la nueva cedula del paciente: \x1b[38;5;33m";
        getline(cin, newCi);

        cout << "\n\x1b[0m\x1b[0JIngrese el nuevo email del paciente: \x1b[38;5;33m";
        getline(cin, newEmail);

        cout << "\n\x1b[0m\x1b[0JIngrese el nuevo telefono del paciente: \x1b[38;5;33m";
        getline(cin, newPhone);

        this->updatePatient(codeToUpdate, newFullName, newCi, newEmail, newPhone);

        cout << endl
             << "\x1b[0J\x1b[0m" << setw(60) << "==========================Paciente=========================="
             << endl
             << "\x1b[0J\x1b[38;5;33m" << setw(60) << left << "CODIGO: \x1b[0m" << patientFind.id << endl
             << "\x1b[0J\x1b[38;5;33m" << setw(60) << left << "NOMBRE: \x1b[0m" << newFullName << endl
             << "\x1b[38;5;33m" << setw(60) << left << "CI: \x1b[0m" << newCi << endl
             << "\x1b[38;5;33m" << setw(60) << left << "EMAIL: \x1b[0m" << newEmail << endl
             << endl
             << setw(60) << "=========================\x1b[38;5;20mActualizado\x1b[0m========================" << endl;
    }
    else
    {
        cout << "\n\x1b[38;5;52mNo hay pacientes registrados \x1b[0mε(´סּ︵סּ`)з\n";
    }
};

/**
 * Menu/Vista para eliminar un paciente
 */
void Patients::deletePatientMenu()
{
    if (this->cantPatients > 0)
    {
        int codeToDelete;
        Patient patientFind;
        this->getPatientsMenu();
        cout << "\n\x1b[0J\x1b[38;5;166mIngrese el codigo del paciente a eliminar: \x1b[s\x1b[0m";
        do
        {
            cout << "\x1b[u\x1b[0J";
            cin >> codeToDelete;
            patientFind = this->getPatient(codeToDelete);

            if (patientFind.id != -1)
            {
                break;
            }
        } while (true);

        this->deletePatient(codeToDelete);
        cout << endl
             << "\x1b[0J\x1b[0m" << setw(60) << "==========================Paciente=========================="
             << endl
             << "\x1b[0J\x1b[38;5;33m" << setw(60) << left << "CODIGO: \x1b[0m" << patientFind.id << endl
             << "\x1b[0J\x1b[38;5;33m" << setw(60) << left << "NOMBRE: \x1b[0m" << patientFind.fullName << endl
             << "\x1b[38;5;33m" << setw(60) << left << "CI: \x1b[0m" << patientFind.ci << endl
             << "\x1b[38;5;33m" << setw(60) << left << "EMAIL: \x1b[0m" << patientFind.email << endl
             << endl
             << setw(60) << "=========================\x1b[38;5;160mEliminado\x1b[0m==========================" << endl;
    }
    else
    {
        cout << "\n\x1b[38;5;52mNo hay pacientes registrados \x1b[0mε(´סּ︵סּ`)з\n";
    }
};