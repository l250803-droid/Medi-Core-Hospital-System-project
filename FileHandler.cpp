#include "FileHandler.h"
#include <cstdio>
#include <fstream>
using namespace std;

// HELPER — reads one line from file
// returns true if line was read
// returns false if end of file
static bool readLine(ifstream& file, char* buffer, int maxLen) {
    int i = 0;
    char c;
    while (file.get(c)) {
        if (c == '\n') break;
        if (c == '\r') continue;    // handle windows line endings
        if (i < maxLen - 1) {
            buffer[i] = c;
            i++;
        }
    }
    buffer[i] = '\0';
    return i > 0 || !file.eof();
}

// HELPER — extracts one field from csv line
// stops at comma or end of string
// moves start position forward
static int extractField(const char* line, int start,
    char* field, int maxLen) {
    int i = 0;
    while (line[start] != ',' &&
        line[start] != '\0' &&
        i < maxLen - 1) {
        field[i] = line[start];
        i++;
        start++;
    }
    field[i] = '\0';
    if (line[start] == ',') start++;
    return start;
}


// HELPER — converts string to int manually

static int charToInt(const char* str) {
    int result = 0;
    int i = 0;
    while (str[i] >= '0' && str[i] <= '9') {
        result = result * 10 + (str[i] - '0');
        i++;
    }
    return result;
}

// HELPER — converts string to float manually

static float charToFloat(const char* str) {
    float result = 0;
    int i = 0;
    while (str[i] >= '0' && str[i] <= '9') {
        result = result * 10 + (str[i] - '0');
        i++;
    }
    if (str[i] == '.') {
        i++;
        float decimal = 0.1f;
        while (str[i] >= '0' && str[i] <= '9') {
            result = result + (str[i] - '0') * decimal;
            decimal = decimal * 0.1f;
            i++;
        }
    }
    return result;
}


// LdAdmin
// reads admin.txt
// format: admin_id,name,password
// creates Admin object and assigns to pointer

bool FileHandler::LdAdmin(Admin*& adn) {

    ifstream file("admin.txt");

    if (!file.is_open()) {
        return false;
    }

    char line[300];

    // read first line only — only one admin
    if (readLine(file, line, 300)) {

        char idStr[20];
        char name[100];
        char password[100];

        int pos = 0;
        pos = extractField(line, pos, idStr, 20);
        pos = extractField(line, pos, name, 100);
        pos = extractField(line, pos, password, 100);

        int id = charToInt(idStr);

        adn = new Admin(id, name, password);
    }

    file.close();
    return true;
}


// reads patients.txt
// loads all patients from patients.txt
// reads patient records line by line
// creates patient objects and stores them in storage

bool FileHandler::LdPatients(Storage<Patient>& storage) {

    ifstream file("patients.txt");

    if (file.is_open() == false) {
        return false;
    }

    char* line = new char[500];

    while (readLine(file, line, 500) == true) {

        // only process line if it is not empty
        if (line[0] != '\0') {

            char* idStr = new char[20];

            char* name = new char[100];

            char* ageStr = new char[10];

            char* genderStr = new char[5];

            char* contact = new char[20];

            char* password = new char[100];

            char* balanceStr = new char[20];

            int pos = 0;

            pos = extractField(line, pos, idStr, 20);

            pos = extractField(line, pos, name, 100);

            pos = extractField(line, pos, ageStr, 10);

            pos = extractField(line, pos, genderStr, 5);

            pos = extractField(line, pos, contact, 20);

            pos = extractField(line, pos, password, 100);

            pos = extractField(line, pos, balanceStr, 20);

            int id = charToInt(idStr);

            int age = charToInt(ageStr);

            char gender = genderStr[0];

            float balance = charToFloat(balanceStr);

            Patient p(
                id,
                name,
                age,
                gender,
                contact,
                password,
                balance
            );

            storage.add(p);

            delete[] idStr;

            delete[] name;

            delete[] ageStr;

            delete[] genderStr;

            delete[] contact;

            delete[] password;

            delete[] balanceStr;
        }
    }

    delete[] line;

    file.close();

    return true;
}

