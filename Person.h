#pragma once


class Person {
protected:
    int id;
    char* name;
    char* password;

public:
 
    Person(int i, const char* n, const char* p);
    Person(const Person& obj);
    virtual ~Person();
    int getID() const;
    const char* getPassword() const;
    const char* getName() const;
    bool checkPassword(const char* in) const;
    virtual void displayMenu() = 0;
    virtual const char* getRole() const = 0;
    Person& operator=(const Person& rhs);
};




















