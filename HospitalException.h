#pragma once

// exception class for Medi  Core   sys tem
class HospitalException {
protected:
    char* message;

public:
    HospitalException(const char* msg);
    virtual const char* what() const;
    virtual ~HospitalException();
};

// intiateid when a fule canot be opened 
class FileNotFoundException : public HospitalException {
public:
    FileNotFoundException(const char* filename);
};

// whe atienbt balance <req amount 
class InsufficientFundsException : public HospitalException {
public:
    InsufficientFundsException(float required, float available);
};

// when user input is invalid
class InvalidInputException : public HospitalException {
public:
    InvalidInputException(const char* detail);
};

// shows when tinmer slot already booked
class SlotUnavailableException : public HospitalException {
public:
    SlotUnavailableException(const char* slot);
};