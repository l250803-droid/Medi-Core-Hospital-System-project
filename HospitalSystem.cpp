#include "HospitalSystem.h"

// manual string match helper
static bool SysMatch(const char* a, const char* b) {
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

// initializes system and loads all data using FileHandler
HospitalSystem::HospitalSystem() {
    admin = nullptr;
    currentUser = nullptr;
    ldAll();
}

// cleans up admin pointer and resets current user
HospitalSystem::~HospitalSystem() {
    if (admin != nullptr) {
        delete admin;
        admin = nullptr;
    }
    currentUser = nullptr;
}

// calls FileHandler functions to load all system data
// throws FileNotFoundException if any file fails to load
void HospitalSystem::ldAll() {
    if (!fileHandler.LdPatients(patients)) {
        throw FileNotFoundException("patients.txt");
    }
    if (!fileHandler.LdDoctors(doctors)) {
        throw FileNotFoundException("doctors.txt");
    }
    if (!fileHandler.LdAppointments(appointments)) {
        throw FileNotFoundException("appointments.txt");
    }
    if (!fileHandler.LdBills(bills)) {
        throw FileNotFoundException("bills.txt");
    }
    if (!fileHandler.LdPrescriptions(prescriptions)) {
        throw FileNotFoundException("prescriptions.txt");
    }
    if (!fileHandler.LdAdmin(admin)) {
        throw FileNotFoundException("admin.txt");
    }
}

// checks role id and password
// returns pointer to logged in user
Person* HospitalSystem::login(int role, int id,
    const char* password, int& failCount) {

    Person* found = nullptr;

    if (role == 1) {
        Patient* p = patients.findByID(id);
        if (p != nullptr) {
            if (p->checkPassword(password)) {
                found = p;
            }
        }
    }
    else if (role == 2) {
        Doctor* d = doctors.findByID(id);
        if (d != nullptr) {
            if (d->checkPassword(password)) {
                found = d;
            }
        }
    }
    else if (role == 3) {
        if (admin != nullptr && admin->getID() == id) {
            if (admin->checkPassword(password)) {
                found = admin;
            }
        }
    }

    if (found != nullptr) {
        failCount = 0;
        currentUser = found;
        return found;
    }

    failCount++;

    if (failCount >= 3) {
        fileHandler.LogSecurityEvent("", "User", id, "FAILED");
        failCount = 0;
    }

    return nullptr;
}

// checks doctor patient slot and balance
// creates appointment and bill
// throws exceptions if slot taken or balance low
bool HospitalSystem::BKAppointment(int patientID, int docID,
    const char* date, const char* slot) {

    Doctor* doc = doctors.findByID(docID);
    if (doc == nullptr) return false;

    Patient* pat = patients.findByID(patientID);
    if (pat == nullptr) return false;

    // check if slot is already taken
    int total = appointments.getCount();
    Appointment* all = appointments.getAll();

    for (int i = 0; i < total; i++) {
        Appointment temp(0, patientID, docID, date, slot, "pending");
        if (all[i] == temp) {
            throw SlotUnavailableException(slot);
        }
    }

    // check if patient has enough balance
    if (pat->getBalance() < doc->getFee()) {
        throw InsufficientFundsException(doc->getFee(), pat->getBalance());
    }

    // deduct fee from patient
    *pat -= doc->getFee();

    // generate proper IDs using existing ones
    int apptCount = appointments.getCount();
    int* apptIDs = new int[apptCount + 1];
    for (int i = 0; i < apptCount; i++) {
        apptIDs[i] = appointments.getAll()[i].getID();
    }
    int newApptID = generateNewID(apptIDs, apptCount);
    delete[] apptIDs;

    int billCount = bills.getCount();
    int* billIDs = new int[billCount + 1];
    for (int i = 0; i < billCount; i++) {
        billIDs[i] = bills.getAll()[i].getID();
    }
    int newBillID = generateNewID(billIDs, billCount);
    delete[] billIDs;

    // create and add appointment
    Appointment newAppt(newApptID, patientID, docID, date, slot, "pending");
    appointments.add(newAppt);

    // create and add bill
    Bill newBill(newBillID, patientID, newApptID,
        doc->getFee(), "unpaid", date);
    bills.add(newBill);

    // update files
    fileHandler.UPdtPatient(*pat);
    fileHandler.SveAppointment(newAppt);
    fileHandler.SveBill(newBill);

    return true;
}

// cancels appointment refunds patient and cancels bill
bool HospitalSystem::CANCELAppointment(int patientID, int appnttID) {

    Appointment* appt = appointments.findByID(appnttID);
    if (appt == nullptr) return false;

    if (!appt->belongsToPatient(patientID)) return false;
    if (!appt->isPending()) return false;

    Doctor* doc = doctors.findByID(appt->getDoctorID());
    Patient* pat = patients.findByID(patientID);

    if (doc == nullptr || pat == nullptr) return false;

    // cancel appointment
    appt->setStatus("cancelled");

    // refund fee to patient
    *pat += doc->getFee();

    // find and cancel the bill for this appointment
    int total = bills.getCount();
    Bill* allBills = bills.getAll();

    for (int i = 0; i < total; i++) {
        if (allBills[i].getAppointmentID() == appnttID) {
            allBills[i].setStatus("cancelled");
            fileHandler.UPdtBill(allBills[i]);
            break;
        }
    }

    fileHandler.UPdtAppointment(*appt);
    fileHandler.UPdtPatient(*pat);

    return true;
}

// returns all appointments of a patient sorted by date ascending
Appointment* HospitalSystem::getPatientAppointments(int patientID,
    int& count) {

    Appointment* all = appointments.getAll();
    int total = appointments.getCount();

    count = 0;
    for (int i = 0; i < total; i++) {
        if (all[i].belongsToPatient(patientID)) {
            count++;
        }
    }

    if (count == 0) return nullptr;

    Appointment* result = new Appointment[count];
    int index = 0;

    for (int i = 0; i < total; i++) {
        if (all[i].belongsToPatient(patientID)) {
            result[index] = all[i];
            index++;
        }
    }

    // bubble sort ascending by date
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            const char* d1 = result[j].getDate();
            const char* d2 = result[j + 1].getDate();

            int y1 = (d1[6] - '0') * 1000 + (d1[7] - '0') * 100 +
                (d1[8] - '0') * 10 + (d1[9] - '0');
            int y2 = (d2[6] - '0') * 1000 + (d2[7] - '0') * 100 +
                (d2[8] - '0') * 10 + (d2[9] - '0');
            int m1 = (d1[3] - '0') * 10 + (d1[4] - '0');
            int m2 = (d2[3] - '0') * 10 + (d2[4] - '0');
            int day1 = (d1[0] - '0') * 10 + (d1[1] - '0');
            int day2 = (d2[0] - '0') * 10 + (d2[1] - '0');

            int total1 = y1 * 10000 + m1 * 100 + day1;
            int total2 = y2 * 10000 + m2 * 100 + day2;

            if (total1 > total2) {
                Appointment temp = result[j];
                result[j] = result[j + 1];
                result[j + 1] = temp;
            }
        }
    }

    return result;
}

