#include "Vehicle.h"

Vehicle::Vehicle() : id(""), type(""), waitTime(0), isEmergency(false), emergencyPriority(0), insertionOrder(0) {}

Vehicle::Vehicle(std::string id, std::string type, int waitTime, bool isEmergency, int emergencyPriority, int insertionOrder)
    : id(id), type(type), waitTime(waitTime), isEmergency(isEmergency), emergencyPriority(emergencyPriority), insertionOrder(insertionOrder) {}

bool Vehicle::operator<(const Vehicle& other) const {
    if (emergencyPriority != other.emergencyPriority) {
        return emergencyPriority > other.emergencyPriority; 
    }
    return insertionOrder > other.insertionOrder;
}
