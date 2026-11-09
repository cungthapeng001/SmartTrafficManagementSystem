# Smart Traffic Management System

## Project Overview

The Smart Traffic Management System is a university-level C++ console application that simulates and manages traffic flow. The project is implemented in progressive phases:

- **Phase 2** — Single-intersection traffic management using queues and priority queues.
- **Phase 2.5** — JSON backend protocol enabling a Python + CustomTkinter desktop UI.
- **Phase 3** — City-wide road network with graph representation, Dijkstra's shortest path, dynamic traffic weights, and adaptive signal simulation.

---

## Phase 2: Single Intersection

### Features
- **Normal Vehicle Queue (FIFO):** Cars, Buses, and other normal vehicles are processed in arrival order.
- **Emergency Priority Queue:** Ambulances (priority 1) > Fire Trucks (priority 2) > Police (priority 3). Vehicles of the same type are processed in insertion order.
- **Simulated Sensor Updates:** Set `vehicleCount` and `maxWaitingTime` for North and South roads.
- **Input Validation:** Duplicate vehicle IDs rejected globally; invalid road IDs and negative values rejected.
- **Independent Road Queues:** North Road and South Road are managed separately.
- **Backend Mode:** JSON protocol over stdin/stdout for the Python UI.

### Key Classes
| Class | Purpose |
|---|---|
| `Vehicle` | Struct: id, type, waitTime, isEmergency, emergencyPriority |
| `Road` | Manages normal FIFO queue + emergency priority queue for one road |
| `TrafficManager` | Orchestrates both roads, validates input, exposes backend API |
| `BackendProtocol` | JSON communication layer for the Python UI |

---

## Phase 3: City-Wide Road Network

### Part 1 — Graph Representation
- Default road network with exactly seven junctions: **Junction A through Junction G**.
- 10 bidirectional road connections with positive travel-cost edge weights.
- Adjacency list implementation: extensible for adding custom junctions and roads.
- Prevents duplicate road connections and duplicate junctions.
- `addJunction()`, `connectRoad()`, `getJunctionNames()`, `getRoadConnections()`, `displayGraph()`.

### Part 2 — Dijkstra's Shortest Path
- Finds the minimum-cost route between any two junctions across the 7-junction network.
- Tested routes: A ➔ G (cost: 9), G ➔ A (cost: 9), B ➔ F (cost: 5), C ➔ E (cost: 4), D ➔ G (cost: 8).
- Robust error handling: identical start/end validation, non-existent junctions, isolated/unconnected junctions.
- Simple beginner-friendly implementation with `dist[]`, `visited[]`, `prev[]` arrays.

### Part 3 — Dynamic Traffic Weights
- Each road stores a `baseWeight` (fixed) and `weight` (effective, updated at runtime).
- `updateTrafficOnRoad(from, to, vehicleCount, maxWaitTime)` classifies and applies:

| Traffic Level | Condition | Weight Multiplier |
|---|---|---|
| LOW | ≤ 5 vehicles AND ≤ 30s wait | × 1.0 (no change) |
| MEDIUM | ≤ 15 vehicles AND ≤ 60s wait | × 1.5 |
| HIGH | > 15 vehicles OR > 60s wait | × 2.5 |

- Dijkstra re-runs on updated weights and automatically finds new shortest routes.

### Part 4 — Adaptive Traffic Signal Simulation
- Reads simulated sensor values (`sensorVehicleCount`, `maxWaitingTime`) from `Road` objects.
- Classifies each road and assigns green-light durations:

| Level | Green Duration |
|---|---|
| LOW | 20 seconds |
| MEDIUM | 35 seconds |
| HIGH | 50 seconds |
| EMERGENCY override | 60 seconds |

- Emergency vehicles always take priority; ties broken by vehicle urgency (Ambulance > Fire Truck > Police).

---

## Project Structure

