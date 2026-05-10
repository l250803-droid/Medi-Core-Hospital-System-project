#include "PatientScreen.h"
#include "HospitalException.h"
#include <iostream>
#include <ctime>
using namespace std;

// ?????????????????????????????????????????
// helpers
// ?????????????????????????????????????????

void PatientScreen::setMessage(const char* msg) {
    int i = 0;
    while (msg[i] != '\0' && i < 298) {
        message[i] = msg[i];
        i++;
    }
    message[i] = '\0';
}

void PatientScreen::clearInput() {
    for (int i = 0; i < 200; i++) inputBuf[i] = '\0';
    inputLen = 0;
}

void PatientScreen::handleInput(char32_t unicode) {
    if (unicode == 8) {
        if (inputLen > 0) {
            inputLen--;
            inputBuf[inputLen] = '\0';
        }
        return;
    }
    if (unicode >= 32 && unicode <= 126 && inputLen < 198) {
        inputBuf[inputLen] = (char)unicode;
        inputLen++;
        inputBuf[inputLen] = '\0';
    }
}

// shows a simple input screen with one text field
// returns true when confirm clicked, false when back clicked
bool PatientScreen::runInputScreen(const char* title,
    const char* label,
    char* outBuf,
    int maxLen) {
    clearInput();
    bool active = true;

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                return false;
            }
            if (auto* t = event->getIf<sf::Event::TextEntered>()) {
                if (active) handleInput(t->unicode);
            }
            if (auto* m = event->getIf<sf::Event::MouseButtonPressed>()) {
                float mx = (float)m->position.x;
                float my = (float)m->position.y;

                // input box click
                active = isClicked(mx, my, 150, 280, 500, 40);

                // confirm
                if (isClicked(mx, my, 200, 360, 180, 40)) {
                    int i = 0;
                    while (inputBuf[i] != '\0' && i < maxLen - 1) {
                        outBuf[i] = inputBuf[i];
                        i++;
                    }
                    outBuf[i] = '\0';
                    return true;
                }
                // back
                if (isClicked(mx, my, 420, 360, 180, 40)) {
                    return false;
                }
            }
        }

        window.clear(sf::Color(30, 30, 40));
        drawText(window, font, title, 150, 80, 26, sf::Color::Cyan);
        drawText(window, font, label, 150, 240, 18, sf::Color::White);
        drawInputBox(window, font, inputBuf, 150, 280, 500, 40, active);
        drawButton(window, font, "Confirm",
            200, 360, 180, 40, sf::Color(0, 150, 80), sf::Color::White);
        drawButton(window, font, "Back",
            420, 360, 180, 40, sf::Color(160, 60, 60), sf::Color::White);
        window.display();
    }
    return false;
}

// ?????????????????????????????????????????
// constructor
// ?????????????????????????????????????????
PatientScreen::PatientScreen(
    sf::RenderWindow& win,
    sf::Font& fnt,
    HospitalSystem& sys,
    Patient* pat
) : window(win), font(fnt), system(sys), patient(pat) {
    message[0] = '\0';
    inputBuf[0] = '\0';
    inputLen = 0;
}

