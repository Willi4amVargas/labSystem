#include "include/Patients.h"

int Patients::cantPatients = 0;

Patients::Patients(str fullName, str ci, str email, str phone)
{
    this->fullName = fullName;
    this->ci = ci;
    this->email = email;
    this->phone = phone;
    Patients::cantPatients += 1;
}

str Patients::getPatientFullName()
{
    return this->fullName;
}

str Patients::getPatientCI()
{
    return this->ci;
}

str Patients::getPatientEmail()
{
    return this->email;
}

str Patients::getPatientPhone()
{
    return this->phone;
}

void Patients::updatePatient(str fullName, str ci, str email, str phone)
{
    this->fullName = fullName;
    this->ci = ci;
    this->email = email;
    this->phone = phone;
}