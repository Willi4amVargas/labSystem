#pragma once
#include <iostream>

using std::string;
typedef string str;

class Patients
{
private:
    str fullName;
    str ci;
    str email;
    str phone;

public:
    static int cantPatients;
    Patients(str fullName, str ci, str email, str phone);
    str getPatientFullName();
    str getPatientCI();
    str getPatientEmail();
    str getPatientPhone();
    void updatePatient(str fullName, str ci, str email, str phone);
};