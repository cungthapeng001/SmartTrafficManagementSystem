#include <iostream>
#include <queue>
#include <string>

using namespace std;

// Vehicle structure
struct Vehicle {
    int id;
    string type;
    bool isEmergency;
    int waitTime;

    // Simple priority rule: higher waitTime gets higher priority
    bool operator<(const Vehicle& other) const {
        return waitTime < other.waitTime;
    }
};

// Road class
class Road {
public:
    string roadName;
    int vehicleCount;
    queue<Vehicle> normalTraffic;
    priority_queue<Vehicle> emergencyTraffic;
};

int main() {
    cout << "--- Smart Traffic Management System ---" << endl;
    cout << "Phase 2: System Design Initialized." << endl;
    cout << "===============================================" << endl;

    // Simulated sensor inputs
    int sensorVehicleCountNorth = 15;
    int sensorVehicleCountSouth = 5;
    int maxWaitingTimeNorth = 45;
    bool emergencyVehicleDetected = true;

    cout << "[Sensor Data Log]" << endl;
    cout << "North Road Vehicle Count: " << sensorVehicleCountNorth << endl;
    cout << "South Road Vehicle Count: " << sensorVehicleCountSouth << endl;
    cout << "North Road Max Waiting Time: " << maxWaitingTimeNorth << "s" << endl;
    cout << "Emergency Vehicle Detected: " << (emergencyVehicleDetected ? "Yes" : "No") << endl;

    cout << "\nEnd of Phase 1 Simulation." << endl;

    return 0;
}
