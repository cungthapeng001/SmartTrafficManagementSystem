#pragma once
#include "TrafficManager.h"
#include <string>
#include <nlohmann/json.hpp>

class BackendProtocol {
private:
    TrafficManager tm;

    void handleAddNormalVehicle(const nlohmann::json& req, nlohmann::json& res);
    void handleAddEmergencyVehicle(const nlohmann::json& req, nlohmann::json& res);
    void handleProcessNormalVehicle(const nlohmann::json& req, nlohmann::json& res);
    void handleProcessEmergencyVehicle(const nlohmann::json& req, nlohmann::json& res);
    void handleGetNormalVehicles(const nlohmann::json& req, nlohmann::json& res);
    void handleGetEmergencyVehicles(const nlohmann::json& req, nlohmann::json& res);
    void handleGetTrafficInfo(const nlohmann::json& req, nlohmann::json& res);
    void handleUpdateSensorInfo(const nlohmann::json& req, nlohmann::json& res);
    void handlePing(const nlohmann::json& req, nlohmann::json& res);

    nlohmann::json vehicleToJson(const Vehicle& v);

public:
    BackendProtocol();
    void run();
};