// ?????????????????????????????????????????
// Book Appointment
// ?????????????????????????????????????????
void PatientScreen::showBookAppointment() {

    // step 1 — get specialization
    char spec[100];
    if (!runInputScreen(
        "Book Appointment",
        "Enter specialization (e.g. Cardiology):",
        spec, 100)) return;

    // find matching doctors
    int docCount = 0;
    Doctor* matched = system.fndDocsBySpecialisation(spec, docCount);

    if (docCount == 0 || matched == nullptr) {
        setMessage("No doctors available for that specialization.");
        if (matched != nullptr) delete[] matched;
        return;
    }

    // show doctors list
    // build display string
    char docList[1500];
    int dl = 0;
    for (int i = 0; i < docCount && dl < 1400; i++) {
        // ID
        char numBuf[20];
        int id = matched[i].getID();
        int ni = 0;
        char tmp[10];
        int ti = 0;
        if (id == 0) { tmp[ti++] = '0'; }
        while (id > 0) { tmp[ti++] = '0' + (id % 10); id /= 10; }
        for (int k = ti - 1; k >= 0; k--) numBuf[ni++] = tmp[k];
        numBuf[ni] = '\0';

        const char* p1 = "ID:";
        int j = 0;
        while (p1[j] != '\0' && dl < 1490) docList[dl++] = p1[j++];
        j = 0;
        while (numBuf[j] != '\0' && dl < 1490) docList[dl++] = numBuf[j++];

        const char* p2 = "  Name:";
        j = 0;
        while (p2[j] != '\0' && dl < 1490) docList[dl++] = p2[j++];
        const char* dname = matched[i].getName();
        j = 0;
        while (dname[j] != '\0' && dl < 1490) docList[dl++] = dname[j++];

        const char* p3 = "  Fee:PKR";
        j = 0;
        while (p3[j] != '\0' && dl < 1490) docList[dl++] = p3[j++];

        float fee = matched[i].getFee();
        int fInt = (int)fee;
        ti = 0;
        if (fInt == 0) { tmp[ti++] = '0'; }
        while (fInt > 0) { tmp[ti++] = '0' + (fInt % 10); fInt /= 10; }
        ni = 0;
        for (int k = ti - 1; k >= 0; k--) numBuf[ni++] = tmp[k];
        numBuf[ni] = '\0';
        j = 0;
        while (numBuf[j] != '\0' && dl < 1490) docList[dl++] = numBuf[j++];

        docList[dl++] = '\n';
    }
    docList[dl] = '\0';

    delete[] matched;

    // show doctors and get doctor ID
    // show list screen
    bool waitingDocID = true;
    clearInput();
    bool inputActive = false;

    while (waitingDocID && window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close(); return;
            }
            if (auto* t = event->getIf<sf::Event::TextEntered>()) {
                if (inputActive) handleInput(t->unicode);
            }
            if (auto* m = event->getIf<sf::Event::MouseButtonPressed>()) {
                float mx2 = (float)m->position.x;
                float my2 = (float)m->position.y;
                inputActive = isClicked(mx2, my2, 150, 430, 200, 35);
                if (isClicked(mx2, my2, 370, 430, 120, 35)) {
                    waitingDocID = false;
                }
                if (isClicked(mx2, my2, 510, 430, 120, 35)) {
                    return;
                }
            }
        }
        window.clear(sf::Color(30, 30, 40));
        drawText(window, font, "Available Doctors:", 150, 60, 22, sf::Color::Cyan);
        drawMessageBox(window, font, docList, 150, 100, 500, 280);
        drawText(window, font, "Enter Doctor ID:", 150, 405, 18, sf::Color::White);
        drawInputBox(window, font, inputBuf, 150, 430, 200, 35, inputActive);
        drawButton(window, font, "Select", 370, 430, 120, 35,
            sf::Color(0, 140, 80), sf::Color::White);
        drawButton(window, font, "Back", 510, 430, 120, 35,
            sf::Color(160, 60, 60), sf::Color::White);
        window.display();
    }

    if (!window.isOpen()) return;

    // convert doc id
    int docID = 0;
    int ii = 0;
    while (inputBuf[ii] >= '0' && inputBuf[ii] <= '9') {
        docID = docID * 10 + (inputBuf[ii] - '0');
        ii++;
    }

    // step 3 — get date
    char date[20];
    if (!runInputScreen("Book Appointment",
        "Enter date (DD-MM-YYYY):", date, 20)) return;

    if (!Validator::CheckDateValidity(date)) {
        setMessage("Invalid date. Use DD-MM-YYYY format.");
        return;
    }

    // step 4 — show available slots and get slot
    // the 8 valid slots
    const char* slots[8] = {
        "09:00","10:00","11:00","12:00",
        "13:00","14:00","15:00","16:00"
    };

    // build available slots string
    char slotList[300];
    int sl = 0;
    const char* avail = "Available: ";
    int ai = 0;
    while (avail[ai] != '\0') slotList[sl++] = avail[ai++];

    for (int i = 0; i < 8; i++) {
        // check if slot is taken
        bool taken = false;
        int apCount = system.getAppnts().getCount();
        Appointment* allAp = system.getAppnts().getAll();
        for (int j = 0; j < apCount; j++) {
            if (allAp[j].getDoctorID() == docID &&
                allAp[j].isOnDate(date) &&
                !allAp[j].isCancelled()) {
                const char* ts = allAp[j].getTimeSlot();
                bool match = true;
                int k = 0;
                while (slots[i][k] != '\0' && ts[k] != '\0') {
                    if (slots[i][k] != ts[k]) { match = false; break; }
                    k++;
                }
                if (match && slots[i][k] == '\0' && ts[k] == '\0') {
                    taken = true; break;
                }
            }
        }
        if (!taken) {
            int k = 0;
            while (slots[i][k] != '\0' && sl < 295) {
                slotList[sl++] = slots[i][k++];
            }
            slotList[sl++] = ' ';
        }
    }
    slotList[sl] = '\0';

    char slot[10];
    bool waitSlot = true;
    clearInput();
    bool slotActive = false;

    while (waitSlot && window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close(); return;
            }
            if (auto* t = event->getIf<sf::Event::TextEntered>()) {
                if (slotActive) handleInput(t->unicode);
            }
            if (auto* m = event->getIf<sf::Event::MouseButtonPressed>()) {
                float mx3 = (float)m->position.x;
                float my3 = (float)m->position.y;
                slotActive = isClicked(mx3, my3, 150, 380, 200, 35);
                if (isClicked(mx3, my3, 370, 380, 120, 35)) waitSlot = false;
                if (isClicked(mx3, my3, 510, 380, 120, 35)) return;
            }
        }
        window.clear(sf::Color(30, 30, 40));
        drawText(window, font, "Select Time Slot:", 150, 60, 22, sf::Color::Cyan);
        drawMessageBox(window, font, slotList, 150, 110, 500, 60);
        drawText(window, font, "Enter slot (e.g. 09:00):", 150, 345, 18, sf::Color::White);
        drawInputBox(window, font, inputBuf, 150, 380, 200, 35, slotActive);
        drawButton(window, font, "Confirm", 370, 380, 120, 35,
            sf::Color(0, 140, 80), sf::Color::White);
        drawButton(window, font, "Back", 510, 380, 120, 35,
            sf::Color(160, 60, 60), sf::Color::White);
        window.display();
    }
    if (!window.isOpen()) return;

    // copy slot
    int si = 0;
    while (inputBuf[si] != '\0' && si < 9) {
        slot[si] = inputBuf[si]; si++;
    }
    slot[si] = '\0';

    if (!Validator::CheckValidTimeSlot(slot)) {
        setMessage("Invalid time slot entered.");
        return;
    }

    // try booking
    try {
        bool ok = system.BKAppointment(
            patient->getID(), docID, date, slot);
        if (ok) {
            setMessage("Appointment booked successfully.");
        }
        else {
            setMessage("Booking failed. Check doctor ID.");
        }
    }
    catch (SlotUnavailableException& e) {
        setMessage(e.what());
    }
    catch (InsufficientFundsException& e) {
        setMessage(e.what());
    }
}

