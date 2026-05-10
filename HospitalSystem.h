#pragma once
#include "Storage.h"
#include "Patient.h"
#include "Doctor.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include "Admin.h"
#include "FileHandler.h"
#include "Person.h"
#include "HospitalException.h"
#include "Validator.h"
#include <ctime>

class HospitalSystem {
private:
    Storage<Patient>      patients;
    Storage<Doctor>       doctors;
    Storage<Appointment>  appointments;
    Storage<Bill>         bills;
    Storage<Prescription> prescriptions;
    Admin* admin;
    FileHandler           fileHandler;
    Person* currentUser;

public:
    // constructor for this loads all of it from the files
    HospitalSystem();

    // cleans up administrator portion and all of the system and currentUser
    ~HospitalSystem();

    // loads all data from files through the incorporation of the FileHandler
    void ldAll();

    // login function through which i will integrate the sfml thing later
    Person* login(int role, int id, const char* password, int& failCount);
    Person* getCurrentUser();

    bool BKAppointment(int patientID, int docID,
        const char* date, const char* slot);

    bool CANCELAppointment(int patientID, int appnttID);

    Appointment* getPatientAppointments(int patientID, int& count);

    Prescription* getPatientMedicalRecords(int patientID, int& count);

    Bill* getPatientBills(int patientID, int& count);

    float getPatientOutstandingAmnt(int patientID);

    bool payBill(int patientID, int bilID);

    bool topUpBalance(int patientID, float amnt);

    // doctor functions
    Appointment* getDoctorTodayAppointments(int docID,
        const char* today, int& count);

    bool markAppointmentComplete(int docID, int appntID,
        const char* today);

    bool markAppnntNoShow(int docID, int appntID,
        const char* today);

    bool writePrescr(int docID, int appnntID,
        const char* meds, const char* notes,
        const char* today);

    Prescription* getDocPatientHistory(int docID, int patientID,
        int& count);

   
    Doctor* fndDocsBySpecialisation(const char* spec, int& count);

   
    int generateNewID(int* existID, int count);

    // getters for storages needed by Admin functions
    Storage<Patient>& getPatients() {
        return patients;
    }
    Storage<Doctor>& getDocs() {
        return doctors;
    }
    Storage<Appointment>& getAppnts() {
        return appointments;
    }
    Storage<Bill>& getBills() {
        return bills;
    }
    Storage<Prescription>& getPrescriptions() {
        return prescriptions;
    }
    Admin* getAdmin() {
        return admin;
    }
};