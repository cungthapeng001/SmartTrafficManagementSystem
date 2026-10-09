# Phase 1: Planning and Analysis

## 1. Project Introduction
The Smart Traffic Management System is a console-based C++ simulation that models a single traffic intersection. It aims to demonstrate how foundational Data Structures and Algorithms (DSA) can be applied to optimize traffic signal timings and prioritize emergency vehicles.

## 2. Problem Statement
Traffic congestion at intersections leads to increased waiting times, fuel consumption, and pollution. Traditional static timer-based traffic lights fail to adapt to real-time traffic density, often causing vehicles to wait unnecessarily at empty intersections or delaying critical emergency vehicles like ambulances and fire trucks.

## 3. Project Objectives
- Develop a basic C++ console application to simulate an intersection.
- Apply appropriate data structures to manage normal traffic flow.
- Ensure emergency vehicles receive priority passage.
- Lay the foundation for a future adaptive signal-control system.

## 4. Project Scope and Limitations
**Scope:**
- The system will model ONE four-way intersection (North, South, East, West).
- Traffic conditions will be provided via predefined simulated data or simple user input (e.g., number of waiting cars, wait time).
- Normal and emergency vehicles will be segregated and processed based on priority rules.

**Limitations:**
- No GUI or web frontend (purely console-based).
- No database integration or permanent data storage.
- No OS-specific modules or real-time IoT hardware sensors.
- No complex graph algorithms or machine learning models.

## 5. Existing Solution Analysis
- **Fixed-Time Traffic Lights:** Simple but highly inefficient as they do not react to actual traffic volume.
- **Sensor-Driven Lights:** Detect vehicles but often lack the sophisticated logic needed to handle emergency overrides cleanly without disrupting the entire intersection cycle.
Our project bridges this gap by using a priority-based queueing system to handle exceptions dynamically.

## 6. Functional Requirements
- The system must capture and store the number of vehicles at each road.
- The system must capture the maximum waiting time for vehicles.
- The system must detect the presence of an emergency vehicle.
- The system must output the current state (vehicle counts, waiting times, emergency status) to the console.

## 7. Non-functional Requirements
- **Simplicity:** Code must be beginner-friendly, clean, and easily understandable.
- **Modularity:** The design should allow smooth transitions into Phase 2 (implementation).
- **Efficiency:** The chosen data structures should allow fast insertion and retrieval.

## 8. Suitable DSA and C++ Concepts
- **Classes and Objects:** Used to model entities. For example, a `Road` class and a `Vehicle` structure encapsulate relevant properties and behaviors.
- **Queue (FIFO):** A First-In-First-Out data structure is perfect for normal traffic. The first car arriving at a red light must be the first to leave when it turns green.
- **Priority Queue:** Essential for emergency vehicles. Regardless of when an emergency vehicle arrives, it is given the highest priority and processed before normal traffic.

## 9. Traffic Data Collection/Simulation Approach
Actual hardware sensors and real-time APIs are excluded. Instead, traffic data will be simulated within the `main.cpp` file using static variables (e.g., `int sensorVehicleCountNorth = 15;`) to mimic the inputs a real sensor would provide.

## 10. Initial System Architecture
The high-level architecture consists of:
- **Vehicle Entity:** A simple struct containing basic properties (e.g., type).
- **Road Entity:** A class representing a single direction, holding a Queue for normal vehicles and a Priority Queue for emergency vehicles.
- **Main Controller:** A simulated loop in `main.cpp` that reads sensor data, prints the log, and eventually triggers the processing logic.

## 11. Phase 1 Conclusion
Phase 1 has successfully defined the scope, requirements, and theoretical foundation of the Smart Traffic Management System. We have identified the problem, evaluated existing solutions, and selected appropriate C++ and DSA tools (Queue, Priority Queue, Classes) for the upcoming implementation phases. No actual processing logic has been developed yet, adhering strictly to the planning scope.