// ?????????????????????????????????????????
// Cancel Appointment
// ?????????????????????????????????????????
void PatientScreen::showCancelAppointment() {

    int count = 0;
    Appointment* appts = system.getPatientAppointments(
        patient->getID(), count);

    // filter pending only
    char pendingList[1500];
    int pl = 0;
    bool hasPending = false;

    if (appts != nullptr) {
        for (int i = 0; i < count; i++) {
            if (appts[i].isPending()) {
                hasPending = true;
                // build line manually
                const char* idPfx = "ID:";
                int j = 0;
                while (idPfx[j] != '\0' && pl < 1490)
                    pendingList[pl++] = idPfx[j++];

                // appointment id as string
                int aid = appts[i].getAppointmentID();
                char tmp[10]; int ti = 0;
                if (aid == 0) tmp[ti++] = '0';
                while (aid > 0) {
                    tmp[ti++] = '0' + (aid % 10); aid /= 10;
                }
                for (int k = ti - 1; k >= 0 && pl < 1490; k--)
                    pendingList[pl++] = tmp[k];

                const char* datePfx = "  Date:";
                j = 0;
                while (datePfx[j] != '\0' && pl < 1490)
                    pendingList[pl++] = datePfx[j++];
                const char* d = appts[i].getDate();
                j = 0;
                while (d[j] != '\0' && pl < 1490)
                    pendingList[pl++] = d[j++];

                const char* slotPfx = "  Slot:";
                j = 0;
                while (slotPfx[j] != '\0' && pl < 1490)
                    pendingList[pl++] = slotPfx[j++];
                const char* ts = appts[i].getTimeSlot();
                j = 0;
                while (ts[j] != '\0' && pl < 1490)
                    pendingList[pl++] = ts[j++];

                pendingList[pl++] = '\n';
            }
        }
        delete[] appts;
    }
    pendingList[pl] = '\0';

    if (!hasPending) {
        setMessage("You have no pending appointments.");
        return;
    }

    // get appointment ID to cancel
    char apptIDStr[20];
    bool waitCancel = true;
    clearInput();
    bool active = false;

    while (waitCancel && window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close(); return;
            }
            if (auto* t = event->getIf<sf::Event::TextEntered>()) {
                if (active) handleInput(t->unicode);
            }
            if (auto* m = event->getIf<sf::Event::MouseButtonPressed>()) {
                float mx2 = (float)m->position.x;
                float my2 = (float)m->position.y;
                active = isClicked(mx2, my2, 150, 430, 200, 35);
                if (isClicked(mx2, my2, 370, 430, 120, 35)) waitCancel = false;
                if (isClicked(mx2, my2, 510, 430, 120, 35)) return;
            }
        }
        window.clear(sf::Color(30, 30, 40));
        drawText(window, font, "Cancel Appointment:", 150, 60, 22, sf::Color::Cyan);
        drawMessageBox(window, font, pendingList, 150, 100, 500, 280);
        drawText(window, font, "Enter Appointment ID:", 150, 405, 18, sf::Color::White);
        drawInputBox(window, font, inputBuf, 150, 430, 200, 35, active);
        drawButton(window, font, "Cancel It", 370, 430, 120, 35,
            sf::Color(180, 60, 60), sf::Color::White);
        drawButton(window, font, "Back", 510, 430, 120, 35,
            sf::Color(80, 80, 80), sf::Color::White);
        window.display();
    }
    if (!window.isOpen()) return;

    int apptID = 0;
    int ii = 0;
    while (inputBuf[ii] >= '0' && inputBuf[ii] <= '9') {
        apptID = apptID * 10 + (inputBuf[ii] - '0');
        ii++;
    }

    bool ok = system.CANCELAppointment(patient->getID(), apptID);
    if (ok) {
        setMessage("Appointment cancelled. Fee refunded.");
    }
    else {
        setMessage("Invalid appointment ID.");
    }
}

