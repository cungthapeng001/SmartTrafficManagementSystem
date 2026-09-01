 #include <iostream>
#include <string>
#include "TrafficManagementSystem.h"
#include "Vehicle.h"

using namespace std;

void displayMenu() {
    cout << "\n=== Smart Traffic Management System ===" << endl;
    cout << "1. Add Vehicle" << endl;
    cout << "2. Remove Vehicle" << endl;
    cout << "3. Display Vehicle Queue" << endl;
    cout << "4. Show Traffic Status" << endl;
    cout << "5. Traffic Signal Control" << endl;
    cout << "6. Exit" << endl;
    cout << "=======================================" << endl;
    cout << "Enter your choice: ";
}

int main() {
    TrafficManagementSystem tms;
    int choice;

    while (true) {
        displayMenu();
        
        if (!(cin >> choice)) {
            // မမှန်ကန်သော ထည့်သွင်းမှုကို ကိုင်တွယ်ဖြေရှင်းပါ (ဥပမာ- ဂဏန်းများအစား စာလုံးများ)
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        if (choice == 1) {
            string id, type, road;
            cout << "Enter Vehicle ID: ";
            cin >> id;
            cout << "Enter Vehicle Type (e.g., Car, Bus, Bike): ";
            cin >> type;
            cout << "Enter Road Name: ";
            cin >> road;
            
            Vehicle v(id, type, road);
            tms.addVehicle(v);
            cout << "Vehicle added to the queue successfully." << endl;

        } else if (choice == 2) {
            // ယာဉ်ကို ဖယ်ရှားခြင်းသည် FIFO (Queue မူ) ကို အသုံးပြုသည်
            Vehicle removed("", "", "");
            if (tms.removeVehicle(removed)) {
                cout << "Vehicle removed successfully: ";
                removed.display();
            } else {
                cout << "The queue is empty! No vehicles to remove." << endl;
            }

        } else if (choice == 3) {
            tms.displayQueue();

        } else if (choice == 4) {
            tms.displayTrafficStatus();

        } else if (choice == 5) {
            tms.displaySignalControl();

        } else if (choice == 6) {
            cout << "Exiting system. Goodbye!" << endl;
            break;

        } else {
            cout << "Invalid choice. Please select from 1 to 6." << endl;
        }
    }

    return 0;
}