// returns prescriptions for patient sorted by date descending
Prescription* HospitalSystem::getPatientMedicalRecords(int patientID,
    int& count) {

    Prescription* all = prescriptions.getAll();
    int total = prescriptions.getCount();

    count = 0;
    for (int i = 0; i < total; i++) {
        if (all[i].AboutPatient(patientID)) {
            count++;
        }
    }

    if (count == 0) return nullptr;

    Prescription* result = new Prescription[count];
    int index = 0;

    for (int i = 0; i < total; i++) {
        if (all[i].AboutPatient(patientID)) {
            result[index] = all[i];
            index++;
        }
    }

    // bubble sort descending by date
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            const char* d1 = result[j].getDate();
            const char* d2 = result[j + 1].getDate();

            int y1 = (d1[6] - '0') * 1000 + (d1[7] - '0') * 100 +
                (d1[8] - '0') * 10 + (d1[9] - '0');
            int y2 = (d2[6] - '0') * 1000 + (d2[7] - '0') * 100 +
                (d2[8] - '0') * 10 + (d2[9] - '0');
            int m1 = (d1[3] - '0') * 10 + (d1[4] - '0');
            int m2 = (d2[3] - '0') * 10 + (d2[4] - '0');
            int day1 = (d1[0] - '0') * 10 + (d1[1] - '0');
            int day2 = (d2[0] - '0') * 10 + (d2[1] - '0');

            int total1 = y1 * 10000 + m1 * 100 + day1;
            int total2 = y2 * 10000 + m2 * 100 + day2;

            // descending swap if left smaller
            if (total1 < total2) {
                Prescription temp = result[j];
                result[j] = result[j + 1];
                result[j + 1] = temp;
            }
        }
    }

    return result;
}

// returns all bills for a patient
Bill* HospitalSystem::getPatientBills(int patientID, int& count) {

    Bill* all = bills.getAll();
    int total = bills.getCount();

    count = 0;
    for (int i = 0; i < total; i++) {
        if (all[i].belongsToPatient(patientID)) {
            count++;
        }
    }

    if (count == 0) return nullptr;

    Bill* result = new Bill[count];
    int index = 0;

    for (int i = 0; i < total; i++) {
        if (all[i].belongsToPatient(patientID)) {
            result[index] = all[i];
            index++;
        }
    }

    return result;
}

