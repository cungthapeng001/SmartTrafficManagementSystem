# Phase 3 Implementation — Smart Traffic Management System

## Overview

Phase 3 extends the Smart Traffic Management System with a city-wide road network simulation built on top of the existing Phase 2 single-intersection foundation. It introduces four progressive capabilities implemented across four parts.

---

## Project Architecture After Phase 3

```
SmartTrafficManagementSystem/
├── include/
│   ├── Vehicle.h          — Vehicle struct (Phase 2)
│   ├── Road.h             — Road class with queues (Phase 2)
│   ├── TrafficManager.h   — Orchestration class (Phase 2)
│   ├── BackendProtocol.h  — JSON backend layer (Phase 2)
│   ├── Graph.h            — Graph, Junction, Edge (Phase 3 Parts 1-3)
│   ├── TrafficSignal.h    — Adaptive signal controller (Phase 3 Part 4)
│   └── nlohmann/json.hpp  — Third-party JSON library (bundled)
├── src/
│   ├── main.cpp           — Entry point + all demo functions
│   ├── Vehicle.cpp        — Vehicle implementation
│   ├── Road.cpp           — Road implementation
│   ├── TrafficManager.cpp — TrafficManager implementation
│   ├── BackendProtocol.cpp— Backend JSON protocol implementation
│   ├── Graph.cpp          — Graph implementation
│   └── TrafficSignal.cpp  — TrafficSignal implementation
├── docs/
│   └── phase3_implementation.md  — This file
├── ui/                    — Python + CustomTkinter frontend
├── CMakeLists.txt
└── README.md
```

---

## Part 1 — Graph Representation

### Files
- `include/Graph.h` — `Edge`, `Junction` structs, `RoadConnectionInfo`, and `Graph` class declaration
- `src/Graph.cpp` — Full implementation with 7-junction default network (A through G)

### Default Seven-Junction Network (A–G)
The road network is initialized with exactly seven default junctions (`Junction A` to `Junction G`) and 10 positive bidirectional travel-cost edges:
- `Junction A` <--(4)--> `Junction B`
- `Junction A` <--(2)--> `Junction C`
- `Junction B` <--(1)--> `Junction C`
- `Junction B` <--(5)--> `Junction D`
- `Junction B` <--(3)--> `Junction E`
- `Junction C` <--(8)--> `Junction D`
- `Junction D` <--(4)--> `Junction F`
- `Junction E` <--(2)--> `Junction F`
- `Junction E` <--(3)--> `Junction G`
- `Junction F` <--(4)--> `Junction G`

### Data Structures

**`Edge`** — one directed road connection:
```cpp
struct Edge {
    std::string destination; // name of junction this road leads to
    int baseWeight;          // original weight (never changes)
    int weight;              // effective weight (updated by traffic)
    TrafficLevel trafficLevel; // LOW / MEDIUM / HIGH
};
```

**`Junction`** — one road intersection (graph vertex):
```cpp
struct Junction {
    std::string name;        // e.g. "A", "Junction_North"
    std::vector<Edge> roads; // adjacency list of outgoing roads
};
```

**`Graph`** — the full road network:
```cpp
class Graph {
    std::vector<Junction> junctions; // all junctions in the network
    // methods: addJunction, connectRoad, displayGraph,
    //          displayGraphWithTraffic, updateTrafficOnRoad,
    //          findShortestPath
};
```

### Key Design Decisions
- **Adjacency list** (not adjacency matrix) — scales well for sparse road networks
- Bidirectional roads: `connectRoad(A, B, w)` creates edges in both directions
- `findJunctionIndex()` is a private linear-scan helper — simple and readable

---

## Part 2 — Dijkstra's Shortest Path Algorithm

### Method
`Graph::findShortestPath(startName, endName)`

### Algorithm Steps (simplified)

1. Set every junction's distance to ∞ (`INT_MAX`), except start = 0
2. Repeat until all junctions are finalised:
   - Pick the unvisited junction with the smallest known distance (`pickClosestUnvisited`)
   - Mark it as visited (finalised)
   - For every road leaving it: if `dist[current] + road.weight < dist[neighbour]`, update and record `prev[neighbour] = current`
