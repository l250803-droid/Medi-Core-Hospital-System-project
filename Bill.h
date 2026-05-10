#pragma once

class Bill {
private:
    int billID;
    int patientID;
    int appointmentID;
    float amount;

    char* status;   // unpaid paid cancelled etc

    char* date;      

public:

   
    Bill();

  
    Bill(int billID, int patientID, int apptID,
        float amount, const char* status, const char* date);

   
    Bill(const Bill& rhs);

    int getID() const;
   
    Bill& operator=(const Bill& rhs);

  
    ~Bill();

  
    int getBillID() const;
    int getPatientID() const;
    int getAppointmentID() const;
    float getAmount() const;
    const char* getStatus() const;
    const char* getDate() const;

   
    void setStatus(const char* newStatus);

  
    bool isUnpaid() const;
    bool isPaid() const;
    bool belongsToPatient(int pid) const;
    bool isOverdue(const char* todayDate) const;
};