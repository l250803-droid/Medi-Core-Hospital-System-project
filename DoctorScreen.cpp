#include "DoctorScreen.h"
#include <iostream>
#include <ctime>
using namespace std;

void DoctorScreen::setMessage(const char* msg) {
    int i = 0;
    while (msg[i] != '\0' && i < 298) {
        message[i] = msg[i]; i++;
    }
    message[i] = '\0';
}

void DoctorScreen::clearInput() {
    for (int i = 0; i < 200; i++) inputBuf[i] = '\0';
    inputLen = 0;
}

void DoctorScreen::handleInput(char32_t unicode) {
    if (unicode == 8) {
        if (inputLen > 0) { inputLen--; inputBuf[inputLen] = '\0'; }
        return;
    }
    if (unicode >= 32 && unicode <= 126 && inputLen < 198) {
        inputBuf[inputLen] = (char)unicode;
        inputLen++;
        inputBuf[inputLen] = '\0';
    }
}

void DoctorScreen::getTodayDate(char* outDate) {

    time_t t = time(nullptr);

    tm now;

    localtime_s(&now, &t);

    // format DD-MM-YYYY manually

    int day = now.tm_mday;

    int month = now.tm_mon + 1;

    int year = now.tm_year + 1900;

    outDate[0] = '0' + (day / 10);

    outDate[1] = '0' + (day % 10);

    outDate[2] = '-';

    outDate[3] = '0' + (month / 10);

    outDate[4] = '0' + (month % 10);

    outDate[5] = '-';

    outDate[6] = '0' + (year / 1000);

    outDate[7] = '0' + ((year / 100) % 10);

    outDate[8] = '0' + ((year / 10) % 10);

    outDate[9] = '0' + (year % 10);

    outDate[10] = '\0';
}

bool DoctorScreen::runInputScreen(const char* title,
    const char* label,
    char* outBuf,
    int maxLen) {
    clearInput();
    bool active = true;
    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close(); return false;
            }
            if (auto* t = event->getIf<sf::Event::TextEntered>()) {
                if (active) handleInput(t->unicode);
            }
            if (auto* m = event->getIf<sf::Event::MouseButtonPressed>()) {
                float mx = (float)m->position.x;
                float my = (float)m->position.y;
                active = isClicked(mx, my, 150, 280, 500, 40);
                if (isClicked(mx, my, 200, 360, 180, 40)) {
                    int i = 0;
                    while (inputBuf[i] != '\0' && i < maxLen - 1) {
                        outBuf[i] = inputBuf[i]; i++;
                    }
                    outBuf[i] = '\0';
                    return true;
                }
                if (isClicked(mx, my, 420, 360, 180, 40)) return false;
            }
        }
        window.clear(sf::Color(20, 30, 40));
        drawText(window, font, title, 150, 80, 26, sf::Color::Cyan);
        drawText(window, font, label, 150, 240, 18, sf::Color::White);
        drawInputBox(window, font, inputBuf, 150, 280, 500, 40, active);
        drawButton(window, font, "Confirm", 200, 360, 180, 40,
            sf::Color(0, 150, 80), sf::Color::White);
        drawButton(window, font, "Back", 420, 360, 180, 40,
            sf::Color(160, 60, 60), sf::Color::White);
        window.display();
    }
    return false;
}

DoctorScreen::DoctorScreen(
    sf::RenderWindow& win,
    sf::Font& fnt,
    HospitalSystem& sys,
    Doctor* doc
) : window(win), font(fnt), system(sys), doctor(doc) {
    message[0] = '\0';
    inputBuf[0] = '\0';
    inputLen = 0;
}

