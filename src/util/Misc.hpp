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
    const float ReachDistance = 2.f;
    const float BrakeStrength = 20;

    const auto KeyAddNode = KEY_E;
    const auto KeyLinkNode = KEY_Q;

    const auto KeyDebugMenu = KEY_F1;
    const auto KeyDebugVehicles = KEY_F2;
    const auto KeyDebugNodes = KEY_F3;

    const auto KeySpawnVehicle = KEY_S;

    const float TurnDampening = 10.f;

    const Vector3 WarnBoxSize = {2, 2, 4};
    const Vector3 BrakeBoxSize = {2, 2, 2};

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

    void DrawOBB(Vector3 pos, Vector3 size, float angleY, Color color);
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

    /// @brief Linear interpolation for an angle
    /// @param a 
    /// @param b 
    /// @param lerpFactor 
    /// @return 
    float lerpRadians(float a, float b, float lerpFactor);

    /// @brief Uses the "Separating axis theorem" to determine if two oriented bounding boxes A and B are colliding
    /// @param posA Position of the first hitbox
    /// @param sizeA Size of the first hitbox
    /// @param ax X-Axis relative to box A
    /// @param ay Y-Axis relative to box A
    /// @param posB Position of the second hitbox
    /// @param sizeB Size of the second hitbox
    /// @param rotB Rotation of the second hitbox
    /// @return 
    bool boxCollision(Vector2 posA, Vector2 sizeA, Vector2 ax, Vector2 ay, Vector2 posB, Vector2 sizeB, float rotB);
}