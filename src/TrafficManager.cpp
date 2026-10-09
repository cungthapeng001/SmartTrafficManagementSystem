#include "TrafficManager.h"
#include <iostream>
#include <string>
#include <limits>

using namespace std;

TrafficManager::TrafficManager() : emergencyCounter(0) {
    northRoad = Road("North", 15, 45); // Initial mock data from phase 1
    southRoad = Road("South", 5, 20);
}

int TrafficManager::getNextEmergencyInsertionOrder() {
    return ++emergencyCounter;
}

void TrafficManager::clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

Road* TrafficManager::selectRoad() {
    int choice;
    cout << "Select Road:\n1. North Road\n2. South Road\nChoice: ";
    cin >> choice;
    if (cin.fail() || (choice != 1 && choice != 2)) {
        clearInputBuffer();
        cout << "Invalid choice. Defaulting to North Road.\n";
        return &northRoad;
    }
    return (choice == 1) ? &northRoad : &southRoad;
}

void TrafficManager::runMenu() {
    int choice = 0;
    while (choice != 9) {
        cout << "\n========== SMART TRAFFIC MANAGEMENT SYSTEM ==========\n";
        cout << "1. Add Normal Vehicle\n";
        cout << "2. Add Emergency Vehicle\n";
        cout << "3. Process Normal Vehicle\n";
        cout << "4. Process Emergency Vehicle\n";
        cout << "5. Display Normal Vehicle Queues\n";
        cout << "6. Display Emergency Priority Queues\n";
        cout << "7. Enter or Update Sensor Information\n";
        cout << "8. Display Traffic Information\n";
        cout << "9. Exit\n";
        cout << "=======================================================\n";
        cout << "Enter your choice: ";
        
        cin >> choice;

        if (cin.fail()) {
            clearInputBuffer();
            cout << "Invalid input! Please enter a number between 1 and 9.\n";
            continue;
        }

        switch (choice) {
            case 1: addNormalVehicleMenu(); break;
            case 2: addEmergencyVehicleMenu(); break;
            case 3: processNormalVehicleMenu(); break;
            case 4: processEmergencyVehicleMenu(); break;
            case 5: displayNormalQueuesMenu(); break;
            case 6: displayEmergencyQueuesMenu(); break;
            case 7: updateSensorInfoMenu(); break;
            case 8: displayTrafficInfoMenu(); break;
            case 9: cout << "Exiting system. Goodbye!\n"; break;
            default: cout << "Invalid choice! Please select 1-9.\n"; break;
        }
    }
}

void TrafficManager::addNormalVehicleMenu() {
    cout << "\n[Add Normal Vehicle]\n";
    Road* road = selectRoad();
    
    int id;
    string type;
    int waitTime;
    cout << "Enter vehicle ID: ";
    cin >> id;
    if (cin.fail()) {
        clearInputBuffer();
        cout << "Invalid vehicle ID.\n";
        return;
    }
    cout << "Enter vehicle type (e.g., Car, Bus): ";
    cin >> type;
    cout << "Enter waiting time (seconds): ";
    cin >> waitTime;
    
    if (cin.fail() || waitTime < 0) {
        clearInputBuffer();
        cout << "Invalid wait time. Must be a positive integer.\n";
        return;
    }

    Vehicle v(id, type, waitTime, false, 0, 0);
    road->addNormalVehicle(v);
}

void TrafficManager::addEmergencyVehicleMenu() {
    cout << "\n[Add Emergency Vehicle]\n";
    Road* road = selectRoad();
    
    int typeChoice;
    cout << "Select Emergency Type:\n1. Ambulance (Priority 1)\n2. Fire Truck (Priority 2)\n3. Police Vehicle (Priority 3)\nChoice: ";
    cin >> typeChoice;
    
    if (cin.fail() || typeChoice < 1 || typeChoice > 3) {
        clearInputBuffer();
        cout << "Invalid choice.\n";
        return;
    }
    
    string type;
    int priority;
    if (typeChoice == 1) { type = "Ambulance"; priority = 1; }
    else if (typeChoice == 2) { type = "Fire Truck"; priority = 2; }
    else { type = "Police Vehicle"; priority = 3; }

    int id;
    cout << "Enter vehicle ID: ";
    cin >> id;
    if (cin.fail()) {
        clearInputBuffer();
        cout << "Invalid vehicle ID.\n";
        return;
    }

    int waitTime;
    cout << "Enter waiting time (seconds): ";
    cin >> waitTime;
    if (cin.fail() || waitTime < 0) {
        clearInputBuffer();
        cout << "Invalid wait time. Must be a positive integer.\n";
        return;
    }

    Vehicle v(id, type, waitTime, true, priority, getNextEmergencyInsertionOrder());
    road->addEmergencyVehicle(v);
}

void TrafficManager::processNormalVehicleMenu() {
    cout << "\n[Process Normal Vehicle]\n";
    Road* road = selectRoad();
    Vehicle processedVehicle;
    if (road->processNormalVehicle(processedVehicle)) {
        cout << "Processed Normal Vehicle -> ID: " << processedVehicle.id << ", Type: " << processedVehicle.type << "\n";
    } else {
        cout << "No normal vehicles waiting on " << road->direction << " Road.\n";
    }
}

void TrafficManager::processEmergencyVehicleMenu() {
    cout << "\n[Process Emergency Vehicle]\n";
    Road* road = selectRoad();
    Vehicle processedVehicle;
    if (road->processEmergencyVehicle(processedVehicle)) {
        cout << "Processed Emergency Vehicle -> ID: " << processedVehicle.id 
             << ", Type: " << processedVehicle.type << ", Priority: " << processedVehicle.emergencyPriority << "\n";
    } else {
        cout << "No emergency vehicles waiting on " << road->direction << " Road.\n";
    }
}

void TrafficManager::displayNormalQueuesMenu() {
    cout << "\n";
    northRoad.displayNormalQueue();
    cout << "\n";
    southRoad.displayNormalQueue();
}

void TrafficManager::displayEmergencyQueuesMenu() {
    cout << "\n";
    northRoad.displayEmergencyQueue();
    cout << "\n";
    southRoad.displayEmergencyQueue();
}

void TrafficManager::updateSensorInfoMenu() {
    cout << "\n[Update Sensor Information]\n";
    Road* road = selectRoad();
    
    int count, time;
    cout << "Enter simulated vehicle count (>= 0): ";
    cin >> count;
    if (cin.fail() || count < 0) {
        clearInputBuffer();
        cout << "Invalid count.\n";
        return;
    }
    
    cout << "Enter max waiting time (>= 0): ";
    cin >> time;
    if (cin.fail() || time < 0) {
        clearInputBuffer();
        cout << "Invalid time.\n";
        return;
    }
    
    road->sensorVehicleCount = count;
    road->maxWaitingTime = time;
    cout << "Sensor information updated for " << road->direction << " Road.\n";
}

void TrafficManager::displayTrafficInfoMenu() {
    cout << "\n";
    northRoad.displayTrafficInfo();
    cout << "\n";
    southRoad.displayTrafficInfo();
}
