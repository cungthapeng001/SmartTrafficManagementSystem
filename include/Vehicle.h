#pragma once
#include <string>

struct Vehicle {
    std::string id;
    std::string type;
    int waitTime;
    bool isEmergency;
    
    int emergencyPriority; 
    
    int insertionOrder; 

    Vehicle();

    Vehicle(std::string id, std::string type, int waitTime, bool isEmergency, int emergencyPriority, int insertionOrder);

    bool operator<(const Vehicle& other) const;
};
