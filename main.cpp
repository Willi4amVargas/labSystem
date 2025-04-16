#include "laboratory.h"
#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using std::getline;

Laboratory *lab;

void menu()
{
    bool exit = false;
    str exitText = "\n\x1b[38;5;136m0. Salir\nOtro. Continuar al menu\n\x1b[38;5;52mopcion: \x1b[0m";

    str options[5] = {
        "Ver Datos del laboratorio",
        "Ver Productos del laboratorio",
        "Crear Producto para el laboratorio",
        "Actualizar Producto del laboratorio",
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
                lab->getLabProducts();
                cout << exitText;
                cin >> goBack;
                if (goBack == 0)
                {
                    exit = true;
                }
                break;
            case 2:
                lab->createProduct();
                cout << exitText;
                cin >> goBack;
                if (goBack == 0)
                {
                    exit = true;
                }
                break;
            case 3:
                lab->updateProduct();
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

int main()
{
    menu();
    delete lab;
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