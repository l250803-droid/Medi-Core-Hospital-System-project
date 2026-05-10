#pragma once
#include <SFML/Graphics.hpp>
#include "HospitalSystem.h"
#include "UIHelper.h"

// which screen to show next after login
enum class ScreenResult {
    None,
    PatientMenu,
    DoctorMenu,
    AdminMenu,
    Exit
};

class LoginScreen {
private:
    sf::RenderWindow& window;
    sf::Font& font;
    HospitalSystem& system;

    // input buffers
    char idInput[20];
    char passInput[50];
    int idLen;
    int passLen;

    // which box is active
    bool idActive;
    bool passActive;

    // selected role
    int selectedRole;   // 1=Patient 2=Doctor 3=Admin

    // message to show user
    char message[200];

    // fail count for lockout
    int failCount;

    // internal helpers
    void clearInputs();
    void handleTextInput(char32_t unicode);

public:
    LoginScreen(sf::RenderWindow& win,
        sf::Font& fnt,
        HospitalSystem& sys);

    // runs the login screen loop
    // returns which screen to show next
    ScreenResult run();
};