// ?????????????????????????????????????????
// View Appointments
// ?????????????????????????????????????????
void PatientScreen::showViewAppointments() {

    int count = 0;
    Appointment* appts = system.getPatientAppointments(
        patient->getID(), count);

    char listStr[2000];
    int ls = 0;

    if (count == 0 || appts == nullptr) {
        const char* msg = "No appointments found.";
        int i = 0;
        while (msg[i] != '\0') { listStr[ls++] = msg[i++]; }
    }
    else {
        for (int i = 0; i < count && ls < 1900; i++) {
            // ID
            const char* p1 = "ID:";
            int j = 0;
            while (p1[j] != '\0' && ls < 1990) listStr[ls++] = p1[j++];
            int aid = appts[i].getAppointmentID();
            char tmp[10]; int ti = 0;
            if (aid == 0) tmp[ti++] = '0';
            while (aid > 0) { tmp[ti++] = '0' + (aid % 10); aid /= 10; }
            for (int k = ti - 1; k >= 0 && ls < 1990; k--)
                listStr[ls++] = tmp[k];

            // date
            const char* p2 = "  Date:";
            j = 0;
            while (p2[j] != '\0' && ls < 1990) listStr[ls++] = p2[j++];
            const char* d = appts[i].getDate();
            j = 0;
            while (d[j] != '\0' && ls < 1990) listStr[ls++] = d[j++];

            // slot
            const char* p3 = "  Slot:";
            j = 0;
            while (p3[j] != '\0' && ls < 1990) listStr[ls++] = p3[j++];
            const char* ts = appts[i].getTimeSlot();
            j = 0;
            while (ts[j] != '\0' && ls < 1990) listStr[ls++] = ts[j++];

            // status
            const char* p4 = "  Status:";
            j = 0;
            while (p4[j] != '\0' && ls < 1990) listStr[ls++] = p4[j++];
            const char* st = appts[i].getStatus();
            j = 0;
            while (st[j] != '\0' && ls < 1990) listStr[ls++] = st[j++];

            listStr[ls++] = '\n';
        }
        delete[] appts;
    }
    listStr[ls] = '\0';

    // show list screen with just a back button
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
        window.clear(sf::Color(30, 30, 40));
        drawText(window, font, "My Appointments:", 150, 50, 22, sf::Color::Cyan);
        drawMessageBox(window, font, listStr, 80, 100, 640, 420);
        drawButton(window, font, "Back", 330, 560, 140, 40,
            sf::Color(80, 80, 80), sf::Color::White);
        window.display();
    }
}

