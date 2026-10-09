#include "Vehicle.h"

Vehicle::Vehicle() : id(""), type(""), waitTime(0), isEmergency(false), emergencyPriority(0), insertionOrder(0) {}

Vehicle::Vehicle(std::string id, std::string type, int waitTime, bool isEmergency, int emergencyPriority, int insertionOrder)
    : id(id), type(type), waitTime(waitTime), isEmergency(isEmergency), emergencyPriority(emergencyPriority), insertionOrder(insertionOrder) {}

bool Vehicle::operator<(const Vehicle& other) const {
    // If priorities are different, the one with the LARGER priority number has LOWER priority.
    // Since priority_queue puts the max element at the top, we want operator< to return true
    // if 'this' has a lower priority (i.e. larger priority number) than 'other'.
    if (emergencyPriority != other.emergencyPriority) {
        return emergencyPriority > other.emergencyPriority; 
    }
    // If priorities are the same, the one that arrived later (larger insertionOrder) has LOWER priority.
    // So 'this' is less than 'other' if 'this' arrived later.
    return insertionOrder > other.insertionOrder;
}
