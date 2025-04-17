#include <iostream>
#include "laboratory.h"
#include "include/Products.h"
#include "include/Patients.h"
#include <limits>

typedef std::string str;
typedef std::vector<Products> arrayProducts;
typedef std::vector<Patients> arrayPatients;

using std::cin;
using std::cout;
using std::endl;
using std::getline;

Laboratory::Laboratory(str labName, str labRif, str labPlace)
{
    this->labName = labName;
    this->labRif = labRif;
    this->labPlace = labPlace;
    this->labProducts = new arrayProducts();
    this->labPatients = new arrayPatients();
};

Laboratory::~Laboratory()
{
    cout << "\nLaboratorio " << this->labName << " Eliminado";
    delete labProducts;
}

void Laboratory::getLabData()
{
    cout << "\x1b[u\x1b[0J\tNOMBRE\t\t\tRIF\n"
         << "\x1b[0J\x1b[38;5;20m\t" << this->labName << "\t\t" << this->labRif << "\t\n"
         << "\x1b[0J\t\x1b[0mDIRECCIÓN: \x1b[38;5;76m" << this->labPlace << "\x1b[0m" << endl;
}

void Laboratory::getLabProducts()
{
    int productsSize = this->labProducts->size();
    arrayProducts &p = *(this->labProducts);
    cout << "\x1b[u\x1b[0J\x1b[48;5;111mCODIGO\tNOMBRE\tDESCRIPCION\tTOTAL DE PRODUCTOS: " << Products::cantProducts << "\n\x1b[0m";
    for (int i = 0; i < productsSize; i++)
    {
        cout << "\x1b[0J" << i << "\t\t\t" << p[i].getName() << "\t\t\t" << p[i].getDescription() << "\n";
    }
}

int Laboratory::getLabProductsSize()
{
    return this->labProducts->size();
}

void Laboratory::createProduct()
{

    str name, description;
    int code = this->labProducts->size();

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    cout << "\x1b[u\x1b[0m\x1b[0JNOMBRE DEL PRODUCTO: \x1b[38;5;76m";
    getline(cin, name);

    cout << "\x1b[0m\x1b[0JDESCRIPCION DEL PRODUCTO: \x1b[38;5;76m";
    getline(cin, description);

    this->labProducts->emplace_back(name, description);

    cout << "\n\x1b[0J---Producto--- \n\x1b[0J\x1b[38;5;33mCODIGO: " << code
         << "\n\x1b[0JNOMBRE: " << name << "\t\t\t\x1b[48;5;76m\x1b[38;5;255mCREADO\x1b[0m\x1b[38;5;33m"
         << "\n\x1b[0JDESCRIPCION: " << description << "\t\x1b[0m\n";
}

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

        arrayProducts &allProducts = *this->labProducts;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        str newName, newDescription;
        cout << "\n\x1b[0m\x1b[0JIngrese el nuevo nombre del producto: \x1b[38;5;33m";
        getline(cin, newName);

        cout << "\n\x1b[0m\x1b[0JIngrese la nueva descripcion del producto: \x1b[38;5;33m";
        getline(cin, newDescription);

        allProducts[codeToUpdate].updateProduct(newName, newDescription);
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
    cout << "\x1b[u\x1b[0J\x1b[48;5;111mCODIGO\tNOMBRE\tCEDULA\tEMAIL\tTELEFONO\t\tTOTAL DE PACIENTES: " << Products::cantProducts << "\n\x1b[0m";
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