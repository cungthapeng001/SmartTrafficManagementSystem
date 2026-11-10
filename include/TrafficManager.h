#pragma once
#include "Road.h"
#include <unordered_set>
#include <vector>
#include <string>

class TrafficManager {
private:
    std::vector<Road> roads;
    int emergencyCounter;
    std::unordered_set<std::string> activeVehicleIDs;

    int getNextEmergencyInsertionOrder();
    void clearInputBuffer();

    Road* selectRoadConsole();

public:
    TrafficManager();

    bool addRoad(const std::string& roadId, const std::string& displayName, int sensorCount, int waitTime, std::string& errorMessage);
    Road* getRoad(const std::string& roadId);
    const std::vector<Road>& getAllRoads() const;
    std::vector<std::string> getRoadIds() const;

    bool addNormalVehicle(const std::string& roadId, const std::string& vehicleId, const std::string& type, int waitTime, std::string& errorMessage);
    bool addEmergencyVehicle(const std::string& roadId, const std::string& vehicleId, const std::string& type, int waitTime, int priority, std::string& errorMessage);
    bool processNormalVehicle(const std::string& roadId, Vehicle& processedVehicle, std::string& errorMessage);
    bool processEmergencyVehicle(const std::string& roadId, Vehicle& processedVehicle, std::string& errorMessage);
    bool updateSensorInfo(const std::string& roadId, int count, int waitTime, std::string& errorMessage);

    bool isVehicleIDExists(const std::string& id) const;

    const Road& getNorthRoad() const;
    const Road& getSouthRoad() const;

    void runMenu();
    void addRoadMenu();
    void addNormalVehicleMenu();
    void addEmergencyVehicleMenu();
    void processNormalVehicleMenu();
    void processEmergencyVehicleMenu();
    void displayNormalQueuesMenu();
    void displayEmergencyQueuesMenu();
    void updateSensorInfoMenu();
    void displayTrafficInfoMenu();
    void displayAllRoadsMenu();
};
