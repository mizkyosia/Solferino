#include "Car.hpp"
#include <random>

Car::Car(const float &x, const float &y, const float &initialRotation, bool &paused, Model &model) : Vehicle(x, y, initialRotation, paused, model)
{
    _acceleration = accelMin + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (accelMax - accelMin)));
    _maxSpeed = maxSpeedMin + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxSpeedMax - maxSpeedMin)));
    _weight = weightMin + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (weightMax - weightMin)));

    TraceLog(LOG_INFO, "max speed : %f, speed range : %f - %f", _maxSpeed, maxSpeedMin, maxSpeedMax);
}

Car::~Car()
{
}

void Car::test()
{
    // _maxSpeed;
}