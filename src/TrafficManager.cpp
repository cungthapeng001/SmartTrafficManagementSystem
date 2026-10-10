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

Road* TrafficManager::selectRoadConsole() {
    int choice;
    while (true) {
        cout << "Select Road:\n1. North Road\n2. South Road\nChoice: ";
        cin >> choice;
        if (cin.fail() || (choice != 1 && choice != 2)) {
            clearInputBuffer();
            cout << "Invalid choice. Please select 1 or 2.\n";
        } else {
            return (choice == 1) ? &northRoad : &southRoad;
        }
    }
}

Road* TrafficManager::getRoad(const std::string& roadId) {
    if (roadId == "NORTH") return &northRoad;
    if (roadId == "SOUTH") return &southRoad;
    return nullptr;
}

bool TrafficManager::isVehicleIDExists(const std::string& id) const {
    return activeVehicleIDs.find(id) != activeVehicleIDs.end();
}

bool TrafficManager::addNormalVehicle(const std::string& roadId, const std::string& vehicleId, const std::string& type, int waitTime, std::string& errorMessage) {
    if (vehicleId.empty()) {
        errorMessage = "Vehicle ID cannot be empty.";
        return false;
    }
    if (isVehicleIDExists(vehicleId)) {
        errorMessage = "Vehicle ID already exists.";
        return false;
    }
    if (type.empty()) {
        errorMessage = "Vehicle type cannot be empty.";
        return false;
    }
    if (waitTime < 0) {
        errorMessage = "Waiting time cannot be negative.";
        return false;
    }
    Road* road = getRoad(roadId);
    if (!road) {
        errorMessage = "Invalid road ID.";
        return false;
    }

    Vehicle v(vehicleId, type, waitTime, false, 0, 0);
    road->addNormalVehicle(v);
    activeVehicleIDs.insert(vehicleId);
    return true;
}

bool TrafficManager::addEmergencyVehicle(const std::string& roadId, const std::string& vehicleId, const std::string& type, int waitTime, int priority, std::string& errorMessage) {
    if (vehicleId.empty()) {
        errorMessage = "Vehicle ID cannot be empty.";
        return false;
    }
    if (isVehicleIDExists(vehicleId)) {
        errorMessage = "Vehicle ID already exists.";
        return false;
    }
    if (type.empty()) {
        errorMessage = "Vehicle type cannot be empty.";
        return false;
    }
    if (waitTime < 0) {
        errorMessage = "Waiting time cannot be negative.";
        return false;
    }
    if (priority < 1 || priority > 3) {
        errorMessage = "Invalid priority. Must be 1, 2, or 3.";
        return false;
    }
    Road* road = getRoad(roadId);
    if (!road) {
        errorMessage = "Invalid road ID.";
        return false;
    }

    Vehicle v(vehicleId, type, waitTime, true, priority, getNextEmergencyInsertionOrder());
    road->addEmergencyVehicle(v);
    activeVehicleIDs.insert(vehicleId);
    return true;
}

bool TrafficManager::processNormalVehicle(const std::string& roadId, Vehicle& processedVehicle, std::string& errorMessage) {
    Road* road = getRoad(roadId);
    if (!road) {
        errorMessage = "Invalid road ID.";
        return false;
    }
    if (road->processNormalVehicle(processedVehicle)) {
        activeVehicleIDs.erase(processedVehicle.id);
        return true;
    }
    errorMessage = "No normal vehicles waiting on this road.";
    return false;
}

bool TrafficManager::processEmergencyVehicle(const std::string& roadId, Vehicle& processedVehicle, std::string& errorMessage) {
    Road* road = getRoad(roadId);
    if (!road) {
        errorMessage = "Invalid road ID.";
        return false;
    }
    if (road->processEmergencyVehicle(processedVehicle)) {
        activeVehicleIDs.erase(processedVehicle.id);
        return true;
    }
    errorMessage = "No emergency vehicles waiting on this road.";
    return false;
}