void DoctorScreen::showTodayAppointments() {
    char today[11];
    getTodayDate(today);

    int count = 0;
    Appointment* appts = system.getDoctorTodayAppointments(
        doctor->getID(), today, count);

    char listStr[2000];
    int ls = 0;

    if (count == 0 || appts == nullptr) {
        const char* msg = "No appointments scheduled for today.";
        int i = 0;
        while (msg[i] != '\0') listStr[ls++] = msg[i++];
    }
    else {
        for (int i = 0; i < count && ls < 1900; i++) {
            const char* p1 = "ID:";
            int j = 0;
            while (p1[j] != '\0' && ls < 1990) listStr[ls++] = p1[j++];
            int aid = appts[i].getAppointmentID();
            char tmp[10]; int ti = 0;
            if (aid == 0) tmp[ti++] = '0';
            while (aid > 0) { tmp[ti++] = '0' + (aid % 10); aid /= 10; }
            for (int k = ti - 1; k >= 0 && ls < 1990; k--)
                listStr[ls++] = tmp[k];

            const char* p2 = "  Slot:";
            j = 0;
            while (p2[j] != '\0' && ls < 1990) listStr[ls++] = p2[j++];
            const char* ts = appts[i].getTimeSlot();
            j = 0;
            while (ts[j] != '\0' && ls < 1990) listStr[ls++] = ts[j++];

            const char* p3 = "  Status:";
            j = 0;
            while (p3[j] != '\0' && ls < 1990) listStr[ls++] = p3[j++];
            const char* st = appts[i].getStatus();
            j = 0;
            while (st[j] != '\0' && ls < 1990) listStr[ls++] = st[j++];

            listStr[ls++] = '\n';
        }
        delete[] appts;
    }
    listStr[ls] = '\0';

    bool waiting = true;
    while (waiting && window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close(); return;
            }
            if (auto* m = event->getIf<sf::Event::MouseButtonPressed>()) {
                float mx2 = (float)m->position.x;
                float my2 = (float)m->position.y;
                if (isClicked(mx2, my2, 330, 560, 140, 40)) waiting = false;
            }
        }
        window.clear(sf::Color(20, 30, 40));
        drawText(window, font, "Today's Appointments:", 150, 50, 22, sf::Color::Cyan);
        drawMessageBox(window, font, listStr, 80, 100, 640, 420);
        drawButton(window, font, "Back", 330, 560, 140, 40,
            sf::Color(80, 80, 80), sf::Color::White);
        window.display();
    }
}

void DoctorScreen::showMarkComplete() {
    char today[11];
    getTodayDate(today);

    char apptIDStr[20];
    if (!runInputScreen("Mark Appointment Complete",
        "Enter Appointment ID:", apptIDStr, 20)) return;

    int apptID = 0;
    int i = 0;
    while (apptIDStr[i] >= '0' && apptIDStr[i] <= '9') {
        apptID = apptID * 10 + (apptIDStr[i] - '0');
        i++;
    }

    bool ok = system.markAppointmentComplete(
        doctor->getID(), apptID, today);

    if (ok) setMessage("Appointment marked as completed.");
    else    setMessage("Invalid appointment ID or not today.");
}

void DoctorScreen::showMarkNoShow() {
    char today[11];
    getTodayDate(today);

    char apptIDStr[20];
    if (!runInputScreen("Mark No-Show",
        "Enter Appointment ID:", apptIDStr, 20)) return;

    int apptID = 0;
    int i = 0;
    while (apptIDStr[i] >= '0' && apptIDStr[i] <= '9') {
        apptID = apptID * 10 + (apptIDStr[i] - '0');
        i++;
    }

    bool ok = system.markAppnntNoShow(
        doctor->getID(), apptID, today);

    if (ok) setMessage("Appointment marked as no-show.");
    else    setMessage("Invalid appointment ID or not today.");
}

void DoctorScreen::showWritePrescription() {

    char apptIDStr[20];
    if (!runInputScreen("Write Prescription",
        "Enter Appointment ID:", apptIDStr, 20)) return;

    int apptID = 0;
    int i = 0;
    while (apptIDStr[i] >= '0' && apptIDStr[i] <= '9') {
        apptID = apptID * 10 + (apptIDStr[i] - '0');
        i++;
    }

    char medicines[500];
    if (!runInputScreen("Write Prescription",
        "Enter medicines (Med Dose;Med Dose):",
        medicines, 500)) return;

    char notes[300];
    if (!runInputScreen("Write Prescription",
        "Enter notes:", notes, 300)) return;

    // get today's date
    char today[11];
    getTodayDate(today);

    bool ok = system.writePrescr(
        doctor->getID(), apptID, medicines, notes, today);

    if (ok) setMessage("Prescription saved.");
    else    setMessage("Invalid appointment or prescription already exists.");
}

