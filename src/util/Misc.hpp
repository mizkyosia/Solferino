#pragma once

#include <raylib.h>
#include <raymath.h>

#include <string>

/// @brief Collection of constants relative to the simulation
namespace Util
{
    const float CameraSpeed = .5f;
    const int BaseZoom = 50;

    const float GroundLevel = 2.f;

    const float ZoomMultiplier = 2;
    const float ZoomMax = 200;
    const float ZoomMin = 20;
    const float CameraMovementSmoothing = .15f;

    const float SecurityDistance = 2;
    const float ReachDistance = .4f;

    const auto KeyAddNode = KEY_E;
    const auto KeyLinkNode = KEY_Q;

    const auto KeyDebugMenu = KEY_F1;
    const auto KeyDebugVehicles = KEY_F2;
    const auto KeyDebugNodes = KEY_F3;

    const auto KeySpawnVehicle = KEY_S;

    const float TurnDampening = 10.f;

    const int CarVariantsNb = 26;
    inline Model CarModels[CarVariantsNb];

    enum class VehicleType : char
    {
        None = 0b0000,
        Car = 0b0001,
        Bus = 0b0010,
        Pedestrian = 0b0100,
        Bike = 0b1000,
    };

    inline VehicleType operator|(VehicleType a, VehicleType b)
    {
        return static_cast<VehicleType>(static_cast<char>(a) | static_cast<char>(b));
    }

    inline VehicleType operator&(VehicleType a, VehicleType b)
    {
        return static_cast<VehicleType>(static_cast<char>(a) & static_cast<char>(b));
    }

    inline Model Road;

    inline bool SimPaused = false;
    inline bool DebugNodes = false;
    inline bool DebugVehicles = false;

    /// @brief Load all models needed by the program
    void LoadAllModels();

    /// @brief Unloads all models used by the program
    void UnloadAllModels();
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

    /// @brief Returns a random int in the range [min, max[
    /// @param max
    /// @param min
    /// @return
    int randomRange(int max, int min = 0);
}