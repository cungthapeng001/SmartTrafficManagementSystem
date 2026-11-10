#include "TrafficSignal.h"
#include <iostream>
#include <queue>

using namespace std;

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

int TrafficSignal::greenDuration(SignalTrafficLevel level) const {
    if (level == SIGNAL_LOW)    return GREEN_LOW;
    if (level == SIGNAL_MEDIUM) return GREEN_MEDIUM;
    return GREEN_HIGH;
}

string TrafficSignal::levelToString(SignalTrafficLevel level) const {
    if (level == SIGNAL_LOW)    return "LOW";
    if (level == SIGNAL_MEDIUM) return "MEDIUM";
    return "HIGH";
}

int TrafficSignal::scoreForComparison(int vehicleCount, int waitTime) const {
    return (vehicleCount * 2) + (waitTime / 10);
}

void TrafficSignal::runSimulation(const Road& northRoad,
                                  const Road& southRoad) const {

    cout << "\n============================================================\n";
    cout << "         ADAPTIVE TRAFFIC SIGNAL SIMULATION\n";
    cout << "============================================================\n";

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

    bool northHasEmergency = !northRoad.emergencyQueue.empty();
    bool southHasEmergency = !southRoad.emergencyQueue.empty();

    if (northHasEmergency || southHasEmergency) {
        cout << "\n[!] EMERGENCY VEHICLE DETECTED\n";

        string chosenRoad;

        if (northHasEmergency && !southHasEmergency) {

            chosenRoad = "North";
        } else if (southHasEmergency && !northHasEmergency) {

            chosenRoad = "South";
        } else {

            int northTopPriority = northRoad.emergencyQueue.top().emergencyPriority;
            int southTopPriority = southRoad.emergencyQueue.top().emergencyPriority;

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
        return;
    }

    SignalTrafficLevel northLevel = classifyTraffic(
        northRoad.sensorVehicleCount, northRoad.maxWaitingTime);
    SignalTrafficLevel southLevel = classifyTraffic(
        southRoad.sensorVehicleCount, southRoad.maxWaitingTime);

    cout << "\n[Traffic Classification]\n";
    cout << "  North Road: " << levelToString(northLevel) << "\n";
    cout << "  South Road: " << levelToString(southLevel) << "\n";

    string chosenRoad;
    SignalTrafficLevel chosenLevel;

    if (northLevel > southLevel) {

        chosenRoad  = "North";
        chosenLevel = northLevel;
        cout << "  Decision: North Road has higher traffic demand.\n";

    } else if (southLevel > northLevel) {

        chosenRoad  = "South";
        chosenLevel = southLevel;
        cout << "  Decision: South Road has higher traffic demand.\n";

    } else {

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

            chosenRoad  = "North";
            chosenLevel = northLevel;
            if (northScore == southScore) {
                cout << "  Decision: Scores are equal — defaulting to North Road.\n";
            } else {
                cout << "  Decision: North Road has higher demand score.\n";
            }
        }
    }

    int duration = greenDuration(chosenLevel);

    cout << "\n[Signal Decision]\n";
    cout << "  Selected road    : " << chosenRoad << " Road\n";
    cout << "  Traffic category : " << levelToString(chosenLevel) << "\n";
    cout << "  Green duration   : " << duration << " seconds\n";
    cout << "\n  >> " << chosenRoad
         << " Road receives GREEN for " << duration << "s\n";
    cout << "============================================================\n";
}