bool TrafficManager::updateSensorInfo(const std::string& roadId, int count, int waitTime, std::string& errorMessage) {
    if (count < 0) {
        errorMessage = "Vehicle count cannot be negative.";
        return false;
    }
    if (waitTime < 0) {
        errorMessage = "Waiting time cannot be negative.";
        return false;
    }
    Road* road = getRoad(roadId);
    if (!road) {
        errorMessage = "Invalid road ID.";
        return false;
    }
    road->sensorVehicleCount = count;
    road->maxWaitingTime = waitTime;
    return true;
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
    
    string id;
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
    if (cin.fail()) {
        clearInputBuffer();
        cout << "Invalid vehicle type.\n";
        return;
    }
    
    Road* road = selectRoadConsole();

    cout << "Enter waiting time (seconds): ";
    cin >> waitTime;
    if (cin.fail()) {
        clearInputBuffer();
        cout << "Invalid wait time.\n";
        return;
    }

    string roadId = (road == &northRoad) ? "NORTH" : "SOUTH";
    string errorMsg;
    if (addNormalVehicle(roadId, id, type, waitTime, errorMsg)) {
        cout << "Added Normal Vehicle (ID: " << id << ", Type: " << type << ") to " << road->direction << " Road.\n";
    } else {
        cout << "Error: " << errorMsg << "\n";
    }
}

void TrafficManager::addEmergencyVehicleMenu() {
    cout << "\n[Add Emergency Vehicle]\n";
    
    string id;
    cout << "Enter vehicle ID: ";
    cin >> id;
    if (cin.fail()) {
        clearInputBuffer();
        cout << "Invalid vehicle ID.\n";
        return;
    }

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

    Road* road = selectRoadConsole();

    int waitTime;
    cout << "Enter waiting time (seconds): ";
    cin >> waitTime;
    if (cin.fail()) {
        clearInputBuffer();
        cout << "Invalid wait time.\n";
        return;
    }

    string roadId = (road == &northRoad) ? "NORTH" : "SOUTH";
    string errorMsg;
    if (addEmergencyVehicle(roadId, id, type, waitTime, priority, errorMsg)) {
        cout << "Added Emergency Vehicle (ID: " << id << ", Type: " << type 
             << ", Priority: " << priority << ") to " << road->direction << " Road.\n";
    } else {
        cout << "Error: " << errorMsg << "\n";
    }
}

void TrafficManager::processNormalVehicleMenu() {
    cout << "\n[Process Normal Vehicle]\n";
    Road* road = selectRoadConsole();
    Vehicle processedVehicle;
    string roadId = (road == &northRoad) ? "NORTH" : "SOUTH";
    string errorMsg;
    if (processNormalVehicle(roadId, processedVehicle, errorMsg)) {
        cout << "Processed Normal Vehicle -> ID: " << processedVehicle.id << ", Type: " << processedVehicle.type << "\n";
    } else {
        cout << errorMsg << "\n";
    }
}

void TrafficManager::processEmergencyVehicleMenu() {
    cout << "\n[Process Emergency Vehicle]\n";
    Road* road = selectRoadConsole();
    Vehicle processedVehicle;
    string roadId = (road == &northRoad) ? "NORTH" : "SOUTH";
    string errorMsg;
    if (processEmergencyVehicle(roadId, processedVehicle, errorMsg)) {
        cout << "Processed Emergency Vehicle -> ID: " << processedVehicle.id 
             << ", Type: " << processedVehicle.type << ", Priority: " << processedVehicle.emergencyPriority << "\n";
    } else {
        cout << errorMsg << "\n";
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
    Road* road = selectRoadConsole();
    
    int count, time;
    cout << "Enter simulated vehicle count (>= 0): ";
    cin >> count;
    if (cin.fail()) {
        clearInputBuffer();
        cout << "Invalid input.\n";
        return;
    }
    
    cout << "Enter max waiting time (>= 0): ";
    cin >> time;
    if (cin.fail()) {
        clearInputBuffer();
        cout << "Invalid input.\n";
        return;
    }
    
    string roadId = (road == &northRoad) ? "NORTH" : "SOUTH";
    string errorMsg;
    if (updateSensorInfo(roadId, count, time, errorMsg)) {
        cout << "Sensor information updated for " << road->direction << " Road.\n";
    } else {
        cout << "Error: " << errorMsg << "\n";
    }
}

void TrafficManager::displayTrafficInfoMenu() {
    cout << "\n";
    northRoad.displayTrafficInfo();
    cout << "\n";
    southRoad.displayTrafficInfo();
}
