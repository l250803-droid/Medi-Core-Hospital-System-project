#include "Doctor.h"
using namespace std;

// length calcuationd
int DoctorLen(const char* str) {
    int len = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        len++;
    }
    return len;
}

// Constructor
Doctor::Doctor(int i, const char* n, const char* p,
    const char* s, const char* c, float f)
    : Person(i, n, p) {

    // specialization implemented 
    int len1 = DoctorLen(s);
    specialization = new char[len1 + 1];

    for (int j = 0; j < len1; j++) {
        specialization[j] = s[j];
    }
    specialization[len1] = '\0';

    // contact implemented 
    int len2 = DoctorLen(c);
    contact = new char[len2 + 1];

    for (int j = 0; j < len2; j++) {
        contact[j] = c[j];
    }
    contact[len2] = '\0';

    fee = f;
}

Doctor::Doctor() : Person(0, "", "") {
    specialization = new char[1];
    specialization[0] = '\0';
    contact = new char[1];
    contact[0] = '\0';
    fee = 0;
}


// Copy portion
Doctor::Doctor(const Doctor& rhs)
    : Person(rhs) {

    int len1 = DoctorLen(rhs.specialization);
    specialization = new char[len1 + 1];

    for (int j = 0; j < len1; j++) {
        specialization[j] = rhs.specialization[j];
    }
    specialization[len1] = '\0';

    int len2 = DoctorLen(rhs.contact);
    contact = new char[len2 + 1];

    for (int j = 0; j < len2; j++) {
        contact[j] = rhs.contact[j];
    }
    contact[len2] = '\0';

    fee = rhs.fee;
}

// Assignment ope r  ator
Doctor& Doctor::operator=(const Doctor& rhs) {

    if (this == &rhs) {
        return *this;
    }

    Person::operator=(rhs);

    delete[] specialization;
    delete[] contact;

    int len1 = DoctorLen(rhs.specialization);
    specialization = new char[len1 + 1];

    for (int j = 0; j < len1; j++) {
        specialization[j] = rhs.specialization[j];
    }
    specialization[len1] = '\0';

    int len2 = DoctorLen(rhs.contact);
    contact = new char[len2 + 1];

    for (int j = 0; j < len2; j++) {
        contact[j] = rhs.contact[j];
    }
    contact[len2] = '\0';

    fee = rhs.fee;

    return *this;
}

// Destructor impmetnation
Doctor::~Doctor() {
    delete[] specialization;
    delete[] contact;
}

// Setters  
// 
// 
// 
// / Getters
void Doctor::setSpecialization(const char* s) {

    delete[] specialization;

    int len = DoctorLen(s);
    specialization = new char[len + 1];

    for (int j = 0; j < len; j++) {
        specialization[j] = s[j];
    }
    specialization[len] = '\0';
}

const char* Doctor::getSpecialization() const {
    return specialization;
}

void Doctor::setContact(const char* c) {

    delete[] contact;

    int len = DoctorLen(c);
    contact = new char[len + 1];

    for (int j = 0; j < len; j++) {
        contact[j] = c[j];
    }
    contact[len] = '\0';
}

const char* Doctor::getContact() const {
    return contact;
}

void Doctor::setFee(float f) {
    fee = f;
}

float Doctor::getFee() const {
    return fee;
}  

// Overridig as per requimrent
void Doctor::displayMenu() {
    // logic later on to be impmented
}

const char* Doctor::getRole() const {
    return "Doctor";
}
bool Doctor::operator==(const Doctor& other) const {

    if (id == other.id) {
        return true;
    }
    else {
        return false;
    }

}

ostream& operator<<(ostream& out, const Doctor& obj ) {
    out << obj.id << "," << obj.name << ","
        << obj.specialization << "," << obj.contact
        << "," << obj.fee;
    return out;
}


