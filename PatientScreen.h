#pragma once
#include <SFML/Graphics.hpp>
#include "HospitalSystem.h"
#include "UIHelper.h"
#include <ctime>

class PatientScreen {
private:
    sf::RenderWindow& window;
    sf::Font& font;
    HospitalSystem& system;
    Patient* patient;

    // message shown at bottom of screen
    char message[300];

    // input buffer for sub screens
    char inputBuf[200];
    int inputLen;

    // helpers
    void setMessage(const char* msg);
    void clearInput();
    void handleInput(char32_t unicode);

    // sub screen functions
    void showBookAppointment();
    void showCancelAppointment();
    void showViewAppointments();
    void showMedicalRecords();
    void showViewBills();
    void showPayBill();
    void showTopUp();

    // helper to draw input prompt screen
    // shows a label, an input box, confirm and back buttons
    // returns true when confirm is clicked
    bool runInputScreen(const char* title,
        const char* label,
        char* outBuf,
        int maxLen);

public:
    PatientScreen(
        sf::RenderWindow& win,
        sf::Font& fnt,
        HospitalSystem& sys,
        Patient* pat
    );
    void run();
};