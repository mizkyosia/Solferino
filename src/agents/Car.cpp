#include "Car.hpp"
#include <random>

Car::Car(const float &x, const float &y, const float &initialRotation, std::vector<Node *> path) : Vehicle(x, y, initialRotation, path, Util::CarModels[Math::randomRange(Util::CarVariantsNb)])
{
    _acceleration = accelMin + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (accelMax - accelMin)));
    _maxSpeed = maxSpeedMin + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxSpeedMax - maxSpeedMin)));
    _weight = weightMin + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (weightMax - weightMin)));
}

Car::~Car()
{
}