void DoctorScreen::showPatientHistory() {

    char patIDStr[20];
    if (!runInputScreen("View Patient History",
        "Enter Patient ID:", patIDStr, 20)) return;

    int patID = 0;
    int i = 0;
    while (patIDStr[i] >= '0' && patIDStr[i] <= '9') {
        patID = patID * 10 + (patIDStr[i] - '0');
        i++;
    }

    int count = 0;
    Prescription* pres = system.getDocPatientHistory(
        doctor->getID(), patID, count);

    if (pres == nullptr || count == 0) {
        setMessage("Access denied or no records found.");
        return;
    }

    char listStr[2000];
    int ls = 0;

    for (int j = 0; j < count && ls < 1900; j++) {
        const char* p1 = "Date:";
        int k = 0;
        while (p1[k] != '\0' && ls < 1990) listStr[ls++] = p1[k++];
        const char* d = pres[j].getDate();
        k = 0;
        while (d[k] != '\0' && ls < 1990) listStr[ls++] = d[k++];

        const char* p2 = "  Medicines:";
        k = 0;
        while (p2[k] != '\0' && ls < 1990) listStr[ls++] = p2[k++];
        const char* med = pres[j].getMedicines();
        k = 0;
        while (med[k] != '\0' && ls < 1990) listStr[ls++] = med[k++];

        listStr[ls++] = '\n';
    }
    listStr[ls] = '\0';

    delete[] pres;

    bool waiting = true;
    while (waiting && window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close(); return;
            }
            if (auto* m = event->getIf<sf::Event::MouseButtonPressed>()) {
                float mx2 = (float)m->position.x;
                float my2 = (float)m->position.y;
                if (isClicked(mx2, my2, 330, 560, 140, 40)) waiting = false;
            }
        }
        window.clear(sf::Color(20, 30, 40));
        drawText(window, font, "Patient History:", 150, 50, 22, sf::Color::Cyan);
        drawMessageBox(window, font, listStr, 80, 100, 640, 420);
        drawButton(window, font, "Back", 330, 560, 140, 40,
            sf::Color(80, 80, 80), sf::Color::White);
        window.display();
    }
}

void DoctorScreen::run() {
    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            if (auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
                int mx = mouse->position.x;
                int my = mouse->position.y;
                if (isClicked(mx, my, 550, 560, 180, 45)) return;
                if (isClicked(mx, my, 200, 150, 400, 45)) showTodayAppointments();
                if (isClicked(mx, my, 200, 210, 400, 45)) showMarkComplete();
                if (isClicked(mx, my, 200, 270, 400, 45)) showMarkNoShow();
                if (isClicked(mx, my, 200, 330, 400, 45)) showWritePrescription();
                if (isClicked(mx, my, 200, 390, 400, 45)) showPatientHistory();
                message[0] = '\0';
            }
        }

        window.clear(sf::Color(20, 30, 40));
        drawText(window, font, "Doctor Dashboard", 230, 40, 32, sf::Color::Cyan);

        char welcome[200];
        const char* prefix = "Dr. ";
        int wi = 0;
        while (prefix[wi] != '\0') { welcome[wi] = prefix[wi]; wi++; }
        const char* dname = doctor->getName();
        int di = 0;
        while (dname[di] != '\0') { welcome[wi] = dname[di]; wi++; di++; }
        welcome[wi] = '\0';
        drawText(window, font, welcome, 220, 90, 22, sf::Color::White);

        char specStr[200];
        const char* specPfx = "Specialization: ";
        int si = 0;
        while (specPfx[si] != '\0') { specStr[si] = specPfx[si]; si++; }
        const char* spec = doctor->getSpecialization();
        int sj = 0;
        while (spec[sj] != '\0') { specStr[si] = spec[sj]; si++; sj++; }
        specStr[si] = '\0';
        drawText(window, font, specStr, 220, 115, 18, sf::Color::Yellow);

        drawButton(window, font, "1. View Today's Appointments",
            200, 150, 400, 45, sf::Color(50, 100, 160), sf::Color::White);
        drawButton(window, font, "2. Mark Appointment Complete",
            200, 210, 400, 45, sf::Color(50, 100, 160), sf::Color::White);
        drawButton(window, font, "3. Mark Appointment No-Show",
            200, 270, 400, 45, sf::Color(50, 100, 160), sf::Color::White);
        drawButton(window, font, "4. Write Prescription",
            200, 330, 400, 45, sf::Color(50, 100, 160), sf::Color::White);
        drawButton(window, font, "5. View Patient Medical History",
            200, 390, 400, 45, sf::Color(50, 100, 160), sf::Color::White);
        drawButton(window, font, "6. Logout",
            550, 560, 180, 45, sf::Color(180, 70, 70), sf::Color::White);

        if (message[0] != '\0') {
            drawText(window, font, message, 150, 480, 18, sf::Color::Yellow);
        }

        window.display();
    }
}