// ?????????????????????????????????????????
// View Medical Records
// ?????????????????????????????????????????
void PatientScreen::showMedicalRecords() {

    int count = 0;
    Prescription* pres = system.getPatientMedicalRecords(
        patient->getID(), count);

    char listStr[2000];
    int ls = 0;

    if (count == 0 || pres == nullptr) {
        const char* msg = "No medical records found.";
        int i = 0;
        while (msg[i] != '\0') listStr[ls++] = msg[i++];
    }
    else {
        for (int i = 0; i < count && ls < 1900; i++) {
            const char* p1 = "Date:";
            int j = 0;
            while (p1[j] != '\0' && ls < 1990) listStr[ls++] = p1[j++];
            const char* d = pres[i].getDate();
            j = 0;
            while (d[j] != '\0' && ls < 1990) listStr[ls++] = d[j++];

            const char* p2 = "  Medicines:";
            j = 0;
            while (p2[j] != '\0' && ls < 1990) listStr[ls++] = p2[j++];
            const char* med = pres[i].getMedicines();
            j = 0;
            while (med[j] != '\0' && ls < 1990) listStr[ls++] = med[j++];

            const char* p3 = "\n  Notes:";
            j = 0;
            while (p3[j] != '\0' && ls < 1990) listStr[ls++] = p3[j++];
            const char* notes = pres[i].getNotes();
            j = 0;
            while (notes[j] != '\0' && ls < 1990) listStr[ls++] = notes[j++];

            listStr[ls++] = '\n';
            listStr[ls++] = '\n';
        }
        delete[] pres;
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
        window.clear(sf::Color(30, 30, 40));
        drawText(window, font, "Medical Records:", 150, 50, 22, sf::Color::Cyan);
        drawMessageBox(window, font, listStr, 80, 100, 640, 420);
        drawButton(window, font, "Back", 330, 560, 140, 40,
            sf::Color(80, 80, 80), sf::Color::White);
        window.display();
    }
}

