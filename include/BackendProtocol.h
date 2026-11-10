#pragma once
#include "TrafficManager.h"
#include "Graph.h"
#include <string>
#include <nlohmann/json.hpp>

class BackendProtocol {
private:
    TrafficManager tm;
    Graph graph;

    void handleAddNormalVehicle(const nlohmann::json& req, nlohmann::json& res);
    void handleAddEmergencyVehicle(const nlohmann::json& req, nlohmann::json& res);
    void handleProcessNormalVehicle(const nlohmann::json& req, nlohmann::json& res);
    void handleProcessEmergencyVehicle(const nlohmann::json& req, nlohmann::json& res);
    void handleGetNormalVehicles(const nlohmann::json& req, nlohmann::json& res);
    void handleGetEmergencyVehicles(const nlohmann::json& req, nlohmann::json& res);
    void handleGetTrafficInfo(const nlohmann::json& req, nlohmann::json& res);
    void handleUpdateSensorInfo(const nlohmann::json& req, nlohmann::json& res);
    void handleGetRoads(const nlohmann::json& req, nlohmann::json& res);
    void handleAddRoad(const nlohmann::json& req, nlohmann::json& res);

    void handleGetJunctions(const nlohmann::json& req, nlohmann::json& res);
    void handleGetGraphConnections(const nlohmann::json& req, nlohmann::json& res);
    void handleFindShortestPath(const nlohmann::json& req, nlohmann::json& res);
    void handleAddJunction(const nlohmann::json& req, nlohmann::json& res);
    void handleConnectRoad(const nlohmann::json& req, nlohmann::json& res);

    void handlePing(const nlohmann::json& req, nlohmann::json& res);

    nlohmann::json vehicleToJson(const Vehicle& v);

public:
    BackendProtocol();
    void run();
};
