#include "Misc.hpp"

namespace Math
{
    template <typename T>
    int sgn(T val)
    {
        return (T(0) < val) - (val < T(0));
    }

    Vector3 sphericalToCartesian(Vector3 vec)
    {
        return (Vector3){vec.x * sinf(vec.y) * cosf(vec.z), vec.x * cosf(vec.y), vec.x * sinf(vec.y) * sinf(vec.z)};
    }

    Vector3 cartesianToSpherical(Vector3 vec)
    {
        float r = sqrtf(powf(vec.x, 2) + powf(vec.z, 2) + powf(vec.y, 2));
        float d = sqrtf(powf(vec.x, 2) + powf(vec.z, 2));

        return Vector3(
            r,
            vec.y == r ? 0 : acosf(vec.y / r),
            d == 0 ? 0 : sgn(vec.z) * acosf(vec.x / d));
    }
}