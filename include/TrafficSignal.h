#pragma once
#include "Road.h"
#include <string>

enum SignalTrafficLevel {
    SIGNAL_LOW    = 0,
    SIGNAL_MEDIUM = 1,
    SIGNAL_HIGH   = 2
};

const int GREEN_LOW       = 20;
const int GREEN_MEDIUM    = 35;
const int GREEN_HIGH      = 50;
const int GREEN_EMERGENCY = 60;

class TrafficSignal {
private:

    SignalTrafficLevel classifyTraffic(int vehicleCount, int waitTime) const;

    int greenDuration(SignalTrafficLevel level) const;

    std::string levelToString(SignalTrafficLevel level) const;

    int scoreForComparison(int vehicleCount, int waitTime) const;

public:

    void runSimulation(const Road& northRoad, const Road& southRoad) const;
};
