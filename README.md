# Smart Traffic Management System

## Project Overview
The Smart Traffic Management System is a university-level C++ console application designed to simulate and manage traffic flow. The current implementation completes Phase 2 of the project, focusing on data structure concepts such as Queues and Priority Queues for an individual intersection.

We have now added a **Phase 2.5 Backend Mode** to prepare the application for integration with a Python + CustomTkinter desktop UI.

## Phase 2: System Design & Implementation Complete
The core logic for traffic handling has been implemented successfully during Phase 2.

### Features Implemented
* **Interactive Console Mode:** Users can continuously add vehicles, process queues, and view simulated sensor data.
* **Backend Mode:** Allows external UI programs (like Python) to interact with the system via standard input/output using a JSON protocol.
* **Normal Vehicle Management:** Normal vehicles (Cars, Buses, etc.) are processed in a strict FIFO order.
* **Emergency Vehicle Management:** Emergency vehicles are prioritized based on their type (Ambulance > Fire Truck > Police). 
* **Safe Input Validation:** The system correctly handles invalid data inputs to prevent crashes. Invalid road identifiers are rejected, and duplicate vehicle IDs are strictly prevented across all queues.
* **Independent Road Queues:** Traffic on the North Road is managed entirely independently from the South Road.
* **Simulated Sensor Updates:** Users can dynamically update the current sensor data (vehicle count and waiting times).

### Project Structure
* **`Vehicle`**: A struct capturing `vehicleID`, `type`, `waitTime`, `isEmergency`, and `emergencyPriority`. It contains the priority comparison logic.
* **`Road`**: A class that independently manages the normal `queue`, emergency `priority_queue`, and simulated data for a specific road direction.
* **`TrafficManager`**: The overarching system that orchestrates the roads, tracking active vehicle IDs, and providing core backend operations and the console interactive menu.
* **`BackendProtocol`**: The JSON communication layer that interprets commands from standard input and delegates them to the `TrafficManager`, returning responses via standard output.

## Build Instructions
This project uses CMake and requires C++17. It also depends on the `nlohmann/json` library, which is included locally in `include/nlohmann/json.hpp`.

To build with CMake:
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

Alternatively, compile directly using `g++` (if CMake is unavailable):
```bash
g++ -std=c++17 -Iinclude src/main.cpp src/Vehicle.cpp src/Road.cpp src/TrafficManager.cpp src/BackendProtocol.cpp -o SmartTrafficManagementSystem
```

## How to Run

### Console Mode (Default)
To run the interactive console menu, simply launch the executable:
```bash
./SmartTrafficManagementSystem
```

### Backend Mode
To launch the backend process for the Python UI to communicate with, use the `--backend` flag:
```bash
./SmartTrafficManagementSystem --backend
```

## JSON Communication Protocol
In Backend Mode, the process reads one JSON object per line from standard input and outputs one JSON object per line to standard output. 

### Supported Operations
- `ping`: Checks if the backend is alive.
- `add_normal_vehicle`: Adds a normal vehicle to the queue.
- `add_emergency_vehicle`: Adds an emergency vehicle with priority (1=Ambulance, 2=Fire Truck, 3=Police).
- `process_normal_vehicle`: Removes and returns the first vehicle in the normal queue.
- `process_emergency_vehicle`: Removes and returns the highest-priority vehicle in the emergency queue.
- `get_normal_vehicles`: Retrieves a list of vehicles in the normal queue (without processing them).
- `get_emergency_vehicles`: Retrieves a list of vehicles in the emergency queue (without processing them).
- `get_traffic_info`: Retrieves sensor info and queue sizes for a given road.
- `update_sensor_info`: Updates the simulated sensor values.
- `shutdown`: Exits the backend process safely.

### Example Request
```json
{"operation":"add_normal_vehicle","road_id":"NORTH","vehicle_id":"V101","vehicle_type":"Car","waiting_time":30}
```

### Example Success Response
```json
{"success":true,"message":"Normal vehicle added successfully","data":{"vehicle_id":"V101","road_id":"NORTH"}}
```

### Example Error Response
```json
{"success":false,"message":"Vehicle ID already exists.","data":null}
```

## Error Handling
The backend will never crash on malformed input or logic errors. It will simply return a structured JSON response with `"success": false` and a descriptive message. Constraints include:
- Invalid or unknown operations are rejected safely.
- Duplicate vehicle IDs are forbidden globally.
- Only "NORTH" and "SOUTH" are accepted for `road_id`.
- Waiting times, sensor counts, and priorities must be valid integers and in standard ranges.

## Python UI (Phase 2.5)

A professional desktop graphical user interface has been added using **Python 3** and **CustomTkinter**. The UI acts as a frontend that communicates with the C++ backend. 

### UI Installation

1. Ensure you have Python 3 installed (Python 3.8+ recommended).
2. Create and activate a virtual environment (optional but recommended):
   ```bash
   cd ui
   python3 -m venv venv
   # On macOS/Linux:
   source venv/bin/activate
   # On Windows:
   venv\Scripts\activate
   ```
3. Install the required dependencies:
   ```bash
   pip install -r requirements.txt
   ```

### Running the Application

Once the C++ backend is compiled (as described in the Build Instructions above), start the Python UI by running:

```bash
cd ui
python3 main.py
```

The Python UI will automatically detect and launch the C++ backend executable in the background. Ensure that the `SmartTrafficManagementSystem` executable is present in the main project directory.

## Current Limitations
* Only models one intersection (North/South roads).
* Simulated sensor values don't currently affect processing rates (adaptive signal algorithms aren't implemented yet).

## Future Plans (Phase 3)
The next evolution of this project will support a full city road network:
* **Graph Integration**: Using intersections as vertices and roads as edges.
* **Dijkstra's Algorithm**: For calculating route paths based on weight/congestion.
