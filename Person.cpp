#include "Person.h"


int myLen(const char* str) {
    int len = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        len++;
    }

    return len;
}
Person::Person(int i, const char* n, const char* p) {
    id = i;

 
    int len1 = myLen(n);
    name = new char[len1 + 1];

    for (int i = 0; i < len1; i++) {
        name[i] = n[i];
    }
    name[len1] = '\0';

    
    int len2 = myLen(p);
    password = new char[len2 + 1];

    for (int i = 0; i < len2; i++) {
        password[i] = p[i];
    }
    password[len2] = '\0';
}


Person::Person(const Person& rhs) {
    id = rhs.id;

  
    int len1 = myLen(rhs.name);
    name = new char[len1 + 1];

    for (int i = 0; i < len1; i++) {
        name[i] = rhs.name[i];
    }
    name[len1] = '\0';

    int len2 = myLen(rhs.password);
    password = new char[len2 + 1];

    for (int i = 0; i < len2; i++) {
        password[i] = rhs.password[i];
    }
    password[len2] = '\0';
}
Person::~Person() {
    if (name != nullptr) {
        delete[] name;
    }

    if (password != nullptr) {
        delete[] password;
    }



}

int Person::getID() const {
    return id;
}

const char* Person::getName() const {
    return name;
}


bool Person::checkPassword(const char* in) const {

    int i = 0;
    while (password[i] != '\0' && in[i] != '\0') {

        if (password[i] != in[i]) {
            return false;
        }

        i++;
    }
    if (password[i] == '\0' && in[i] == '\0') {
        return true;
    }
    else {
        return false;
    }

}
Person& Person::operator=(const Person& rhs) {
    if (this == &rhs) {
        return *this;
    }

    delete[] name;
    delete[] password;

    id = rhs.id;

    int len1 = myLen(rhs.name);
    name = new char[len1 + 1];
    for (int i = 0; i < len1; i++) {
        name[i] = rhs.name[i];
    }
    name[len1] = '\0';

    int len2 = myLen(rhs.password);
    password = new char[len2 + 1];
    for (int i = 0; i < len2; i++) {
        password[i] = rhs.password[i];
    }
        
    
    password[len2] = '\0';

    return *this;
}

const char* Person::getPassword() const {

    return password;
}