#include "Validator.h"



// checks if id is greater than 0
bool Validator::CheckIDValidity(int i) {

    if (i > 0) {
        return true;
    }

    return false;
}




// checks format DD-MM-YYYY and valid range of the written char arr
bool Validator::CheckDateValidity(const char* d) {

    // finding  length
    int len = 0;
    int i = 0;

    while (d[i] != '\0') {
        len = len + 1;
        i = i + 1;
    }


    if (len != 10) {
        return false;
    }

    //  checking  dashes 
    if (d[2] != '-') {
        return false;
    }

    if (d[5] != '-') {
        return false;
    }

    //each charactar is checked one by one 

    for (int i = 0; i < 10; i = i + 1) {

        // dash positions checks applying here

        if (i == 2) {
            
        }
        else if (i == 5) {
          
        }
        else {

            if (d[i] < '0' || d[i] > '9') {
                return false;
            }

        }
    }

    // working for the  day
    int d1 = d[0] - '0';

    int d2 = d[1] - '0';


    int day = d1 * 10 + d2;

    //  getting the  month of appointment 
    int m1 = d[3] - '0';
    int m2 = d[4] - '0';
    int month = m1 * 10 + m2;

    // getting the  year
    int y1 = d[6] - '0';

    int y2 = d[7] - '0';



    int y3 = d[8] - '0';



    int y4 = d[9] - '0';

    int year = y1 * 1000 + y2 * 100 + y3 * 10 + y4;

    //  checking  ranges if dne correctly
    if (day < 1) {
        return false;
    }

    if (day > 31) {
        return false;
    }

    if (month < 1) {
        return false;
    }

    if (month > 12) {
        return false;
    }

    //  get current year 
    time_t t;
    t = time(0);
    tm noww;
    localtime_s(&noww, &t);

    int currentYear = noww.tm_year + 1900;

    // compar year number
    if (year < currentYear) {


        return false;

    }

   
    return true;
}
// CheckValidTimeSlot  checks if slot is one of the 8 times that we have
bool Validator::CheckValidTimeSlot(const char* s) {

    // list of valid time slots
    const char* validSlots[8] = {
        "09:00", "10:00", "11:00", "12:00",
        "13:00", "14:00", "15:00", "16:00"
    };

    for (int k = 0; k < 8; k = k + 1) {
        const char* valid = validSlots[k];
        int i = 0;
        bool same = true;



        // char arr comparisino done here 
        while (valid[i] != '\0' && s[i] != '\0') {
            if (valid[i] != s[i]) {
                same = false;
                break;
            }

            i = i + 1;
        }
        
        if (same == true) {

            if (valid[i] == '\0') {

                if (s[i] == '\0') {
                    return true;
                }

            }




        }


    }
    return false;
}

// CheckValidFee must be greater than 0

bool Validator::CheckValidFee(float f) {

    if (f > 0) {
        return true;
    }

    return false;
}



// CheckValidContact must be exactly 11 digits

bool Validator::CheckValidContact(const char* c) {

    int len = 0;

    while (c[len] != '\0') {
        len++;
    }

    if (len != 11) {
        return false;
    }

    for (int i = 0; i < len; i++) {

        if (c[i] < '0' || c[i] > '9') {
            return false;
        }
    }

    return true;
}



// CheckValidPassword must be at least 6 characters

bool Validator::CheckValidPassword(const char* pass) {

    int len = 0;

    while (pass[len] != '\0') {
        len++;
    }

    if (len >= 6) {
        return true;
    }

    return false;
}



// CheckValidAmount must be greater than 0
bool Validator::CheckValidAmount(float amnt) {

    if (amnt > 0) {
        return true;
    }

    return false;
}



// CheckValidMenuChoice must be within given range

bool Validator::CheckValidMenuChoice(int choice, int min, int max) {

    if (choice >= min && choice <= max) {
        return true;
    }

    return false;
}



// CheckValidGender only M or F allowed

bool Validator::CheckValidGender(char gend) {

    if (gend == 'M' || gend == 'F') {
        return true;
    }

    return false;
}



// CheckValidAge realistic human age

bool Validator::CheckValidAge(int age) {

    if (age > 0 && age < 120) {
        return true;
    }

    return false;
}



// ToLowerCase converts uppercase to lowercase manually

char Validator::ToLowerCase(char c) {

    char result = c;
    if (c >= 'A') {
        if (c <= 'Z') {
            result = c + 32;
        }
    }
    return result;
}


// Match compares two strings ignoring case

bool Validator::Match(const char* a, const char* b) {

    int i = 0;

    while (a[i] != '\0' && b[i] != '\0') {

        if (ToLowerCase(a[i]) != ToLowerCase(b[i])) {
            return false;
        }

        i++;
    }

    if (a[i] == '\0' && b[i] == '\0') {
        return true;
    }

    return false;
}