3. Trace `prev[]` backwards from destination → start, then reverse to get the route

### Edge Cases Handled
| Scenario | Response |
|---|---|
| Junction does not exist | Error message printed |
| Start == destination | "Total cost: 0", route shows just the junction |
| No route exists (isolated junction) | "No route found" message |
| Multiple paths | Algorithm automatically picks the cheapest |

### Complexity
- Time: O(V²) where V = number of junctions (simple linear scan instead of a priority queue — intentionally beginner-friendly)
- Space: O(V) for the `dist`, `visited`, and `prev` arrays

---

## Part 3 — Dynamic Traffic-Based Road Weights

### Method
`Graph::updateTrafficOnRoad(fromName, toName, vehicleCount, maxWaitTime)`

### Traffic Classification Rules
| `vehicleCount` | `maxWaitTime` | Level |
|---|---|---|
| ≤ 5 | ≤ 30s | **LOW** |
| ≤ 15 | ≤ 60s | **MEDIUM** |
| > 15 or > 60s | — | **HIGH** |

> These thresholds mirror the existing `Road::sensorVehicleCount` and `Road::maxWaitingTime` fields in Phase 2 — the system stays internally consistent.

### Weight Multipliers (integer arithmetic — no floats)
| Level | Formula | Example (base = 10) |
|---|---|---|
| LOW | `base × 1` | 10 |
| MEDIUM | `(base × 3) / 2` | 15 |
| HIGH | `(base × 5) / 2` | 25 |

### How It Interacts with Dijkstra
Dijkstra always reads `Edge::weight` (the effective weight), never `baseWeight`. Calling `updateTrafficOnRoad()` before `findShortestPath()` automatically causes the algorithm to route around congested roads.

### Display Method
`Graph::displayGraphWithTraffic()` — shows base weight, traffic level, and effective weight for every road in every junction.

---

## Part 4 — Adaptive Traffic Signal Simulation

### Files
- `include/TrafficSignal.h`
- `src/TrafficSignal.cpp`

### Class
`TrafficSignal` — stateless controller class with one public method:
```cpp
void runSimulation(const Road& northRoad, const Road& southRoad) const;
```

### Decision Logic (5 steps)

```
Step 1 — Emergency check
         If ANY road has an emergency vehicle, skip to Step 5.
         If BOTH roads have emergency vehicles, the road whose
         top vehicle has a LOWER priority number wins
         (1=Ambulance > 2=Fire Truck > 3=Police).

Step 2 — Classify each road (uses same thresholds as Part 3)
         vehicleCount + maxWaitingTime → LOW / MEDIUM / HIGH

Step 3 — Compare levels
         Higher level wins.
         Tie → compare demand score = vehicleCount×2 + waitTime/10
         Tie → default to North Road

Step 4 — Assign green-light duration
         LOW    →  20 seconds
         MEDIUM →  35 seconds
         HIGH   →  50 seconds
         EMERGENCY → 60 seconds (overrides all)

Step 5 — Print decision report
```

### Green-Light Duration Table
| Traffic Level | Green Duration |
|---|---|
| LOW | 20 seconds |
| MEDIUM | 35 seconds |
| HIGH | 50 seconds |
| EMERGENCY override | 60 seconds |

### Integration with Phase 2
`TrafficSignal::runSimulation()` reads directly from the `Road` class fields — `sensorVehicleCount`, `maxWaitingTime`, and `emergencyQueue` — which are the same fields updated by `TrafficManager::updateSensorInfo()` in Phase 2. No data duplication.

---

## Test Results

### Part 1 — Graph Creation and Display
| Test | Result |
|---|---|
| Add 4 junctions (A, B, C, D) | ✅ All added |
| Connect 4 bidirectional roads | ✅ All connected |
| Display adjacency list | ✅ All directions shown correctly |
| Duplicate junction rejected | ✅ "already exists" message |

### Part 2 — Dijkstra's Algorithm
| Test | Expected | Result |
|---|---|---|
| Normal route: A→D | A→C→B→D, cost 8 | ✅ |
| Same start/end: A→A | Route: A, cost 0 | ✅ |
| Invalid junction: A→Z | Error message | ✅ |
| No route: A→E (isolated) | "No route found" | ✅ |
| Multiple paths: A→B | A→C→B, cost 3 | ✅ |

