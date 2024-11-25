#include "Vec2.hpp"
#include <cmath>

namespace solferino
{

    const Vec2 Vec2::zero()
    {
        return Vec2(0, 0);
    }

    const Vec2 Vec2::fromAngle(const float &theta)
    {
        return Vec2(cos(theta), sin(theta));
    }

    Vec2::Vec2(const float &x, const float &y) : x(x), y(y)
    {
    }

    Vec2::~Vec2()
    {
    }

    Vec2 Vec2::operator+(const Vec2 &vec) const
    {
        return Vec2(x + vec.x, y + vec.y);
    }

    Vec2 Vec2::operator-(const Vec2 &vec) const
    {
        return Vec2(x - vec.x, y - vec.y);
    }

    float Vec2::operator*(const Vec2 &vec) const
    {
        return x * vec.x + y * vec.y;
    }

    Vec2 Vec2::operator*(const float &n) const
    {
        return Vec2(x * n, y * n);
    }

    Vec2 Vec2::operator/(const float &n) const
    {
        if (x == 0)
            return Vec2(0, 0);
        return Vec2(x / n, y / n);
    }

    bool Vec2::operator==(const Vec2 &vec) const
    {
        return x == vec.x && y == vec.y;
    }

    float Vec2::getMagnitude() const
    {
        return sqrt(pow(x, 2) + pow(y, 2));
    }

    Vec2 Vec2::abs() const
    {
        return Vec2(std::abs(x), std::abs(y));
    }

    float Vec2::toAngle() const
    {
        return atan2(x, y);
    }

    void Vec2::normalize()
    {
        float m = getMagnitude();
        if (m == 0)
            return;
        x /= m;
        y /= m;
    }

    float Vec2::distanceTo(const Vec2 &vec) const
    {
        return sqrt(pow(x - vec.x, 2) + pow(y - vec.y, 2));
    }

    bool Vec2::inTriangle(const Vec2 &p1, const Vec2 &p2, const Vec2 &p3) const
    {
        float den = (p2.y - p3.y) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.y - p3.y),
            a = ((p2.y - p3.y) * (x - p3.x) + (p3.x - p2.x) * (y - p3.y)) / den,
            b = ((p3.y - p1.y) * (x - p3.x) + (p1.x - p3.x) * (y - p3.y)) / den,
            c = 1 - a - b;
        
        return 0 <= a && a <= 1 && 0 <= b && b <= 1 && 0 <= c && c <= 1;
    }

    std::ostream& operator<<(std::ostream& os, const Vec2& other) {
        os << "Vector2(" << other.x << ", " << other.y << ")";
        return os;
    }

}