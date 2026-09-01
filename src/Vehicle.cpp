#include "Vehicle.h"
#include <iostream>

Vehicle::Vehicle(std::string id, std::string type, std::string road) {
    this->vehicleID = id;
    this->vehicleType = type;
    this->roadName = road;
}

std::string Vehicle::getVehicleID() const {
    return vehicleID;
}

std::string Vehicle::getVehicleType() const {
    return vehicleType;
}

std::string Vehicle::getRoadName() const {
    return roadName;
}

void Vehicle::display() const {
    std::cout << "ID: " << vehicleID 
              << " | Type: " << vehicleType 
              << " | Road: " << roadName << std::endl;
}
