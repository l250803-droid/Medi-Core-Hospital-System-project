#include "AdminScreen.h"
#include <iostream>
#include <ctime>
using namespace std;

void AdminScreen::setMessage(const char* msg) {
    int i = 0;
    while (msg[i] != '\0' && i < 298) { message[i] = msg[i]; i++; }
    message[i] = '\0';
}

void AdminScreen::clearInput() {
    for (int i = 0; i < 200; i++) inputBuf[i] = '\0';
    inputLen = 0;
}

void AdminScreen::handleInput(char32_t unicode) {
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

void AdminScreen::getTodayDate(char* outDate) {

    time_t t = time(nullptr);

    tm now;

    localtime_s(&now, &t);

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

bool AdminScreen::runInputScreen(const char* title,
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
        window.clear(sf::Color(20, 20, 35));
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

AdminScreen::AdminScreen(
    sf::RenderWindow& win,
    sf::Font& fnt,
    HospitalSystem& sys,
    Admin* adm
) : window(win), font(fnt), system(sys), admin(adm) {
    message[0] = '\0';
    inputBuf[0] = '\0';
    inputLen = 0;
}

// ?????????????????????????????????????????
// Add Doctor
// ?????????????????????????????????????????
void AdminScreen::showAddDoctor() {
    char name[100], spec[100], contact[20], pass[60];
    char feeStr[20];

    if (!runInputScreen("Add Doctor", "Enter doctor name:", name, 100)) return;
    if (!runInputScreen("Add Doctor", "Enter specialization:", spec, 100)) return;
    if (!runInputScreen("Add Doctor", "Enter contact (11 digits):", contact, 20)) return;
    if (!runInputScreen("Add Doctor", "Enter password (min 6 chars):", pass, 60)) return;
    if (!runInputScreen("Add Doctor", "Enter consultation fee:", feeStr, 20)) return;

    // validate
    if (!Validator::CheckValidContact(contact)) {
        setMessage("Invalid contact. Must be 11 digits.");
        return;
    }
    if (!Validator::CheckValidPassword(pass)) {
        setMessage("Invalid password. Must be at least 6 characters.");
        return;
    }

    float fee = 0.0f;
    int i = 0;
    while (feeStr[i] >= '0' && feeStr[i] <= '9') {
        fee = fee * 10 + (feeStr[i] - '0'); i++;
    }
    if (feeStr[i] == '.') {
        i++;
        float dec = 0.1f;
        while (feeStr[i] >= '0' && feeStr[i] <= '9') {
            fee += (feeStr[i] - '0') * dec;
            dec *= 0.1f; i++;
        }
    }

    if (!Validator::CheckValidFee(fee)) {
        setMessage("Invalid fee. Must be positive.");
        return;
    }

    bool ok = admin->addDoctor(
        system.getDocs(), name, spec, contact, pass, fee);

    if (ok) {
        // save to file
        int count = system.getDocs().getCount();
        Doctor* all = system.getDocs().getAll();
        fileHandler.SveDoctor(all[count - 1]);
        setMessage("Doctor added successfully.");
    }
    else {
        setMessage("Failed to add doctor.");
    }
}

// ?????????????????????????????????????????
// Remove Doctor
// ?????????????????????????????????????????
void AdminScreen::showRemoveDoctor() {
    char docIDStr[20];
    if (!runInputScreen("Remove Doctor",
        "Enter Doctor ID to remove:", docIDStr, 20)) return;

    int docID = 0;
    int i = 0;
    while (docIDStr[i] >= '0' && docIDStr[i] <= '9') {
        docID = docID * 10 + (docIDStr[i] - '0'); i++;
    }

    bool ok = admin->removeDoctor(
        system.getDocs(), system.getAppnts(), docID);

    if (ok) {
        fileHandler.DelDoctor(docID);
        setMessage("Doctor removed successfully.");
    }
    else {
        setMessage("Cannot remove. Doctor has pending appointments.");
    }
}

// ?????????????????????????????????????????
// View All Patients
// ?????????????????????????????????????????
void AdminScreen::showAllPatients() {
    int count = 0;
    Patient* all = admin->getAllPatients(system.getPatients(), count);

    char listStr[2000];
    int ls = 0;

    if (count == 0) {
        const char* msg = "No patients found.";
        int i = 0;
        while (msg[i] != '\0') listStr[ls++] = msg[i++];
    }
    else {
        for (int i = 0; i < count && ls < 1900; i++) {
            const char* p1 = "ID:";
            int j = 0;
            while (p1[j] != '\0' && ls < 1990) listStr[ls++] = p1[j++];
            int pid = all[i].getID();
            char tmp[10]; int ti = 0;
            if (pid == 0) tmp[ti++] = '0';
            while (pid > 0) { tmp[ti++] = '0' + (pid % 10); pid /= 10; }
            for (int k = ti - 1; k >= 0 && ls < 1990; k--)
                listStr[ls++] = tmp[k];

            const char* p2 = "  Name:";
            j = 0;
            while (p2[j] != '\0' && ls < 1990) listStr[ls++] = p2[j++];
            const char* pname = all[i].getName();
            j = 0;
            while (pname[j] != '\0' && ls < 1990) listStr[ls++] = pname[j++];

            const char* p3 = "  Bal:PKR";
            j = 0;
            while (p3[j] != '\0' && ls < 1990) listStr[ls++] = p3[j++];
            int bal = (int)all[i].getBalance();
            ti = 0;
            if (bal == 0) tmp[ti++] = '0';
            while (bal > 0) { tmp[ti++] = '0' + (bal % 10); bal /= 10; }
            for (int k = ti - 1; k >= 0 && ls < 1990; k--)
                listStr[ls++] = tmp[k];

            listStr[ls++] = '\n';
        }
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
        window.clear(sf::Color(20, 20, 35));
        drawText(window, font, "All Patients:", 150, 50, 22, sf::Color::Cyan);
        drawMessageBox(window, font, listStr, 80, 100, 640, 420);
        drawButton(window, font, "Back", 330, 560, 140, 40,
            sf::Color(80, 80, 80), sf::Color::White);
        window.display();
    }
}

// ?????????????????????????????????????????
// View All Doctors
// ?????????????????????????????????????????
void AdminScreen::showAllDoctors() {
    int count = 0;
    Doctor* all = admin->getAllDoctors(system.getDocs(), count);

    char listStr[2000];
    int ls = 0;

    if (count == 0) {
        const char* msg = "No doctors found.";
        int i = 0;
        while (msg[i] != '\0') listStr[ls++] = msg[i++];
    }
    else {
        for (int i = 0; i < count && ls < 1900; i++) {
            const char* p1 = "ID:";
            int j = 0;
            while (p1[j] != '\0' && ls < 1990) listStr[ls++] = p1[j++];
            int did = all[i].getID();
            char tmp[10]; int ti = 0;
            if (did == 0) tmp[ti++] = '0';
            while (did > 0) { tmp[ti++] = '0' + (did % 10); did /= 10; }
            for (int k = ti - 1; k >= 0 && ls < 1990; k--)
                listStr[ls++] = tmp[k];

            const char* p2 = "  Name:";
            j = 0;
            while (p2[j] != '\0' && ls < 1990) listStr[ls++] = p2[j++];
            const char* dname = all[i].getName();
            j = 0;
            while (dname[j] != '\0' && ls < 1990) listStr[ls++] = dname[j++];

            const char* p3 = "  Spec:";
            j = 0;
            while (p3[j] != '\0' && ls < 1990) listStr[ls++] = p3[j++];
            const char* spec = all[i].getSpecialization();
            j = 0;
            while (spec[j] != '\0' && ls < 1990) listStr[ls++] = spec[j++];

            listStr[ls++] = '\n';
        }
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
        window.clear(sf::Color(20, 20, 35));
        drawText(window, font, "All Doctors:", 150, 50, 22, sf::Color::Cyan);
        drawMessageBox(window, font, listStr, 80, 100, 640, 420);
        drawButton(window, font, "Back", 330, 560, 140, 40,
            sf::Color(80, 80, 80), sf::Color::White);
        window.display();
    }
}

// ?????????????????????????????????????????
// View All Appointments
// ?????????????????????????????????????????
void AdminScreen::showAllAppointments() {
    int count = 0;
    Appointment* all = admin->getAllAppointments(
        system.getAppnts(), count);

    char listStr[2000];
    int ls = 0;

    if (count == 0) {
        const char* msg = "No appointments found.";
        int i = 0;
        while (msg[i] != '\0') listStr[ls++] = msg[i++];
    }
    else {
        for (int i = 0; i < count && ls < 1900; i++) {
            const char* p1 = "ID:";
            int j = 0;
            while (p1[j] != '\0' && ls < 1990) listStr[ls++] = p1[j++];
            int aid = all[i].getAppointmentID();
            char tmp[10]; int ti = 0;
            if (aid == 0) tmp[ti++] = '0';
            while (aid > 0) { tmp[ti++] = '0' + (aid % 10); aid /= 10; }
            for (int k = ti - 1; k >= 0 && ls < 1990; k--)
                listStr[ls++] = tmp[k];

            const char* p2 = "  Date:";
            j = 0;
            while (p2[j] != '\0' && ls < 1990) listStr[ls++] = p2[j++];
            const char* d = all[i].getDate();
            j = 0;
            while (d[j] != '\0' && ls < 1990) listStr[ls++] = d[j++];

            const char* p3 = "  Status:";
            j = 0;
            while (p3[j] != '\0' && ls < 1990) listStr[ls++] = p3[j++];
            const char* st = all[i].getStatus();
            j = 0;
            while (st[j] != '\0' && ls < 1990) listStr[ls++] = st[j++];

            listStr[ls++] = '\n';
        }
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
        window.clear(sf::Color(20, 20, 35));
        drawText(window, font, "All Appointments:", 150, 50, 22, sf::Color::Cyan);
        drawMessageBox(window, font, listStr, 80, 100, 640, 420);
        drawButton(window, font, "Back", 330, 560, 140, 40,
            sf::Color(80, 80, 80), sf::Color::White);
        window.display();
    }
}

// ?????????????????????????????????????????
// View Unpaid Bills
// ?????????????????????????????????????????
void AdminScreen::showUnpaidBills() {
    int count = 0;
    Bill* bills = admin->getUnpaidBills(system.getBills(), count);

    char today[11];
    getTodayDate(today);

    char listStr[2000];
    int ls = 0;

    if (count == 0 || bills == nullptr) {
        const char* msg = "No unpaid bills.";
        int i = 0;
        while (msg[i] != '\0') listStr[ls++] = msg[i++];
    }
    else {
        for (int i = 0; i < count && ls < 1900; i++) {
            const char* p1 = "BillID:";
            int j = 0;
            while (p1[j] != '\0' && ls < 1990) listStr[ls++] = p1[j++];
            int bid = bills[i].getBillID();
            char tmp[10]; int ti = 0;
            if (bid == 0) tmp[ti++] = '0';
            while (bid > 0) { tmp[ti++] = '0' + (bid % 10); bid /= 10; }
            for (int k = ti - 1; k >= 0 && ls < 1990; k--)
                listStr[ls++] = tmp[k];

            const char* p2 = "  PKR:";
            j = 0;
            while (p2[j] != '\0' && ls < 1990) listStr[ls++] = p2[j++];
            int amt = (int)bills[i].getAmount();
            ti = 0;
            if (amt == 0) tmp[ti++] = '0';
            while (amt > 0) { tmp[ti++] = '0' + (amt % 10); amt /= 10; }
            for (int k = ti - 1; k >= 0 && ls < 1990; k--)
                listStr[ls++] = tmp[k];

            const char* p3 = "  Date:";
            j = 0;
            while (p3[j] != '\0' && ls < 1990) listStr[ls++] = p3[j++];
            const char* d = bills[i].getDate();
            j = 0;
            while (d[j] != '\0' && ls < 1990) listStr[ls++] = d[j++];

            // check overdue
            if (bills[i].isOverdue(today)) {
                const char* ov = " [OVERDUE]";
                j = 0;
                while (ov[j] != '\0' && ls < 1990) listStr[ls++] = ov[j++];
            }

            listStr[ls++] = '\n';
        }
        delete[] bills;
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
        window.clear(sf::Color(20, 20, 35));
        drawText(window, font, "Unpaid Bills:", 150, 50, 22, sf::Color::Cyan);
        drawMessageBox(window, font, listStr, 80, 100, 640, 420);
        drawButton(window, font, "Back", 330, 560, 140, 40,
            sf::Color(80, 80, 80), sf::Color::White);
        window.display();
    }
}

// ?????????????????????????????????????????
// Discharge Patient
// ?????????????????????????????????????????
void AdminScreen::showDischargePatient() {
    char patIDStr[20];
    if (!runInputScreen("Discharge Patient",
        "Enter Patient ID:", patIDStr, 20)) return;

    int patID = 0;
    int i = 0;
    while (patIDStr[i] >= '0' && patIDStr[i] <= '9') {
        patID = patID * 10 + (patIDStr[i] - '0'); i++;
    }

    // archive first
    bool archived = fileHandler.ArchivePatient(
        patID,
        system.getPatients(),
        system.getAppnts(),
        system.getBills(),
        system.getPrescriptions()
    );

    if (!archived) {
        setMessage("Patient not found.");
        return;
    }

    bool ok = admin->dischargePatient(
        patID,
        system.getPatients(),
        system.getAppnts(),
        system.getBills(),
        system.getPrescriptions()
    );

    if (ok) {
        fileHandler.DelPatient(patID);
        setMessage("Patient discharged and archived successfully.");
    }
    else {
        setMessage("Cannot discharge. Unpaid bills or pending appointments.");
    }
}

// ?????????????????????????????????????????
// View Security Log
// ?????????????????????????????????????????
void AdminScreen::showSecurityLog() {
    char* buffer = nullptr;
    bool ok = fileHandler.ReadSecurityLog(buffer);

    const char* displayStr = "No security events logged.";
    bool hasBuffer = false;

    if (ok && buffer != nullptr && buffer[0] != '\0') {
        displayStr = buffer;
        hasBuffer = true;
    }

    bool waiting = true;
    while (waiting && window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                if (hasBuffer) delete[] buffer;
                return;
            }
            if (auto* m = event->getIf<sf::Event::MouseButtonPressed>()) {
                float mx2 = (float)m->position.x;
                float my2 = (float)m->position.y;
                if (isClicked(mx2, my2, 330, 560, 140, 40)) waiting = false;
            }
        }
        window.clear(sf::Color(20, 20, 35));
        drawText(window, font, "Security Log:", 150, 50, 22, sf::Color::Cyan);
        drawMessageBox(window, font, displayStr, 80, 100, 640, 420);
        drawButton(window, font, "Back", 330, 560, 140, 40,
            sf::Color(80, 80, 80), sf::Color::White);
        window.display();
    }

    if (hasBuffer) delete[] buffer;
}

// ?????????????????????????????????????????
// Daily Report
// ?????????????????????????????????????????
void AdminScreen::showDailyReport() {
    char today[11];
    getTodayDate(today);

    char* report = admin->generateDailyReport(
        system.getAppnts(),
        system.getBills(),
        system.getPatients(),
        system.getDocs(),
        today
    );

    bool waiting = true;
    while (waiting && window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                delete[] report;
                return;
            }
            if (auto* m = event->getIf<sf::Event::MouseButtonPressed>()) {
                float mx2 = (float)m->position.x;
                float my2 = (float)m->position.y;
                if (isClicked(mx2, my2, 330, 560, 140, 40)) waiting = false;
            }
        }
        window.clear(sf::Color(20, 20, 35));
        drawText(window, font, "Daily Report:", 150, 50, 22, sf::Color::Cyan);
        drawMessageBox(window, font, report, 80, 100, 640, 420);
        drawButton(window, font, "Back", 330, 560, 140, 40,
            sf::Color(80, 80, 80), sf::Color::White);
        window.display();
    }

    delete[] report;
}

