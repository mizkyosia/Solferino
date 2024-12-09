#pragma once

#include <raylib.h>
#include <raymath.h>
#include <thread>
#include <vector>

#include "pathfinding/Node.hpp"
#include "util/Misc.hpp"
#include "TrafficLights.hpp"

enum class VehicleState {
    Accelerating,
    Slowing,
    Braking
};

class Vehicle
{
protected:
    /// @brief Variables for managing the vehicle's transform in space
    float _velocity, _acceleration;
    /// @brief Variables managing the vehicle's vision range (to see if any other vehicles might block its path)
    float _visionDistance, _visionWidth;
    /// @brief Rotation of the vehicle relative to the Y axis
    float _rot;
    /// @brief The exact weight of the vehicle
    float _weight;
    /// @brief Maximum speed of the vehicle
    float _maxSpeed;
    /// @brief Time of the last update. Used to keep track of the delta time between updates
    float _lastUpdateTime;

    /// @brief Can the vehicle move ?
    VehicleState _state = VehicleState::Slowing;
    /// @brief Should the vehice despawn next frame ?
    bool _despawn = false;

    /// @brief Position of the vehicle in 3D space
    Vector3 _pos;
    /// @brief Model's offset. The vehicles might not be 100% aligned with the origin, so we have to account for that in order to display them properly
    Vector3 _offset;
    /// @brief Model used by the vehicle
    Model _model;
    /// @brief Path of nodes the vehicle must take
    std::vector<Node *> _path;
    /// @brief Safe reference to the vehicles of the simulation
    std::set<Vehicle*> & _vehicles;
    /// @brief Safe reference to the traffic lights & hitboxes
    TrafficLightController& _trafficLights;

    /// @brief The thread of this vehicle. The vehicle does not exist only within the thread, but all of its actions must be performed within it
    std::thread _t;

    /// @brief Variable representing the bounds of this vehicle type's sprite
    Vector3 _size;

    /// @brief Variables for generating the max speed of this vehicle
    const static float maxSpeedMin, maxSpeedMax;
    /// @brief Variables for generating the acceleration of this vehicle
    const static float accelMin, accelMax;
    /// @brief Variables for generating the weight of this vehicle type. Used for deceleration
    const static float weightMin, weightMax;

public:

    Vehicle(const float &x, const float &y, const float &initialRotation, std::set<Vehicle*> &vehicles, TrafficLightController &trafficLights, std::vector<Node *> path, const Model &model);
    virtual ~Vehicle();

    /// @brief Updates the vehicle : calculates new acceleration, velocity, position, direction...
    virtual void update();

    /// @brief Draws the vehicle, by drawing its model in 3D space
    /// @warning Only use this function in the RENDERING THREAD. Using it anywhere else may cause crashng, or unwanted graphical effects
    void draw(bool debug = false);

    /// @brief Gets the position of the car, which is postioned at its center
    /// @return
    Vector3 getPos() const;

    /// @brief Gets the size of the vehicle's mesh
    /// @return 
    Vector3 getSize() const;

    /// @brief Get the rotation of the vehicle
    /// @return
    float getRotation() const;

    /// @brief Returns the full transform matrix of the vehicle
    /// @return 
    Matrix getTransform() const;

    /// @brief Returns if the node should despawn
    /// @return 
    bool shouldDespawn() const;

    /// @brief Checks if the vehcle has had any collision, or if it has detected another vehicle in its path
    void checkCollisions();

    /// @brief Adds the UI class as friend, allowing for use of otherwise private fields
    friend class UI;
    friend class CustomCamera;
};