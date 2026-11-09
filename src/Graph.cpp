#include "Graph.h"
#include <iostream>
#include <vector>
#include <string>
#include <climits>   // for INT_MAX
#include <cctype>
#include <algorithm>

using namespace std;

// Helper: convert string to uppercase
static string toUpperString(const string& s) {
    string res = s;
    for (char& c : res) {
        c = static_cast<char>(toupper(static_cast<unsigned char>(c)));
    }
    return res;
}

// ---------------------------------------------------------
// Constructors
// ---------------------------------------------------------
Graph::Graph() {
    // Empty graph by default
}

Graph::Graph(bool initDefault) {
    if (initDefault) {
        initSevenJunctionNetwork();
    }
}

void Graph::clear() {
    junctions.clear();
}

// ---------------------------------------------------------
// initSevenJunctionNetwork
// Initializes exactly seven default junctions (A through G)
// with positive travel-cost weights connecting them in a
// realistic, multi-path road network.
// ---------------------------------------------------------
void Graph::initSevenJunctionNetwork(bool verbose) {
    junctions.clear();

    // 1. Add all seven default junctions: A through G
    addJunction("Junction A", verbose);
    addJunction("Junction B", verbose);
    addJunction("Junction C", verbose);
    addJunction("Junction D", verbose);
    addJunction("Junction E", verbose);
    addJunction("Junction F", verbose);
    addJunction("Junction G", verbose);

    // 2. Connect roads with positive example travel costs (bidirectional)
    connectRoad("Junction A", "Junction B", 4, verbose);
    connectRoad("Junction A", "Junction C", 2, verbose);
    connectRoad("Junction B", "Junction C", 1, verbose);
    connectRoad("Junction B", "Junction D", 5, verbose);
    connectRoad("Junction B", "Junction E", 3, verbose);
    connectRoad("Junction C", "Junction D", 8, verbose);
    connectRoad("Junction D", "Junction F", 4, verbose);
    connectRoad("Junction E", "Junction F", 2, verbose);
    connectRoad("Junction E", "Junction G", 3, verbose);
    connectRoad("Junction F", "Junction G", 4, verbose);
}

// ---------------------------------------------------------
// findJunctionIndex
// Flexible lookup: supports exact match, case-insensitivity,
// and letter-only lookup (e.g. "A" matches "Junction A").
// ---------------------------------------------------------
int Graph::findJunctionIndex(const string& name) const {
    if (name.empty()) return -1;

    string target = toUpperString(name);

    // 1. Direct match
    for (size_t i = 0; i < junctions.size(); ++i) {
        if (toUpperString(junctions[i].name) == target) {
            return static_cast<int>(i);
        }
    }

    // 2. Prefix/Suffix match ("A" matches "JUNCTION A", or "JUNCTION A" matches "A")
    for (size_t i = 0; i < junctions.size(); ++i) {
        string jName = toUpperString(junctions[i].name);
        if (jName == "JUNCTION " + target || target == "JUNCTION " + jName) {
            return static_cast<int>(i);
        }
        if (target.length() == 1 && jName.length() >= 2 && jName.back() == target[0]) {
            return static_cast<int>(i);
        }
        if (jName.length() == 1 && target.length() >= 2 && target.back() == jName[0]) {
            return static_cast<int>(i);
        }
    }

    return -1; // Not found
}

// ---------------------------------------------------------
// addJunction
// Adds a new junction to the graph if it doesn't exist yet.
// Keeps the graph extensible for future junctions.
// ---------------------------------------------------------
void Graph::addJunction(const string& name, bool verbose) {
    if (name.empty()) {
        if (verbose) cout << "Error: Junction name cannot be empty.\n";
        return;
    }

    if (findJunctionIndex(name) != -1) {
        if (verbose) cout << "Junction \"" << name << "\" already exists. Skipping.\n";
        return;
    }

    Junction newJunction;
    newJunction.name = name;
    junctions.push_back(newJunction);
    if (verbose) cout << "Junction \"" << name << "\" added.\n";
}

