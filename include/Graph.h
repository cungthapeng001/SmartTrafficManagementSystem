#pragma once
#include <string>
#include <vector>

enum TrafficLevel {
    LOW    = 0,
    MEDIUM = 1,
    HIGH   = 2
};

struct Edge {
    std::string destination;
    int baseWeight;
    int weight;
    TrafficLevel trafficLevel;
};

struct Junction {
    std::string name;
    std::vector<Edge> roads;
};

class Graph {
public:

    struct RoadConnectionInfo {
        std::string from;
        std::string to;
        int baseWeight;
        int weight;
        TrafficLevel trafficLevel;
    };

private:

    std::vector<Junction> junctions;

    int findJunctionIndex(const std::string& name) const;

    int pickClosestUnvisited(const std::vector<int>& dist,
                             const std::vector<bool>& visited) const;

public:

    Graph();
    explicit Graph(bool initDefault);

    void initSevenJunctionNetwork(bool verbose = false);

    void clear();

    void addJunction(const std::string& name, bool verbose = false);

    void connectRoad(const std::string& fromName,
                     const std::string& toName,
                     int weight,
                     bool verbose = false);

    std::vector<std::string> getJunctionNames() const;

    std::vector<RoadConnectionInfo> getRoadConnections() const;

    const std::vector<Junction>& getJunctions() const;

    void displayGraph() const;

    void displayGraphWithTraffic() const;

    void updateTrafficOnRoad(const std::string& fromName,
                             const std::string& toName,
                             int vehicleCount,
                             int maxWaitTime);

    bool computeShortestPath(const std::string& startName,
                             const std::string& endName,
                             std::vector<std::string>& path,
                             int& totalCost,
                             std::string& errorMessage) const;

    void findShortestPath(const std::string& startName,
                          const std::string& endName) const;
};
