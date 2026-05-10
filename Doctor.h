#pragma once
#include "Person.h"
#include <iostream>

class Doctor : public Person {
private:
    char* specialization;
    char* contact;   
    float fee;

public:
    Doctor(int i, const char* n, const char* p,
        const char* s, const char* c, float f);

    Doctor(const Doctor& rhs);
    Doctor();
    Doctor& operator=(const Doctor& rhs);

    const char* getRole() const;

    friend std::ostream& operator<<(std::ostream& out, const Doctor& obj);


    bool operator==(const Doctor& other) const;
    void setSpecialization(const char* s);
    const char* getSpecialization() const;

    void setContact(const char* c);
    const char* getContact() const;

    void setFee(float f);
    float getFee() const;

    void displayMenu();

    ~Doctor();
};