### Part 3 — Dynamic Traffic Weights
| Round | Change | Expected Route | Expected Cost | Result |
|---|---|---|---|---|
| 1 | All LOW | A→C→B→D | 8 | ✅ |
| 2 | A-C → HIGH (weight 2→5) | A→B→D | 9 | ✅ |
| 3 | A-C→LOW, B-D→HIGH (weight 5→12) | A→C→D | 10 | ✅ |
| 4 | A-B→MEDIUM, C-B→MEDIUM | A→C→D | 10 | ✅ |

### Part 4 — Adaptive Traffic Signal
| Test | Scenario | Expected | Result |
|---|---|---|---|
| 1 | North HIGH, South LOW | North, 50s | ✅ |
| 2 | North LOW, South HIGH | South, 50s | ✅ |
| 3 | Both MEDIUM (tie-break) | South (score 32>25), 35s | ✅ |
| 4 | Both LOW | North (score 10>7), 20s | ✅ |
| 5 | Both MEDIUM | North (score 29>26), 35s | ✅ |
| 6 | Both HIGH (tie-break) | South (score 59>50), 50s | ✅ |
| 7 | Ambulance on North, South HIGH | North (emergency), 60s | ✅ |
| 7b | Police on North, Ambulance on South | South (priority 1<3), 60s | ✅ |

### Phase 2 Backend Protocol Validation
| Operation | Result |
|---|---|
| `ping` | ✅ pong |
| `add_normal_vehicle` (NORTH) | ✅ success |
| `add_emergency_vehicle` (SOUTH, priority 1) | ✅ success |
| `get_normal_vehicles` | ✅ correct vehicle list |
| `get_emergency_vehicles` | ✅ correct vehicle with priority |
| `update_sensor_info` | ✅ success |
| `get_traffic_info` | ✅ correct sensor + queue counts |
| `process_normal_vehicle` | ✅ correct FIFO order |
| `process_emergency_vehicle` | ✅ highest priority vehicle returned |
| Duplicate vehicle ID rejected | ✅ "Vehicle ID already exists" |
| `shutdown` | ✅ clean exit |

---

## Build Status

**Build tool:** Clang++ (Apple clang 21.0.0, C++17)  
**Build result:** ✅ SUCCESS — 0 errors

**Warnings (all pre-existing, not from Phase 3 code):**
- 8 × `nlohmann/json.hpp` deprecated literal operator — third-party library, unrelated to project code
- 1 × `unused parameter 'req'` in `BackendProtocol::handlePing` — existed before Phase 3

**All 7 source files included in `CMakeLists.txt`:**
```
src/main.cpp
src/Vehicle.cpp
src/Road.cpp
src/TrafficManager.cpp
src/BackendProtocol.cpp
src/Graph.cpp
src/TrafficSignal.cpp
```

---

## Remaining Issues / Known Limitations

1. **Single intersection for Phase 2 features:** The `TrafficManager` and `BackendProtocol` still manage only North and South roads. The Graph is an independent simulation layer — the two systems are not yet wired together so that a Dijkstra route's roads automatically pull traffic weights from `TrafficManager`.

2. **Linear scan in Dijkstra:** `pickClosestUnvisited()` runs in O(V) per iteration, giving O(V²) overall. This is intentional (beginner-friendly). For production use, a min-heap priority queue would give O((V+E) log V).

3. **Integer rounding in MEDIUM weight:** `(1 × 3) / 2 = 1` in integer arithmetic (rounds down). Roads with a base weight of 1 do not increase under MEDIUM traffic. This is a known and acceptable simplification.

4. **No persistence:** All data is in-memory only. Restarting the program resets everything. (Per project rules — no database.)

---

## Restrictions Confirmed

- ✅ No database
- ✅ No OS module
- ✅ No real sensors or external APIs
- ✅ No machine learning
- ✅ No advanced C++ (no templates, no smart pointers, no lambdas beyond simple local use)
- ✅ All existing Phase 2 features preserved and functional
- ✅ Python UI backend protocol (`--backend` mode) fully intact

