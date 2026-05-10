#pragma once


class Appointment {
private:
    int appointmentID;
    int patientID;
    int doctorID;

    char* date;       
    char* timeSlot;  
    char* status;     

public:
    
    Appointment();

    
    Appointment(int apptID, int patientID, int docID,
        const char* date, const char* appslot, const char* status);

    
    Appointment(const Appointment& rhs);

    
    Appointment& operator=(const Appointment& rhs);

  
    ~Appointment();

    
    int getAppointmentID() const;
    int getPatientID() const;
    int getDoctorID() const;
    const char* getDate() const;
    const char* getTimeSlot() const;
    const char* getStatus() const;

   
    void setStatus(const char* newStatus);

    int getID() const;
    bool isPending() const;
    bool isCancelled() const;
    bool isCompleted() const;
    bool belongsToPatient(int pid) const;
    bool belongsToDoctor(int did) const;
    bool isOnDate(const char* d) const;

   
    bool operator==(const Appointment& other) const;

   
    // friend std::ostream& operator<<(std::ostream& os, const Appointment& a);
};