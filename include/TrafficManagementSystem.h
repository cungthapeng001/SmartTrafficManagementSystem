#ifndef TRAFFIC_MANAGEMENT_SYSTEM_H
#define TRAFFIC_MANAGEMENT_SYSTEM_H

#include "Vehicle.h"
#include <queue>
#include <string>

/**
 * @class TrafficManagementSystem
 * @brief လမ်းဆုံတစ်ခုရှိ ယာဉ်များ၏ queue ကို စီမံခန့်ခွဲသည်။
 */
class TrafficManagementSystem {
private:
    // အဆင့် ၁ အတွက် အဓိက Data Structure: Queue
    // မီးပွိုင့်တွင် စောင့်ဆိုင်းနေသော ယာဉ်များကို ကိုယ်စားပြုသည်။
    // FIFO (ပထမဆုံးဝင်လာသူ ပထမဆုံးထွက်ရမည်) မူကို လိုက်နာသည်။
    std::queue<Vehicle> vehicleQueue;

public:
    // Constructor (တည်ဆောက်သူ)
    TrafficManagementSystem();

    // ၁. Queue ထဲသို့ ယာဉ်ထည့်ရန် (Enqueue)
    void addVehicle(const Vehicle& v);

    // ၂. Queue ထဲမှ ယာဉ်ကို ဖယ်ရှားရန် (Dequeue)
    bool removeVehicle(Vehicle& removedVehicle);

    // ၃. လက်ရှိ ယာဉ် queue ကို ပြသရန်
    void displayQueue() const;

    // အဆင့် ၁ ယုတ္တိဗေဒအတွက် အကူအညီပေးသော လုပ်ဆောင်ချက်များ
    int getVehicleCount() const;
    std::string getTrafficDensity() const;
    int getGreenSignalTime() const;

    // ၄. ယာဉ်ကြောအခြေအနေ ပြသရန်
    void displayTrafficStatus() const;

    // ၅. ယာဉ်ကြောမီးပွိုင့် ထိန်းချုပ်မှု
    void displaySignalControl() const;
};

#endif // TRAFFIC_MANAGEMENT_SYSTEM_H