// ---------------------------------------------------------
// connectRoad
// Creates a bidirectional road between two junctions with
// positive weight. Prevents duplicate edges.
// ---------------------------------------------------------
void Graph::connectRoad(const string& fromName, const string& toName, int weight, bool verbose) {
    if (weight <= 0) {
        if (verbose) cout << "Error: Road weight must be a positive number.\n";
        return;
    }

    int fromIndex = findJunctionIndex(fromName);
    int toIndex   = findJunctionIndex(toName);

    if (fromIndex == -1) {
        if (verbose) cout << "Error: Junction \"" << fromName << "\" not found.\n";
        return;
    }
    if (toIndex == -1) {
        if (verbose) cout << "Error: Junction \"" << toName << "\" not found.\n";
        return;
    }
    if (fromIndex == toIndex) {
        if (verbose) cout << "Error: Cannot connect a junction to itself.\n";
        return;
    }

    string actualFrom = junctions[fromIndex].name;
    string actualTo   = junctions[toIndex].name;

    // Check for existing connection to avoid duplicates
    for (size_t r = 0; r < junctions[fromIndex].roads.size(); ++r) {
        if (junctions[fromIndex].roads[r].destination == actualTo) {
            // Update weight if already connected
            junctions[fromIndex].roads[r].baseWeight = weight;
            junctions[fromIndex].roads[r].weight = weight;
            for (size_t rb = 0; rb < junctions[toIndex].roads.size(); ++rb) {
                if (junctions[toIndex].roads[rb].destination == actualFrom) {
                    junctions[toIndex].roads[rb].baseWeight = weight;
                    junctions[toIndex].roads[rb].weight = weight;
                    break;
                }
            }
            if (verbose) cout << "Road updated: " << actualFrom << " <--(" << weight << ")--> " << actualTo << "\n";
            return;
        }
    }

    // Build the edge going from -> to
    Edge edgeForward;
    edgeForward.destination  = actualTo;
    edgeForward.baseWeight   = weight;
    edgeForward.weight       = weight;
    edgeForward.trafficLevel = LOW;

    // Build the edge going to -> from (bidirectional road)
    Edge edgeBackward;
    edgeBackward.destination  = actualFrom;
    edgeBackward.baseWeight   = weight;
    edgeBackward.weight       = weight;
    edgeBackward.trafficLevel = LOW;

    junctions[fromIndex].roads.push_back(edgeForward);
    junctions[toIndex].roads.push_back(edgeBackward);

    if (verbose) cout << "Road connected: " << actualFrom << " <--(" << weight << ")--> " << actualTo << "\n";
}

std::vector<std::string> Graph::getJunctionNames() const {
    std::vector<std::string> names;
    for (const auto& j : junctions) {
        names.push_back(j.name);
    }
    return names;
}

std::vector<Graph::RoadConnectionInfo> Graph::getRoadConnections() const {
    std::vector<RoadConnectionInfo> result;
    for (size_t i = 0; i < junctions.size(); ++i) {
        for (size_t r = 0; r < junctions[i].roads.size(); ++r) {
            const Edge& edge = junctions[i].roads[r];
            // To report each bidirectional edge once, order by name
            if (junctions[i].name < edge.destination) {
                RoadConnectionInfo info;
                info.from = junctions[i].name;
                info.to = edge.destination;
                info.baseWeight = edge.baseWeight;
                info.weight = edge.weight;
                info.trafficLevel = edge.trafficLevel;
                result.push_back(info);
            }
        }
    }
    return result;
}

const std::vector<Junction>& Graph::getJunctions() const {
    return junctions;
}

