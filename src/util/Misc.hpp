#pragma once

#include <raylib.h>
#include <raymath.h>

/// @brief Collection of constants relative to the simulation
namespace Constants
{
    const float CameraSpeed = .5f;
    const int BaseZoom = 50;

    const float GroundLevel = 2.f;

    const float ZoomMultiplier = 2;
    const float ZoomMax = 200;
    const float ZoomMin = 20;
    const float CameraMovementSmoothing = .15f;

    const int CarVariantsNb = 2;

    const float SecurityDistance = 2;

    const char Car          = 0b0001;
    const char Bus          = 0b0010;
    const char Bike         = 0b0100;
    const char Pedestrian   = 0b1000;

    const auto KeyAddNode = KEY_E;
    const auto KeyLinkNode = KEY_Q;

    const auto KeyDebugMenu = KEY_F1;
    const auto KeyDebugVehicles = KEY_F2;
    const auto KeyDebugNodes = KEY_F3;
}

namespace Math
{
    /// @brief Returns the numerical sign of a number
    /// @tparam T
    /// @param val
    /// @return
    template <typename T>
    int sgn(T val);

    /// @brief Converts spherical coordinates into cartesian ones
    /// @param vec
    /// @return
    Vector3 sphericalToCartesian(Vector3 vec);

    /// @brief Converts cartesian coordinates into spherical ones
    /// @param vec
    /// @return
    Vector3 cartesianToSpherical(Vector3 vec);
}