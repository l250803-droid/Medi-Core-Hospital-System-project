#pragma once

class Prescription {
private:
    int prescriptionID;
    int appointmentID;
    int patientID;
    int docID;

    char* date;
    char* medicines;
    char* notes;

public:

   
    Prescription();

   
    Prescription(int presID, int apptID, int patID, int docID,
        const char* date, const char* medicine, const char* notes);

   
    Prescription(const Prescription& rhs);

   
    Prescription& operator=(const Prescription& rhs);

  
    ~Prescription();

   
    int getPrescriptionID() const;

    int getID() const;

    int getAppointmentID() const;

    int getPatientID() const;

    int getDoctorID() const;

    const char* getDate() const;

    const char* getMedicines() const;



    const char* getNotes() const;

   
    bool AboutPatient(int pid) const;

    bool AboutDoctor(int did) const;

    bool AboutAppointment(int apptID) const;
};
