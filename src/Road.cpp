#include "Road.h"
#include <iostream>
#include <vector>

using namespace std;

Road::Road() : direction("Unknown"), sensorVehicleCount(0), maxWaitingTime(0) {}

Road::Road(string dir, int initVehicleCount, int initWaitTime)
    : direction(dir), sensorVehicleCount(initVehicleCount), maxWaitingTime(initWaitTime) {}

void Road::addNormalVehicle(const Vehicle& v) {
    normalQueue.push(v);
}

void Road::addEmergencyVehicle(const Vehicle& v) {
    emergencyQueue.push(v);
}

bool Road::processNormalVehicle(Vehicle& v) {
    if (normalQueue.empty()) {
        return false;
    }
    v = normalQueue.front();
    normalQueue.pop();
    return true;
}

bool Road::processEmergencyVehicle(Vehicle& v) {
    if (emergencyQueue.empty()) {
        return false;
    }
    v = emergencyQueue.top();
    emergencyQueue.pop();
    return true;
}

void Road::displayNormalQueue() const {
    cout << "--- " << direction << " Road: Normal Vehicles ---\n";
    if (normalQueue.empty()) {
        cout << "Queue is empty.\n";
        return;
    }
    
    // We cannot iterate a std::queue directly without modifying it.
    // We create a copy to display its contents safely.
    queue<Vehicle> tempQueue = normalQueue;
    cout << "Total waiting: " << tempQueue.size() << "\n";
    int pos = 1;
    while (!tempQueue.empty()) {
        Vehicle v = tempQueue.front();
        cout << pos << ". ID: " << v.id << ", Type: " << v.type << ", Wait Time: " << v.waitTime << "s\n";
        tempQueue.pop();
        pos++;
    }
}

void Road::displayEmergencyQueue() const {
    cout << "--- " << direction << " Road: Emergency Vehicles ---\n";
    if (emergencyQueue.empty()) {
        cout << "Queue is empty.\n";
        return;
    }
    
    // Create a copy to display its contents safely.
    priority_queue<Vehicle> tempQueue = emergencyQueue;
    cout << "Total waiting: " << tempQueue.size() << "\n";
    int pos = 1;
    while (!tempQueue.empty()) {
        Vehicle v = tempQueue.top();
        string prioStr;
        if (v.emergencyPriority == 1) prioStr = "Ambulance";
        else if (v.emergencyPriority == 2) prioStr = "Fire Truck";
        else if (v.emergencyPriority == 3) prioStr = "Police";
        else prioStr = "Other";

        cout << pos << ". ID: " << v.id << ", Type: " << v.type << " (" << prioStr 
             << "), Wait Time: " << v.waitTime << "s\n";
        tempQueue.pop();
        pos++;
    }
}

void Road::displayTrafficInfo() const {
    cout << "[" << direction << " Road Info]\n";
    cout << "Simulated Sensor Count: " << sensorVehicleCount << "\n";
    cout << "Simulated Max Wait Time: " << maxWaitingTime << "s\n";
    cout << "Normal Queue Size: " << normalQueue.size() << "\n";
    cout << "Emergency Queue Size: " << emergencyQueue.size() << "\n";
}
