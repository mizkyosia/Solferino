#include "Vehicle.hpp"
#include <cmath>
#include <chrono>

using namespace std::chrono;

Vehicle::Vehicle(const float &x,
                 const float &y,
                 const float &initialRotation,
                 std::vector<Node *> path,
                 const Model &model) : _velocity(0),
                                       _acceleration(0),
                                       _rot(initialRotation),
                                       _moving(true),
                                       _pos((Vector3){x, y, 0.f}),
                                       _model(model),
                                       _path(path)
{
    auto bbox = GetModelBoundingBox(_model);
    _size = (bbox.max - bbox.min);
    _lastUpdateTime = GetTime();

    _offset = bbox.min;

    if(path.size() > 0) _pos = path.back()->getPos();

    TraceLog(LOG_INFO, "Spawned vehicle with size : %f, %f, %f and offset : %f, %f, %f ", _size.x, _size.y, _size.z, _offset.x, _offset.y, _offset.z);
}

Vehicle::~Vehicle()
{
}

void Vehicle::update()
{
    if(_path.size() == 0) {
        _despawn = true;
        return;
    }

    // Handle time updates
    auto newTime = GetTime();
    float dt = newTime - _lastUpdateTime;
    _lastUpdateTime = newTime;

    // If the simulation is paused, don't do anything
    // Same if we don't have any nodes to follow
    if (Util::SimPaused)
        return;

    const Node *target = _path.back();
    const Vector3 target_dir = _pos - target->getPos();

    const float theta = atan2f(target_dir.x, target_dir.z);

    // _moving = IsKeyDown(KEY_W);
    _moving = true;

    // Accelerate the vehicle if it's moving
    if (_moving)
        _velocity += _acceleration * dt;
    // Decelerate the vehicle if not
    else
        _velocity -= _weight * dt;
    _velocity = std::clamp(_velocity, 0.f, _maxSpeed);

    // Temporary controls
    // if (IsKeyDown(KEY_A))
    //     _rot += _velocity * dt / Util::TurnDampening;
    // if (IsKeyDown(KEY_D))
    //     _rot -= _velocity * dt / Util::TurnDampening;

    _rot = theta;

    // Direction as a vector
    Vector3 dir = Vector3RotateByAxisAngle(Vector3UnitZ, Vector3UnitY, _rot);

    // Update position
    _pos -= dir * _velocity * dt;

    // Check if we have reached our target yet
    if(Vector3Distance(target->getPos(), _pos) < Util::ReachDistance)
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
        DrawLine3D(_pos + Vector3(0, 2, 0), _pos + Vector3RotateByAxisAngle(Vector3(0, 2, -5), Vector3UnitY, _rot), BLUE);

        Vector3 pos = Vector3Transform(Vector3Zeros, _model.transform);

        Vector3 max = _size / 2;
        Vector3 min = Vector3Zeros - _size / 2;

        Vector3 end1 = pos + Vector3RotateByAxisAngle({max.x, max.y, max.z}, Vector3UnitY, _rot),
                end2 = pos + Vector3RotateByAxisAngle({max.x, max.y, min.z}, Vector3UnitY, _rot),
                end3 = pos + Vector3RotateByAxisAngle({max.x, min.y, min.z}, Vector3UnitY, _rot),
                end4 = pos + Vector3RotateByAxisAngle({max.x, min.y, max.z}, Vector3UnitY, _rot),
                front1 = pos + Vector3RotateByAxisAngle({min.x, max.y, max.z}, Vector3UnitY, _rot),
                front2 = pos + Vector3RotateByAxisAngle({min.x, max.y, min.z}, Vector3UnitY, _rot),
                front3 = pos + Vector3RotateByAxisAngle({min.x, min.y, min.z}, Vector3UnitY, _rot),
                front4 = pos + Vector3RotateByAxisAngle({min.x, min.y, max.z}, Vector3UnitY, _rot);

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

bool Vehicle::shouldDespawn() const
{
    return _despawn;
}
