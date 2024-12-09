#include "Vehicle.hpp"
#include <cmath>
#include <chrono>

using namespace std::chrono;

Vehicle::Vehicle(const float &x,
                 const float &y,
                 const float &initialRotation,
                 std::set<Vehicle *> &vehicles,
                 TrafficLightController &trafficLights,
                 std::vector<Node *> path,
                 const Model &model) : _velocity(0),
                                       _acceleration(0),
                                       _rot(initialRotation),
                                       _pos((Vector3){x, 0.f, y}),
                                       _model(model),
                                       _path(path),
                                       _vehicles(vehicles),
                                       _trafficLights(trafficLights),
                                       _t{}
{
    auto bbox = GetModelBoundingBox(_model);
    _size = (bbox.max - bbox.min);
    _lastUpdateTime = GetTime();

    _offset = bbox.min;

    if (path.size() > 1)
    {
        _pos = path.back()->getPos();
        auto diff = _pos - path[path.size() - 2]->getPos();
        _rot = atan2f(diff.x, diff.z);
    }

    _t = std::thread
    {
        [&] {
            while (!this->shouldDespawn())
            {
                this->update();
            }
            
        }
    };

    TraceLog(LOG_INFO, "Spawned vehicle with size : %f, %f, %f and offset : %f, %f, %f", _size.x, _size.y, _size.z, _offset.x, _offset.y, _offset.z);
}

Vehicle::~Vehicle()
{
    // Wait for the thread to end, then destroy it
    _despawn = true;
    _t.join();
}

void Vehicle::update()
{
    if (_path.size() == 0)
    {
        _despawn = true;
        return;
    }

    // Handle time updates
    auto newTime = GetTime();
    float dt = newTime - _lastUpdateTime;
    _lastUpdateTime = newTime;

    checkCollisions();

    // If the simulation is paused, don't do anything
    // Same if we don't have any nodes to follow
    if (Util::SimPaused)
        return;

    const Node *target = _path.back();
    const Vector3 target_dir = _pos - target->getPos();

    const float theta = atan2f(target_dir.x, target_dir.z);

    // _moving = IsKeyDown(KEY_W);

    if (_state == VehicleState::Accelerating)
        _velocity += _acceleration * dt;
    else if (_state == VehicleState::Braking)
        _velocity -= dt * Util::BrakeStrength;
    else
        _velocity -= _velocity * .5f * dt;

    _velocity = std::clamp(_velocity, 0.f, _maxSpeed);

    _rot = Math::lerpRadians(_rot, theta, _velocity * dt / 2.f);

    // Direction as a vector
    Vector3 dir = Vector3RotateByAxisAngle(Vector3UnitZ, Vector3UnitY, _rot);

    // Update position
    _pos -= dir * _velocity * dt;

    // Check if we have reached our target yet
    if (Vector3Distance(target->getPos(), _pos) < Util::ReachDistance)
        _path.pop_back();

    // Size offset of the model, taking rotation into account
    Vector3 v = Vector3RotateByAxisAngle(_size / 2 + _offset, Vector3UnitY, _rot);

    // Update the model's transform
    _model.transform = MatrixRotateY(_rot) * MatrixTranslate(_pos.x - v.x, _pos.y - v.y, _pos.z - v.z);
}
void Vehicle::draw(bool debug)
{
    // Draw the model, with its own transform
    DrawModel(_model, Vector3Zeros, 1, WHITE);
    if (debug)
    {
        DrawLine3D(_pos, _pos + Vector3RotateByAxisAngle(Vector3(0, 0, -5), Vector3UnitY, _rot), BLUE);

        Util::DrawOBB(_pos, _size, _rot, _state == VehicleState::Accelerating ? GREEN : RED);
        Util::DrawOBB(_pos + Vector3RotateByAxisAngle({0, 0, -(_size.z + Util::WarnBoxSize.z) / 2.f}, Vector3UnitY, _rot), Util::WarnBoxSize, _rot, BLUE);
        Util::DrawOBB(_pos + Vector3RotateByAxisAngle({0, 0, -(_size.z + Util::BrakeBoxSize.z) / 2.f}, Vector3UnitY, _rot), Util::BrakeBoxSize, _rot, ORANGE);
    }
}

Vector3 Vehicle::getPos() const
{
    return _pos;
}

Vector3 Vehicle::getSize() const
{
    return _size;
}

float Vehicle::getRotation() const
{
    return _rot;
}

Matrix Vehicle::getTransform() const
{
    return _model.transform;
}

bool Vehicle::shouldDespawn() const
{
    return _despawn;
}

void Vehicle::checkCollisions()
{
    auto temp = VehicleState::Accelerating;

    Vector2 pos2d = {_pos.x, _pos.z},
            size2d = {_size.x, _size.z},
            warnPos = pos2d + Vector2Rotate({0, -(_size.z + Util::WarnBoxSize.z) / 2.f}, -_rot),
            warnSize = {Util::WarnBoxSize.x, Util::WarnBoxSize.z},
            brakePos = pos2d + Vector2Rotate({0, -(_size.z + Util::BrakeBoxSize.z) / 2.f}, -_rot),
            brakeSize = {Util::WarnBoxSize.x, Util::BrakeBoxSize.z},
            ax = Vector2Normalize(Vector2Rotate(Vector2UnitX, -_rot)),
            ay = Vector2Normalize(Vector2Rotate(Vector2UnitY, -_rot));

    // We're using the Separating axis theorem, and applying it to 2d oriented bounding boxes here
    for (auto v : _vehicles)
    {
        if (v == this)
            continue;

        // Checks if a vehicle is in front of this one
        if (Math::boxCollision(warnPos, warnSize, ax, ay, {v->_pos.x, v->_pos.z}, {v->_size.x, v->_size.z}, v->_rot))
        {
            // If it's close enough, emergency brake
            if (Math::boxCollision(brakePos, brakeSize, ax, ay, {v->_pos.x, v->_pos.z}, {v->_size.x, v->_size.z}, v->_rot))
                temp = VehicleState::Braking;
            else
                temp = VehicleState::Slowing;
        }
        else
        {
            Vector2 colPos = (Vector2){v->_pos.x, v->_pos.z} + Vector2Rotate({0, -(v->_size.z + Util::WarnBoxSize.z) / 2.f}, -v->_rot),
                    vy = Vector2Normalize(Vector2Rotate(Vector2UnitY, -v->_rot)),
                    dy = vy - ay;

            float angle = Vector2Angle(dy, ay);

            TraceLog(LOG_WARNING, "Angle : %f", angle);

            // Handles right priority
            if (Math::boxCollision(warnPos, warnSize, ax, ay, colPos, warnSize, v->_rot) && v->_state == VehicleState::Accelerating && angle < 0)
            {
                // If we're too close, emergency brake
                if (Math::boxCollision(brakePos, brakeSize, ax, ay, colPos, warnSize, v->_rot))
                    temp = VehicleState::Braking;
                else
                    temp = VehicleState::Slowing;
            }
        }
    }
    for (auto l : _trafficLights.getRef())
    {
        // Checks if we're approaching a traffic light
        if (l->active && Math::boxCollision(warnPos, warnSize, ax, ay, l->position, l->size, 0))
        {
            // If we're too close, emergency braking
            if (l->active && Math::boxCollision(brakePos, brakeSize, ax, ay, l->position, l->size, 0))
                temp = VehicleState::Braking;
            else
                temp = VehicleState::Slowing;
        }
    }
    _state = temp;
}