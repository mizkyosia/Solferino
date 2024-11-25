#include "Vehicle.hpp"
#include <cmath>

namespace solferino
{
    Vehicle::Vehicle(const float &x,
                     const float &y,
                     const Vec2 &initialRotation) : _pos(x, y), _rot(initialRotation),
                                                  _velocity(0), _acceleration(0)
    {
        _rot.normalize();
    }

    Vehicle::~Vehicle()
    {
    }

    void Vehicle::update()
    {
        const Node *target = _path.front();
        const Vec2 target_pos = target->getPos();

        const float theta = atan2f(target_pos.x, target_pos.y) - _rot.toAngle();

        
    }
    Vec2 Vehicle::getPos() const
    {
        return _pos;
    }
    Vec2 Vehicle::getRotation() const
    {
        return _rot;
    }
}