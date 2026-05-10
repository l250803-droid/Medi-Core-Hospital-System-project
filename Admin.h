#pragma once
#include "Person.h"
#include "Storage.h"
#include "Doctor.h"
#include "Patient.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"

class Admin : public Person {
public:
   
    Admin(int id, const char* n, const char* pass);

  
    Admin();

  
    ~Admin();

    
    const char* getRole() const override;
    void displayMenu() override;


    // adds a new doc to  system 
    bool addDoctor(Storage<Doctor>& docs, const char* n,const char* spec, const char* contact,const char* pass, float fee);

   
    bool removeDoctor(Storage<Doctor>& docs,
        Storage<Appointment>& appnts,
        int docID);

    // ret  to all patients and this will be printed by sfml
    Patient* getAllPatients(Storage<Patient>& patients, int& pcount);

    // returns pointer to all doctors PRIT BY sfml
    Doctor* getAllDoctors(Storage<Doctor>& docs, int& pcount);

    // ret all appnts sorted by date descending
    Appointment* getAllAppointments(Storage<Appointment>& appnts,
        int& pcount);

    // ret unpaid bills of all patients
    Bill* getUnpaidBills(Storage<Bill>& bil, int& pcount);

    // discharges and removes  patient if none unpaid bill and pending appointment
    bool dischargePatient(int patID,
        Storage<Patient>& patients,
        Storage<Appointment>& appnts,
        Storage<Bill>& bil,
        Storage<Prescription>& prescrip);

   
    char* generateDailyReport(Storage<Appointment>& appnts,
        Storage<Bill>& bil,
        Storage<Patient>& patients,
        Storage<Doctor>& docs,
        const char* today);
};