// ?????????????????????????????????????????
// main run loop
// ?????????????????????????????????????????
void AdminScreen::run() {
    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            if (auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
                int mx = mouse->position.x;
                int my = mouse->position.y;
                if (isClicked(mx, my, 550, 570, 180, 45)) return;
                if (isClicked(mx, my, 80, 150, 280, 40))  showAddDoctor();
                if (isClicked(mx, my, 80, 205, 280, 40))  showRemoveDoctor();
                if (isClicked(mx, my, 80, 260, 280, 40))  showAllPatients();
                if (isClicked(mx, my, 80, 315, 280, 40))  showAllDoctors();
                if (isClicked(mx, my, 80, 370, 280, 40))  showAllAppointments();
                if (isClicked(mx, my, 420, 150, 280, 40)) showUnpaidBills();
                if (isClicked(mx, my, 420, 205, 280, 40)) showDischargePatient();
                if (isClicked(mx, my, 420, 260, 280, 40)) showSecurityLog();
                if (isClicked(mx, my, 420, 315, 280, 40)) showDailyReport();
                message[0] = '\0';
            }
        }

        window.clear(sf::Color(20, 20, 35));
        drawText(window, font, "Admin Panel - MediCore",
            220, 40, 30, sf::Color::Cyan);

        char adminWelcome[200];
        const char* apfx = "Admin: ";
        int ai = 0;
        while (apfx[ai] != '\0') { adminWelcome[ai] = apfx[ai]; ai++; }
        const char* aname = admin->getName();
        int aj = 0;
        while (aname[aj] != '\0') { adminWelcome[ai] = aname[aj]; ai++; aj++; }
        adminWelcome[ai] = '\0';
        drawText(window, font, adminWelcome, 220, 88, 20, sf::Color::White);

        drawButton(window, font, "1. Add Doctor",
            80, 150, 280, 40, sf::Color(60, 100, 160), sf::Color::White);
        drawButton(window, font, "2. Remove Doctor",
            80, 205, 280, 40, sf::Color(60, 100, 160), sf::Color::White);
        drawButton(window, font, "3. View All Patients",
            80, 260, 280, 40, sf::Color(60, 100, 160), sf::Color::White);
        drawButton(window, font, "4. View All Doctors",
            80, 315, 280, 40, sf::Color(60, 100, 160), sf::Color::White);
        drawButton(window, font, "5. View All Appointments",
            80, 370, 280, 40, sf::Color(60, 100, 160), sf::Color::White);
        drawButton(window, font, "6. View Unpaid Bills",
            420, 150, 280, 40, sf::Color(100, 60, 160), sf::Color::White);
        drawButton(window, font, "7. Discharge Patient",
            420, 205, 280, 40, sf::Color(100, 60, 160), sf::Color::White);
        drawButton(window, font, "8. View Security Log",
            420, 260, 280, 40, sf::Color(100, 60, 160), sf::Color::White);
        drawButton(window, font, "9. Generate Daily Report",
            420, 315, 280, 40, sf::Color(100, 60, 160), sf::Color::White);
        drawButton(window, font, "10. Logout",
            550, 570, 180, 45, sf::Color(180, 70, 70), sf::Color::White);

        if (message[0] != '\0') {
            drawText(window, font, message, 80, 490, 18, sf::Color::Yellow);
        }

        window.display();
    }
}