// reads doctors
// creates Doctor object for each line
// loads all doctors from doctors.txt
// reads one doctor record line by line
// creates doctor objects and stores them in storage
bool FileHandler::LdDoctors(Storage<Doctor>& storage) {

    ifstream file("doctors.txt");

    if (file.is_open() == false) {
        return false;
    }

    char* line = new char[500];

    while (readLine(file, line, 500) == true) {

        if (line[0] != '\0') {

            char* idStr = new char[20];

            char* name = new char[100];

            char* spec = new char[100];

            char* contact = new char[20];

            char* password = new char[100];

            char* feeStr = new char[20];

            int pos = 0;

            pos = extractField(line, pos, idStr, 20);

            pos = extractField(line, pos, name, 100);

            pos = extractField(line, pos, spec, 100);

            pos = extractField(line, pos, contact, 20);

            pos = extractField(line, pos, password, 100);

            pos = extractField(line, pos, feeStr, 20);

            int id = charToInt(idStr);

            float fee = charToFloat(feeStr);

            Doctor d(
                id,
                name,
                password,
                spec,
                contact,
                fee
            );

            storage.add(d);

            delete[] idStr;

            delete[] name;

            delete[] spec;

            delete[] contact;

            delete[] password;

            delete[] feeStr;
        }
    }

    delete[] line;

    file.close();

    return true;
}


// LdAppointments
// reads appointments.txt
// format: appointment_id,patient_id,doctor_id,date,time_slot,status
// creates Appointment object for each line

// loads all appointments from appointments.txt
// reads every line and creates appointment objects
// then stores them inside appointment storage

bool FileHandler::LdAppointments(Storage<Appointment>& storage) {

    ifstream file("appointments.txt");

    if (file.is_open() == false) {
        return false;
    }

    char* line = new char[500];

    while (readLine(file, line, 500) == true) {

        if (line[0] != '\0') {

            char* apptIdStr = new char[20];

            char* patIdStr = new char[20];

            char* docIdStr = new char[20];

            char* date = new char[20];

            char* timeSlot = new char[10];

            char* status = new char[20];

            int pos = 0;

            pos = extractField(line, pos, apptIdStr, 20);

            pos = extractField(line, pos, patIdStr, 20);

            pos = extractField(line, pos, docIdStr, 20);

            pos = extractField(line, pos, date, 20);

            pos = extractField(line, pos, timeSlot, 10);

            pos = extractField(line, pos, status, 20);

            int apptID = charToInt(apptIdStr);

            int patID = charToInt(patIdStr);

            int docID = charToInt(docIdStr);

            Appointment a(
                apptID,
                patID,
                docID,
                date,
                timeSlot,
                status
            );

            storage.add(a);

            delete[] apptIdStr;

            delete[] patIdStr;

            delete[] docIdStr;

            delete[] date;

            delete[] timeSlot;

            delete[] status;
        }
    }

    delete[] line;

    file.close();

    return true;
}
// LdBills
// reads bills.txt
// format: bill_id,patient_id,appointment_id,amount,status,date
// creates Bill object for each line

bool FileHandler::LdBills(Storage<Bill>& storage) {

    ifstream file("bills.txt");

    if (!file.is_open()) {
        return false;
    }

    char line[500];

    while (readLine(file, line, 500)) {

        if (line[0] == '\0') continue;

        char billIdStr[20];
        char patIdStr[20];
        char apptIdStr[20];
        char amountStr[20];
        char status[20];
        char date[20];

        int pos = 0;
        pos = extractField(line, pos, billIdStr, 20);
        pos = extractField(line, pos, patIdStr, 20);
        pos = extractField(line, pos, apptIdStr, 20);
        pos = extractField(line, pos, amountStr, 20);
        pos = extractField(line, pos, status, 20);
        pos = extractField(line, pos, date, 20);

        int billID = charToInt(billIdStr);
        int patID = charToInt(patIdStr);
        int apptID = charToInt(apptIdStr);
        float amount = charToFloat(amountStr);

        Bill b(billID, patID, apptID, amount, status, date);
        storage.add(b);
    }

    file.close();
    return true;
}