```
SmartTrafficManagementSystem/
├── include/
│   ├── Vehicle.h
│   ├── Road.h
│   ├── TrafficManager.h
│   ├── BackendProtocol.h
│   ├── Graph.h            ← Phase 3
│   ├── TrafficSignal.h    ← Phase 3
│   └── nlohmann/json.hpp
├── src/
│   ├── main.cpp
│   ├── Vehicle.cpp
│   ├── Road.cpp
│   ├── TrafficManager.cpp
│   ├── BackendProtocol.cpp
│   ├── Graph.cpp          ← Phase 3
│   └── TrafficSignal.cpp  ← Phase 3
├── docs/
│   └── phase3_implementation.md
├── ui/                    ← Python frontend
└── CMakeLists.txt
```

---

## Build Instructions

This project uses **CMake** and requires **C++17**. The `nlohmann/json` library is bundled in `include/nlohmann/json.hpp`.

### Build with CMake
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Build directly with clang++ or g++ (if CMake is unavailable)
```bash
clang++ -std=c++17 -Iinclude \
    src/main.cpp src/Vehicle.cpp src/Road.cpp \
    src/TrafficManager.cpp src/BackendProtocol.cpp \
    src/Graph.cpp src/TrafficSignal.cpp \
    -o SmartTrafficManagementSystem
```

---

## How to Run

### Console Mode (default)
Runs all Phase 3 demonstrations followed by the interactive Phase 2 menu:
```bash
./SmartTrafficManagementSystem
```

### Graph & Signal Demo only (Phase 3 testing)
Runs only the Phase 3 demonstrations and exits cleanly:
```bash
./SmartTrafficManagementSystem --graph-demo
```

### Backend Mode (for Python UI)
Reads JSON from stdin, writes JSON to stdout — used by the Python frontend:
```bash
./SmartTrafficManagementSystem --backend
```

---

## JSON Backend Protocol

Supported operations in `--backend` mode:

| Operation | Description |
|---|---|
| `ping` | Health check — returns `"pong"` |
| `add_normal_vehicle` | Add a vehicle to the FIFO queue |
| `add_emergency_vehicle` | Add an emergency vehicle with priority (1/2/3) |
| `process_normal_vehicle` | Dequeue and return the first normal vehicle |
| `process_emergency_vehicle` | Dequeue and return the highest-priority emergency vehicle |
| `get_normal_vehicles` | List all normal vehicles without removing them |
| `get_emergency_vehicles` | List all emergency vehicles without removing them |
| `get_traffic_info` | Return sensor counts and queue sizes for a road |
| `update_sensor_info` | Update simulated sensor values for a road |
| `get_roads` | Return list of all roads in multi-road management |
| `add_road` | Add a new road dynamically |
| `get_junctions` | Return list of all network junctions (A through G) |
| `get_graph_connections` | Return all bidirectional road edges and weights |
| `find_shortest_path` | Execute Dijkstra algorithm to find lowest travel-cost route |
| `add_junction` | Add a new junction to the graph dynamically |
| `connect_junction_road` | Connect two junctions with a positive bidirectional weight |
| `shutdown` | Cleanly exit the backend process |

### Example
```bash
echo '{"operation":"add_normal_vehicle","road_id":"NORTH","vehicle_id":"V101","vehicle_type":"Car","waiting_time":30}' \
  | ./SmartTrafficManagementSystem --backend
```
```json
{"data":{"road_id":"NORTH","vehicle_id":"V101"},"message":"Normal vehicle added successfully","success":true}
```

---

## Python UI

A desktop graphical interface built with Python 3 and CustomTkinter communicates with the C++ backend via the JSON protocol.

### Setup
```bash
cd ui
python3 -m venv venv
source venv/bin/activate      # macOS/Linux
# venv\Scripts\activate       # Windows
pip install -r requirements.txt
```

### Run
```bash
python3 ui/main.py
```

The UI automatically starts the C++ backend in the background. Ensure the executable is compiled first.

---

## Project Restrictions

- ✅ Beginner-friendly C++ only (no templates, no smart pointers, simple control flow)
- ✅ No database
- ✅ No OS module
- ✅ No real traffic sensors or external APIs
- ✅ No machine learning

---

## Documentation

- Phase 3 detailed implementation notes: [`docs/phase3_implementation.md`](docs/phase3_implementation.md)
