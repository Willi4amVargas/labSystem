#pragma once
#include <iostream>
#include <vector>
#include <sqlite3.h>
#include <iomanip>
#include <limits>

using std::cin;
using std::cout;
using std::endl;
using std::left;
using std::setw;
using std::string;
using std::to_string;
using std::vector;

struct Patient
{
    int id;
    string fullName;
    string ci;
    string email;
    string phone;
};

class Patients
{
private:
    sqlite3 *db;

    vector<Patient> getPatients();
    Patient getPatient(int code);
    int createPatient(string fullName, string ci, string email, string phone);
    int updatePatient(int code, string fullName, string ci, string email, string phone);
    int deletePatient(int code);

public:
    static int cantPatients;
    Patients(sqlite3 *db);

    void menu();
    void getPatientsMenu();
    void getPatientMenu();
    void createPatientMenu();
    void updatePatientMenu();
    void deletePatientMenu();
};