// reads prescriptions
// creates Prescription object for each line
// loads all prescriptions from prescriptions.txt
// reads one line at a time and creates objects
// then adds them into prescription storage
bool FileHandler::LdPrescriptions(Storage<Prescription>& storage) {

    ifstream file("prescriptions.txt");

    if (file.is_open() == false) {
        return false;
    }

    char* line = new char[1000];

    while (readLine(file, line, 1000) == true) {

        if (line[0] != '\0') {

            char* presIdStr = new char[20];

            char* apptIdStr = new char[20];

            char* patIdStr = new char[20];

            char* docIdStr = new char[20];

            char* date = new char[20];

            char* medicines = new char[500];

            char* notes = new char[300];

            int pos = 0;

            pos = extractField(line, pos, presIdStr, 20);

            pos = extractField(line, pos, apptIdStr, 20);

            pos = extractField(line, pos, patIdStr, 20);

            pos = extractField(line, pos, docIdStr, 20);

            pos = extractField(line, pos, date, 20);

            pos = extractField(line, pos, medicines, 500);

            pos = extractField(line, pos, notes, 300);

            int presID = charToInt(presIdStr);

            int apptID = charToInt(apptIdStr);

            int patID = charToInt(patIdStr);

            int docID = charToInt(docIdStr);

            Prescription p(
                presID,
                apptID,
                patID,
                docID,
                date,
                medicines,
                notes
            );

            storage.add(p);

            delete[] presIdStr;

            delete[] apptIdStr;

            delete[] patIdStr;

            delete[] docIdStr;

            delete[] date;

            delete[] medicines;

            delete[] notes;
        }
    }

    delete[] line;

    file.close();

    return true;
}

// writes one security event into security_log.txt
// used for login attempts and security tracking

bool FileHandler::LogSecurityEvent(
    const char* timstmp,
    const char* role,
    int enterID,
    const char* result) {

    ofstream file("security_log.txt", ios::app);

    if (file.is_open() == false) {
        return false;
    }

    file << timstmp << ",";
    file << role << ",";
    file << enterID << ",";
    file << result;

    file << endl;

    file.close();

    return true;
}



// reads full security log file into dynamic char buffer
// later SFML can display this data

bool FileHandler::ReadSecurityLog(char*& outBuffer) {

    ifstream file("security_log.txt");

    if (file.is_open() == false) {
        return false;
    }

    int size = 0;

    char ch;

    while (file.get(ch)) {
        size++;
    }

    file.clear();

    file.seekg(0);

    outBuffer = new char[size + 1];

    int i = 0;

    while (file.get(ch)) {

        outBuffer[i] = ch;

        i++;
    }

    outBuffer[i] = '\0';

    file.close();

    return true;
}



// moves patient data into discharged.txt
// later patient records can be removed from active system

bool FileHandler::ArchivePatient(
    int patientID,
    Storage<Patient>& patients,
    Storage<Appointment>& appnts,
    Storage<Bill>& bil,
    Storage<Prescription>& presc) {

    ofstream file("discharged.txt", ios::app);

    if (file.is_open() == false) {
        return false;
    }

    Patient* allPatients = patients.getAll();

    int total = patients.getCount();

    bool found = false;

    for (int i = 0; i < total; i++) {

        if (allPatients[i].getID() == patientID) {

            file << allPatients[i].getID() << ",";
            file << allPatients[i].getName() << ",";
            file << allPatients[i].getAge() << ",";
            file << allPatients[i].getGender() << ",";
            file << allPatients[i].getContact() << ",";
            file << allPatients[i].getPassword() << ",";
            file << allPatients[i].getBalance();

            file << endl;

            found = true;
        }
    }

    file.close();

    return found;
}


// SvePatient
// adds one new patient record at end of patients.txt
// called when a new patient registers
bool FileHandler::SvePatient(const Patient& p) {

    ofstream file("patients.txt", ios::app);

    if (!file.is_open()) {
        return false;
    }

    file << p.getID() << ",";
    file << p.getName() << ",";
    file << p.getAge() << ",";
    file << p.getGender() << ",";
    file << p.getContact() << ",";
    file << p.getPassword() << ",";
    file << p.getBalance();

    file << endl;

    file.close();

    return true;
}



// SveDoctor
// adds one doctor record to doctors.txt
// called when admin adds new doctor

bool FileHandler::SveDoctor(const Doctor& d) {

    ofstream file("doctors.txt", ios::app);

    if (!file.is_open()) {
        return false;
    }

    file << d.getID() << ",";
    file << d.getName() << ",";
    file << d.getSpecialization() << ",";
    file << d.getContact() << ",";
    file << d.getPassword() << ",";
    file << d.getFee();

    file << endl;

    file.close();

    return true;
}


// SveAppointment
// saves one appointment into appointments.txt
// called when patient books appointment

