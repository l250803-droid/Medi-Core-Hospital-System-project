#include "Patient.h"



static int PatientLen(const char* s) {
    int len = 0;
    while (s[len] != '\0') {
        len++;
    }
    return len;
}



static void PatientCopy(char* d, const char* s) {
    int i = 0;
    while (s[i] != '\0') {
        d[i] = s[i];
        i++;
    }
    d[i] = '\0';
}



Patient::Patient() : Person(0, "", "") {
    age = 0;
    gender = 'M';
    contact = new char[1];
    contact[0] = '\0';
    balance = 0;
}



Patient::Patient(int i, const char* n, int age, char gend,
    const char* contact, const char* pass, float bal)
    : Person(i, n, pass) {

    this->age = age;
    gender = gend;

    int len = PatientLen(contact);
    this->contact = new char[len + 1];
    PatientCopy(this->contact, contact);

    balance = bal;
}


Patient::Patient(const Patient& rhs) : Person(rhs) {

    age = rhs.age;
    gender = rhs.gender;
    balance = rhs.balance;

    int len = PatientLen(rhs.contact);
    contact = new char[len + 1];
    PatientCopy(contact, rhs.contact);
}



Patient& Patient::operator=(const Patient& rhs) {

    if (this == &rhs) {
        return *this;
    }

    Person::operator=(rhs);

    delete[] contact;

    age = rhs.age;
    gender = rhs.gender;
    balance = rhs.balance;

    int len = PatientLen(rhs.contact);
    contact = new char[len + 1];
    PatientCopy(contact, rhs.contact);

    return *this;
}


Patient::~Patient() {
    delete[] contact;
    contact = nullptr;
}


int Patient::getAge() const {
    return age;
}

char Patient::getGender() const {
    return gender;
}

const char* Patient::getContact() const {
    return contact;
}

float Patient::getBalance() const {
    return balance;
}



void Patient::setBalance(float amnt) {
    balance = amnt;
}



Patient& Patient::operator+=(float amnt) {
    balance = balance + amnt;
    return *this;
}



Patient& Patient::operator-=(float amnt) {
    balance = balance - amnt;
    return *this;
}



bool Patient::operator==(const Patient& rhs) const {
    if (id == rhs.id) {
        return true;
    }
    return false;
}


void Patient::displayMenu() {
    // SFML later on do it will
}


const char* Patient::getRole() const {
    return "Patient";
}