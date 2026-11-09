#pragma once
#include "Road.h"
#include <string>

// ---------------------------------------------------------
// SignalTrafficLevel: How busy a road is right now.
// (Defined here separately from Graph.h's TrafficLevel so
//  TrafficSignal stays self-contained and easy to understand.)
//
//   SIGNAL_LOW    — road is clear,     few vehicles waiting
//   SIGNAL_MEDIUM — moderate activity, some delay
//   SIGNAL_HIGH   — heavy congestion,  long delays
// ---------------------------------------------------------
enum SignalTrafficLevel {
    SIGNAL_LOW    = 0,
    SIGNAL_MEDIUM = 1,
    SIGNAL_HIGH   = 2
};

// ---------------------------------------------------------
// GREEN_LOW    — green-light duration for low traffic    (s)
// GREEN_MEDIUM — green-light duration for medium traffic (s)
// GREEN_HIGH   — green-light duration for high traffic   (s)
// GREEN_EMERGENCY — override duration when an emergency
//                   vehicle is waiting on the chosen road (s)
// ---------------------------------------------------------
const int GREEN_LOW       = 20;
const int GREEN_MEDIUM    = 35;
const int GREEN_HIGH      = 50;
const int GREEN_EMERGENCY = 60;

// ---------------------------------------------------------
// TrafficSignal
// Simulates an adaptive traffic signal controller.
//
// It reads the simulated sensor values already stored inside
// the Road objects (sensorVehicleCount, maxWaitingTime, and
// emergencyQueue) and decides:
//
//   1. Which road gets the green light this cycle.
//   2. How long that green light should last.
//
// NO real hardware, sensors, or external APIs are used.
// This is a pure simulation for university demonstration.
// ---------------------------------------------------------
class TrafficSignal {
private:
    // ---------------------------------------------------------
    // classifyTraffic
    // Decides the traffic level for one road based on:
    //   vehicleCount — simulated sensor vehicle count
    //   waitTime     — simulated maximum waiting time (seconds)
    //
    // Classification rules (same thresholds as Part 3):
    //   vehicleCount <= 5  AND waitTime <= 30s  => LOW
    //   vehicleCount <= 15 AND waitTime <= 60s  => MEDIUM
    //   otherwise                               => HIGH
    // ---------------------------------------------------------
    SignalTrafficLevel classifyTraffic(int vehicleCount, int waitTime) const;

    // ---------------------------------------------------------
    // greenDuration
    // Maps a traffic level to a green-light duration in seconds.
    //   LOW    => GREEN_LOW    (20 s)
    //   MEDIUM => GREEN_MEDIUM (35 s)
    //   HIGH   => GREEN_HIGH   (50 s)
    // ---------------------------------------------------------
    int greenDuration(SignalTrafficLevel level) const;

    // ---------------------------------------------------------
    // levelToString
    // Returns a human-readable label for a traffic level.
    // ---------------------------------------------------------
    std::string levelToString(SignalTrafficLevel level) const;

    // ---------------------------------------------------------
    // scoreForComparison
    // Gives each road a numeric score used to break ties when
    // both roads have the same traffic level.
    // Score = vehicleCount * 2 + (waitTime / 10)
    // Higher score => higher actual demand.
    // ---------------------------------------------------------
    int scoreForComparison(int vehicleCount, int waitTime) const;

public:
    // ---------------------------------------------------------
    // runSimulation
    // The main entry point.  Accepts the two Road objects from
    // TrafficManager and performs the full signal decision:
    //
    //   Step 1 — Check for emergency vehicles.
    //            If either road has one waiting, that road gets
    //            immediate priority (GREEN_EMERGENCY seconds).
    //            If BOTH roads have emergency vehicles, the road
    //            whose top emergency vehicle has higher priority
    //            (lower emergencyPriority number = more urgent)
    //            is chosen first.
    //
    //   Step 2 — Classify each road (LOW / MEDIUM / HIGH).
    //
    //   Step 3 — Pick the road with the higher traffic level.
    //            If equal, use the numeric score to break the tie.
    //            If still equal, default to North Road.
    //
    //   Step 4 — Assign green-light duration from the table.
    //
    //   Step 5 — Display the full decision report.
    // ---------------------------------------------------------
    void runSimulation(const Road& northRoad, const Road& southRoad) const;
};

