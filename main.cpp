#include <iostream>
#include "laboratory.h"
#include "include/Products.h"

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::string;

Laboratory *lab = new Laboratory();

int main()
{
    mainMenu();
    delete lab;
}

void mainMenu()
{
    bool exit = false;
    string exitText = "\n\x1b[38;5;136m0. Salir del programa\nOtro. Continuar al menu\n\x1b[38;5;52mopcion: \x1b[0m";

    string options[6] = {
        "Ver Datos del laboratorio",
        "Operaciones con productos",
        "Operaciones con los pacientes",
        "Operaciones de inventario",
        "Operaciones con los examenes",
        "Salir"};
    int optionsLen = sizeof(options) / sizeof(string);
    int selectedOption;

    do
    {
        cout << "\x1b[2J\x1b[0;0H\x1b[48;5;20m\nBienvenido a el Programa de Gestion de Laboratorios\x1b[0m\n\x1b[s";

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
                lab->labProducts->menu();
                break;
            case 2:
                lab->labPatients->menu();
                break;
            case 3:
                lab->labInventoryOperations->menu();
                break;
            case 4:
                cout << "\x1b[38;5;124mEsta funcion no esta implementada\x1b[0m\n";
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

// void testsMenu()
// {
//     bool exit = false;
//     string exitText = "\n\x1b[38;5;136m0. Salir al menu principal\nOtro. Continuar al menu de examenes\n\x1b[38;5;52mopcion: \x1b[0m";
//     string options[5] = {
//         "Ver examenenes del laboratorio",
//         "Crear examenenes del laboratorio",
//         "Actualizar examenenes del laboratorio",
//         "Eliminar examenenes del laboratorio",
//         "Regresar"};
//     int optionsLen = sizeof(options) / sizeof(string);
//     int selectedOption;

//     do
//     {

//         cout << "\x1b[2J\x1b[0;0H\x1b[48;5;20m\nOperaciones con los examenes del laboratorio\x1b[0m\n\x1b[s";

//         cout << "\x1b[u";

//         for (int i = 0; i < optionsLen; i++)
//         {
//             cout << i << ". " << options[i] << "\x1b[0J\n";
//         }
//         cout << "\x1b[38;5;33mopcion: \x1b[0m";
//         cin >> selectedOption;

//         if (selectedOption <= optionsLen - 1)
//         {
//             int goBack;
//             switch (selectedOption)
//             {
//             case 0:
//                 lab->getLabTests();
//                 cout << exitText;
//                 cin >> goBack;
//                 if (goBack == 0)
//                 {
//                     exit = true;
//                 }
//                 break;
//             case 1:
//                 lab->createLabTest();
//                 cout << exitText;
//                 cin >> goBack;
//                 if (goBack == 0)
//                 {
//                     exit = true;
//                 }
//                 break;
//             case 2:
//                 lab->updateLabTest();
//                 cout << exitText;
//                 cin >> goBack;
//                 if (goBack == 0)
//                 {
//                     exit = true;
//                 }
//                 break;
//             case 3:
//                 lab->deleteLabTest();
//                 cout << exitText;
//                 cin >> goBack;
//                 if (goBack == 0)
//                 {
//                     exit = true;
//                 }
//                 break;
//             case 4:
//                 exit = true;
//                 break;
//             }
//         }
//     } while (!exit);
// }

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