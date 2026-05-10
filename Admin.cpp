#include "Admin.h"

static bool AdminMatch(const char* a, const char* b) {
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


static void intToStr(int num, char* buf) {

    int i = 0;

    if (num == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }

    char temp[20];
    int t = 0;

    while (num > 0) {
        temp[t] = '0' + (num % 10);
        t++;
        num = num / 10;
    }

    for (int j = t - 1; j >= 0; j--) {
        buf[i] = temp[j];
        i++;
    }

    buf[i] = '\0';
}


static void floatToStr(float num, char* temp) {
    int intPart = (int)num;
    intToStr(intPart, temp);
}


static int appendStr(char* into, int index, const char* s) {

    int i = 0;

    while (s[i] != '\0') {
        into[index] = s[i];
        index++;
        i++;
    }

    return index;
}



Admin::Admin(int id, const char* n, const char* pass)
    : Person(id, n, pass) {
}

Admin::Admin() : Person(0, "", "") {
}

Admin::~Admin() {
}



const char* Admin::getRole() const {
    return "Admin";
}

void Admin::displayMenu() {
    // will be implemented later
}

//adds a doctor into the system

bool Admin::addDoctor(Storage<Doctor>& docs, const char* n,
    const char* spec, const char* contact,
    const char* pass, float fee) {

    int maxID = 0;

    Doctor* all = docs.getAll();
    int count = docs.getCount();

    for (int i = 0; i < count; i++) {
        if (all[i].getID() > maxID) {
            maxID = all[i].getID();
        }
    }

    int newID = maxID + 1;

    Doctor d(newID, n, pass, spec, contact, fee);

    return docs.add(d);
}

//removes a doctor

bool Admin::removeDoctor(Storage<Doctor>& docs,
    Storage<Appointment>& appnts,
    int docID) {

    Appointment* arr = appnts.getAll();
    int count = appnts.getCount();

    for (int i = 0; i < count; i++) {

        if (arr[i].belongsToDoctor(docID)) {

            if (arr[i].isPending()) {
                return false;
            }
        }
    }

    return docs.removeByID(docID);
}

// get all th epatients in the systmre 

Patient* Admin::getAllPatients(Storage<Patient>& patients, int& pcount) {
    pcount = patients.getCount();
    return patients.getAll();
}

// gets all doctors into the system

Doctor* Admin::getAllDoctors(Storage<Doctor>& docs, int& pcount) {
    pcount = docs.getCount();
    return docs.getAll();
}

// This function takes all appnts from storage of appnts arrange them in date order with the latest firs so descenidng orderr.
// compares dats into numbers  and sorts and then reutrns the entire arr

Appointment* Admin::getAllAppointments(Storage<Appointment>& appnts,
    int& pcount) {

   
    pcount = appnts.getCount();
    Appointment* arr = appnts.getAll();

  
    for (int i = 0; i < pcount - 1; i++) {

        for (int j = 0; j < pcount - i - 1; j++) {

         
            const char* d1 = arr[j].getDate();
            const char* d2 = arr[j + 1].getDate();

          
            int y1 = (d1[6] - '0') * 1000 + (d1[7] - '0') * 100 +
                (d1[8] - '0') * 10 + (d1[9] - '0');

            int m1 = (d1[3] - '0') * 10 + (d1[4] - '0');

            int day1 = (d1[0] - '0') * 10 + (d1[1] - '0');

          
            int y2 = (d2[6] - '0') * 1000 + (d2[7] - '0') * 100 +
                (d2[8] - '0') * 10 + (d2[9] - '0');

            int m2 = (d2[3] - '0') * 10 + (d2[4] - '0');

            int day2 = (d2[0] - '0') * 10 + (d2[1] - '0');

           
            int total1 = y1 * 10000 + m1 * 100 + day1;
            int total2 = y2 * 10000 + m2 * 100 + day2;

         
            if (total1 < total2) {

                Appointment temp;
                temp = arr[j];

                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }

    return arr;
}

Bill* Admin::getUnpaidBills(Storage<Bill>& bil, int& pcount) {

    int total = bil.getCount();
    Bill* all = bil.getAll();

    int count = 0;

    for (int i = 0; i < total; i++) {

        if (all[i].isUnpaid()) {
            count++;
        }
    }

    pcount = count;

    if (count == 0) {
        return nullptr;
    }

    Bill* result = new Bill[count];

    int index = 0;

    for (int i = 0; i < total; i++) {

        if (all[i].isUnpaid()) {
            result[index] = all[i];
            index++;
        }
    }

    return result;
}


//used to discharge a patient and checks if there a re unpaid bills of left back appointmenst and also removes ts bills then prescriptions
//and funally the doctros form the system and alsthe appointmenst
bool Admin::dischargePatient(int patID,Storage<Patient>& patients,Storage<Appointment>& appnts,Storage<Bill>& bil,Storage<Prescription>& prescrip) {

    Bill* b = bil.getAll();
    int bcount = bil.getCount();

    for (int i = 0; i < bcount; i++) {

        if (b[i].belongsToPatient(patID)) {

            if (b[i].isUnpaid()) {
                return false;
            }
        }
    }

    Appointment* a = appnts.getAll();

    int acount = appnts.getCount();

    for (int i = 0; i < acount; i++) {

        if (a[i].belongsToPatient(patID)) {

            if (a[i].isPending()) {
                return false;
            }
        }
    }

    patients.removeByID(patID);

    for (int i = appnts.getCount() - 1; i >= 0; i--) {
        if (appnts.getAll()[i].belongsToPatient(patID)) {
            appnts.removeByID(appnts.getAll()[i].getID());
        }
    }

    for (int i = bil.getCount() - 1; i >= 0; i--) {
        if (bil.getAll()[i].belongsToPatient(patID)) {
            bil.removeByID(bil.getAll()[i].getID());
        }
    }

    for (int i = prescrip.getCount() - 1; i >= 0; i--) {
        if (prescrip.getAll()[i].AboutPatient(patID)) {
            prescrip.removeByID(prescrip.getAll()[i].getID());
        }
    }

    return true;
}



char* Admin::generateDailyReport(Storage<Appointment>& appointments,Storage<Bill>& bills,Storage<Patient>& patients, Storage<Doctor>& doctors,const char* today) {

    int totalToday = 0;
    int pending = 0;
    int completed = 0;
    int noshow = 0;
    int cancelled = 0;
    float revenue = 0;

    Appointment* allAppts = appointments.getAll();
    int apptTotal = appointments.getCount();

    for (int i = 0; i < apptTotal; i++) {

        if (AdminMatch(allAppts[i].getDate(), today)) {

            totalToday++;

            if (allAppts[i].isPending()) pending++;

            if (allAppts[i].isCompleted()) completed++;

            if (AdminMatch(allAppts[i].getStatus(), "no-show")) noshow++;


            if (allAppts[i].isCancelled()) cancelled++;
        }
    }

    Bill* allBills = bills.getAll();

    int billTotal = bills.getCount();

    for (int i = 0; i < billTotal; i++) {

        if (AdminMatch(allBills[i].getDate(), today)) {

            if (allBills[i].isPaid()) {
                revenue = revenue + allBills[i].getAmount();
            }
        }
    }

    char* report = new char[2000];
    int pos = 0;

    char numBuf[50];

    pos = appendStr(report, pos, " Daily Report ");
    pos = appendStr(report, pos, today);
    pos = appendStr(report, pos, " \n");

    pos = appendStr(report, pos, "Total appnts for the day: ");
    intToStr(totalToday, numBuf);
    pos = appendStr(report, pos, numBuf);
    pos = appendStr(report, pos, "\n");

    pos = appendStr(report, pos, "Pending ");
    intToStr(pending, numBuf);
    pos = appendStr(report, pos, numBuf);
    pos = appendStr(report, pos, "\n");

    pos = appendStr(report, pos, "Completed ");
    intToStr(completed, numBuf);
    pos = appendStr(report, pos, numBuf);
    pos = appendStr(report, pos, "\n");

    pos = appendStr(report, pos, "No-show ");
    intToStr(noshow, numBuf);
    pos = appendStr(report, pos, numBuf);
    pos = appendStr(report, pos, "\n");

    pos = appendStr(report, pos, "Cancelled ");
    intToStr(cancelled, numBuf);
    pos = appendStr(report, pos, numBuf);
    pos = appendStr(report, pos, "\n");

    pos = appendStr(report, pos, "Revenue ");
    floatToStr(revenue, numBuf);
    pos = appendStr(report, pos, numBuf);
    pos = appendStr(report, pos, "\n");

    report[pos] = '\0';

    return report;
}