#include <iostream>
#include <string>
#include "../include/TrafficManager.h"
#include "../include/BackendProtocol.h"
#include "../include/Graph.h"
#include "../include/TrafficSignal.h"

void demonstrateGraph() {
    std::cout << "\n========== Phase 3 Part 1: Graph Representation ==========\n";

    Graph g1;
    std::cout << "\n--- Adding Junctions ---\n";
    g1.addJunction("A");
    g1.addJunction("B");
    g1.addJunction("C");
    g1.addJunction("D");

    std::cout << "\n--- Connecting Roads ---\n";
    g1.connectRoad("A", "B", 10);
    g1.connectRoad("A", "C", 15);
    g1.connectRoad("B", "D", 20);
    g1.connectRoad("C", "D", 25);

    g1.displayGraph();

    std::cout << "\n========== Phase 3 Part 2: Dijkstra Shortest Path ==========\n";

    Graph g2;
    std::cout << "\n--- Building Dijkstra Test Network ---\n";
    g2.addJunction("A");
    g2.addJunction("B");
    g2.addJunction("C");
    g2.addJunction("D");
    g2.addJunction("E");

    g2.connectRoad("A", "B", 4);
    g2.connectRoad("A", "C", 2);
    g2.connectRoad("C", "B", 1);
    g2.connectRoad("B", "D", 5);
    g2.connectRoad("C", "D", 8);

    g2.displayGraph();

    std::cout << "\n[TEST 1] Normal shortest route: A -> D\n";
    g2.findShortestPath("A", "D");

    std::cout << "\n[TEST 2] Same start and destination: A -> A\n";
    g2.findShortestPath("A", "A");

    std::cout << "\n[TEST 3] Invalid destination junction: A -> Z\n";
    g2.findShortestPath("A", "Z");

    std::cout << "\n[TEST 4] No available route: A -> E (E is isolated)\n";
    g2.findShortestPath("A", "E");

    std::cout << "\n[TEST 5] Multiple routes: A -> B  (two paths exist)\n";
    g2.findShortestPath("A", "B");

    std::cout << "\n========== Seven-Junction Network (A–G) Dijkstra Tests ==========\n";
    Graph g7(true);

    g7.addJunction("Junction H");

    g7.displayGraph();

    std::cout << "\n[TEST 6] Shortest route: A -> G\n";
    g7.findShortestPath("Junction A", "Junction G");

    std::cout << "\n[TEST 7] Shortest route: G -> A\n";
    g7.findShortestPath("Junction G", "Junction A");

    std::cout << "\n[TEST 8] Shortest route: B -> F\n";
    g7.findShortestPath("Junction B", "Junction F");

    std::cout << "\n[TEST 9] Shortest route: C -> E\n";
    g7.findShortestPath("Junction C", "Junction E");

    std::cout << "\n[TEST 10] Shortest route: D -> G\n";
    g7.findShortestPath("Junction D", "Junction G");

    std::cout << "\n[TEST 11] Unconnected route: A -> H (H is isolated)\n";
    g7.findShortestPath("Junction A", "Junction H");

    std::cout << "\n[TEST 12] Invalid junction: A -> Z\n";
    g7.findShortestPath("Junction A", "Junction Z");

    std::cout << "\n[TEST 13] Same junction: A -> A\n";
    g7.findShortestPath("Junction A", "Junction A");

    std::cout << "\n=============================================================\n\n";
}

