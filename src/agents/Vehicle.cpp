#include "Vehicle.hpp"
#include <cmath>
#include <chrono>

using namespace std::chrono;

Vehicle::Vehicle(const float &x,
                 const float &y,
                 const float &initialRotation,
                 bool &paused,
                 Model &model) : _pos((Vector3){x, y, 0.f}), _rot(initialRotation), _model(model), _paused(paused),
                                 _velocity(0), _acceleration(0)
{
    auto bbox = GetModelBoundingBox(_model);
    _size = bbox.max - bbox.min;
    _size.y = 0;
    _lastUpdateTime = GetTime();
}

Vehicle::~Vehicle()
{
}

void Vehicle::update()
{
    // Handle time updates
    auto newTime = GetTime();
    float dt = newTime - _lastUpdateTime;
    _lastUpdateTime = newTime;

    // If the simulation is paused, don't do anything
    if (_paused)
        return;
    // const Node *target = _path.front();
    // const Vector2 target_pos = target->getPos();

    // const float theta = atan2f(target_pos.x, target_pos.y) - _rot;

    Vector3 dir = Vector3RotateByAxisAngle(Vector3UnitZ, Vector3UnitY, _rot);

    _velocity += _acceleration * dt;
    if(_velocity > _maxSpeed) _velocity = _maxSpeed;

    _pos -= dir * _velocity * dt;
}
void Vehicle::draw(bool debug)
{
    // Draw the model. Since pos represents the center of it, we have to offset the position by half the size,
    // taking the rotation of the vehicle into account
    DrawModelEx(_model, _pos - Vector3RotateByAxisAngle(_size / 2, Vector3UnitY, _rot), Vector3UnitY, _rot * RAD2DEG, Vector3Ones, WHITE);
    if (debug)
    {
        DrawLine3D(_pos + Vector3(0, 2, 0), _pos + Vector3RotateByAxisAngle(Vector3(0, 2, -5), Vector3UnitY, _rot), BLUE);
    }
}

Vector3 Vehicle::getPos() const
{
    return _pos;
}

float Vehicle::getRotation() const
{
    return _rot;
}