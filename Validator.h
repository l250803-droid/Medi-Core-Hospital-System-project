#pragma once
#include <ctime>

class Validator {
public:

    // checks if id is valid
    static bool CheckIDValidity(int i);

    // checks date format DD-MM-YYYY and range
    static bool CheckDateValidity(const char* d);

    // checks if time slot is among total 8 valid slots
    static bool CheckValidTimeSlot(const char* s);

    // checks conatact length 11 digs
    static bool CheckValidContact(const char* c);

    // checks password>= 6 charac
    static bool CheckValidPassword(const char* pass);

    // checks  fee>0
    static bool CheckValidFee(float f);

    // checks if amount is >0
    static bool CheckValidAmount(float amnt);

    // checks if choice is in range
    static bool CheckValidMenuChoice(int choice, int min, int max);

    // checks M or F gender
    static bool CheckValidGender(char gend);

    // checks  age is possible or not 
    static bool CheckValidAge(int age);

    // char to lower case 
    static char ToLowerCase(char c);

    // compares two strings ignoring their upper or lower case stuff and done
    static bool Match(const char* a, const char* b);
};