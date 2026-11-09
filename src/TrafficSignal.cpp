#include "TrafficSignal.h"
#include <iostream>
#include <queue>    // needed to inspect priority_queue by copy

using namespace std;

// ---------------------------------------------------------
// classifyTraffic
// Converts raw sensor numbers into a simple traffic category.
// Uses the same thresholds as Part 3 (dynamic road weights)
// so the system is internally consistent.
// ---------------------------------------------------------
SignalTrafficLevel TrafficSignal::classifyTraffic(int vehicleCount,
                                                  int waitTime) const {
    if (vehicleCount <= 5 && waitTime <= 30) {
        return SIGNAL_LOW;
    } else if (vehicleCount <= 15 && waitTime <= 60) {
        return SIGNAL_MEDIUM;
    } else {
        return SIGNAL_HIGH;
    }
}

// ---------------------------------------------------------
// greenDuration
// Returns the number of green-light seconds for a given level.
// ---------------------------------------------------------
int TrafficSignal::greenDuration(SignalTrafficLevel level) const {
    if (level == SIGNAL_LOW)    return GREEN_LOW;
    if (level == SIGNAL_MEDIUM) return GREEN_MEDIUM;
    return GREEN_HIGH; // SIGNAL_HIGH
}

// ---------------------------------------------------------
// levelToString
// Converts the enum to a printable label.
// ---------------------------------------------------------
string TrafficSignal::levelToString(SignalTrafficLevel level) const {
    if (level == SIGNAL_LOW)    return "LOW";
    if (level == SIGNAL_MEDIUM) return "MEDIUM";
    return "HIGH";
}

// ---------------------------------------------------------
// scoreForComparison
// Produces a single number representing overall road pressure.
// Used to break ties when both roads share the same level.
// Formula: vehicleCount * 2 + (waitTime / 10)
//   - Vehicle count is weighted more heavily (each car = 2 pts)
//   - Wait time adds a small bonus per 10 seconds
// ---------------------------------------------------------
int TrafficSignal::scoreForComparison(int vehicleCount, int waitTime) const {
    return (vehicleCount * 2) + (waitTime / 10);
}

