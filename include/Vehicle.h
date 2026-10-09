#pragma once
#include <string>

// Structure to represent a vehicle in the simulation
struct Vehicle {
    std::string id;
    std::string type; // e.g., Car, Bus, Ambulance
    int waitTime; // Waiting time in seconds
    bool isEmergency;
    
    // Emergency priority: 1 = Ambulance, 2 = Fire Truck, 3 = Police Vehicle
    int emergencyPriority; 
    
    // Used to keep FIFO order when emergency vehicles have the same priority
    int insertionOrder; 

    // Default constructor
    Vehicle();

    // Parameterized constructor
    Vehicle(std::string id, std::string type, int waitTime, bool isEmergency, int emergencyPriority, int insertionOrder);

    // Comparison operator for std::priority_queue
    // std::priority_queue puts the "largest" element at the top.
    // We want smaller priority number to be processed first (1 is higher priority than 2).
    // If priority is the same, we want smaller insertionOrder to be processed first.
    bool operator<(const Vehicle& other) const;
};
