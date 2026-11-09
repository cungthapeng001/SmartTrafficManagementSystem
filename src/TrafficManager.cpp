#include "TrafficManager.h"
#include <iostream>
#include <limits>
#include <string>
#include <algorithm>
#include <cctype>

using namespace std;

// Helper: convert string to uppercase for case-insensitive lookup
static string toUpperStr(const string& s) {
    string res = s;
    for (char& c : res) {
        c = static_cast<char>(toupper(static_cast<unsigned char>(c)));
    }
    return res;
}

TrafficManager::TrafficManager() : emergencyCounter(0) {
    // Initialize default set of roads representing a multi-road network
    roads.push_back(Road("NORTH", "North Road", 15, 45));
    roads.push_back(Road("SOUTH", "South Road", 5, 20));
    roads.push_back(Road("EAST", "East Road", 8, 30));
    roads.push_back(Road("WEST", "West Road", 3, 15));
    roads.push_back(Road("CENTRAL", "Central Road", 12, 50));
}

int TrafficManager::getNextEmergencyInsertionOrder() {
    return ++emergencyCounter;
}

void TrafficManager::clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

Road* TrafficManager::selectRoadConsole() {
    if (roads.empty()) {
        cout << "No roads currently available.\n";
        return nullptr;
    }

    int choice;
    while (true) {
        cout << "\nSelect Road:\n";
        for (size_t i = 0; i < roads.size(); ++i) {
            cout << (i + 1) << ". " << roads[i].direction << " (" << roads[i].roadId << ")\n";
        }
        cout << "Choice (1-" << roads.size() << "): ";
        cin >> choice;

        if (cin.fail() || choice < 1 || choice > static_cast<int>(roads.size())) {
            clearInputBuffer();
            cout << "Invalid choice! Please select a number between 1 and " << roads.size() << ".\n";
        } else {
            return &roads[choice - 1];
        }
    }
}

Road* TrafficManager::getRoad(const std::string& roadId) {
    if (roadId.empty()) return nullptr;

    string target = toUpperStr(roadId);

    // Exact match on roadId or direction
    for (size_t i = 0; i < roads.size(); ++i) {
        if (toUpperStr(roads[i].roadId) == target || toUpperStr(roads[i].direction) == target) {
            return &roads[i];
        }
    }

    // Substring / contains match (e.g. "NORTH" matches "North Road")
    for (size_t i = 0; i < roads.size(); ++i) {
        string rId = toUpperStr(roads[i].roadId);
        string rDir = toUpperStr(roads[i].direction);
        if (rDir.find(target) != string::npos || target.find(rId) != string::npos) {
            return &roads[i];
        }
    }

    return nullptr;
}

const std::vector<Road>& TrafficManager::getAllRoads() const {
    return roads;
}

std::vector<std::string> TrafficManager::getRoadIds() const {
    std::vector<std::string> ids;
    for (const auto& r : roads) {
        ids.push_back(r.roadId);
    }
    return ids;
}

bool TrafficManager::addRoad(const std::string& roadId, const std::string& displayName, int sensorCount, int waitTime, std::string& errorMessage) {
    if (roadId.empty()) {
        errorMessage = "Road ID cannot be empty.";
        return false;
    }

    // Check if ID already exists
    if (getRoad(roadId) != nullptr) {
        errorMessage = "A road with this ID or name already exists.";
        return false;
    }

    string name = displayName.empty() ? roadId : displayName;
    int count = sensorCount < 0 ? 0 : sensorCount;
    int wait = waitTime < 0 ? 0 : waitTime;

    roads.push_back(Road(roadId, name, count, wait));
    return true;
}

bool TrafficManager::isVehicleIDExists(const std::string& id) const {
    return activeVehicleIDs.find(id) != activeVehicleIDs.end();
}

bool TrafficManager::addNormalVehicle(const std::string& roadId,
                                      const std::string& vehicleId,
                                      const std::string& type, int waitTime,
                                      std::string& errorMessage) {
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
        errorMessage = "Invalid road ID: " + roadId;
        return false;
    }

    Vehicle v(vehicleId, type, waitTime, false, 0, 0);
    road->addNormalVehicle(v);
    activeVehicleIDs.insert(vehicleId);
    return true;
}

bool TrafficManager::addEmergencyVehicle(const std::string& roadId,
                                         const std::string& vehicleId,
                                         const std::string& type, int waitTime,
                                         int priority,
                                         std::string& errorMessage) {
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
        errorMessage = "Invalid road ID: " + roadId;
        return false;
    }

    Vehicle v(vehicleId, type, waitTime, true, priority, getNextEmergencyInsertionOrder());
    road->addEmergencyVehicle(v);
    activeVehicleIDs.insert(vehicleId);
    return true;
}

