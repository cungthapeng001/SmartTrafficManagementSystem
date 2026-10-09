#pragma once
#include "Road.h"

// Class representing the Traffic Manager system
class TrafficManager {
private:
    Road northRoad;
    Road southRoad;
    int vehicleCounter;
    int emergencyCounter; // To keep track of insertion order for emergency vehicles

    int getNextVehicleId();
    int getNextEmergencyInsertionOrder();
    void clearInputBuffer();

    Road* selectRoad();

public:
    TrafficManager();

    void runMenu();
    void addNormalVehicleMenu();
    void addEmergencyVehicleMenu();
    void processNormalVehicleMenu();
    void processEmergencyVehicleMenu();
    void displayNormalQueuesMenu();
    void displayEmergencyQueuesMenu();
    void updateSensorInfoMenu();
    void displayTrafficInfoMenu();
};
