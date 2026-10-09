# Phase 2: System Workflow

## 1. Planned Workflow Flow
The Smart Traffic Management System operates in a continuous loop during execution, transitioning through four distinct stages:

1. **Traffic Input:** The user (or simulated sensor) inputs the number of arriving vehicles and their types (Normal/Emergency) for a specific road.
2. **Data Storage:** The system routes Normal vehicles to the standard `Queue` and Emergency vehicles to the `Priority Queue`.
3. **Traffic Analysis:** The system counts the vehicles in the queues and categorizes the road's traffic density as Low, Medium, or High.
4. **Signal Control:** The system calculates the proposed green-light duration based on the traffic density, applying an immediate override if the Priority Queue is not empty.

## 2. Text-Based Architecture Diagram

```text
[ User / Simulated Sensor ]
          |
          v
+-------------------------+
|    Main Controller      |  <-- Displays Console Menu
+-------------------------+
          |
          v
+-------------------------+
|       Road Class        |  <-- E.g., North Road
|                         |
|  +-------------------+  |
|  | Priority Queue    |  |  <-- Emergency Vehicles (Processed First)
|  +-------------------+  |
|                         |
|  +-------------------+  |
|  | Queue (FIFO)      |  |  <-- Normal Vehicles (Processed FIFO)
|  +-------------------+  |
+-------------------------+
          |
          v
[ Traffic Analysis Module ]  <-- Determines Low/Med/High
          |
          v
[ Signal Timing Output    ]  <-- Calculates Green Light Duration
```

## 3. Pseudocode
This pseudocode outlines the intended logic for Phase 3 implementation.

```text
function main():
    Road northRoad
    
    while user_does_not_exit:
        display_menu()
        choice = get_user_choice()
        
        if choice == 1:
            // 1. Traffic Input
            count, hasEmergency = get_simulated_input()
            
            // 2. Data Storage
            if hasEmergency:
                northRoad.emergencyTraffic.push(new EmergencyVehicle)
            
            for i from 1 to count:
                northRoad.normalTraffic.push(new NormalVehicle)
                
        else if choice == 5:
            // 3. Traffic Analysis
            density = analyze_traffic(northRoad.normalTraffic.size())
            print("Traffic Density is: " + density)
            
        else if choice == 6:
            // 4. Signal Control
            if not northRoad.emergencyTraffic.empty():
                print("EMERGENCY OVERRIDE: Green light for 15s")
            else:
                timing = calculate_timing(density)
                print("Proposed Green Light: " + timing + "s")
```
