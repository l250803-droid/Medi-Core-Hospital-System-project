#pragma once
#include "Person.h"

class Patient : public Person {
private:
    int age;
    char gender;
    char* contact;
    float balance;

public:
    // default constructor 
    Patient();

    // parameterized constructor
    Patient(int i, const char* n, int age, char gend,
        const char* contact, const char* pass, float bal);

    // copy constructor
    Patient(const Patient& rhs);

    // assignment operator
    Patient& operator=(const Patient& rhs);

    // destructor
    ~Patient();

    // getters
    int getAge() const;
    char getGender() const;
    const char* getContact() const;
    float getBalance() const;

    // setter
    void setBalance(float amnt);

    // operator overloads
    Patient& operator+=(float amnt);
    Patient& operator-=(float amnt);
    bool operator==(const Patient& rhs) const;

    // friend std::ostream& operator<<(std::ostream& os, const Patient& p);
  

    // overrides from Person class dipslay and its rolw form person class 
    void displayMenu() override;
    const char* getRole() const override;
};