#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>

/**
 * @class Vehicle
 * @brief ယာဉ်ကြောစနစ်အတွင်းရှိ ယာဉ်တစ်စီးကို ကိုယ်စားပြုသည်။
 */
class Vehicle {
private:
    std::string vehicleID;
    std::string vehicleType;
    std::string roadName;

public:
    // Constructor (တည်ဆောက်သူ)
    Vehicle(std::string id, std::string type, std::string road);

    // Getters (ရယူသူများ)
    std::string getVehicleID() const;
    std::string getVehicleType() const;
    std::string getRoadName() const;

    // ယာဉ်အချက်အလက်ကို ပြသရန်
    void display() const;
};

#endif // VEHICLE_H
