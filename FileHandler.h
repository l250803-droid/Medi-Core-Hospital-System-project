#pragma once

#include "Storage.h"
#include "Patient.h"
#include "Doctor.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include "Admin.h"

class FileHandler {
public:

   
    bool LdPatients(Storage<Patient>& storage);
    bool LdDoctors(Storage<Doctor>& storage);
    bool LdAppointments(Storage<Appointment>& storage);
    bool LdBills(Storage<Bill>& storage);
    bool LdPrescriptions(Storage<Prescription>& storage);
    bool LdAdmin(Admin*& adn);

   
    bool SvePatient(const Patient& p);
    bool SveDoctor(const Doctor& d);
    bool SveAppointment(const Appointment& a);
    bool SveBill(const Bill& b);
    bool SvePrescription(const Prescription& p);

   
    bool UPdtPatient(const Patient& p);
    bool UPdtAppointment(const Appointment& a);
    bool UPdtBill(const Bill& b);

  
    bool DelPatient(int patientID);
    bool DelDoctor(int docID);

 
    bool LogSecurityEvent(const char* timstmp,
        const char* role,
        int enterID,
        const char* result);

    bool ReadSecurityLog(char*& outBuffer);

   
    bool ArchivePatient(int patientID,
        Storage<Patient>& patients,
        Storage<Appointment>& appnts,
        Storage<Bill>& bil,
        Storage<Prescription>& presc);
};