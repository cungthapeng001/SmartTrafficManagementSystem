#include "BackendProtocol.h"
#include <iostream>

using namespace std;
using json = nlohmann::json;

BackendProtocol::BackendProtocol() {}

nlohmann::json BackendProtocol::vehicleToJson(const Vehicle& v) {
    json j;
    j["vehicle_id"] = v.id;
    j["vehicle_type"] = v.type;
    j["wait_time"] = v.waitTime;
    j["is_emergency"] = v.isEmergency;
    if (v.isEmergency) {
        j["priority"] = v.emergencyPriority;
    }
    return j;
}

void BackendProtocol::run() {
    string line;
    while (getline(cin, line)) {
        if (line.empty()) continue;
        
        json res;
        try {
            json req = json::parse(line);
            string op = req.value("operation", "");
            
            if (op == "shutdown") {
                res["success"] = true;
                res["message"] = "Shutting down";
                res["data"] = nullptr;
                cout << res.dump() << "\n";
                break;
            } else if (op == "add_normal_vehicle") {
                handleAddNormalVehicle(req, res);
            } else if (op == "add_emergency_vehicle") {
                handleAddEmergencyVehicle(req, res);
            } else if (op == "process_normal_vehicle") {
                handleProcessNormalVehicle(req, res);
            } else if (op == "process_emergency_vehicle") {
                handleProcessEmergencyVehicle(req, res);
            } else if (op == "get_normal_vehicles") {
                handleGetNormalVehicles(req, res);
            } else if (op == "get_emergency_vehicles") {
                handleGetEmergencyVehicles(req, res);
            } else if (op == "get_traffic_info") {
                handleGetTrafficInfo(req, res);
            } else if (op == "update_sensor_info") {
                handleUpdateSensorInfo(req, res);
            } else if (op == "ping") {
                handlePing(req, res);
            } else {
                res["success"] = false;
                res["message"] = "Unknown operation: " + op;
                res["data"] = nullptr;
            }
        } catch (json::exception& e) {
            res["success"] = false;
            res["message"] = "Invalid JSON format: " + string(e.what());
            res["data"] = nullptr;
        } catch (exception& e) {
            res["success"] = false;
            res["message"] = "Internal error: " + string(e.what());
            res["data"] = nullptr;
        }
        
        cout << res.dump() << "\n";
        cout.flush();
    }
}

void BackendProtocol::handleAddNormalVehicle(const json& req, json& res) {
    string roadId = req.value("road_id", "");
    string vehicleId = req.value("vehicle_id", "");
    string type = req.value("vehicle_type", "");
    int waitTime = req.value("waiting_time", -1);
    
    string errorMsg;
    if (tm.addNormalVehicle(roadId, vehicleId, type, waitTime, errorMsg)) {
        res["success"] = true;
        res["message"] = "Normal vehicle added successfully";
        res["data"] = {{"vehicle_id", vehicleId}, {"road_id", roadId}};
    } else {
        res["success"] = false;
        res["message"] = errorMsg;
        res["data"] = nullptr;
    }
}

void BackendProtocol::handleAddEmergencyVehicle(const json& req, json& res) {
    string roadId = req.value("road_id", "");
    string vehicleId = req.value("vehicle_id", "");
    string type = req.value("vehicle_type", "");
    int waitTime = req.value("waiting_time", -1);
    int priority = req.value("priority", -1);
    
    string errorMsg;
    if (tm.addEmergencyVehicle(roadId, vehicleId, type, waitTime, priority, errorMsg)) {
        res["success"] = true;
        res["message"] = "Emergency vehicle added successfully";
        res["data"] = {{"vehicle_id", vehicleId}, {"road_id", roadId}};
    } else {
        res["success"] = false;
        res["message"] = errorMsg;
        res["data"] = nullptr;
    }
}

