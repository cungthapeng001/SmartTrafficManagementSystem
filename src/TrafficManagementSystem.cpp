#include "TrafficManagementSystem.h"
#include <iostream>

TrafficManagementSystem::TrafficManagementSystem() {
    // ပုံမှန်အားဖြင့် Queue သည် အလွတ်ဖြစ်သည်
}

void TrafficManagementSystem::addVehicle(const Vehicle& v) {
    // Enqueue လုပ်ဆောင်ချက်: Queue ၏ အနောက်သို့ ယာဉ်တစ်စီးကို ထည့်သည်
    vehicleQueue.push(v);
}

bool TrafficManagementSystem::removeVehicle(Vehicle& removedVehicle) {
    // ဖယ်ရှားရန် မကြိုးစားမီ Queue အလွတ်ဖြစ်မဖြစ် စစ်ဆေးပါ
    if (vehicleQueue.empty()) {
        return false;
    }

    // Dequeue လုပ်ဆောင်ချက်:
    // ၁. ရှေ့ဆုံးမှ ယာဉ်ကို ရယူပါ
    removedVehicle = vehicleQueue.front();
    // ၂. Queue ထဲမှ ၎င်းကို ဖယ်ရှားပါ (FIFO)
    vehicleQueue.pop();
    
    return true;
}

void TrafficManagementSystem::displayQueue() const {
    if (vehicleQueue.empty()) {
        std::cout << "The traffic queue is currently empty." << std::endl;
        return;
    }

    // Queue ကို မပျက်စီးစေဘဲ ပြသရန်အတွက် ကျွန်ုပ်တို့သည် မိတ္တူတစ်ခုကို ဖန်တီးပါသည်
    std::queue<Vehicle> tempQueue = vehicleQueue;
    int position = 1;

    std::cout << "\n--- Current Vehicle Queue ---" << std::endl;
    while (!tempQueue.empty()) {
        std::cout << "Position " << position << ": ";
        tempQueue.front().display();
        tempQueue.pop();
        position++;
    }
    std::cout << "-----------------------------" << std::endl;
}

int TrafficManagementSystem::getVehicleCount() const {
    return vehicleQueue.size();
}

std::string TrafficManagementSystem::getTrafficDensity() const {
    int count = getVehicleCount();
    if (count <= 5) {
        return "LOW";
    } else if (count <= 10) {
        return "MEDIUM";
    } else {
        return "HIGH";
    }
}

int TrafficManagementSystem::getGreenSignalTime() const {
    std::string density = getTrafficDensity();
    if (density == "LOW") {
        return 30;
    } else if (density == "MEDIUM") {
        return 60;
    } else {
        return 90; // HIGH (အများဆုံး)
    }
}

void TrafficManagementSystem::displayTrafficStatus() const {
    std::cout << "\n--- Traffic Status ---" << std::endl;
    std::cout << "Vehicles Waiting: " << getVehicleCount() << std::endl;
    std::cout << "Traffic Density: " << getTrafficDensity() << std::endl;
    std::cout << "----------------------" << std::endl;
}

void TrafficManagementSystem::displaySignalControl() const {
    std::cout << "\n--- Traffic Signal Control ---" << std::endl;
    std::cout << "Vehicles Waiting: " << getVehicleCount() << std::endl;
    std::cout << "Traffic Density: " << getTrafficDensity() << std::endl;
    std::cout << "Signal: GREEN" << std::endl;
    std::cout << "Green Time: " << getGreenSignalTime() << " seconds" << std::endl;
    std::cout << "------------------------------" << std::endl;
}