// ?????????????????????????????????????????
// View Bills
// ?????????????????????????????????????????
void PatientScreen::showViewBills() {

    int count = 0;
    Bill* bills = system.getPatientBills(patient->getID(), count);
    float outstanding = system.getPatientOutstandingAmnt(patient->getID());

    char listStr[2000];
    int ls = 0;

    if (count == 0 || bills == nullptr) {
        const char* msg = "No bills found.";
        int i = 0;
        while (msg[i] != '\0') listStr[ls++] = msg[i++];
    }
    else {
        for (int i = 0; i < count && ls < 1800; i++) {
            const char* p1 = "BillID:";
            int j = 0;
            while (p1[j] != '\0' && ls < 1990) listStr[ls++] = p1[j++];
            int bid = bills[i].getBillID();
            char tmp[10]; int ti = 0;
            if (bid == 0) tmp[ti++] = '0';
            while (bid > 0) { tmp[ti++] = '0' + (bid % 10); bid /= 10; }
            for (int k = ti - 1; k >= 0 && ls < 1990; k--)
                listStr[ls++] = tmp[k];

            const char* p2 = "  Amount:PKR";
            j = 0;
            while (p2[j] != '\0' && ls < 1990) listStr[ls++] = p2[j++];
            int amt = (int)bills[i].getAmount();
            ti = 0;
            if (amt == 0) tmp[ti++] = '0';
            while (amt > 0) { tmp[ti++] = '0' + (amt % 10); amt /= 10; }
            for (int k = ti - 1; k >= 0 && ls < 1990; k--)
                listStr[ls++] = tmp[k];

            const char* p3 = "  Status:";
            j = 0;
            while (p3[j] != '\0' && ls < 1990) listStr[ls++] = p3[j++];
            const char* st = bills[i].getStatus();
            j = 0;
            while (st[j] != '\0' && ls < 1990) listStr[ls++] = st[j++];

            const char* p4 = "  Date:";
            j = 0;
            while (p4[j] != '\0' && ls < 1990) listStr[ls++] = p4[j++];
            const char* d = bills[i].getDate();
            j = 0;
            while (d[j] != '\0' && ls < 1990) listStr[ls++] = d[j++];

            listStr[ls++] = '\n';
        }

        // outstanding total
        const char* tot = "Total Outstanding: PKR ";
        int j = 0;
        while (tot[j] != '\0' && ls < 1990) listStr[ls++] = tot[j++];
        int outs = (int)outstanding;
        char tmp[10]; int ti = 0;
        if (outs == 0) tmp[ti++] = '0';
        while (outs > 0) { tmp[ti++] = '0' + (outs % 10); outs /= 10; }
        for (int k = ti - 1; k >= 0 && ls < 1990; k--)
            listStr[ls++] = tmp[k];

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
        window.clear(sf::Color(30, 30, 40));
        drawText(window, font, "My Bills:", 150, 50, 22, sf::Color::Cyan);
        drawMessageBox(window, font, listStr, 80, 100, 640, 420);
        drawButton(window, font, "Back", 330, 560, 140, 40,
            sf::Color(80, 80, 80), sf::Color::White);
        window.display();
    }
}

