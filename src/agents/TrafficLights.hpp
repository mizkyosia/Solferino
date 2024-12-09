#pragma once

#include <raylib.h>
#include <raymath.h>

#include <vector>
#include <thread>
#include <chrono>

enum class TrafficSchedule {
    X,
    Pause1,
    Z,
    Pause2
};

class TrafficHitbox {
public:
    /// @brief Size of the hitbox in 2D. Height does not matter for this box
    Vector2 size;
    /// @brief Position of the hitbox in 2D. Altitude does not matter for this box
    Vector2 position;
    /// @brief Schedule of the hitbox (when it must be active)
    TrafficSchedule schedule;
    /// @brief Is the hitbox active ?
    bool active;
};

class TrafficLightController
{
private:
    /// @brief Collection of hitboxes for stopping cars
    std::vector<TrafficHitbox*> _boxes;

    /// @brief Current traffic schedule
    TrafficSchedule _schedule;

    /// @brief Thread of the controller
    std::thread _thread;

    /// @brief Is the simulation running ?
    bool _simRunning = true;
public:
    TrafficLightController(/* args */);
    ~TrafficLightController();

    void update();

    void draw();

    void addHitbox(TrafficSchedule schedule, Vector2 pos, Vector2 size);

    std::vector<TrafficHitbox*> & getRef();
};
