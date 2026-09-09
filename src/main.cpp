/*
Phase 1: Planning and Analysis
- Define basic architecture (Vehicle, Road).
- Plan Data Structures: std::queue (normal traffic), std::priority_queue (emergency).
*/

#include <iostream>
#include <queue>
#include <string>

using namespace std;

// Vehicle structure
struct Vehicle {
    // Properties to be added in Phase 2 (e.g., string type, int id)
};

// Road class
class Road {
public:
    // queue<Vehicle> normalTraffic;
    // priority_queue<Vehicle> emergencyTraffic;
    // int currentGreenLightDuration;
};

int main() {
    cout << "--- Smart Traffic Management System ---" << endl;
    cout << "Phase 1: Planning and Architecture Initialized." << endl;
    cout << "===============================================" << endl;

    // Simulated sensor inputs
    int sensorVehicleCountNorth = 15;
    int sensorVehicleCountSouth = 5;
    int maxWaitingTimeNorth = 45;
    bool emergencyVehicleDetected = true;

    cout << "[Sensor Data Log]" << endl;
    cout << "North Road Vehicle Count: " << sensorVehicleCountNorth << endl;
    cout << "South Road Vehicle Count: " << sensorVehicleCountSouth << endl;
    cout << "Emergency Vehicle Detected: " << (emergencyVehicleDetected ? "Yes" : "No") << endl;

    cout << "\nEnd of Phase 1 Simulation." << endl;

    return 0;
}
