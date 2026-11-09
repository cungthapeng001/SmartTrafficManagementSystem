#pragma once
#include <string>
#include <vector>

// ---------------------------------------------------------
// TrafficLevel: Represents how busy a road is at a given
// moment. This is simulated — no real sensors are used.
//
//   LOW    — few vehicles, road is clear
//   MEDIUM — moderate traffic, some delay
//   HIGH   — heavy congestion, significant delay
// ---------------------------------------------------------
enum TrafficLevel {
    LOW    = 0,
    MEDIUM = 1,
    HIGH   = 2
};

// ---------------------------------------------------------
// Edge: Represents a road connection between two junctions.
//
// Fields:
//   destination  — name of the junction this road leads to
//   baseWeight   — the original road weight set when the road
//                  was first connected; this never changes
//   weight       — the EFFECTIVE weight used by Dijkstra;
//                  updated whenever traffic conditions change
//   trafficLevel — current traffic category (LOW/MEDIUM/HIGH)
// ---------------------------------------------------------
struct Edge {
    std::string destination;   // Name of the connected junction
    int baseWeight;            // Original road weight (fixed reference)
    int weight;                // Effective weight after traffic adjustment
    TrafficLevel trafficLevel; // Current traffic level on this road
};

// ---------------------------------------------------------
// Junction: Represents a single road junction (a vertex in
// the graph). It stores the junction name and a list of all
// roads (edges) leading out from it.
// ---------------------------------------------------------
struct Junction {
    std::string name;          // Name of this junction (e.g., "Junction A", "Junction B")
    std::vector<Edge> roads;   // List of roads connected from this junction
};

// ---------------------------------------------------------
// Graph: Manages a collection of road junctions and their road
// connections using an adjacency list representation.
// ---------------------------------------------------------
class Graph {
public:
    // Information structure for reporting road connections to UI or callers
    struct RoadConnectionInfo {
        std::string from;
        std::string to;
        int baseWeight;
        int weight;
        TrafficLevel trafficLevel;
    };

private:
    // The main adjacency list of all junctions in the network
    std::vector<Junction> junctions;

    // Helper: finds the index of a junction by name or short letter (e.g., "Junction A" or "A").
    // Returns -1 if not found.
    int findJunctionIndex(const std::string& name) const;

    // Helper used by Dijkstra: picks the unvisited junction
    // that currently has the smallest known distance.
    int pickClosestUnvisited(const std::vector<int>& dist,
                             const std::vector<bool>& visited) const;

public:
    // Constructors:
    // Graph() creates an empty graph.
    // Graph(true) initializes the 7 default junctions (A through G) and their connections.
    Graph();
    explicit Graph(bool initDefault);

    // Initializes exactly seven default junctions (A through G) with bidirectional connections.
    void initSevenJunctionNetwork(bool verbose = false);

    // Clears all junctions and roads
    void clear();

    // Add a new junction (vertex) to the graph.
    // Does nothing if a junction with the same name already exists.
    void addJunction(const std::string& name, bool verbose = false);

    // Connect two junctions with a road (edge).
    // The connection is bidirectional (undirected graph): fromName <---> toName.
    // Both junctions must already exist. Weight must be positive.
    // Avoids creating duplicate road connections unintentionally.
    void connectRoad(const std::string& fromName,
                     const std::string& toName,
                     int weight,
                     bool verbose = false);

    // Get list of all junction names (e.g. "Junction A", "Junction B", ...)
    std::vector<std::string> getJunctionNames() const;

    // Get list of all unique bidirectional road connections
    std::vector<RoadConnectionInfo> getRoadConnections() const;

    // Get access to all junctions
    const std::vector<Junction>& getJunctions() const;

    // Display all junctions and their connected roads to console.
    void displayGraph() const;

    // Display each road's base weight, current traffic level, and effective weight.
    void displayGraphWithTraffic() const;

    // Update traffic level and weight based on simulated sensor counts
    void updateTrafficOnRoad(const std::string& fromName,
                             const std::string& toName,
                             int vehicleCount,
                             int maxWaitTime);

    // ---------------------------------------------------------
    // computeShortestPath (Dijkstra's Algorithm)
    // Computes the shortest path from startName to endName.
    // Returns true on success and populates path and totalCost.
    // Returns false if same junction, invalid junction, or unreachable.
    // ---------------------------------------------------------
    bool computeShortestPath(const std::string& startName,
                             const std::string& endName,
                             std::vector<std::string>& path,
                             int& totalCost,
                             std::string& errorMessage) const;

    // Console display wrapper around computeShortestPath
    void findShortestPath(const std::string& startName,
                          const std::string& endName) const;
};