void demonstrateDynamicTraffic() {
    std::cout << "\n========== Phase 3 Part 3: Dynamic Traffic Weights ==========\n";

    Graph g;
    std::cout << "\n--- Building Network ---\n";
    g.addJunction("A");
    g.addJunction("B");
    g.addJunction("C");
    g.addJunction("D");

    g.connectRoad("A", "B", 4);
    g.connectRoad("A", "C", 2);
    g.connectRoad("C", "B", 1);
    g.connectRoad("B", "D", 5);
    g.connectRoad("C", "D", 8);

    std::cout << "\n------------------------------------------------------------";
    std::cout << "\nROUND 1: All roads at LOW traffic (default)\n";
    std::cout << "------------------------------------------------------------\n";
    g.displayGraphWithTraffic();
    std::cout << "\nRunning Dijkstra (A -> D):\n";
    g.findShortestPath("A", "D");

    std::cout << "\n------------------------------------------------------------";
    std::cout << "\nROUND 2: HIGH traffic on road A-C\n";
    std::cout << "         (vehicles=20, waitTime=90s)\n";
    std::cout << "------------------------------------------------------------\n";
    std::cout << "\nUpdating traffic on A-C:\n";
    g.updateTrafficOnRoad("A", "C", 20, 90);

    g.displayGraphWithTraffic();
    std::cout << "\nRunning Dijkstra (A -> D) after HIGH traffic on A-C:\n";
    g.findShortestPath("A", "D");

    std::cout << "\n------------------------------------------------------------";
    std::cout << "\nROUND 3: Reset A-C to LOW, HIGH traffic on B-D\n";
    std::cout << "         (A-C: vehicles=3 wait=10s | B-D: vehicles=18 wait=80s)\n";
    std::cout << "------------------------------------------------------------\n";
    std::cout << "\nUpdating traffic:\n";
    g.updateTrafficOnRoad("A", "C", 3,  10);
    g.updateTrafficOnRoad("B", "D", 18, 80);

    g.displayGraphWithTraffic();
    std::cout << "\nRunning Dijkstra (A -> D) after HIGH traffic on B-D:\n";
    g.findShortestPath("A", "D");

    std::cout << "\n------------------------------------------------------------";
    std::cout << "\nROUND 4: MEDIUM traffic on A-B and C-B\n";
    std::cout << "         (A-B: vehicles=10 wait=45s | C-B: vehicles=12 wait=50s)\n";
    std::cout << "------------------------------------------------------------\n";
    std::cout << "\nUpdating traffic:\n";
    g.updateTrafficOnRoad("A", "B", 10, 45);
    g.updateTrafficOnRoad("C", "B", 12, 50);

    g.displayGraphWithTraffic();
    std::cout << "\nRunning Dijkstra (A -> D) with MEDIUM on A-B and C-B:\n";
    g.findShortestPath("A", "D");

    std::cout << "\n=============================================================\n\n";
}

void demonstrateSignal() {
    std::cout << "\n========== Phase 3 Part 4: Adaptive Traffic Signal Simulation ==========\n";

    TrafficSignal signal;

    std::cout << "\n\n[TEST 1] North Road has HIGHER traffic than South\n";
    {
        Road north("North", 18, 70);
        Road south("South",  3, 20);
        signal.runSimulation(north, south);
    }

    std::cout << "\n\n[TEST 2] South Road has HIGHER traffic than North\n";
    {
        Road north("North",  2, 15);
        Road south("South", 20, 80);
        signal.runSimulation(north, south);
    }

    std::cout << "\n\n[TEST 3] Both roads have EQUAL level (MEDIUM) — tie-break by score\n";
    {
        Road north("North", 10, 50);
        Road south("South", 14, 40);
        signal.runSimulation(north, south);
    }

    std::cout << "\n\n[TEST 4] Both roads have LOW traffic\n";
    {
        Road north("North", 4, 20);
        Road south("South", 3, 15);
        signal.runSimulation(north, south);
    }

    std::cout << "\n\n[TEST 5] Both roads have MEDIUM traffic\n";
    {
        Road north("North", 12, 55);
        Road south("South", 10, 60);
        signal.runSimulation(north, south);
    }

    std::cout << "\n\n[TEST 6] Both roads have HIGH traffic\n";
    {
        Road north("North", 20, 100);
        Road south("South", 25,  90);
        signal.runSimulation(north, south);
    }

    std::cout << "\n\n[TEST 7] Emergency vehicle detected on North Road\n";
    {
        Road north("North",  5, 25);
        Road south("South", 20, 90);

        Vehicle ambulance("AMB-001", "Ambulance", 30, true, 1, 1);
        north.addEmergencyVehicle(ambulance);

        signal.runSimulation(north, south);
    }

    std::cout << "\n\n[TEST 7b] Emergency vehicles on BOTH roads — most urgent wins\n";
    {
        Road north("North", 10, 50);
        Road south("South", 10, 50);

        Vehicle police("POL-001", "Police Vehicle", 20, true, 3, 1);
        Vehicle ambulance("AMB-002", "Ambulance",   10, true, 1, 2);

        north.addEmergencyVehicle(police);
        south.addEmergencyVehicle(ambulance);

        signal.runSimulation(north, south);
    }

    std::cout << "\n\n=========================================================================\n\n";
}

int main(int argc, char* argv[]) {
    bool backendMode = false;
    bool graphDemo   = false;

    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--backend") {
            backendMode = true;
        }
        if (std::string(argv[i]) == "--graph-demo") {
            graphDemo = true;
        }
    }

    if (backendMode) {
        BackendProtocol bp;
        bp.run();
    } else if (graphDemo) {

        demonstrateGraph();
        demonstrateDynamicTraffic();
        demonstrateSignal();
    } else {

        demonstrateGraph();
        demonstrateDynamicTraffic();
        demonstrateSignal();
        TrafficManager tm;
        tm.runMenu();
    }

    return 0;
}