bool FileHandler::SveAppointment(const Appointment& a) {

    ofstream file("appointments.txt", ios::app);

    if (!file.is_open()) {
        return false;
    }

    file << a.getAppointmentID() << ",";
    file << a.getPatientID() << ",";
    file << a.getDoctorID() << ",";
    file << a.getDate() << ",";
    file << a.getTimeSlot() << ",";
    file << a.getStatus();

    file << endl;

    file.close();

    return true;
}


// SveBill
// saves one bill into bills.txt
// called when bill is generated

bool FileHandler::SveBill(const Bill& b) {

    ofstream file("bills.txt", ios::app);

    if (!file.is_open()) {
        return false;
    }

    file << b.getID() << ",";
    file << b.getPatientID() << ",";
    file << b.getAppointmentID() << ",";
    file << b.getAmount() << ",";
    file << b.getStatus() << ",";
    file << b.getDate();

    file << endl;

    file.close();

    return true;
}



// SvePrescription
// saves prescription into prescriptions.txt
// called when doctor writes prescription
bool FileHandler::SvePrescription(const Prescription& p) {

    ofstream file("prescriptions.txt", ios::app);

    if (!file.is_open()) {
        return false;
    }

    file << p.getPrescriptionID() << ",";
    file << p.getAppointmentID() << ",";
    file << p.getPatientID() << ",";
    file << p.getDoctorID() << ",";
    file << p.getDate() << ",";
    file << p.getMedicines() << ",";
    file << p.getNotes();

    file << endl;

    file.close();

    return true;
}

// UPdtPatient
// updates one patient record in patients.txt
// used when patient data changes

bool FileHandler::UPdtPatient(const Patient& p) {

    ifstream file("patients.txt");

    ofstream temp("temp.txt");

    if (!file.is_open() || !temp.is_open()) {
        return false;
    }

    Patient current;

    while (true) {

        int id;
        char name[100];
        int age;
        char gender;
        char contact[100];
        char password[100];
        float balance;

        file >> id;
        file.ignore();

        file.getline(name, 100, ',');

        file >> age;
        file.ignore();

        file >> gender;
        file.ignore();

        file.getline(contact, 100, ',');

        file.getline(password, 100, ',');

        file >> balance;

        if (file.fail()) {
            break;
        }

        file.ignore();

        if (id == p.getID()) {

            temp << p.getID() << ",";
            temp << p.getName() << ",";
            temp << p.getAge() << ",";
            temp << p.getGender() << ",";
            temp << p.getContact() << ",";
            temp << p.getPassword() << ",";
            temp << p.getBalance();
        }
        else {

            temp << id << ",";
            temp << name << ",";
            temp << age << ",";
            temp << gender << ",";
            temp << contact << ",";
            temp << password << ",";
            temp << balance;
        }

        temp << endl;
    }

    file.close();
    temp.close();

    remove("patients.txt");

    rename("temp.txt", "patients.txt");

    return true;
}


// UPdtAppointment
// updates appointment record
// mainly used for status changes

bool FileHandler::UPdtAppointment(const Appointment& a) {

    ifstream file("appointments.txt");

    ofstream temp("temp.txt");

    if (!file.is_open() || !temp.is_open()) {
        return false;
    }

    while (true) {

        int appID;
        int patientID;
        int doctorID;

        char date[100];
        char slot[100];
        char status[100];

        file >> appID;
        file.ignore();

        file >> patientID;
        file.ignore();

        file >> doctorID;
        file.ignore();

        file.getline(date, 100, ',');

        file.getline(slot, 100, ',');

        file.getline(status, 100);

        if (file.fail()) {
            break;
        }

        if (appID == a.getAppointmentID()) {

            temp << a.getAppointmentID() << ",";
            temp << a.getPatientID() << ",";
            temp << a.getDoctorID() << ",";
            temp << a.getDate() << ",";
            temp << a.getTimeSlot() << ",";
            temp << a.getStatus();
        }
        else {

            temp << appID << ",";
            temp << patientID << ",";
            temp << doctorID << ",";
            temp << date << ",";
            temp << slot << ",";
            temp << status;
        }

        temp << endl;
    }

    file.close();
    temp.close();

    remove("appointments.txt");

    rename("temp.txt", "appointments.txt");

    return true;
}



// UPdtBill
// updates bill record
// mainly used when payment status changes

