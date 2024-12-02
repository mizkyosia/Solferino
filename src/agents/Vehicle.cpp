#include "Vehicle.hpp"
#include <cmath>
#include <chrono>

using namespace std::chrono;

Vehicle::Vehicle(const float &x,
                 const float &y,
                 const float &initialRotation,
                 bool &paused,
                 const Model &model) : _pos((Vector3){x, y, 0.f}), _rot(initialRotation), _model(model), _paused(&paused),
                                       _velocity(0), _acceleration(0), _moving(true)
{
    auto bbox = GetModelBoundingBox(_model);
    _size = bbox.max - bbox.min;
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
    if (*_paused)
        return;
    // const Node *target = _path.front();
    // const Vector2 target_pos = target->getPos();

    // const float theta = atan2f(target_pos.x, target_pos.y) - _rot;

    _moving = IsKeyDown(KEY_W);

    // Accelerate the vehicle if it's moving
    if (_moving)
        _velocity += _acceleration * dt;
    // Decelerate the vehicle if not
    else
        _velocity -= _weight * dt;
    _velocity = std::clamp(_velocity, 0.f, _maxSpeed);

    // Temporary controls
    if (IsKeyDown(KEY_A))
        _rot += _velocity * dt / 1.f;
    if (IsKeyDown(KEY_D))
        _rot -= _velocity * dt / 1.f;

    // Direction as a vector
    Vector3 dir = Vector3RotateByAxisAngle(Vector3UnitZ, Vector3UnitY, _rot);

    _pos -= dir * _velocity * dt;

    // Size offset of the model, taking rotation into account
    Vector3 v = Vector3RotateByAxisAngle(_size / 2, Vector3UnitY, _rot);

    // Update the model's transform
    _model.transform = MatrixRotateY(_rot) * MatrixTranslate(_pos.x - v.x, _pos.y - v.y, _pos.z - v.z);
}
void Vehicle::draw(bool debug)
{
    // Draw the model, with its own transform
    DrawModel(_model, Vector3Zeros, 1, WHITE);
    if (debug)
    {
        DrawLine3D(_pos + Vector3(0, 2, 0), _pos + Vector3RotateByAxisAngle(Vector3(0, 2, -5), Vector3UnitY, _rot), BLUE);

        Vector3 max = {_size.x / 2, _size.y / 2, _size.z / 2};
        Vector3 min = {-_size.x / 2, -_size.y / 2, -_size.z / 2};

        Vector3 end1    = _pos + Vector3RotateByAxisAngle({max.x, max.y, max.z}, Vector3UnitY, _rot),
                end2    = _pos + Vector3RotateByAxisAngle({max.x, max.y, min.z}, Vector3UnitY, _rot),
                end3    = _pos + Vector3RotateByAxisAngle({max.x, min.y, min.z}, Vector3UnitY, _rot),
                end4    = _pos + Vector3RotateByAxisAngle({max.x, min.y, max.z}, Vector3UnitY, _rot),
                front1  = _pos + Vector3RotateByAxisAngle({min.x, max.y, max.z}, Vector3UnitY, _rot),
                front2  = _pos + Vector3RotateByAxisAngle({min.x, max.y, min.z}, Vector3UnitY, _rot),
                front3  = _pos + Vector3RotateByAxisAngle({min.x, min.y, min.z}, Vector3UnitY, _rot),
                front4  = _pos + Vector3RotateByAxisAngle({min.x, min.y, max.z}, Vector3UnitY, _rot);

        // End face
        DrawLine3D(end1, end2, GREEN);
        DrawLine3D(end3, end2, GREEN);
        DrawLine3D(end3, end4, GREEN);
        DrawLine3D(end1, end4, GREEN);

        // Front face
        DrawLine3D(front1, front2, GREEN);
        DrawLine3D(front3, front2, GREEN);
        DrawLine3D(front3, front4, GREEN);
        DrawLine3D(front1, front4, GREEN);

        // Sides
        DrawLine3D(end1, front1, GREEN);
        DrawLine3D(end2, front2, GREEN);
        DrawLine3D(end3, front3, GREEN);
        DrawLine3D(end4, front4, GREEN);
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
