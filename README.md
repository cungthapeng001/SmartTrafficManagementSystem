# Smart Traffic Management System

## Project Overview
The Smart Traffic Management System is a university-level C++ console application designed to simulate and manage traffic flow. The current implementation completes Phase 2 of the project, focusing on data structure concepts such as Queues and Priority Queues for an individual intersection. 

## Phase 2: System Design & Implementation Complete
The core logic for traffic handling has been implemented successfully during Phase 2.

### Features Implemented
* **Interactive Console Menu:** Users can continuously add vehicles, process queues, and view simulated sensor data.
* **Normal Vehicle Management:** Normal vehicles (Cars, Buses, etc.) are processed in a strict FIFO order.
* **Emergency Vehicle Management:** Emergency vehicles are prioritized based on their type (Ambulance > Fire Truck > Police). 
* **Safe Input Validation:** The system correctly handles invalid data inputs to prevent crashes.
* **Independent Road Queues:** Traffic on the North Road is managed entirely independently from the South Road.
* **Simulated Sensor Updates:** Users can dynamically update the current sensor data (vehicle count and waiting times).

### Project Structure (Phase 2)
* **`Vehicle`**: A struct capturing `vehicleID`, `type`, `waitTime`, `isEmergency`, and `emergencyPriority`. It contains the priority comparison logic.
* **`Road`**: A class that independently manages the normal `queue`, emergency `priority_queue`, and simulated data for a specific road direction.
* **`TrafficManager`**: The overarching system that binds the roads and the console interactive menu.

### Data Structures Used
1. **`std::queue` (Normal Traffic)**
   * Follows the **First-In, First-Out (FIFO)** principle.
   * The vehicle added first to a road's normal queue is guaranteed to be processed and removed first.
2. **`std::priority_queue` (Emergency Traffic)**
   * Used to guarantee that high-priority emergency vehicles are processed before lower-priority ones.
   * **Priority Logic:**
     1. Ambulance (Highest, Priority 1)
     2. Fire Truck (Priority 2)
     3. Police Vehicle (Priority 3)
   * **Tie-Breaker Rule:** If two emergency vehicles have the same priority (e.g. two Ambulances), the one that arrived earlier (based on an insertion-order tracker) is processed first.

## Build Instructions
This project uses CMake.

To build:
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

Alternatively, compile directly using `g++`:
```bash
g++ -std=c++17 -Iinclude src/main.cpp src/Vehicle.cpp src/Road.cpp src/TrafficManager.cpp -o SmartTrafficManagementSystem
```

To run:
```bash
./SmartTrafficManagementSystem
```

## Example Menu Operations
1. Add an Ambulance to the South Road (Option 2).
2. Add a Fire Truck to the South Road (Option 2).
3. Display the Emergency Priority Queues to verify the Ambulance is ranked higher (Option 6).
4. Process the Emergency Vehicle, which will output the Ambulance (Option 4).

## Current Limitations
* Only models one intersection (North/South roads).
* Simulated sensor values don't currently affect processing rates (adaptive signal algorithms aren't implemented yet).

## Phase 3: Planned Graph and Route Planning Integration
The next evolution of this project will support a full city road network:
* **Junctions as Vertices:** Intersections will become Graph vertices.
* **Roads as Edges:** Road connections will become Graph edges.
* **Weights:** Edges will store travel time or congestion metrics as weights.
* **Dijkstra's Algorithm:** Used to calculate minimum-total-cost routes between junctions.
* **Adaptive Systems:** Simulated congestion will affect edge weights dynamically.