// ---------------------------------------------------------
// runSimulation
// Performs the complete adaptive signal decision and prints
// a formatted report to the console.
// ---------------------------------------------------------
void TrafficSignal::runSimulation(const Road& northRoad,
                                  const Road& southRoad) const {

    cout << "\n============================================================\n";
    cout << "         ADAPTIVE TRAFFIC SIGNAL SIMULATION\n";
    cout << "============================================================\n";

    // ---- Print the sensor readings for both roads --------
    cout << "\n[Sensor Readings]\n";
    cout << "  North Road: "
         << northRoad.sensorVehicleCount << " vehicles, "
         << northRoad.maxWaitingTime    << "s max wait"
         << ",  emergency queue size: "
         << northRoad.emergencyQueue.size() << "\n";
    cout << "  South Road: "
         << southRoad.sensorVehicleCount << " vehicles, "
         << southRoad.maxWaitingTime    << "s max wait"
         << ",  emergency queue size: "
         << southRoad.emergencyQueue.size() << "\n";

    // =====================================================
    // STEP 1 — Emergency vehicle check
    //
    // An emergency vehicle in the queue overrides all normal
    // traffic logic.  The road with the highest-priority
    // emergency vehicle (lowest priority number = most urgent)
    // gets the green light immediately.
    // =====================================================
    bool northHasEmergency = !northRoad.emergencyQueue.empty();
    bool southHasEmergency = !southRoad.emergencyQueue.empty();

    if (northHasEmergency || southHasEmergency) {
        cout << "\n[!] EMERGENCY VEHICLE DETECTED\n";

        string chosenRoad;

        if (northHasEmergency && !southHasEmergency) {
            // Only North has an emergency
            chosenRoad = "North";
        } else if (southHasEmergency && !northHasEmergency) {
            // Only South has an emergency
            chosenRoad = "South";
        } else {
            // Both roads have emergency vehicles —
            // compare the top vehicle's priority number.
            // Lower priority number = more urgent
            // (1=Ambulance, 2=Fire Truck, 3=Police)
            int northTopPriority = northRoad.emergencyQueue.top().emergencyPriority;
            int southTopPriority = southRoad.emergencyQueue.top().emergencyPriority;

            // LOWER number wins (ambulance beats police)
            if (northTopPriority <= southTopPriority) {
                chosenRoad = "North";
            } else {
                chosenRoad = "South";
            }
            cout << "    Both roads have emergency vehicles.\n";
            cout << "    North top priority: " << northTopPriority
                 << "  |  South top priority: " << southTopPriority << "\n";
        }

        cout << "\n[Signal Decision]\n";
        cout << "  Selected road    : " << chosenRoad << " Road\n";
        cout << "  Traffic category : EMERGENCY OVERRIDE\n";
        cout << "  Green duration   : " << GREEN_EMERGENCY << " seconds\n";
        cout << "\n  >> " << chosenRoad
             << " Road receives GREEN for " << GREEN_EMERGENCY
             << "s (emergency priority)\n";
        cout << "============================================================\n";
        return; // Emergency handled — stop here
    }

    // =====================================================
    // STEP 2 — Classify traffic level for each road
    // =====================================================
    SignalTrafficLevel northLevel = classifyTraffic(
        northRoad.sensorVehicleCount, northRoad.maxWaitingTime);
    SignalTrafficLevel southLevel = classifyTraffic(
        southRoad.sensorVehicleCount, southRoad.maxWaitingTime);

    cout << "\n[Traffic Classification]\n";
    cout << "  North Road: " << levelToString(northLevel) << "\n";
    cout << "  South Road: " << levelToString(southLevel) << "\n";

    // =====================================================
    // STEP 3 — Pick the road with higher traffic demand
    //
    // Compare levels as integers:
    //   SIGNAL_LOW=0, SIGNAL_MEDIUM=1, SIGNAL_HIGH=2
    // Higher integer = higher demand.
    //
    // If equal, use the numeric score to break the tie.
    // If scores are also equal, default to North Road.
    // =====================================================
    string chosenRoad;
    SignalTrafficLevel chosenLevel;

    if (northLevel > southLevel) {
        // North is busier
        chosenRoad  = "North";
        chosenLevel = northLevel;
        cout << "  Decision: North Road has higher traffic demand.\n";

    } else if (southLevel > northLevel) {
        // South is busier
        chosenRoad  = "South";
        chosenLevel = southLevel;
        cout << "  Decision: South Road has higher traffic demand.\n";

    } else {
        // Same level — use score to break the tie
        int northScore = scoreForComparison(northRoad.sensorVehicleCount,
                                            northRoad.maxWaitingTime);
        int southScore = scoreForComparison(southRoad.sensorVehicleCount,
                                            southRoad.maxWaitingTime);

        cout << "  Both roads share the same traffic level ("
             << levelToString(northLevel) << ").\n";
        cout << "  North score: " << northScore
             << "  |  South score: " << southScore << "\n";

        if (southScore > northScore) {
            chosenRoad  = "South";
            chosenLevel = southLevel;
            cout << "  Decision: South Road has higher demand score.\n";
        } else {
            // Equal scores — default to North
            chosenRoad  = "North";
            chosenLevel = northLevel;
            if (northScore == southScore) {
                cout << "  Decision: Scores are equal — defaulting to North Road.\n";
            } else {
                cout << "  Decision: North Road has higher demand score.\n";
            }
        }
    }

    // =====================================================
    // STEP 4 — Assign green-light duration from table
    // =====================================================
    int duration = greenDuration(chosenLevel);

    // =====================================================
    // STEP 5 — Display the final signal decision
    // =====================================================
    cout << "\n[Signal Decision]\n";
    cout << "  Selected road    : " << chosenRoad << " Road\n";
    cout << "  Traffic category : " << levelToString(chosenLevel) << "\n";
    cout << "  Green duration   : " << duration << " seconds\n";
    cout << "\n  >> " << chosenRoad
         << " Road receives GREEN for " << duration << "s\n";
    cout << "============================================================\n";
}

