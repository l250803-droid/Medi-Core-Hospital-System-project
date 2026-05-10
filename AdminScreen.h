#pragma once
#include <SFML/Graphics.hpp>
#include "HospitalSystem.h"
#include "UIHelper.h"
#include "Admin.h"
#include "FileHandler.h"
#include "HospitalException.h"
#include <ctime>

class AdminScreen {
private:
    sf::RenderWindow& window;
    sf::Font& font;
    HospitalSystem& system;
    Admin* admin;
    FileHandler fileHandler;

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

    void showAddDoctor();
    void showRemoveDoctor();
    void showAllPatients();
    void showAllDoctors();
    void showAllAppointments();
    void showUnpaidBills();
    void showDischargePatient();
    void showSecurityLog();
    void showDailyReport();

public:
    AdminScreen(
        sf::RenderWindow& win,
        sf::Font& fnt,
        HospitalSystem& sys,
        Admin* adm
    );
    void run();
};