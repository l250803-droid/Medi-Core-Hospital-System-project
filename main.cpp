#include <iostream>
#include <SFML/Graphics.hpp>
#include "HospitalSystem.h"
#include "HospitalException.h"
#include "LoginScreen.h"
#include "PatientScreen.h"
#include "DoctorScreen.h"
#include "AdminScreen.h"
using namespace std;

int main() {

    HospitalSystem* system = nullptr;

    try {
        system = new HospitalSystem();

        cout << "=== File Loading Report ===" << endl;
        cout << "patients.txt      — Records: "
            << system->getPatients().getCount() << endl;
        cout << "doctors.txt       — Records: "
            << system->getDocs().getCount() << endl;
        cout << "appointments.txt  — Records: "
            << system->getAppnts().getCount() << endl;
        cout << "bills.txt         — Records: "
            << system->getBills().getCount() << endl;
        cout << "prescriptions.txt — Records: "
            << system->getPrescriptions().getCount() << endl;
        if (system->getAdmin() != nullptr) {
            cout << "admin.txt         — Admin: "
                << system->getAdmin()->getName() << endl;
        }
        else {
            cout << "admin.txt         — FAILED" << endl;
        }
        cout << "===========================" << endl;
    }
    catch (FileNotFoundException& e) {
        cout << "STARTUP ERROR: " << e.what() << endl;
        return 1;
    }
    catch (HospitalException& e) {
        cout << "SYSTEM ERROR: " << e.what() << endl;
        return 1;
    }
    catch (...) {
        cout << "Unknown startup error." << endl;
        return 1;
    }

    sf::RenderWindow window(
        sf::VideoMode(sf::Vector2u(800, 650)),
        "MediCore Hospital System"
    );
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.openFromFile("arial.ttf")) {
        cout << "ERROR: arial.ttf not found." << endl;
        delete system;
        return 1;
    }

    while (window.isOpen()) {

        // show login screen
        LoginScreen loginScreen(window, font, *system);
        ScreenResult result = loginScreen.run();

        if (result == ScreenResult::Exit) {
            window.close();
            break;
        }

        // patient logged in
        if (result == ScreenResult::PatientMenu) {
            Patient* pat =
                dynamic_cast<Patient*>(system->getCurrentUser());
            if (pat != nullptr) {
                PatientScreen patientScreen(window, font, *system, pat);
                patientScreen.run();
            }
        }

        // doctor logged in
        else if (result == ScreenResult::DoctorMenu) {
            Doctor* doc =
                dynamic_cast<Doctor*>(system->getCurrentUser());
            if (doc != nullptr) {
                DoctorScreen doctorScreen(window, font, *system, doc);
                doctorScreen.run();
            }
        }

        // admin logged in
        else if (result == ScreenResult::AdminMenu) {
            Admin* adm =
                dynamic_cast<Admin*>(system->getCurrentUser());
            if (adm != nullptr) {
                AdminScreen adminScreen(window, font, *system, adm);
                adminScreen.run();
            }
        }
    }

    delete system;
    system = nullptr;
}