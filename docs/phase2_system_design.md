# Phase 2: System Design

## 1. System Overview
The Smart Traffic Management System is a console application built in C++. It manages traffic for a single four-way intersection by simulating vehicle arrivals, classifying traffic density, and adjusting green-light durations dynamically. It prioritizes emergency vehicles to minimize their wait times.

## 2. Component Responsibilities
- **Vehicle:** Represents an individual car. It holds data about the car's identity and status.
- **Road:** Represents one direction (e.g., North). It acts as a container for vehicles waiting at the light.
- **Traffic Sensor (Simulated):** Provides the input data (number of cars, emergency status) to the system.
- **Main Controller:** Orchestrates the flow, reads inputs, analyzes traffic, and updates signal timings.

## 3. Vehicle Design
The `Vehicle` component will be a simple C++ `struct` containing:
- `int id`: A unique number identifying the vehicle.
- `std::string type`: Identifies the vehicle as "Normal" or "Emergency".
- `bool isEmergency`: A simple true/false flag to quickly check if it's an emergency vehicle.
- `int waitTime`: The time (in simulated seconds) the vehicle has been waiting at the red light.

## 4. Road Design
The `Road` component will be a C++ `class` responsible for a specific direction.
- `std::string roadName`: The name of the road (e.g., "North", "South").
- `int vehicleCount`: The total number of vehicles currently waiting.
- `std::queue<Vehicle> normalTraffic`: Stores the normal vehicles.
- `std::priority_queue<Vehicle> emergencyTraffic`: Stores the emergency vehicles.

## 5. Simulated Sensor Input Design
Physical IoT sensors are excluded. Inputs will be simulated via simple predefined values or console user prompts.
- **Input Variables:**
  - `int inputVehicleCount`: How many vehicles just arrived.
  - `int inputWaitTime`: How long the longest-waiting vehicle has been stopped.
  - `bool inputEmergencyPresence`: Whether an ambulance is detected in this batch.

## 6. Queue and Priority Queue Design
- **Queue (FIFO) for Normal Vehicles:**
  - *Why appropriate:* A First-In-First-Out (FIFO) queue perfectly mimics a real-world road. The first car to stop at a red light should be the first one to leave when it turns green.
  - *Data stored:* Normal `Vehicle` structs.
  - *Basic operations:* `push()` when a car arrives, `pop()` when a car passes the green light.
- **Priority Queue for Emergency Vehicles:**
  - *Why appropriate:* Emergency vehicles must bypass the normal traffic line.
  - *Priority Rule:* Any vehicle with `isEmergency == true` gets top priority. If multiple emergency vehicles exist, the one with the highest `waitTime` goes first.
  - *Data stored:* Emergency `Vehicle` structs.
  - *Basic operations:* `push()` when an ambulance arrives, `pop()` to let it pass immediately on green.

## 7. Traffic-Analysis Rules
The system evaluates traffic density using simple thresholds based on `vehicleCount`.
- **Low Traffic:** 0 to 5 vehicles waiting.
- **Medium Traffic:** 6 to 15 vehicles waiting.
- **High Traffic:** More than 15 vehicles waiting.
- *Note: These are design rules; the actual C++ logic will be written in Phase 3.*

## 8. Adaptive Signal-Control Rules
Green-light duration will adapt based on the traffic analysis categories.
- **Low Traffic:** 20 seconds green light.
- **Medium Traffic:** 40 seconds green light.
- **High Traffic:** 60 seconds green light.
- **Emergency Override:** If an emergency vehicle is detected, the light immediately turns green for that road for 15 seconds (or until the emergency queue is empty), overriding normal timers.

## 9. Proposed Console Menu
When the application runs, the user will see a simple menu:
1. Enter Traffic Data (Simulate arrivals)
2. View Traffic Information (Show counts per road)
3. View Normal Vehicle Queue
4. View Emergency Vehicle Priority
5. Analyze Traffic (Show Low/Medium/High status)
6. View Proposed Signal Timing
7. Exit

## 10. Testing Plan for Future Implementation
- **Test 1:** Input 3 normal vehicles and verify the traffic is categorized as "Low".
- **Test 2:** Input 20 normal vehicles and verify the signal timing adjusts to 60 seconds.
- **Test 3:** Input 1 emergency vehicle and verify it is placed in the Priority Queue and triggers the Emergency Override timing.