bool FileHandler::UPdtBill(const Bill& b) {

    ifstream file("bills.txt");

    ofstream temp("temp.txt");

    if (!file.is_open() || !temp.is_open()) {
        return false;
    }

    while (true) {

        int billID;
        int patientID;
        int appID;

        float amount;

        char status[100];
        char date[100];

        file >> billID;
        file.ignore();

        file >> patientID;
        file.ignore();

        file >> appID;
        file.ignore();

        file >> amount;
        file.ignore();

        file.getline(status, 100, ',');

        file.getline(date, 100);

        if (file.fail()) {
            break;
        }

        if (billID == b.getID()) {

            temp << b.getID() << ",";
            temp << b.getPatientID() << ",";
            temp << b.getAppointmentID() << ",";
            temp << b.getAmount() << ",";
            temp << b.getStatus() << ",";
            temp << b.getDate();
        }
        else {

            temp << billID << ",";
            temp << patientID << ",";
            temp << appID << ",";
            temp << amount << ",";
            temp << status << ",";
            temp << date;
        }

        temp << endl;
    }

    file.close();
    temp.close();

    remove("bills.txt");

    rename("temp.txt", "bills.txt");

    return true;
}





// removes patient line from patients.txt
// used when patient is discharged
// removes one patient record from patients.txt
// copies all remaining patients into temp file
// then replaces original file

bool FileHandler::DelPatient(int patientID) {

    ifstream file("patients.txt");

    ofstream tempFile("temp.txt");

    if (file.is_open() == false || tempFile.is_open() == false) {
        return false;
    }

    bool found = false;

    while (file.eof() == false) {

        int id;

        char* name = new char[100];

        int age;
        char gender;

        char* contact = new char[100];

        char* password = new char[100];

        float balance;

        file >> id;
        file.ignore();

        file.getline(name, 100, ',');

        file >> age;
        file.ignore();

        file >> gender;
        file.ignore();

        file.getline(contact, 100, ',');

        file.getline(password, 100, ',');

        file >> balance;

        if (file.fail() == true) {

            delete[] name;
            delete[] contact;
            delete[] password;

            break;
        }

        file.ignore();

        // only write patient if ids do not match
        if (id != patientID) {

            tempFile << id << ",";
            tempFile << name << ",";
            tempFile << age << ",";
            tempFile << gender << ",";
            tempFile << contact << ",";
            tempFile << password << ",";
            tempFile << balance;

            tempFile << endl;
        }
        else {

            found = true;
        }

        delete[] name;
        delete[] contact;
        delete[] password;
    }

    file.close();
    tempFile.close();

    int removeResult = remove("patients.txt");

    if (removeResult != 0) {
        return false;
    }

    int renameResult = rename("temp.txt", "patients.txt");

    if (renameResult != 0) {
        return false;
    }

    return found;
}



// removes one doctor record from doctors.txt
// copies all remaining doctors into temp file
// then replaces original file

bool FileHandler::DelDoctor(int docID) {

    ifstream file("doctors.txt");

    ofstream tempFile("temp.txt");

    if (file.is_open() == false || tempFile.is_open() == false) {
        return false;
    }

    bool found = false;

    while (file.eof() == false) {

        int id;

        char* name = new char[100];

        char* spec = new char[100];

        char* contact = new char[100];

        char* password = new char[100];

        float fee;

        file >> id;
        file.ignore();

        file.getline(name, 100, ',');

        file.getline(spec, 100, ',');

        file.getline(contact, 100, ',');

        file.getline(password, 100, ',');

        file >> fee;

        if (file.fail() == true) {

            delete[] name;
            delete[] spec;
            delete[] contact;
            delete[] password;

            break;
        }

        file.ignore();

        // only write doctor if ids do not match
        if (id != docID) {

            tempFile << id << ",";
            tempFile << name << ",";
            tempFile << spec << ",";
            tempFile << contact << ",";
            tempFile << password << ",";
            tempFile << fee;

            tempFile << endl;
        }
        else {

            found = true;
        }

        delete[] name;
        delete[] spec;
        delete[] contact;
        delete[] password;
    }

    file.close();
    tempFile.close();

    int removeResult = remove("doctors.txt");

    if (removeResult != 0) {
        return false;
    }

    int renameResult = rename("temp.txt", "doctors.txt");

    if (renameResult != 0) {
        return false;
    }

    return found;
}