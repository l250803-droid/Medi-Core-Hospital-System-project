#include "LoginScreen.h"
#include "HospitalException.h"
#include <iostream>
using namespace std;

LoginScreen::LoginScreen(sf::RenderWindow& win,
    sf::Font& fnt,
    HospitalSystem& sys)
    : window(win), font(fnt), system(sys) {

    clearInputs();
    selectedRole = 0;
    failCount = 0;
    message[0] = '\0';
}

void LoginScreen::clearInputs() {
    for (int i = 0; i < 20; i++) idInput[i] = '\0';
    for (int i = 0; i < 50; i++) passInput[i] = '\0';
    idLen = 0;
    passLen = 0;
    idActive = false;
    passActive = false;
}

void LoginScreen::handleTextInput(char32_t unicode) {

    // only handle printable ascii
    if (unicode < 32 || unicode > 126) return;

    if (idActive && idLen < 18) {
        idInput[idLen] = (char)unicode;
        idLen++;
        idInput[idLen] = '\0';
    }
    else if (passActive && passLen < 48) {
        passInput[passLen] = (char)unicode;
        passLen++;
        passInput[passLen] = '\0';
    }
}

ScreenResult LoginScreen::run() {

    while (window.isOpen()) {

        // handle events
        while (auto event = window.pollEvent()) {

            if (event->is<sf::Event::Closed>()) {
                window.close();
                return ScreenResult::Exit;
            }

            // text typed
            if (auto* textEv = event->getIf<sf::Event::TextEntered>()) {

                // backspace
                if (textEv->unicode == 8) {
                    if (idActive && idLen > 0) {
                        idLen--;
                        idInput[idLen] = '\0';
                    }
                    else if (passActive && passLen > 0) {
                        passLen--;
                        passInput[passLen] = '\0';
                    }
                }
                else {
                    handleTextInput(textEv->unicode);
                }
            }

            // mouse click
            if (auto* click = event->getIf<sf::Event::MouseButtonPressed>()) {

                float mx = (float)click->position.x;
                float my = (float)click->position.y;

                // role buttons
                if (isClicked(mx, my, 150, 180, 150, 45)) selectedRole = 1;
                if (isClicked(mx, my, 320, 180, 150, 45)) selectedRole = 2;
                if (isClicked(mx, my, 490, 180, 150, 45)) selectedRole = 3;

                // id box
                idActive = isClicked(mx, my, 200, 280, 400, 40);
                passActive = isClicked(mx, my, 260, 350, 400, 40);

                // exit button
                if (isClicked(mx, my, 280, 500, 240, 45)) {
                    window.close();
                    return ScreenResult::Exit;
                }

                // login button
                if (isClicked(mx, my, 280, 430, 240, 45)) {

                    if (selectedRole == 0) {
                        char msg[] = "Please select a role first.";
                        int k = 0;
                        while (msg[k] != '\0') {
                            message[k] = msg[k];
                            k++;
                        }
                        message[k] = '\0';
                    }
                    else {
                        int id = 0;
                        int i = 0;
                        while (idInput[i] >= '0' && idInput[i] <= '9') {
                            id = id * 10 + (idInput[i] - '0');
                            i++;
                        }

                        Person* user = system.login(
                            selectedRole, id, passInput, failCount);

                        if (user != nullptr) {
                            clearInputs();
                            message[0] = '\0';

                            if (selectedRole == 1) return ScreenResult::PatientMenu;
                            if (selectedRole == 2) return ScreenResult::DoctorMenu;
                            if (selectedRole == 3) return ScreenResult::AdminMenu;
                        }
                        else {
                            if (failCount == 0) {
                                // was locked out
                                char msg[] = "Account locked. Contact admin.";
                                int k = 0;
                                while (msg[k] != '\0') {
                                    message[k] = msg[k];
                                    k++;
                                }
                                message[k] = '\0';
                            }
                            else {
                                char msg[] = "Invalid ID or password.";
                                int k = 0;
                                while (msg[k] != '\0') {
                                    message[k] = msg[k];
                                    k++;
                                }
                                message[k] = '\0';
                            }
                        }
                    }
                }
            }
        }

        // draw
        window.clear(sf::Color(15, 15, 25));

        // title
        drawText(window, font,
            "Welcome to MediCore Hospital System",
            80, 60, 28, sf::Color::Cyan);

        drawText(window, font,
            "Select Role:", 150, 145, 20, sf::Color::White);

        // role buttons
        drawButton(window, font, "Patient",
            150, 180, 150, 45,
            selectedRole == 1 ? sf::Color(0, 120, 200) : sf::Color(50, 50, 80),
            sf::Color::White);

        drawButton(window, font, "Doctor",
            320, 180, 150, 45,
            selectedRole == 2 ? sf::Color(0, 150, 100) : sf::Color(50, 50, 80),
            sf::Color::White);

        drawButton(window, font, "Admin",
            490, 180, 150, 45,
            selectedRole == 3 ? sf::Color(180, 80, 0) : sf::Color(50, 50, 80),
            sf::Color::White);

        // input fields
        drawText(window, font, "ID:", 150, 285, 18, sf::Color::White);
        drawInputBox(window, font, idInput, 200, 280, 400, 40, idActive);

        drawText(window, font, "Password:", 150, 355, 18, sf::Color::White);

        // mask password with stars
        char masked[50];
        for (int i = 0; i < passLen; i++) masked[i] = '*';
        masked[passLen] = '\0';
        drawInputBox(window, font, masked, 260, 350, 400, 40, passActive);

        // login button
        drawButton(window, font, "Login",
            280, 430, 240, 45,
            sf::Color(0, 160, 80), sf::Color::White);

        // exit button
        drawButton(window, font, "Exit",
            280, 500, 240, 45,
            sf::Color(160, 40, 40), sf::Color::White);

        // message
        if (message[0] != '\0') {
            drawText(window, font, message,
                150, 570, 18, sf::Color::Red);
        }

        window.display();
    }

    return ScreenResult::Exit;
}