// ?????????????????????????????????????????
// Pay Bill
// ?????????????????????????????????????????
void PatientScreen::showPayBill() {

    int count = 0;
    Bill* bills = system.getPatientBills(patient->getID(), count);

    char unpaidList[1000];
    int ul = 0;
    bool hasUnpaid = false;

    if (bills != nullptr) {
        for (int i = 0; i < count; i++) {
            if (bills[i].isUnpaid()) {
                hasUnpaid = true;
                const char* p1 = "BillID:";
                int j = 0;
                while (p1[j] != '\0' && ul < 990) unpaidList[ul++] = p1[j++];
                int bid = bills[i].getBillID();
                char tmp[10]; int ti = 0;
                if (bid == 0) tmp[ti++] = '0';
                while (bid > 0) { tmp[ti++] = '0' + (bid % 10); bid /= 10; }
                for (int k = ti - 1; k >= 0 && ul < 990; k--)
                    unpaidList[ul++] = tmp[k];

                const char* p2 = "  PKR:";
                j = 0;
                while (p2[j] != '\0' && ul < 990) unpaidList[ul++] = p2[j++];
                int amt = (int)bills[i].getAmount();
                ti = 0;
                if (amt == 0) tmp[ti++] = '0';
                while (amt > 0) { tmp[ti++] = '0' + (amt % 10); amt /= 10; }
                for (int k = ti - 1; k >= 0 && ul < 990; k--)
                    unpaidList[ul++] = tmp[k];

                unpaidList[ul++] = '\n';
            }
        }
        delete[] bills;
    }
    unpaidList[ul] = '\0';

    if (!hasUnpaid) {
        setMessage("No unpaid bills.");
        return;
    }

    char billIDStr[20];
    bool waiting = true;
    clearInput();
    bool active = false;

    while (waiting && window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close(); return;
            }
            if (auto* t = event->getIf<sf::Event::TextEntered>()) {
                if (active) handleInput(t->unicode);
            }
            if (auto* m = event->getIf<sf::Event::MouseButtonPressed>()) {
                float mx2 = (float)m->position.x;
                float my2 = (float)m->position.y;
                active = isClicked(mx2, my2, 150, 430, 200, 35);
                if (isClicked(mx2, my2, 370, 430, 120, 35)) waiting = false;
                if (isClicked(mx2, my2, 510, 430, 120, 35)) return;
            }
        }
        window.clear(sf::Color(30, 30, 40));
        drawText(window, font, "Pay Bill:", 150, 60, 22, sf::Color::Cyan);
        drawMessageBox(window, font, unpaidList, 150, 100, 500, 280);
        drawText(window, font, "Enter Bill ID:", 150, 405, 18, sf::Color::White);
        drawInputBox(window, font, inputBuf, 150, 430, 200, 35, active);
        drawButton(window, font, "Pay", 370, 430, 120, 35,
            sf::Color(0, 140, 80), sf::Color::White);
        drawButton(window, font, "Back", 510, 430, 120, 35,
            sf::Color(80, 80, 80), sf::Color::White);
        window.display();
    }
    if (!window.isOpen()) return;

    int billID = 0;
    int ii = 0;
    while (inputBuf[ii] >= '0' && inputBuf[ii] <= '9') {
        billID = billID * 10 + (inputBuf[ii] - '0');
        ii++;
    }

    try {
        bool ok = system.payBill(patient->getID(), billID);
        if (ok) setMessage("Bill paid successfully.");
        else    setMessage("Invalid bill ID.");
    }
    catch (InsufficientFundsException& e) {
        setMessage(e.what());
    }
}

// ?????????????????????????????????????????
// Top Up Balance
// ?????????????????????????????????????????
void PatientScreen::showTopUp() {

    int attempts = 0;
    while (attempts < 3) {

        char amtStr[50];
        if (!runInputScreen("Top Up Balance",
            "Enter amount to add (PKR):",
            amtStr, 50)) return;

        // convert to float manually
        float amount = 0.0f;
        int i = 0;
        while (amtStr[i] >= '0' && amtStr[i] <= '9') {
            amount = amount * 10 + (amtStr[i] - '0');
            i++;
        }
        if (amtStr[i] == '.') {
            i++;
            float dec = 0.1f;
            while (amtStr[i] >= '0' && amtStr[i] <= '9') {
                amount += (amtStr[i] - '0') * dec;
                dec *= 0.1f;
                i++;
            }
        }

        try {
            bool ok = system.topUpBalance(patient->getID(), amount);
            if (ok) {
                setMessage("Balance updated successfully.");
                return;
            }
        }
        catch (InvalidInputException& e) {
            setMessage(e.what());
            attempts++;
        }
    }
    setMessage("Too many invalid attempts. Returning to menu.");
}