// sums all unpaid bills for patient
float HospitalSystem::getPatientOutstandingAmnt(int patientID) {

    float sum = 0;
    Bill* all = bills.getAll();
    int total = bills.getCount();

    for (int i = 0; i < total; i++) {
        if (all[i].belongsToPatient(patientID) && all[i].isUnpaid()) {
            sum = sum + all[i].getAmount();
        }
    }

    return sum;
}

// validates bill belongs to patient and is unpaid
// deducts balance and marks bill paid
bool HospitalSystem::payBill(int patientID, int bilID) {

    Bill* bill = bills.findByID(bilID);
    if (bill == nullptr) return false;

    if (!bill->belongsToPatient(patientID)) return false;

    // must be unpaid
    if (!bill->isUnpaid()) return false;

    Patient* pat = patients.findByID(patientID);
    if (pat == nullptr) return false;

    // check balance and throw if not enough
    if (pat->getBalance() < bill->getAmount()) {
        throw InsufficientFundsException(bill->getAmount(),
            pat->getBalance());
    }

    *pat -= bill->getAmount();
    bill->setStatus("paid");

    fileHandler.UPdtBill(*bill);
    fileHandler.UPdtPatient(*pat);

    return true;
}

// validates amount and adds to patient balance
bool HospitalSystem::topUpBalance(int patientID, float amnt) {

    // validate amount first
    if (!Validator::CheckValidAmount(amnt)) {
        throw InvalidInputException("amount must be greater than 0");
    }

    Patient* pat = patients.findByID(patientID);
    if (pat == nullptr) return false;

    *pat += amnt;

    fileHandler.UPdtPatient(*pat);

    return true;
}

// returns todays appointments for a doctor sorted by time slot
Appointment* HospitalSystem::getDoctorTodayAppointments(int docID,
    const char* today, int& count) {

    Appointment* all = appointments.getAll();
    int total = appointments.getCount();

    count = 0;
    for (int i = 0; i < total; i++) {
        if (all[i].belongsToDoctor(docID) && all[i].isOnDate(today)) {
            count++;
        }
    }

    if (count == 0) return nullptr;

    Appointment* result = new Appointment[count];
    int index = 0;

    for (int i = 0; i < total; i++) {
        if (all[i].belongsToDoctor(docID) && all[i].isOnDate(today)) {
            result[index] = all[i];
            index++;
        }
    }

    // sort by time slot ascending using hour digits
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            const char* t1 = result[j].getTimeSlot();
            const char* t2 = result[j + 1].getTimeSlot();

            int h1 = (t1[0] - '0') * 10 + (t1[1] - '0');
            int h2 = (t2[0] - '0') * 10 + (t2[1] - '0');

            if (h1 > h2) {
                Appointment temp = result[j];
                result[j] = result[j + 1];
                result[j + 1] = temp;
            }
        }
    }

    return result;
}

// marks appointment as completed
bool HospitalSystem::markAppointmentComplete(int docID, int appntID,
    const char* today) {

    Appointment* appt = appointments.findByID(appntID);
    if (appt == nullptr){
        return false;
    }

    if (!appt->belongsToDoctor(docID)) { 
        return false;
    
    }
    if (!appt->isPending()) { 
        return false;
    
    
    }
    if (!appt->isOnDate(today)) {
        return false;
    }
    appt->setStatus("completed");
    fileHandler.UPdtAppointment(*appt);

    return true;
}

// marks appointment as no show and cancels bill without refund
bool HospitalSystem::markAppnntNoShow(int docID, int appntID,
    const char* today) {

    Appointment* appt = appointments.findByID(appntID);
    if (appt == nullptr) return false;

    if (!appt->belongsToDoctor(docID)) return false;
    if (!appt->isPending()) return false;
    if (!appt->isOnDate(today)) return false;

    appt->setStatus("no-show");

    // cancel bill no refund given IN THSI SITUQTION 
    int total = bills.getCount();
    Bill* allBills = bills.getAll();

    for (int i = 0; i < total; i++) {
        if (allBills[i].getAppointmentID() == appntID) {
            allBills[i].setStatus("cancelled");
            fileHandler.UPdtBill(allBills[i]);
            break;
        }
    }

    fileHandler.UPdtAppointment(*appt);

    return true;
}