bool TrafficManager::processNormalVehicle(const std::string& roadId,
                                          Vehicle& processedVehicle,
                                          std::string& errorMessage) {
    Road* road = getRoad(roadId);
    if (!road) {
        errorMessage = "Invalid road ID: " + roadId;
        return false;
    }
    if (road->processNormalVehicle(processedVehicle)) {
        activeVehicleIDs.erase(processedVehicle.id);
        return true;
    }
    errorMessage = "No normal vehicles waiting on " + road->direction + ".";
    return false;
}

bool TrafficManager::processEmergencyVehicle(const std::string& roadId,
                                             Vehicle& processedVehicle,
                                             std::string& errorMessage) {
    Road* road = getRoad(roadId);
    if (!road) {
        errorMessage = "Invalid road ID: " + roadId;
        return false;
    }
    if (road->processEmergencyVehicle(processedVehicle)) {
        activeVehicleIDs.erase(processedVehicle.id);
        return true;
    }
    errorMessage = "No emergency vehicles waiting on " + road->direction + ".";
    return false;
}

bool TrafficManager::updateSensorInfo(const std::string& roadId, int count,
                                      int waitTime, std::string& errorMessage) {
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
        errorMessage = "Invalid road ID: " + roadId;
        return false;
    }
    road->sensorVehicleCount = count;
    road->maxWaitingTime = waitTime;
    return true;
}

const Road& TrafficManager::getNorthRoad() const {
    for (const auto& r : roads) {
        if (toUpperStr(r.roadId) == "NORTH") return r;
    }
    return roads.front();
}

const Road& TrafficManager::getSouthRoad() const {
    for (const auto& r : roads) {
        if (toUpperStr(r.roadId) == "SOUTH") return r;
    }
    return roads.back();
}

void TrafficManager::runMenu() {
    int choice = 0;
    while (choice != 11) {
        cout << "\n========== SMART TRAFFIC MANAGEMENT SYSTEM (MULTI-ROAD) ==========\n";
        cout << "1. Add Normal Vehicle\n";
        cout << "2. Add Emergency Vehicle\n";
        cout << "3. Process Normal Vehicle\n";
        cout << "4. Process Emergency Vehicle\n";
        cout << "5. Display Normal Vehicle Queues\n";
        cout << "6. Display Emergency Priority Queues\n";
        cout << "7. Enter or Update Sensor Information\n";
        cout << "8. Display Traffic Information\n";
        cout << "9. Add New Road\n";
        cout << "10. View All Roads\n";
        cout << "11. Exit\n";
        cout << "===================================================================\n";
        cout << "Enter your choice: ";

        cin >> choice;

        if (cin.fail()) {
            clearInputBuffer();
            cout << "Invalid input! Please enter a number between 1 and 11.\n";
            continue;
        }

        switch (choice) {
        case 1:  addNormalVehicleMenu(); break;
        case 2:  addEmergencyVehicleMenu(); break;
        case 3:  processNormalVehicleMenu(); break;
        case 4:  processEmergencyVehicleMenu(); break;
        case 5:  displayNormalQueuesMenu(); break;
        case 6:  displayEmergencyQueuesMenu(); break;
        case 7:  updateSensorInfoMenu(); break;
        case 8:  displayTrafficInfoMenu(); break;
        case 9:  addRoadMenu(); break;
        case 10: displayAllRoadsMenu(); break;
        case 11: cout << "Exiting system. Goodbye!\n"; break;
        default: cout << "Invalid choice! Please select 1-11.\n"; break;
        }
    }
}

void TrafficManager::addRoadMenu() {
    cout << "\n[Add New Road]\n";
    string id, name;
    int count, wait;

    cout << "Enter unique Road ID (e.g. ROAD_A, EAST_EXPRESSWAY): ";
    cin >> id;
    if (cin.fail()) { clearInputBuffer(); cout << "Invalid road ID.\n"; return; }

    cout << "Enter Display Name (e.g. Road A): ";
    cin.ignore();
    getline(cin, name);

    cout << "Enter simulated vehicle count (>= 0): ";
    cin >> count;
    if (cin.fail()) { clearInputBuffer(); count = 0; }

    cout << "Enter simulated max waiting time (seconds, >= 0): ";
    cin >> wait;
    if (cin.fail()) { clearInputBuffer(); wait = 0; }

    string errorMsg;
    if (addRoad(id, name, count, wait, errorMsg)) {
        cout << "Successfully added road: " << name << " (" << id << ")\n";
    } else {
        cout << "Error: " << errorMsg << "\n";
    }
}

