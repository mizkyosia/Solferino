#pragma once
#include "../pathfinding/Graph.hpp"
#include "../util/Vec2.hpp"

namespace solferino
{
        class Vehicle
        {
        protected:
                /// @brief Variables for managing the vehicle's transform in space
                float _velocity, _acceleration;
                /// @brief Variables managing the vehicle's vision range (to see if any other vehicles might block its path)
                float _visionDistance, _visionWidth;
                /// @brief Position of the vehicle in 2D space
                Vec2 _pos, _rot;
                /// @brief Path of nodes the vehicle must take
                std::queue<Node *> _path;

        public:
                /// @brief Static variable representing the bounds of this vehicle type's sprite
                const static Vec2 _size;
                /// @brief The maximum speed of the vehicle
                const static float _maxSpeed;
                /// @brief Base acceleration value of this vehicle type
                const static float _baseAcceleration;

                Vehicle(const float &x, const float &y, const Vec2& initialRotation);
                ~Vehicle();

                /// @brief Updates the vehicle : calculates new acceleration, velocity, position, direction...
                void update();

                /// @brief Gets the position of the car. The transform of the car is its center
                /// @return
                Vec2 getPos() const;
                /// @brief Get the rotation of the vehicle
                /// @return
                Vec2 getRotation() const;
        };
}