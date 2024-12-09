#pragma once

#include "Vehicle.hpp"

class Car : public Vehicle
{
    /// @brief Variables for generating the max speed of this vehicle
    constexpr static float maxSpeedMin = 12, maxSpeedMax = 15;
    /// @brief Variables for generating the acceleration of this vehicle
    constexpr static float accelMin = 4, accelMax = 5;
    /// @brief Variables for generating the weight of this vehicle type. Used for deceleration
    constexpr static float weightMin = 5, weightMax = 7;

public:
    
    Car(const float &x, const float &y, const float &initialRotation, std::set<Vehicle*> &vehicles, TrafficLightController& trafficLights, std::vector<Node *> path);
    ~Car();
};