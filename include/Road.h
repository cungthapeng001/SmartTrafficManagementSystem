#pragma once
#include <string>
#include <queue>
#include "Vehicle.h"

class Road {
public:
    std::string roadId;
    std::string direction;

    std::queue<Vehicle> normalQueue;
    std::priority_queue<Vehicle> emergencyQueue;

    int sensorVehicleCount;
    int maxWaitingTime;

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
