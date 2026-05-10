#include "HospitalException.h"

// Converts a float value into string till 2dec places
static void floatToStr(float val, char* buff) {

    int i = 0;

    if (val < 0) {
        buff[i] = '-';
        i++;
        val = -val;
    }

    int intPart = (int)val;
    float decPart = val - intPart;

    char temp[50];
    int tempIndex = 0;

    if (intPart == 0) {
        temp[tempIndex] = '0';
        tempIndex++;
    }

    while (intPart > 0) {
        int digit = intPart % 10;
        temp[tempIndex] = '0' + digit;
        tempIndex++;
        intPart = intPart / 10;
    }

    for (int k = tempIndex - 1; k >= 0; k--) {
        buff[i] = temp[k];
        i++;
    }

    buff[i] = '.';
    i++;

    decPart = decPart * 100;
    int dec = (int)decPart;

    int firstDigit = dec / 10;
    int secondDigit = dec % 10;

    buff[i] = '0' + firstDigit;
    i++;

    buff[i] = '0' + secondDigit;
    i++;

    buff[i] = '\0';
}

// Copies one line to another  
static void HosExCopy(char* dest, const char* src) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

// length 
static int HospitalExceptionLen(const char* str) {
    int len = 0;
    while (str[len] != '\0') len++;
    return len;
}

// Retrns the exception indication message
const char* HospitalException::what() const {
    return message;
}


HospitalException::~HospitalException() {
    delete[] message;
    message = nullptr;
}

// gives message when file is not found
FileNotFoundException::FileNotFoundException(const char* fn)
    : HospitalException("") {
    char temp[200];
    const char* prefix = " not found the file: ";
    int i = 0;

    while (prefix[i] != '\0' && i < 199) {
        temp[i] = prefix[i];
        i++;
    }
    int j = 0;
    while (fn[j] != '\0' && i < 199) {
        temp[i] = fn[j];
        i++;
        j++;
    }
    temp[i] = '\0';

    delete[] message;
    int len = HospitalExceptionLen(temp);
    message = new char[len + 1];
    HosExCopy(message, temp);
}

//  message for  less money
InsufficientFundsException::InsufficientFundsException(float req, float avail)
    : HospitalException("") {
    char temp[200];
    int i = 0;

    const char* part1 = "Insufficient money as  required is: ";
    while (part1[i] != '\0' && i < 199) {
        temp[i] = part1[i];
        i++;
    }

    char numBuf[50];
    floatToStr(req, numBuf);
    int j = 0;
    while (numBuf[j] != '\0' && i < 199) {
        temp[i] = numBuf[j];
        i++; j++;
    }

    const char* part2 = ", avalable ";
    j = 0;
    while (part2[j] != '\0' && i < 199) {
        temp[i] = part2[j];
        i++; j++;
    }

    floatToStr(avail, numBuf);
    j = 0;
    while (numBuf[j] != '\0' && i < 199) {
        temp[i] = numBuf[j];
        i++; j++;
    }
    temp[i] = '\0';

    delete[] message;
    int len = HospitalExceptionLen(temp);
    message = new char[len + 1];
    HosExCopy(message, temp);
}

// message when slot already booked
SlotUnavailableException::SlotUnavailableException(const char* s)
    : HospitalException("") {
    char temp[200];
    const char* prefix = "Time slot ";
    const char* suffix = " is already used up";
    int i = 0;

    while (prefix[i] != '\0' && i < 199) {
        temp[i] = prefix[i];
        i++;
    }
    int j = 0;
    while (s[j] != '\0' && i < 199) {
        temp[i] = s[j];
        i++; j++;
    }
    j = 0;
    while (suffix[j] != '\0' && i < 199) {
        temp[i] = suffix[j];
        i++; j++;
    }
    temp[i] = '\0';

    delete[] message;
    int len = HospitalExceptionLen(temp);
    message = new char[len + 1];
    HosExCopy(message, temp);
}

//  initialize message
HospitalException::HospitalException(const char* m) {
    int len = HospitalExceptionLen(m);
    message = new char[len + 1];
    for (int i = 0; i <= len; i++) {
        message[i] = m[i];
    }
}

//  message for invalid input
InvalidInputException::InvalidInputException(const char* detail)
    : HospitalException("") {
    char temp[200];
    const char* prefix = "Invalid entry ";
    int i = 0;

    while (prefix[i] != '\0' && i < 199) {
        temp[i] = prefix[i];
        i++;
    }
    int j = 0;
    while (detail[j] != '\0' && i < 199) {
        temp[i] = detail[j];
        i++; j++;
    }
    temp[i] = '\0';

    delete[] message;
    int len = HospitalExceptionLen(temp);
    message = new char[len + 1];
    HosExCopy(message, temp);
}