// ?????????????????????????????????????????
// main run loop
// ?????????????????????????????????????????
void PatientScreen::run() {

    while (window.isOpen()) {

        while (auto event = window.pollEvent()) {

            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (auto* mouse =
                event->getIf<sf::Event::MouseButtonPressed>()) {

                int mx = mouse->position.x;
                int my = mouse->position.y;

                if (isClicked(mx, my, 550, 560, 180, 45)) return;

                if (isClicked(mx, my, 200, 140, 400, 45))
                    showBookAppointment();

                if (isClicked(mx, my, 200, 200, 400, 45))
                    showCancelAppointment();

                if (isClicked(mx, my, 200, 260, 400, 45))
                    showViewAppointments();

                if (isClicked(mx, my, 200, 320, 400, 45))
                    showMedicalRecords();

                if (isClicked(mx, my, 200, 380, 400, 45))
                    showViewBills();

                if (isClicked(mx, my, 200, 440, 400, 45))
                    showPayBill();

                if (isClicked(mx, my, 200, 500, 400, 45))
                    showTopUp();

                // clear message on any click
                message[0] = '\0';
            }
        }

        window.clear(sf::Color(30, 30, 40));

        drawText(window, font, "Patient Dashboard",
            220, 40, 32, sf::Color::Cyan);

        // welcome
        char welcome[200];
        const char* prefix = "Welcome: ";
        int wi = 0;
        while (prefix[wi] != '\0') { welcome[wi] = prefix[wi]; wi++; }
        const char* pname = patient->getName();
        int pi = 0;
        while (pname[pi] != '\0') { welcome[wi] = pname[pi]; wi++; pi++; }
        welcome[wi] = '\0';
        drawText(window, font, welcome, 220, 90, 22, sf::Color::White);

        // balance
        char balStr[100];
        const char* balPrefix = "Balance: PKR ";
        int bi = 0;
        while (balPrefix[bi] != '\0') { balStr[bi] = balPrefix[bi]; bi++; }
        float bal = patient->getBalance();
        int intPart = (int)bal;
        char temp[20]; int ti = 0;
        if (intPart == 0) temp[ti++] = '0';
        while (intPart > 0) {
            temp[ti++] = '0' + (intPart % 10); intPart /= 10;
        }
        for (int k = ti - 1; k >= 0; k--) { balStr[bi] = temp[k]; bi++; }
        balStr[bi] = '\0';
        drawText(window, font, balStr, 220, 115, 18, sf::Color::Yellow);

        // buttons
        drawButton(window, font, "1. Book Appointment",
            200, 140, 400, 45, sf::Color(70, 130, 180), sf::Color::White);
        drawButton(window, font, "2. Cancel Appointment",
            200, 200, 400, 45, sf::Color(70, 130, 180), sf::Color::White);
        drawButton(window, font, "3. View Appointments",
            200, 260, 400, 45, sf::Color(70, 130, 180), sf::Color::White);
        drawButton(window, font, "4. View Medical Records",
            200, 320, 400, 45, sf::Color(70, 130, 180), sf::Color::White);
        drawButton(window, font, "5. View Bills",
            200, 380, 400, 45, sf::Color(70, 130, 180), sf::Color::White);
        drawButton(window, font, "6. Pay Bills",
            200, 440, 400, 45, sf::Color(70, 130, 180), sf::Color::White);
        drawButton(window, font, "7. Top Up Balance",
            200, 500, 400, 45, sf::Color(70, 130, 180), sf::Color::White);
        drawButton(window, font, "8. Logout",
            550, 560, 180, 45, sf::Color(180, 70, 70), sf::Color::White);

        // message
        if (message[0] != '\0') {
            drawText(window, font, message,
                150, 580, 16, sf::Color::Yellow);
        }

        window.display();
    }
}