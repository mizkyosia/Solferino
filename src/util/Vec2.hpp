#pragma once

#include <iostream>

namespace solferino
{
        struct Vec2
        {
        public:
                float x, y;
                /// @brief Alias to `Vec2(0.f, 0.f)`
                /// @return
                const static Vec2 zero();
                /// @brief Creates a new `Vec2` from a given angle (in radians)
                /// @return
                const static Vec2 fromAngle(const float &theta);

                Vec2(const float &x, const float &y);
                ~Vec2();

                /// @brief Adds two vectors together
                /// @param vec
                /// @return
                Vec2 operator+(const Vec2 &vec) const;
                /// @brief Returns the difference between the two vectors
                /// @param vec
                /// @return
                Vec2 operator-(const Vec2 &vec) const;
                /// @brief Cartesian product of the two vectors, corresponding to `x1*x2 + y1*y2`
                /// @param vec
                /// @return
                float operator*(const Vec2 &vec) const;
                /// @brief Multiplication of a vector by a scalar
                /// @param x
                /// @return
                Vec2 operator*(const float &x) const;
                /// @brief Division of a vector by a scalar
                /// @param x
                /// @return
                Vec2 operator/(const float &x) const;
                /// @brief Checks if two vectors are equal
                /// @param vec
                /// @return
                bool operator==(const Vec2 &vec) const;

                /// @brief Debugging fore Vec2
                /// @param os 
                /// @param other 
                /// @return 
                friend std::ostream& operator<<(std::ostream &os, const Vec2& other);

                /// @brief Gets the magnitude of the vector :    `||v||`
                /// @return
                float getMagnitude() const;
                /// @brief Returns a new vector whose coordinates are the absolute value of this vector's coordinates
                /// @return
                Vec2 abs() const;
                /// @brief Returns this vector as an angle in radians
                /// @return
                float toAngle() const;
                /// @brief Modifies the x and y coordinates such that the magnitude of this vector is equal to 1
                void normalize();
                /// @brief Returns the distance between two vectors
                /// @param vec
                /// @return
                float distanceTo(const Vec2 &vec) const;
                /// @brief Uses the barycentric coordinates to determine if the point represented by this vector is inside a given triangle
                /// @param p1 First point of the triangle
                /// @param p2 Second point of the triangle
                /// @param p3 Third point of the triangle
                /// @return
                bool inTriangle(const Vec2 &p1, const Vec2 &p2, const Vec2 &p3) const;
        };

} // namespace solferino
