#include <iostream>
#include <string>
#include "../include/TrafficManager.h"
#include "../include/BackendProtocol.h"
#include "../include/Graph.h"
#include "../include/TrafficSignal.h"

// ---------------------------------------------------------
// demonstrateGraph
// Part 1: Builds and displays a simple road network:
//
//   A ---10--- B
//   |          |
//  15         20
//   |          |
//   C ---25--- D
//
// Part 2: Runs Dijkstra's shortest-path tests.
//
// Test network (from the task spec):
//   A -> B = 4
//   A -> C = 2
//   C -> B = 1
//   B -> D = 5
//   C -> D = 8
//
// All tests listed in the requirements:
//   1. Normal shortest route           (A -> D)
//   2. Same start and destination      (A -> A)
//   3. Invalid junction name           (A -> Z)
//   4. No available route              (isolated junction E)
//   5. Multiple possible routes        (A -> B, two paths exist)
// ---------------------------------------------------------
void demonstrateGraph() {
    std::cout << "\n========== Phase 3 Part 1: Graph Representation ==========\n";

    // ---- Part 1 graph (from previous phase) ----
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

    // ---- Part 2: Dijkstra tests ----
    std::cout << "\n========== Phase 3 Part 2: Dijkstra Shortest Path ==========\n";

    // Build the test network from the task specification:
    //   A -4-> B
    //   A -2-> C
    //   C -1-> B
    //   B -5-> D
    //   C -8-> D
    //
    // All roads are bidirectional, so shortest paths work
    // in both directions.
    Graph g2;
    std::cout << "\n--- Building Dijkstra Test Network ---\n";
    g2.addJunction("A");
    g2.addJunction("B");
    g2.addJunction("C");
    g2.addJunction("D");
    g2.addJunction("E"); // isolated junction — used for Test 4

    g2.connectRoad("A", "B", 4);  // A <-4-> B
    g2.connectRoad("A", "C", 2);  // A <-2-> C
    g2.connectRoad("C", "B", 1);  // C <-1-> B
    g2.connectRoad("B", "D", 5);  // B <-5-> D
    g2.connectRoad("C", "D", 8);  // C <-8-> D
    // Note: E is added but has NO roads — it is completely isolated.

    g2.displayGraph();

    // --------------------------------------------------
    // TEST 1: Normal shortest route
    // Expected: A -> C -> B -> D   Total cost: 8
    //   A->C = 2, C->B = 1, B->D = 5  =>  2+1+5 = 8
    // (Better than A->B->D = 4+5=9, or A->C->D = 2+8=10)
    // --------------------------------------------------
    std::cout << "\n[TEST 1] Normal shortest route: A -> D\n";
    g2.findShortestPath("A", "D");

    // --------------------------------------------------
    // TEST 2: Same start and destination
    // Expected: Route is just "A", total cost 0
    // --------------------------------------------------
    std::cout << "\n[TEST 2] Same start and destination: A -> A\n";
    g2.findShortestPath("A", "A");

    // --------------------------------------------------
    // TEST 3: Invalid junction name
    // Expected: Error — "Z" does not exist in the graph
    // --------------------------------------------------
    std::cout << "\n[TEST 3] Invalid destination junction: A -> Z\n";
    g2.findShortestPath("A", "Z");

    // --------------------------------------------------
    // TEST 4: No available route
    // E is isolated — no roads connect it to A, B, C, D.
    // Expected: No route found
    // --------------------------------------------------
    std::cout << "\n[TEST 4] No available route: A -> E (E is isolated)\n";
    g2.findShortestPath("A", "E");

    // --------------------------------------------------
    // TEST 5: Multiple possible routes — pick shortest
    // From A to B there are two paths:
    //   Path 1: A -> B          cost = 4
    //   Path 2: A -> C -> B     cost = 2 + 1 = 3   <-- shorter
    // Expected: A -> C -> B     Total cost: 3
    // --------------------------------------------------
    std::cout << "\n[TEST 5] Multiple routes: A -> B  (two paths exist)\n";
    g2.findShortestPath("A", "B");

    // ---- Seven-Junction (A-G) Extended Network Tests ----
    std::cout << "\n========== Seven-Junction Network (A–G) Dijkstra Tests ==========\n";
    Graph g7(true); // initialize 7 default junctions: A through G
    // Add isolated junction H for connectivity test
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

// ---------------------------------------------------------
// demonstrateDynamicTraffic
// Part 3: Shows how road weights change under different
// simulated traffic conditions and how that affects the
// shortest path found by Dijkstra.
//
// Network used:
//
//   A ---4--- B
//   |         |
//   2         5
//   |         |
//   C ---1--- B   (C also connects to B with weight 1)
//   |
//   8
//   |
//   D
//
// (Same network as Part 2 Dijkstra tests)
//
// Test sequence for route A -> D:
//   Round 1: Default weights — LOW traffic everywhere
//   Round 2: Set A-C road to HIGH traffic  => weight A-C jumps
//   Round 3: Set A-C back to LOW, C-B to HIGH => different shift
//
// This demonstrates that Dijkstra re-routes when costs change.
// ---------------------------------------------------------
void demonstrateDynamicTraffic() {
    std::cout << "\n========== Phase 3 Part 3: Dynamic Traffic Weights ==========\n";

    // Build the same network used in Part 2
    Graph g;
    std::cout << "\n--- Building Network ---\n";
    g.addJunction("A");
    g.addJunction("B");
    g.addJunction("C");
    g.addJunction("D");

    g.connectRoad("A", "B", 4);   // base: A <-4-> B
    g.connectRoad("A", "C", 2);   // base: A <-2-> C
    g.connectRoad("C", "B", 1);   // base: C <-1-> B
    g.connectRoad("B", "D", 5);   // base: B <-5-> D
    g.connectRoad("C", "D", 8);   // base: C <-8-> D

    // ======================================================
    // ROUND 1: All roads at LOW traffic (default state)
    // Expected shortest path A->D: A->C->B->D  cost = 2+1+5 = 8
    // ======================================================
    std::cout << "\n------------------------------------------------------------";
    std::cout << "\nROUND 1: All roads at LOW traffic (default)\n";
    std::cout << "------------------------------------------------------------\n";
    g.displayGraphWithTraffic();
    std::cout << "\nRunning Dijkstra (A -> D):\n";
    g.findShortestPath("A", "D");

    // ======================================================
    // ROUND 2: Simulate HIGH traffic on road A-C
    //   vehicleCount = 20, maxWaitTime = 90s => HIGH
    //   New A-C effective weight = 2 * 2.5 = 5
    //
    // All other roads stay LOW.
    //
    // Cost comparison after update:
    //   A->C->B->D = 5 + 1 + 5 = 11
    //   A->B->D    = 4 + 5     =  9  <-- now shorter!
    //
    // Expected shortest path: A->B->D  cost = 9
    // ======================================================
    std::cout << "\n------------------------------------------------------------";
    std::cout << "\nROUND 2: HIGH traffic on road A-C\n";
    std::cout << "         (vehicles=20, waitTime=90s)\n";
    std::cout << "------------------------------------------------------------\n";
    std::cout << "\nUpdating traffic on A-C:\n";
    g.updateTrafficOnRoad("A", "C", 20, 90);  // HIGH => weight 2->5

    g.displayGraphWithTraffic();
    std::cout << "\nRunning Dijkstra (A -> D) after HIGH traffic on A-C:\n";
    g.findShortestPath("A", "D");

    // ======================================================
    // ROUND 3: Reset A-C to LOW, now put HIGH traffic on B-D
    //   A-C: vehicleCount=3, waitTime=10s => LOW  => weight = 2
    //   B-D: vehicleCount=18, waitTime=80s => HIGH => weight = 5*2.5 = 12
    //
    // Cost comparison after update:
    //   A->C->B->D = 2 + 1 + 12 = 15
    //   A->B->D    = 4 + 12     = 16
    //   A->C->D    = 2 + 8      = 10  <-- now shortest!
    //
    // Expected shortest path: A->C->D  cost = 10
    // ======================================================
    std::cout << "\n------------------------------------------------------------";
    std::cout << "\nROUND 3: Reset A-C to LOW, HIGH traffic on B-D\n";
    std::cout << "         (A-C: vehicles=3 wait=10s | B-D: vehicles=18 wait=80s)\n";
    std::cout << "------------------------------------------------------------\n";
    std::cout << "\nUpdating traffic:\n";
    g.updateTrafficOnRoad("A", "C", 3,  10);  // LOW    => weight back to 2
    g.updateTrafficOnRoad("B", "D", 18, 80);  // HIGH   => weight 5->12

    g.displayGraphWithTraffic();
    std::cout << "\nRunning Dijkstra (A -> D) after HIGH traffic on B-D:\n";
    g.findShortestPath("A", "D");

    // ======================================================
    // ROUND 4: Put MEDIUM traffic on A-B and C-B
    //   A-B: vehicleCount=10, waitTime=45s => MEDIUM => 4*1.5 = 6
    //   C-B: vehicleCount=12, waitTime=50s => MEDIUM => 1*1.5 = 1 (rounds down)
    //   B-D stays HIGH (weight=12 from Round 3)
    //   A-C stays LOW  (weight=2)
    //   C-D stays LOW  (weight=8)
    //
    // Cost comparison:
    //   A->C->B->D = 2 + 1 + 12 = 15
    //   A->B->D    = 6 + 12     = 18
    //   A->C->D    = 2 + 8      = 10  <-- still shortest
    //
    // Expected shortest path: A->C->D  cost = 10
    // ======================================================
    std::cout << "\n------------------------------------------------------------";
    std::cout << "\nROUND 4: MEDIUM traffic on A-B and C-B\n";
    std::cout << "         (A-B: vehicles=10 wait=45s | C-B: vehicles=12 wait=50s)\n";
    std::cout << "------------------------------------------------------------\n";
    std::cout << "\nUpdating traffic:\n";
    g.updateTrafficOnRoad("A", "B", 10, 45);  // MEDIUM => weight 4->6
    g.updateTrafficOnRoad("C", "B", 12, 50);  // MEDIUM => weight 1->1 (rounds to 1)

    g.displayGraphWithTraffic();
    std::cout << "\nRunning Dijkstra (A -> D) with MEDIUM on A-B and C-B:\n";
    g.findShortestPath("A", "D");

    std::cout << "\n=============================================================\n\n";
}

// ---------------------------------------------------------
// demonstrateSignal
// Phase 3 Part 4: Runs all 7 required test cases for the
// adaptive traffic signal simulation.
//
// Each test builds two Road objects directly with preset
// sensor values so the output is deterministic and readable.
// No user input is needed — this is a pure demonstration.
//
// Tests:
//   1. North Road has higher traffic
//   2. South Road has higher traffic
//   3. Both roads have equal traffic (tie-break by score)
//   4. Both roads have LOW traffic
//   5. Both roads have MEDIUM traffic
//   6. Both roads have HIGH traffic
//   7. Emergency vehicle detected
// ---------------------------------------------------------
void demonstrateSignal() {
    std::cout << "\n========== Phase 3 Part 4: Adaptive Traffic Signal Simulation ==========\n";

    TrafficSignal signal; // The signal controller — no state, just logic

    // ---- Helper: build a Road with preset values quickly ----
    // We use the existing Road constructor and set fields directly.

    // ==================================================
    // TEST 1: North Road has higher traffic than South
    // North: 18 vehicles, 70s wait => HIGH
    // South:  3 vehicles, 20s wait => LOW
    // Expected: North gets GREEN for 50s
    // ==================================================
    std::cout << "\n\n[TEST 1] North Road has HIGHER traffic than South\n";
    {
        Road north("North", 18, 70); // HIGH traffic
        Road south("South",  3, 20); // LOW  traffic
        signal.runSimulation(north, south);
    }

    // ==================================================
    // TEST 2: South Road has higher traffic than North
    // North:  2 vehicles, 15s wait => LOW
    // South: 20 vehicles, 80s wait => HIGH
    // Expected: South gets GREEN for 50s
    // ==================================================
    std::cout << "\n\n[TEST 2] South Road has HIGHER traffic than North\n";
    {
        Road north("North",  2, 15); // LOW  traffic
        Road south("South", 20, 80); // HIGH traffic
        signal.runSimulation(north, south);
    }

    // ==================================================
    // TEST 3: Both roads have EQUAL traffic level — tie-break
    // North: 10 vehicles, 50s wait => MEDIUM,  score = 10*2 + 50/10 = 25
    // South: 14 vehicles, 40s wait => MEDIUM,  score = 14*2 + 40/10 = 32
    // Both MEDIUM, but South has a higher score.
    // Expected: South gets GREEN for 35s
    // ==================================================
    std::cout << "\n\n[TEST 3] Both roads have EQUAL level (MEDIUM) — tie-break by score\n";
    {
        Road north("North", 10, 50); // MEDIUM, score 25
        Road south("South", 14, 40); // MEDIUM, score 32
        signal.runSimulation(north, south);
    }

    // ==================================================
    // TEST 4: Both roads have LOW traffic
    // North: 4 vehicles, 20s wait => LOW
    // South: 3 vehicles, 15s wait => LOW
    // Scores: North = 4*2+2 = 10, South = 3*2+1 = 7
    // Expected: North gets GREEN for 20s (North higher score)
    // ==================================================
    std::cout << "\n\n[TEST 4] Both roads have LOW traffic\n";
    {
        Road north("North", 4, 20); // LOW, score 10
        Road south("South", 3, 15); // LOW, score 7
        signal.runSimulation(north, south);
    }

    // ==================================================
    // TEST 5: Both roads have MEDIUM traffic
    // North: 12 vehicles, 55s wait => MEDIUM, score = 12*2+5 = 29
    // South: 10 vehicles, 60s wait => MEDIUM, score = 10*2+6 = 26
    // Expected: North gets GREEN for 35s (North higher score)
    // ==================================================
    std::cout << "\n\n[TEST 5] Both roads have MEDIUM traffic\n";
    {
        Road north("North", 12, 55); // MEDIUM, score 29
        Road south("South", 10, 60); // MEDIUM, score 26
        signal.runSimulation(north, south);
    }

    // ==================================================
    // TEST 6: Both roads have HIGH traffic
    // North: 20 vehicles, 100s wait => HIGH, score = 20*2+10 = 50
    // South: 25 vehicles,  90s wait => HIGH, score = 25*2+9  = 59
    // Expected: South gets GREEN for 50s (South higher score)
    // ==================================================
    std::cout << "\n\n[TEST 6] Both roads have HIGH traffic\n";
    {
        Road north("North", 20, 100); // HIGH, score 50
        Road south("South", 25,  90); // HIGH, score 59
        signal.runSimulation(north, south);
    }

    // ==================================================
    // TEST 7: Emergency vehicle detected
    // North:  5 vehicles, 25s wait  => LOW  — has an Ambulance (priority 1)
    // South: 20 vehicles, 90s wait  => HIGH — no emergency
    // Even though South has HIGH traffic, North's emergency
    // vehicle overrides normal logic.
    // Expected: North gets GREEN for 60s (emergency override)
    // ==================================================
    std::cout << "\n\n[TEST 7] Emergency vehicle detected on North Road\n";
    {
        Road north("North",  5, 25);
        Road south("South", 20, 90);

        // Add an Ambulance (priority 1 = most urgent) to North
        // insertionOrder=1 is arbitrary here — only used for
        // tie-breaking within the priority queue.
        Vehicle ambulance("AMB-001", "Ambulance", 30, true, 1, 1);
        north.addEmergencyVehicle(ambulance);

        signal.runSimulation(north, south);
    }

    // ==================================================
    // BONUS TEST 7b: Both roads have emergency vehicles
    // North: Police (priority 3)
    // South: Ambulance (priority 1 = more urgent)
    // Expected: South gets GREEN for 60s (Ambulance wins)
    // ==================================================
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
        // Run all Phase 3 demonstrations
        demonstrateGraph();
        demonstrateDynamicTraffic();
        demonstrateSignal();
    } else {
        // Show all Phase 3 demos, then launch the interactive menu
        demonstrateGraph();
        demonstrateDynamicTraffic();
        demonstrateSignal();
        TrafficManager tm;
        tm.runMenu();
    }

    return 0;
}
