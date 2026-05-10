#include "Bill.h"



static int BillLen(const char* str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}


static void BillCopy(char* dest, const char* src) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}


static bool BillMatch(const char* a, const char* b) {
    int i = 0;

    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) {
            return false;
        }
        i++;
    }

    if (a[i] == '\0' && b[i] == '\0') {
        return true;
    }

    return false;
}


Bill::Bill() {
    billID = 0;
    patientID = 0;
    appointmentID = 0;
    amount = 0;

    status = new char[7]; //unpaid as start for the brginning

    BillCopy(status, "unpaid");

    date = new char[1];
    date[0] = '\0';
}


Bill::Bill(int bID, int pID, int aID,
    float amt, const char* stat, const char* d) {

    billID = bID;
    patientID = pID;
    appointmentID = aID;
    amount = amt;

    int len1 = BillLen(stat);
    status = new char[len1 + 1];
    BillCopy(status, stat);

    int len2 = BillLen(d);
    date = new char[len2 + 1];
    BillCopy(date, d);
}                                                  //do the getid shit first then move forward check that for all the 
                                                   // required classes

Bill::Bill(const Bill& rhs) {

    billID = rhs.billID;
    patientID = rhs.patientID;
    appointmentID = rhs.appointmentID;
    amount = rhs.amount;

    int len1 = BillLen(rhs.status);
    status = new char[len1 + 1];
    BillCopy(status, rhs.status);

    int len2 = BillLen(rhs.date);
    date = new char[len2 + 1];
    BillCopy(date, rhs.date);
}



Bill& Bill::operator=(const Bill& rhs) {

    if (this == &rhs) {
        return *this;
    }

    delete[] status;
    delete[] date;

    billID = rhs.billID;
    patientID = rhs.patientID;
    appointmentID = rhs.appointmentID;
    amount = rhs.amount;

    int len1 = BillLen(rhs.status);
    status = new char[len1 + 1];
    BillCopy(status, rhs.status);

    int len2 = BillLen(rhs.date);
    date = new char[len2 + 1];
    BillCopy(date, rhs.date);

    return *this;
}


Bill::~Bill() {
    delete[] status;
    delete[] date;
}


int Bill::getBillID() const {
    return billID;
}

int Bill::getPatientID() const {
    return patientID;
}

int Bill::getAppointmentID() const {
    return appointmentID;
}

float Bill::getAmount() const {
    return amount;
}

const char* Bill::getStatus() const {
    return status;
}

const char* Bill::getDate() const {
    return date;
}


void Bill::setStatus(const char* newStatus) {

    delete[] status;

    int len = BillLen(newStatus);
    status = new char[len + 1];
    BillCopy(status, newStatus);
}



bool Bill::isUnpaid() const {

    bool result = false;

    if (BillMatch(status, "unpaid")) {
        result = true;
    }

    return result;
}


bool Bill::isPaid() const {

    bool result = false;

    if (BillMatch(status, "paid")) {
        result = true;
    }

    return result;
}


bool Bill::belongsToPatient(int pid) const {

    bool result = false;

    if (patientID == pid) {
        result = true;
    }

    return result;
}


//  extracting  date parts from all of it 
static void extractDate(const char* d, int& day, int& month, int& year) {

    int d1 = d[0] - '0';
    int d2 = d[1] - '0';
    day = d1 * 10 + d2;

    int m1 = d[3] - '0';
    int m2 = d[4] - '0';
    month = m1 * 10 + m2;

    int y1 = d[6] - '0';
    int y2 = d[7] - '0';
    int y3 = d[8] - '0';
    int y4 = d[9] - '0';

    year = y1 * 1000 + y2 * 100 + y3 * 10 + y4;
}


bool Bill::isOverdue(const char* todayDate) const {

    int d1, m1, y1;
    int d2, m2, y2;

    extractDate(date, d1, m1, y1);
    extractDate(todayDate, d2, m2, y2);

   
    int total1 = y1 * 365 + m1 * 30 + d1;
    int total2 = y2 * 365 + m2 * 30 + d2;

    int diff = total2 - total1;

    if (diff > 7) {
        return true;
    }

    return false;
}
int Bill::getID() const {
    return billID;
}