void Graph::displayGraph() const {
    if (junctions.empty()) {
        cout << "The graph has no junctions yet.\n";
        return;
    }

    cout << "\n========== Road Network (Junction Graph) ==========\n";
    cout << "Total junctions: " << junctions.size() << "\n\n";

    for (size_t i = 0; i < junctions.size(); i++) {
        const Junction& j = junctions[i];
        cout << "Junction [" << j.name << "]";

        if (j.roads.empty()) {
            cout << " -- no roads connected\n";
        } else {
            cout << " connects to:\n";
            for (size_t r = 0; r < j.roads.size(); r++) {
                const Edge& e = j.roads[r];
                cout << "    -> " << e.destination
                     << "  (distance/cost: " << e.weight << ")\n";
            }
        }
    }
    cout << "====================================================\n";
}

static string trafficLevelToString(TrafficLevel level) {
    if (level == LOW)    return "LOW";
    if (level == MEDIUM) return "MEDIUM";
    return "HIGH";
}

void Graph::displayGraphWithTraffic() const {
    if (junctions.empty()) {
        cout << "The graph has no junctions yet.\n";
        return;
    }

    cout << "\n========== Road Network with Traffic Info ==========\n";
    cout << "Total junctions: " << junctions.size() << "\n\n";

    for (size_t i = 0; i < junctions.size(); i++) {
        const Junction& j = junctions[i];
        cout << "Junction [" << j.name << "]";

        if (j.roads.empty()) {
            cout << " -- no roads connected\n";
        } else {
            cout << " connects to:\n";
            for (size_t r = 0; r < j.roads.size(); r++) {
                const Edge& e = j.roads[r];
                cout << "    -> " << e.destination
                     << "  | base: " << e.baseWeight
                     << "  | traffic: " << trafficLevelToString(e.trafficLevel)
                     << "  | effective weight: " << e.weight << "\n";
            }
        }
    }
    cout << "=====================================================\n";
}

void Graph::updateTrafficOnRoad(const string& fromName,
                                const string& toName,
                                int vehicleCount,
                                int maxWaitTime) {
    int fromIdx = findJunctionIndex(fromName);
    int toIdx   = findJunctionIndex(toName);

    if (fromIdx == -1) {
        cout << "Error: Junction \"" << fromName << "\" not found.\n";
        return;
    }
    if (toIdx == -1) {
        cout << "Error: Junction \"" << toName << "\" not found.\n";
        return;
    }

    TrafficLevel level;
    if (vehicleCount <= 5 && maxWaitTime <= 30) {
        level = LOW;
    } else if (vehicleCount <= 15 && maxWaitTime <= 60) {
        level = MEDIUM;
    } else {
        level = HIGH;
    }

    auto calcWeight = [](int base, TrafficLevel lvl) -> int {
        if (lvl == LOW)    return base;
        if (lvl == MEDIUM) return (base * 3) / 2;
        return (base * 5) / 2;
    };

    string destTo = junctions[toIdx].name;
    string destFrom = junctions[fromIdx].name;

    for (size_t r = 0; r < junctions[fromIdx].roads.size(); r++) {
        Edge& e = junctions[fromIdx].roads[r];
        if (findJunctionIndex(e.destination) == toIdx) {
            int oldWeight = e.weight;
            e.trafficLevel = level;
            e.weight = calcWeight(e.baseWeight, level);
            cout << "  Road " << junctions[fromIdx].name << " -> " << e.destination
                 << "  | vehicles: " << vehicleCount
                 << "  | wait: " << maxWaitTime << "s"
                 << "  | traffic: " << trafficLevelToString(level)
                 << "  | weight: " << oldWeight << " -> " << e.weight << "\n";
            break;
        }
    }

    for (size_t r = 0; r < junctions[toIdx].roads.size(); r++) {
        Edge& e = junctions[toIdx].roads[r];
        if (findJunctionIndex(e.destination) == fromIdx) {
            int oldWeight = e.weight;
            e.trafficLevel = level;
            e.weight = calcWeight(e.baseWeight, level);
            cout << "  Road " << junctions[toIdx].name << " -> " << e.destination
                 << "  | vehicles: " << vehicleCount
                 << "  | wait: " << maxWaitTime << "s"
                 << "  | traffic: " << trafficLevelToString(level)
                 << "  | weight: " << oldWeight << " -> " << e.weight << "\n";
            break;
        }
    }
}

