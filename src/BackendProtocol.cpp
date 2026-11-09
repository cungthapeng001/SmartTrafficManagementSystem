#include "BackendProtocol.h"
#include <iostream>

using namespace std;
using json = nlohmann::json;

BackendProtocol::BackendProtocol() {
    // Initialize seven default junctions (A through G) with bidirectional connections
    graph.initSevenJunctionNetwork();
}

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
            } else if (op == "get_roads") {
                handleGetRoads(req, res);
            } else if (op == "add_road") {
                handleAddRoad(req, res);
            } else if (op == "get_junctions") {
                handleGetJunctions(req, res);
            } else if (op == "get_graph_connections") {
                handleGetGraphConnections(req, res);
            } else if (op == "find_shortest_path") {
                handleFindShortestPath(req, res);
            } else if (op == "add_junction") {
                handleAddJunction(req, res);
            } else if (op == "connect_junction_road") {
                handleConnectRoad(req, res);
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
        res["message"] = "Invalid road ID: " + roadId;
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
    res["data"] = {{"vehicles", list}, {"count", list.size()}, {"road_id", road->roadId}, {"display_name", road->direction}};
}

void BackendProtocol::handleGetEmergencyVehicles(const json& req, json& res) {
    string roadId = req.value("road_id", "");
    Road* road = tm.getRoad(roadId);
    if (!road) {
        res["success"] = false;
        res["message"] = "Invalid road ID: " + roadId;
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
    res["data"] = {{"vehicles", list}, {"count", list.size()}, {"road_id", road->roadId}, {"display_name", road->direction}};
}

void BackendProtocol::handleGetTrafficInfo(const json& req, json& res) {
    string roadId = req.value("road_id", "");
    Road* road = tm.getRoad(roadId);
    if (!road) {
        res["success"] = false;
        res["message"] = "Invalid road ID: " + roadId;
        res["data"] = nullptr;
        return;
    }
    
    res["success"] = true;
    res["message"] = "Traffic info retrieved successfully";
    res["data"] = {
        {"road_id", road->roadId},
        {"display_name", road->direction},
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

void BackendProtocol::handleGetRoads(const json& req, json& res) {
    const auto& roads = tm.getAllRoads();
    json list = json::array();
    for (const auto& r : roads) {
        json item;
        item["road_id"] = r.roadId;
        item["display_name"] = r.direction;
        item["sensor_vehicle_count"] = r.sensorVehicleCount;
        item["max_waiting_time"] = r.maxWaitingTime;
        item["normal_queue_size"] = r.normalQueue.size();
        item["emergency_queue_size"] = r.emergencyQueue.size();
        list.push_back(item);
    }
    res["success"] = true;
    res["message"] = "Roads retrieved successfully";
    res["data"] = {{"roads", list}, {"count", list.size()}};
}

void BackendProtocol::handleAddRoad(const json& req, json& res) {
    string roadId = req.value("road_id", "");
    string displayName = req.value("display_name", "");
    int count = req.value("sensor_count", 0);
    int wait = req.value("max_wait_time", 0);
    
    string errorMsg;
    if (tm.addRoad(roadId, displayName, count, wait, errorMsg)) {
        res["success"] = true;
        res["message"] = "Road added successfully";
        res["data"] = {
            {"road_id", roadId},
            {"display_name", displayName.empty() ? roadId : displayName}
        };
    } else {
        res["success"] = false;
        res["message"] = errorMsg;
        res["data"] = nullptr;
    }
}

// ---------------------------------------------------------
// Graph & Dijkstra Handlers
// ---------------------------------------------------------
void BackendProtocol::handleGetJunctions(const json& req, json& res) {
    vector<string> names = graph.getJunctionNames();
    json list = json::array();
    for (const auto& name : names) {
        list.push_back({
            {"name", name}
        });
    }
    res["success"] = true;
    res["message"] = "Junctions retrieved successfully";
    res["data"] = {
        {"junctions", list},
        {"count", list.size()}
    };
}

void BackendProtocol::handleGetGraphConnections(const json& req, json& res) {
    auto conns = graph.getRoadConnections();
    json list = json::array();
    for (const auto& c : conns) {
        list.push_back({
            {"from", c.from},
            {"to", c.to},
            {"weight", c.weight},
            {"base_weight", c.baseWeight}
        });
    }
    res["success"] = true;
    res["message"] = "Graph connections retrieved successfully";
    res["data"] = {
        {"connections", list},
        {"count", list.size()}
    };
}

void BackendProtocol::handleFindShortestPath(const json& req, json& res) {
    string start = req.value("start", "");
    string dest  = req.value("destination", "");
    if (start.empty()) start = req.value("from", "");
    if (dest.empty())  dest  = req.value("to", "");

    if (start.empty() || dest.empty()) {
        res["success"] = false;
        res["message"] = "Start and destination junctions must be specified.";
        res["data"] = nullptr;
        return;
    }

    vector<string> path;
    int totalCost = 0;
    string errorMsg;

    if (graph.computeShortestPath(start, dest, path, totalCost, errorMsg)) {
        string pathStr = "";
        for (size_t i = 0; i < path.size(); ++i) {
            pathStr += path[i];
            if (i + 1 < path.size()) pathStr += " -> ";
        }

        res["success"] = true;
        res["message"] = "Shortest route calculated successfully";
        res["data"] = {
            {"start", start},
            {"destination", dest},
            {"path", path},
            {"total_cost", totalCost},
            {"path_string", pathStr}
        };
    } else {
        res["success"] = false;
        res["message"] = errorMsg;
        res["data"] = nullptr;
    }
}

void BackendProtocol::handleAddJunction(const json& req, json& res) {
    string name = req.value("name", "");
    if (name.empty()) {
        res["success"] = false;
        res["message"] = "Junction name cannot be empty.";
        res["data"] = nullptr;
        return;
    }

    graph.addJunction(name);
    res["success"] = true;
    res["message"] = "Junction \"" + name + "\" added successfully.";
    res["data"] = {{"name", name}};
}

void BackendProtocol::handleConnectRoad(const json& req, json& res) {
    string from = req.value("from", "");
    string to   = req.value("to", "");
    int weight  = req.value("weight", 0);

    if (from.empty() || to.empty() || weight <= 0) {
        res["success"] = false;
        res["message"] = "Invalid parameters: from, to, and positive weight required.";
        res["data"] = nullptr;
        return;
    }

    graph.connectRoad(from, to, weight);
    res["success"] = true;
    res["message"] = "Road connected between \"" + from + "\" and \"" + to + "\".";
    res["data"] = {{"from", from}, {"to", to}, {"weight", weight}};
}

void BackendProtocol::handlePing(const json& req, json& res) {
    res["success"] = true;
    res["message"] = "pong";
    res["data"] = nullptr;
}