// writes prescription for completed appointment
bool HospitalSystem::writePrescr(int docID, int appnntID,
    const char* meds, const char* notes, const char* today) {

    Appointment* appt = appointments.findByID(appnntID);
    if (appt == nullptr) return false;

    if (!appt->belongsToDoctor(docID)) return false;
    if (!appt->isCompleted()) return false;

    // check if prescription already exists for this appointment
    int total = prescriptions.getCount();
    Prescription* all = prescriptions.getAll();

    for (int i = 0; i < total; i++) {
        if (all[i].AboutAppointment(appnntID)) {
            return false;  // already written
        }
    }

    // generate new prescription ID
    int* presIDs = new int[total + 1];
    for (int i = 0; i < total; i++) {
        presIDs[i] = all[i].getID();
    }
    int newPresID = generateNewID(presIDs, total);
    delete[] presIDs;

    // create and add prescription
    Prescription newPres(newPresID, appnntID,
        appt->getPatientID(), docID,
        today, meds, notes);

    prescriptions.add(newPres);
    fileHandler.SvePrescription(newPres);

    return true;
}

// checks access then returns prescriptions by this doctor for this patient
// returns all prescriptions written by this doctor for a specific patient
// but only if doctor had at least one completed appointment with that patient first we check if doctor is allowed to see this patient history
// meaning at least one completed appointment must exist
Prescription* HospitalSystem::getDocPatientHistory(int docID,
    int patientID, int& count) {


    bool allowed = false;

    int totalAppts = appointments.getCount();
    Appointment* apptArr = appointments.getAll();

    for (int i = 0; i < totalAppts; i++) {

        if (apptArr[i].belongsToDoctor(docID)) {

            if (apptArr[i].belongsToPatient(patientID)) {

                if (apptArr[i].isCompleted()) {
                    allowed = true;
                    break;
                }
            }
        }
    }

  
    if (allowed == false) {
        count = 0;
        return nullptr;
    }


 

    int total = prescriptions.getCount();
    Prescription* all = prescriptions.getAll();

    count = 0;

    for (int i = 0; i < total; i++) {

        if (all[i].AboutDoctor(docID)) {

            if (all[i].AboutPatient(patientID)) {
                count = count + 1;
            }
        }
    }

    if (count == 0) {
        return nullptr;
    }


    
    Prescription* result = new Prescription[count];

    int idx = 0;

    for (int i = 0; i < total; i++) {

        if (all[i].AboutDoctor(docID) && all[i].AboutPatient(patientID)) {

            result[idx] = all[i];
            idx++;
        }
    }


  

    for (int i = 0; i < count - 1; i++) {

        for (int j = 0; j < count - i - 1; j++) {

            const char* d1 = result[j].getDate();
            const char* d2 = result[j + 1].getDate();

            int y1 = (d1[6] - '0') * 1000 + (d1[7] - '0') * 100 +(d1[8] - '0') * 10 + (d1[9] - '0');

            int y2 = (d2[6] - '0') * 1000 + (d2[7] - '0') * 100 +(d2[8] - '0') * 10 + (d2[9] - '0');

            int m1 = (d1[3] - '0') * 10 + (d1[4] - '0');
            int m2 = (d2[3] - '0') * 10 + (d2[4] - '0');

            int day1 = (d1[0] - '0') * 10 + (d1[1] - '0');
            int day2 = (d2[0] - '0') * 10 + (d2[1] - '0');

            int total1 = y1 * 10000 + m1 * 100 + day1;
            int total2 = y2 * 10000 + m2 * 100 + day2;

            // descending order means lates date comes first
            if (total1 < total2) {

                Prescription temp = result[j];
                result[j] = result[j + 1];
                result[j + 1] = temp;
            }
        }
    }

    return result;
}

// finds doctors by specialization and is also case insenisitve finds all doctors that match a given specialization
//it gets all the doctors from the storage and then sees howe many match then createsa an array and returns them
Doctor* HospitalSystem::fndDocsBySpecialisation(const char* spec,
    int& count) {

   
    int total = doctors.getCount();
    Doctor* allDocs = doctors.getAll();

    count = 0;

   
    for (int i = 0; i < total; i++) {

        if (Validator::Match(allDocs[i].getSpecialization(), spec)) {
            count = count + 1;
        }
    }

   
    if (count == 0) {
        return nullptr;
    }

   
    Doctor* result = new Doctor[count];

    int idx = 0;

    
    for (int i = 0; i < total; i++) {

        if (Validator::Match(allDocs[i].getSpecialization(), spec)) {

            result[idx] = allDocs[i];
            idx = idx + 1;
        }
    }

    return result;
}

// returns max ID plus 1
int HospitalSystem::generateNewID(int* existID, int count) {

    if (count == 0) {
        return 1;
    }

    int max = existID[0];

    for (int i = 1; i < count; i++) {
        if (existID[i] > max) {
            max = existID[i];
        }
    }

    return max + 1;
}


Person* HospitalSystem::getCurrentUser() {

    return currentUser;
}