int Graph::pickClosestUnvisited(const vector<int>& dist,
                                const vector<bool>& visited) const {
    int closestIndex = -1;
    int smallestDist = INT_MAX;

    for (size_t i = 0; i < junctions.size(); i++) {
        if (!visited[i] && dist[i] < smallestDist) {
            smallestDist = dist[i];
            closestIndex = static_cast<int>(i);
        }
    }
    return closestIndex;
}

// ---------------------------------------------------------
// computeShortestPath (Dijkstra's Algorithm)
// ---------------------------------------------------------
bool Graph::computeShortestPath(const string& startName,
                                const string& endName,
                                vector<string>& path,
                                int& totalCost,
                                string& errorMessage) const {
    path.clear();
    totalCost = 0;

    int startIdx = findJunctionIndex(startName);
    int endIdx   = findJunctionIndex(endName);

    if (startIdx == -1) {
        errorMessage = "Starting junction \"" + startName + "\" does not exist in the network.";
        return false;
    }
    if (endIdx == -1) {
        errorMessage = "Destination junction \"" + endName + "\" does not exist in the network.";
        return false;
    }

    if (startIdx == endIdx) {
        errorMessage = "Starting and destination junctions cannot be the same. Please select two distinct junctions.";
        return false;
    }

    int n = static_cast<int>(junctions.size());
    vector<int> dist(n, INT_MAX);
    dist[startIdx] = 0;

    vector<bool> visited(n, false);
    vector<int> prev(n, -1);

    for (int step = 0; step < n; step++) {
        int current = pickClosestUnvisited(dist, visited);
        if (current == -1) {
            break; // No more reachable junctions
        }

        visited[current] = true;
        if (current == endIdx) {
            break; // Found shortest path to destination
        }

        for (size_t r = 0; r < junctions[current].roads.size(); r++) {
            const Edge& road = junctions[current].roads[r];
            int neighbourIdx = findJunctionIndex(road.destination);
            if (neighbourIdx == -1) continue;
            if (visited[neighbourIdx]) continue;
            if (dist[current] == INT_MAX) continue;

            int newCost = dist[current] + road.weight;
            if (newCost < dist[neighbourIdx]) {
                dist[neighbourIdx] = newCost;
                prev[neighbourIdx] = current;
            }
        }
    }

    if (dist[endIdx] == INT_MAX) {
        errorMessage = "No available route found between \"" + junctions[startIdx].name +
                       "\" and \"" + junctions[endIdx].name + "\".";
        return false;
    }

    // Reconstruct path
    int current = endIdx;
    while (current != -1) {
        path.push_back(junctions[current].name);
        current = prev[current];
    }
    reverse(path.begin(), path.end());
    totalCost = dist[endIdx];
    return true;
}

void Graph::findShortestPath(const string& startName,
                             const string& endName) const {
    cout << "\n---------- Dijkstra Shortest Path ----------\n";
    cout << "From: \"" << startName << "\"  To: \"" << endName << "\"\n\n";

    int startIdx = findJunctionIndex(startName);
    int endIdx   = findJunctionIndex(endName);

    if (startIdx != -1 && endIdx != -1 && startIdx == endIdx) {
        cout << "Start and destination are the same junction.\n";
        cout << "Route: " << junctions[startIdx].name << "\n";
        cout << "Total cost: 0\n";
        cout << "--------------------------------------------\n";
        return;
    }

    vector<string> path;
    int totalCost = 0;
    string errorMsg;

    if (computeShortestPath(startName, endName, path, totalCost, errorMsg)) {
        cout << "Shortest route found:\n  ";
        for (size_t i = 0; i < path.size(); i++) {
            cout << path[i];
            if (i + 1 < path.size()) cout << " -> ";
        }
        cout << "\nTotal cost: " << totalCost << "\n";
    } else {
        cout << errorMsg << "\n";
    }
    cout << "--------------------------------------------\n";
}
