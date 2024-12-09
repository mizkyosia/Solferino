#include "TrafficLights.hpp"
#include <chrono>

TrafficLightController::TrafficLightController() : _schedule(TrafficSchedule::Pause2), _thread{}
{
    _thread = std::thread{[&]
                          {
                              while (this->_simRunning)
                              {
                                  this->update();
                              }
                          }};
    TraceLog(LOG_WARNING, "Created traffic controller");

    addHitbox(TrafficSchedule::X, {17.5f, 8}, {3, 1});
    addHitbox(TrafficSchedule::X, {22.5f, 32}, {3, 1});

    addHitbox(TrafficSchedule::Z, {25, 13}, {1, 10});
    addHitbox(TrafficSchedule::Z, {15, 27}, {1, 10});
}

TrafficLightController::~TrafficLightController()
{
    _simRunning = false;
    _thread.join();
}

void TrafficLightController::update()
{
    int seconds;
    if (_schedule == TrafficSchedule::X)
    {
        _schedule = TrafficSchedule::Pause1;
        seconds = 1;
    }
    else if (_schedule == TrafficSchedule::Pause1)
    {
        _schedule = TrafficSchedule::Z;
        seconds = 5;
    }
    else if (_schedule == TrafficSchedule::Z)
    {
        _schedule = TrafficSchedule::Pause2;
        seconds = 1;
    }
    else if (_schedule == TrafficSchedule::Pause2)
    {
        _schedule = TrafficSchedule::X;
        seconds = 5;
    }

    for (auto b : _boxes)
        b->active = b->schedule != _schedule;

    TraceLog(LOG_WARNING, "Schedule change");
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

void TrafficLightController::addHitbox(TrafficSchedule schedule, Vector2 pos, Vector2 size)
{
    _boxes.push_back(new (TrafficHitbox){size, pos, schedule, false});
}

std::vector<TrafficHitbox *> &TrafficLightController::getRef()
{
    return _boxes;
}

void TrafficLightController::draw()
{
    for (auto b : _boxes)
    {
        Vector3 v1 = {b->position.x - b->size.x / 2, -2, b->position.y - b->size.y / 2},
                v2 = {b->position.x + b->size.x / 2, 2, b->position.y + b->size.y / 2};
        DrawBoundingBox({Vector3Min(v1, v2), Vector3Max(v1, v2)}, b->active ? RED : PURPLE);
    }
}
