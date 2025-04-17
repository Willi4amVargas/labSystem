#include "laboratory.h"
#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using std::getline;

Laboratory *lab;
void mainMenu();
void productsMenu();
void patiensMenu();

int main()
{
    mainMenu();
    delete lab;
}

void mainMenu()
{
    bool exit = false;
    str exitText = "\n\x1b[38;5;136m0. Salir del programa\nOtro. Continuar al menu\n\x1b[38;5;52mopcion: \x1b[0m";

    str options[4] = {
        "Ver Datos del laboratorio",
        "Operaciones con productos",
        "Operaciones con los pacientes",
        "Salir"};
    int optionsLen = sizeof(options) / sizeof(str);
    int selectedOption;

    do
    {
        cout << "\x1b[2J\x1b[0;0H\x1b[48;5;20m\nBienvenido a el Programa de Gestion de Laboratorios\x1b[0m\n\x1b[s";

        if (!lab)
        {
            str labName;
            str labRif;
            str labDir;
            cout << "INGRESE EL NOMBRE DEL LABORATORIO: ";
            getline(cin, labName);

            cout << "INGRESE EL RIF DEL LABORATORIO: ";
            getline(cin, labRif);

            cout << "INGRESE DIRECCION DEL LABORATORIO: ";
            getline(cin, labDir);

            lab = new Laboratory(labName, labRif, labDir);
            continue;
        }

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
                lab->getLabData();
                cout << exitText;
                cin >> goBack;
                if (goBack == 0)
                {
                    exit = true;
                }
                break;
            case 1:
                productsMenu();
                // cout << exitText;
                // cin >> goBack;
                // if (goBack == 0)
                // {
                //     exit = true;
                // }
                break;
            case 2:
                patiensMenu();
                // cout << exitText;
                // cin >> goBack;
                // if (goBack == 0)
                // {
                //     exit = true;
                // }
                break;
            case 3:
                exit = true;
                break;
            }
        }
    } while (!exit);
}

void productsMenu()
{
    bool exit = false;
    str exitText = "\n\x1b[38;5;136m0. Salir al menu principal\nOtro. Continuar al menu de productos\n\x1b[38;5;52mopcion: \x1b[0m";
    str options[5] = {
        "Ver Productos del laboratorio",
        "Crear Producto para el laboratorio",
        "Actualizar Producto del laboratorio",
        "Eliminar un Producto del laboratorio",
        "Regresar"};
    int optionsLen = sizeof(options) / sizeof(str);
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
                lab->getLabProducts();
                cout << exitText;
                cin >> goBack;
                if (goBack == 0)
                {
                    exit = true;
                }
                break;
            case 1:
                lab->createProduct();
                cout << exitText;
                cin >> goBack;
                if (goBack == 0)
                {
                    exit = true;
                }
                break;
            case 2:
                lab->updateProduct();
                cout << exitText;
                cin >> goBack;
                if (goBack == 0)
                {
                    exit = true;
                }
                break;
            case 3:
                lab->deleteProduct();
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

void patiensMenu()
{
    bool exit = false;
    str exitText = "\n\x1b[38;5;136m0. Salir al menu principal\nOtro. Continuar al menu de pacientes\n\x1b[38;5;52mopcion: \x1b[0m";
    str options[5] = {
        "Ver Pacientes del laboratorio",
        "Crear Pacientes para el laboratorio",
        "Actualizar Pacientes del laboratorio",
        "Eliminar un Pacientes del laboratorio",
        "Regresar"};
    int optionsLen = sizeof(options) / sizeof(str);
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
                lab->getLabPatients();
                cout << exitText;
                cin >> goBack;
                if (goBack == 0)
                {
                    exit = true;
                }
                break;
            case 1:
                lab->createPatient();
                cout << exitText;
                cin >> goBack;
                if (goBack == 0)
                {
                    exit = true;
                }
                break;
            case 2:
                lab->updatePatient();
                cout << exitText;
                cin >> goBack;
                if (goBack == 0)
                {
                    exit = true;
                }
                break;
            case 3:
                lab->deletePatient();
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
// #include <QApplication>
// #include <QLabel>
// int main(int argc, char *argv[])
// {
//     QApplication app(argc, argv);

//     QLabel ventana("HOLA!!");

//     ventana.resize(400, 300);
//     ventana.show();

//     return app.exec();
// }