void BackendProtocol::handleProcessNormalVehicle(const json& req, json& res) {
    string roadId = req.value("road_id", "");
    Vehicle processedVehicle;
    string errorMsg;
    
    if (tm.processNormalVehicle(roadId, processedVehicle, errorMsg)) {
        res["success"] = true;
        res["message"] = "Normal vehicle processed successfully";
        res["data"] = vehicleToJson(processedVehicle);
    } else {
        res["success"] = false;
        res["message"] = errorMsg;
        res["data"] = nullptr;
    }
}

void BackendProtocol::handleProcessEmergencyVehicle(const json& req, json& res) {
    string roadId = req.value("road_id", "");
    Vehicle processedVehicle;
    string errorMsg;
    
    if (tm.processEmergencyVehicle(roadId, processedVehicle, errorMsg)) {
        res["success"] = true;
        res["message"] = "Emergency vehicle processed successfully";
        res["data"] = vehicleToJson(processedVehicle);
    } else {
        res["success"] = false;
        res["message"] = errorMsg;
        res["data"] = nullptr;
    }
}

void BackendProtocol::handleGetNormalVehicles(const json& req, json& res) {
    string roadId = req.value("road_id", "");
    Road* road = tm.getRoad(roadId);
    if (!road) {
        res["success"] = false;
        res["message"] = "Invalid road ID.";
        res["data"] = nullptr;
        return;
    }
    
    json list = json::array();
    queue<Vehicle> tempQueue = road->normalQueue;
    while (!tempQueue.empty()) {
        list.push_back(vehicleToJson(tempQueue.front()));
        tempQueue.pop();
    }
    
    res["success"] = true;
    res["message"] = "Normal queue retrieved successfully";
    res["data"] = {{"vehicles", list}, {"count", list.size()}};
}

void BackendProtocol::handleGetEmergencyVehicles(const json& req, json& res) {
    string roadId = req.value("road_id", "");
    Road* road = tm.getRoad(roadId);
    if (!road) {
        res["success"] = false;
        res["message"] = "Invalid road ID.";
        res["data"] = nullptr;
        return;
    }
    
    json list = json::array();
    priority_queue<Vehicle> tempQueue = road->emergencyQueue;
    while (!tempQueue.empty()) {
        list.push_back(vehicleToJson(tempQueue.top()));
        tempQueue.pop();
    }
    
    res["success"] = true;
    res["message"] = "Emergency queue retrieved successfully";
    res["data"] = {{"vehicles", list}, {"count", list.size()}};
}

void BackendProtocol::handleGetTrafficInfo(const json& req, json& res) {
    string roadId = req.value("road_id", "");
    Road* road = tm.getRoad(roadId);
    if (!road) {
        res["success"] = false;
        res["message"] = "Invalid road ID.";
        res["data"] = nullptr;
        return;
    }
    
    res["success"] = true;
    res["message"] = "Traffic info retrieved successfully";
    res["data"] = {
        {"road_id", roadId},
        {"sensor_vehicle_count", road->sensorVehicleCount},
        {"max_waiting_time", road->maxWaitingTime},
        {"normal_queue_size", road->normalQueue.size()},
        {"emergency_queue_size", road->emergencyQueue.size()}
    };
}

void BackendProtocol::handleUpdateSensorInfo(const json& req, json& res) {
    string roadId = req.value("road_id", "");
    int count = req.value("sensor_count", -1);
    int waitTime = req.value("max_wait_time", -1);
    
    string errorMsg;
    if (tm.updateSensorInfo(roadId, count, waitTime, errorMsg)) {
        res["success"] = true;
        res["message"] = "Sensor information updated successfully";
        res["data"] = {{"road_id", roadId}};
    } else {
        res["success"] = false;
        res["message"] = errorMsg;
        res["data"] = nullptr;
    }
}

void BackendProtocol::handlePing(const json& req, json& res) {
    res["success"] = true;
    res["message"] = "pong";
    res["data"] = nullptr;
}
