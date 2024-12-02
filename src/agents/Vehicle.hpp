#pragma once

#include <raylib.h>
#include <raymath.h>
#include <thread>

#include "pathfinding/Graph.hpp"
#include "util/Misc.hpp"

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
    bool _moving = false;

    /// @brief Position of the vehicle in 3D space
    Vector3 _pos;
    /// @brief Model used by the vehicle
    Model _model;
    /// @brief Path of nodes the vehicle must take
    std::queue<Node *> _path;

    /// @brief The thread of this vehicle. The vehicle does not exist only within the thread, but all of its actions must be performed within it
    // std::thread _t;

    /// @brief Is the simulation paused ? Using a shared reference so that pauses can be done through all threads
    bool *_paused;

    /// @brief Variable representing the bounds of this vehicle type's sprite
    Vector3 _size;

    /// @brief Variables for generating the max speed of this vehicle
    const static float maxSpeedMin, maxSpeedMax;
    /// @brief Variables for generating the acceleration of this vehicle
    const static float accelMin, accelMax;
    /// @brief Variables for generating the weight of this vehicle type. Used for deceleration
    const static float weightMin, weightMax;

public:

    Vehicle(const float &x, const float &y, const float &initialRotation, bool &paused, const Model &model);
    ~Vehicle();

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

    /// @brief Adds the UI class as friend, allowing for use of otherwise private fields
    friend class UI;
    friend class CustomCamera;
};