#pragma once
#include "Road.h"
#include <unordered_set>
#include <string>

// Class representing the Traffic Manager system
class TrafficManager {
private:
    Road northRoad;
    Road southRoad;
    int emergencyCounter; // To keep track of insertion order for emergency vehicles
    std::unordered_set<std::string> activeVehicleIDs;

    int getNextEmergencyInsertionOrder();
    void clearInputBuffer();

    Road* selectRoadConsole(); // Renamed from selectRoad

public:
    TrafficManager();

    // Core Backend Operations
    bool addNormalVehicle(const std::string& roadId, const std::string& vehicleId, const std::string& type, int waitTime, std::string& errorMessage);
    bool addEmergencyVehicle(const std::string& roadId, const std::string& vehicleId, const std::string& type, int waitTime, int priority, std::string& errorMessage);
    bool processNormalVehicle(const std::string& roadId, Vehicle& processedVehicle, std::string& errorMessage);
    bool processEmergencyVehicle(const std::string& roadId, Vehicle& processedVehicle, std::string& errorMessage);
    bool updateSensorInfo(const std::string& roadId, int count, int waitTime, std::string& errorMessage);
    
    Road* getRoad(const std::string& roadId);
    bool isVehicleIDExists(const std::string& id) const;

    const Road& getNorthRoad() const { return northRoad; }
    const Road& getSouthRoad() const { return southRoad; }

    // Console Menu Operations
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
