#include "Appointment.h"



static int ApptLen(const char* str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}
// friend ostream& operator<<(ostream& out, const Appointment& obj) {
//     out << obj.appointmentID << "," << obj.patientID << ","
//        << obj.doctorID << "," << obj.date << ","
//        << obj.timeSlot << "," << obj.status;
//     return out;
// }


static void ApptCopy(char* cop, const char* s) {
    int i = 0;
    while (s[i] != '\0') {
        cop[i] = s[i];
        i++;
    }
    cop[i] = '\0';
}



static bool ApptMatch(const char* a, const char* b) {
    int i = 0;

    while (a[i] != '\0' && b[i] != '\0') {

        if (a[i] != b[i]) {
            return false;
        }

        i = i + 1;
    }

    if (a[i] == '\0' && b[i] == '\0') {
        return true;
    }

    return false;
}


Appointment::Appointment() {
    appointmentID = 0;
    patientID = 0;
    doctorID = 0;

    date = new char[1];
    date[0] = '\0';

    timeSlot = new char[1];
    timeSlot[0] = '\0';

    status = new char[8];
    ApptCopy(status, "pending");
}



Appointment::Appointment(int apptID, int patientID, int docID,
    const char* d, const char* appslot, const char* stat) {

    appointmentID = apptID;
    this->patientID = patientID;
    doctorID = docID;

    int len1 = ApptLen(d);
    date = new char[len1 + 1];
    ApptCopy(date, d);

    int len2 = ApptLen(appslot);
    timeSlot = new char[len2 + 1];
    ApptCopy(timeSlot, appslot);

    int len3 = ApptLen(stat);
    status = new char[len3 + 1];
    ApptCopy(status, stat);
}



Appointment::Appointment(const Appointment& rhs) {

    appointmentID = rhs.appointmentID;
    patientID = rhs.patientID;
    doctorID = rhs.doctorID;

    int len1 = ApptLen(rhs.date);
    date = new char[len1 + 1];
    ApptCopy(date, rhs.date);

    int len2 = ApptLen(rhs.timeSlot);
    timeSlot = new char[len2 + 1];
    ApptCopy(timeSlot, rhs.timeSlot);

    int len3 = ApptLen(rhs.status);
    status = new char[len3 + 1];
    ApptCopy(status, rhs.status);
}





Appointment::~Appointment() {
    delete[] date;
    delete[] timeSlot;
    delete[] status;
}



int Appointment::getAppointmentID() const {
    return appointmentID;
}

int Appointment::getPatientID() const {
    return patientID;
}

int Appointment::getDoctorID() const {
    return doctorID;
}

const char* Appointment::getDate() const {
    return date;
}

const char* Appointment::getTimeSlot() const {
    return timeSlot;
}

const char* Appointment::getStatus() const {
    return status;
}



void Appointment::setStatus(const char* newStatus) {

    delete[] status;

    int len = ApptLen(newStatus);
    status = new char[len + 1];
    ApptCopy(status, newStatus);
}






bool Appointment::isPending() const {

    bool temo = false;

    if (ApptMatch(status, "pending")) {
        temo = true;
    }

    return temo;
}











bool Appointment::isCancelled() const {

    bool temp = false;

    if (ApptMatch(status, "cancelled")) {
        temp = true;
    }

    return temp;
}

















bool Appointment::isCompleted() const {

    bool result = false;

    if (ApptMatch(status, "completed")) {
        result = true;
    }

    return result;
}


bool Appointment::belongsToPatient(int pid) const {

    bool res = false;

    if (patientID == pid) {
        res = true;
    }

    return res;
}

bool Appointment::belongsToDoctor(int did) const {
    if (doctorID == did) {
        return true;
    }
    return false;
}

bool Appointment::isOnDate(const char* d) const {
    return ApptMatch(date, d);
}
int Appointment::getID() const {
    return appointmentID;
}

bool Appointment::operator==(const Appointment& other) const {

    bool sameDoctor = false;
    bool sameDate = false;
    bool sameTime = false;
    bool cancelled = false;

    if (doctorID == other.doctorID) {
        sameDoctor = true;
    }

    
    if (ApptMatch(date, other.date)==true) {
        sameDate = true;
    }

    
    if (ApptMatch(timeSlot, other.timeSlot)==true) {
        sameTime = true;
    }

   
    if (isCancelled() == true) {
        cancelled = true;
    }

    if (other.isCancelled() == true) {
        cancelled = true;
    }

   
    if (sameDoctor == true) {

        if (sameDate == true) {

            if (sameTime == true) {

                if (cancelled == false) {
                    return true;
                }

            }

        }

    }

    return false;
}
Appointment& Appointment::operator=(const Appointment& rhs) {

    if (this == &rhs) {
        return *this;
    }

    delete[] date;
    delete[] timeSlot;
    delete[] status;

    appointmentID = rhs.appointmentID;
    patientID = rhs.patientID;
    doctorID = rhs.doctorID;

    int len1 = ApptLen(rhs.date);
    date = new char[len1 + 1];
    ApptCopy(date, rhs.date);

    int len2 = ApptLen(rhs.timeSlot);
    timeSlot = new char[len2 + 1];
    ApptCopy(timeSlot, rhs.timeSlot);

    int len3 = ApptLen(rhs.status);
    status = new char[len3 + 1];
    ApptCopy(status, rhs.status);

    return *this;
}

