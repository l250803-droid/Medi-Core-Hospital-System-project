#include "Prescription.h"



static int PresLen(const char* str) {
    int len = 0;

    while (str[len] != '\0') {
        len = len + 1;
    }

    return len;
}



static void PresCopy(char* dest, const char* src) {
    int i = 0;

    while (src[i] != '\0') {
        dest[i] = src[i];
        i = i + 1;
    }

    dest[i] = '\0';
}



Prescription::Prescription() {

    prescriptionID = 0;
    appointmentID = 0;
    patientID = 0;
    docID = 0;

    date = new char[1];
    date[0] = '\0';

    medicines = new char[1];
    medicines[0] = '\0';

    notes = new char[1];
    notes[0] = '\0';
}

Prescription::Prescription(int presID, int apptID, int patID, int dID,
    const char* d, const char* medicine, const char* n) {

    prescriptionID = presID;
    appointmentID = apptID;
    patientID = patID;
    docID = dID;

    int len1 = PresLen(d);
    date = new char[len1 + 1];
    PresCopy(date, d);

    int len2 = PresLen(medicine);
    medicines = new char[len2 + 1];
    PresCopy(medicines, medicine);

    int len3 = PresLen(n);
    notes = new char[len3 + 1];
    PresCopy(notes, n);
}



Prescription::Prescription(const Prescription& rhs) {

    prescriptionID = rhs.prescriptionID;
    appointmentID = rhs.appointmentID;
    patientID = rhs.patientID;
    docID = rhs.docID;

    int len1 = PresLen(rhs.date);
    date = new char[len1 + 1];
    PresCopy(date, rhs.date);

    int len2 = PresLen(rhs.medicines);
    medicines = new char[len2 + 1];
    PresCopy(medicines, rhs.medicines);

    int len3 = PresLen(rhs.notes);
    notes = new char[len3 + 1];
    PresCopy(notes, rhs.notes);
}



Prescription& Prescription::operator=(const Prescription& rhs) {

    if (this == &rhs) {
        return *this;
    }

    delete[] date;
    delete[] medicines;
    delete[] notes;

    prescriptionID = rhs.prescriptionID;
    appointmentID = rhs.appointmentID;
    patientID = rhs.patientID;
    docID = rhs.docID;

    int len1 = PresLen(rhs.date);
    date = new char[len1 + 1];
    PresCopy(date, rhs.date);

    int len2 = PresLen(rhs.medicines);
    medicines = new char[len2 + 1];
    PresCopy(medicines, rhs.medicines);

    int len3 = PresLen(rhs.notes);
    notes = new char[len3 + 1];
    PresCopy(notes, rhs.notes);

    return *this;
}


Prescription::~Prescription() {
    delete[] date;
    delete[] medicines;
    delete[] notes;
}



int Prescription::getPrescriptionID() const {
    return prescriptionID;
}

int Prescription::getAppointmentID() const {
    return appointmentID;
}

int Prescription::getPatientID() const {
    return patientID;
}

int Prescription::getDoctorID() const {
    return docID;
}

const char* Prescription::getDate() const {
    return date;
}

const char* Prescription::getMedicines() const {
    return medicines;
}

const char* Prescription::getNotes() const {
    return notes;
}




bool Prescription::AboutPatient(int pid) const {

    bool result = false;

    if (patientID == pid) {
        result = true;
    }

    return result;
}


bool Prescription::AboutDoctor(int did) const {

    bool result = false;

    if (docID == did) {
        result = true;
    }

    return result;
}
int Prescription::getID() const {
    return prescriptionID;
}

bool Prescription::AboutAppointment(int apptID) const {

    bool result = false;

    if (appointmentID == apptID) {
        result = true;
    }

    return result;
}
