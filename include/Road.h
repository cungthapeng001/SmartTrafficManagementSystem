#pragma once
#include <string>
#include <queue>
#include "Vehicle.h"

// Road class represents an individual road in the Multi-Road Traffic Management System.
// Each road maintains its own FIFO normal queue, priority emergency queue, and sensor readings.
class Road {
public:
    std::string roadId;        // Unique identifier (e.g., "NORTH", "SOUTH", "EAST", "ROAD_A")
    std::string direction;     // Display name (e.g., "North Road", "East Road")
    
    std::queue<Vehicle> normalQueue;              // FIFO queue for normal vehicles
    std::priority_queue<Vehicle> emergencyQueue;  // Priority queue for emergency vehicles
    
    int sensorVehicleCount;    // Simulated sensor count of waiting vehicles
    int maxWaitingTime;        // Simulated maximum waiting time in seconds

    Road();
    Road(std::string dir, int initVehicleCount, int initWaitTime);
    Road(std::string id, std::string dir, int initVehicleCount, int initWaitTime);

    void addNormalVehicle(const Vehicle& v);
    void addEmergencyVehicle(const Vehicle& v);
    bool processNormalVehicle(Vehicle& v);
    bool processEmergencyVehicle(Vehicle& v);
    void displayNormalQueue() const;
    void displayEmergencyQueue() const;
    void displayTrafficInfo() const;
};
