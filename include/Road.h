#pragma once
#include <string>
#include <queue>
#include "Vehicle.h"

// Class representing a road at the intersection
class Road {
public:
    std::string direction; // "North" or "South"
    
    // FIFO queue for normal vehicles
    std::queue<Vehicle> normalQueue;
    
    // Priority queue for emergency vehicles
    std::priority_queue<Vehicle> emergencyQueue;
    
    // Simulated sensor inputs
    int sensorVehicleCount;
    int maxWaitingTime;

    Road();
    Road(std::string dir, int initVehicleCount, int initWaitTime);

    void addNormalVehicle(const Vehicle& v);
    void addEmergencyVehicle(const Vehicle& v);
    bool processNormalVehicle(Vehicle& v);
    bool processEmergencyVehicle(Vehicle& v);
    void displayNormalQueue() const;
    void displayEmergencyQueue() const;
    void displayTrafficInfo() const;
};
