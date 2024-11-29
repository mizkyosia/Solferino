#pragma once

#include <raylib.h>
#include <raymath.h>

/// @brief Collection of constants relative to the simulation
namespace Constants {
    const float CameraSpeed = .5f;
    const int BaseZoom = 50;

    const float ZoomMultiplier = 2;
    const float ZoomMax = 200;
    const float ZoomMin = 20;
    const float CameraMovementSmoothing = .15f;

    const int CarVariantsNb = 2;

    const float SecurityDistance = 2;
}

namespace Math {
    /// @brief Returns the numerical sign of a number
    /// @tparam T 
    /// @param val 
    /// @return 
    template <typename T> int sgn(T val);

    /// @brief Converts spherical coordinates into cartesian ones
    /// @param vec 
    /// @return 
    Vector3 sphericalToCartesian(Vector3 vec);

    /// @brief Converts cartesian coordinates into spherical ones
    /// @param vec 
    /// @return 
    Vector3 cartesianToSpherical(Vector3 vec);
}