void TrafficManager::displayAllRoadsMenu() {
    cout << "\n========== ALL MANAGED ROADS (" << roads.size() << ") ==========\n";
    for (size_t i = 0; i < roads.size(); ++i) {
        cout << (i + 1) << ". " << roads[i].direction << " [ID: " << roads[i].roadId << "]\n";
        cout << "   Sensor Vehicles: " << roads[i].sensorVehicleCount
             << " | Max Wait: " << roads[i].maxWaitingTime << "s"
             << " | Normal Queue: " << roads[i].normalQueue.size()
             << " | Emergency Queue: " << roads[i].emergencyQueue.size() << "\n";
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
    if (!road) return;

    cout << "Enter waiting time (seconds): ";
    cin >> waitTime;
    if (cin.fail()) {
        clearInputBuffer();
        cout << "Invalid wait time.\n";
        return;
    }

    string errorMsg;
    if (addNormalVehicle(road->roadId, id, type, waitTime, errorMsg)) {
        cout << "Added Normal Vehicle (ID: " << id << ", Type: " << type << ") to "
             << road->direction << ".\n";
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
    if (typeChoice == 1) {
        type = "Ambulance";
        priority = 1;
    } else if (typeChoice == 2) {
        type = "Fire Truck";
        priority = 2;
    } else {
        type = "Police Vehicle";
        priority = 3;
    }

    Road* road = selectRoadConsole();
    if (!road) return;

    int waitTime;
    cout << "Enter waiting time (seconds): ";
    cin >> waitTime;
    if (cin.fail()) {
        clearInputBuffer();
        cout << "Invalid wait time.\n";
        return;
    }

    string errorMsg;
    if (addEmergencyVehicle(road->roadId, id, type, waitTime, priority, errorMsg)) {
        cout << "Added Emergency Vehicle (ID: " << id << ", Type: " << type
             << ", Priority: " << priority << ") to " << road->direction << ".\n";
    } else {
        cout << "Error: " << errorMsg << "\n";
    }
}

void TrafficManager::processNormalVehicleMenu() {
    cout << "\n[Process Normal Vehicle]\n";
    Road* road = selectRoadConsole();
    if (!road) return;

    Vehicle processedVehicle;
    string errorMsg;
    if (processNormalVehicle(road->roadId, processedVehicle, errorMsg)) {
        cout << "Processed Normal Vehicle -> ID: " << processedVehicle.id
             << ", Type: " << processedVehicle.type << "\n";
    } else {
        cout << errorMsg << "\n";
    }
}

void TrafficManager::processEmergencyVehicleMenu() {
    cout << "\n[Process Emergency Vehicle]\n";
    Road* road = selectRoadConsole();
    if (!road) return;

    Vehicle processedVehicle;
    string errorMsg;
    if (processEmergencyVehicle(road->roadId, processedVehicle, errorMsg)) {
        cout << "Processed Emergency Vehicle -> ID: " << processedVehicle.id
             << ", Type: " << processedVehicle.type
             << ", Priority: " << processedVehicle.emergencyPriority << "\n";
    } else {
        cout << errorMsg << "\n";
    }
}

void TrafficManager::displayNormalQueuesMenu() {
    cout << "\n========== NORMAL VEHICLE QUEUES ==========\n";
    for (const auto& road : roads) {
        cout << "\n";
        road.displayNormalQueue();
    }
}

void TrafficManager::displayEmergencyQueuesMenu() {
    cout << "\n========== EMERGENCY PRIORITY QUEUES ==========\n";
    for (const auto& road : roads) {
        cout << "\n";
        road.displayEmergencyQueue();
    }
}

void TrafficManager::updateSensorInfoMenu() {
    cout << "\n[Update Sensor Information]\n";
    Road* road = selectRoadConsole();
    if (!road) return;

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

    string errorMsg;
    if (updateSensorInfo(road->roadId, count, time, errorMsg)) {
        cout << "Sensor information updated for " << road->direction << ".\n";
    } else {
        cout << "Error: " << errorMsg << "\n";
    }
}

void TrafficManager::displayTrafficInfoMenu() {
    cout << "\n========== TRAFFIC SENSOR & QUEUE INFO ==========\n";
    for (const auto& road : roads) {
        cout << "\n";
        road.displayTrafficInfo();
    }
}
