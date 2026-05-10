#pragma once
#include <SFML/Graphics.hpp>
#include "HospitalSystem.h"
#include "UIHelper.h"
#include "Doctor.h"
#include "HospitalException.h"
#include <ctime>

class DoctorScreen {
private:
    sf::RenderWindow& window;
    sf::Font& font;
    HospitalSystem& system;
    Doctor* doctor;

    char message[300];
    char inputBuf[200];
    int inputLen;

    void setMessage(const char* msg);
    void clearInput();
    void handleInput(char32_t unicode);

    bool runInputScreen(const char* title,
        const char* label,
        char* outBuf,
        int maxLen);

    void getTodayDate(char* outDate);

    void showTodayAppointments();
    void showMarkComplete();
    void showMarkNoShow();
    void showWritePrescription();
    void showPatientHistory();

public:
    DoctorScreen(
        sf::RenderWindow& win,
        sf::Font& fnt,
        HospitalSystem& sys,
        Doctor* doc
    );
    void run();
};