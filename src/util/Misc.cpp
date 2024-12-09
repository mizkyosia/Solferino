#include "Misc.hpp"

namespace Util
{
    void LoadAllModels()
    {
        ChangeDirectory(GetApplicationDirectory());
        TraceLog(LOG_WARNING, GetApplicationDirectory());

        static const std::string CarNames[CarVariantsNb] = {"Car_Basic_0",
                                                            "Car_Basic_1",
                                                            "Car_Basic_2",
                                                            "Car_Basic_3",
                                                            "Car_Basic_4",
                                                            "Car_Basic_5",
                                                            "Car_Basic_6",
                                                            "Car_Basic_7",
                                                            "Car_Basic_8",
                                                            "Car_Basic_9",
                                                            "Car_Long_0",
                                                            "Car_Long_1",
                                                            "Car_Long_2",
                                                            "Car_Long_3",
                                                            "Car_Long_4",
                                                            "Car_Long_5",
                                                            "Car_Long_6",
                                                            "Car_Long_7",
                                                            "Car_Pickup_0",
                                                            "Car_Pickup_1",
                                                            "Car_Pickup_2",
                                                            "Car_Pickup_3",
                                                            "Car_Pickup_4",
                                                            "Car_Pickup_5",
                                                            "Car_Police_0",
                                                            "Car_Police_1"};
        double t0 = GetTime() * 1000.0;

        Road = LoadModel("assets/models/road.vox");
        Road.transform.m13 -= Util::GroundLevel;

        for (int i = 0; i < Util::CarVariantsNb; i++)
        {
            CarModels[i] = LoadModel(("assets/models/cars/" + CarNames[i] + ".glb").c_str());
        }

        double t1 = GetTime() * 1000.0;

        TraceLog(LOG_INFO, GetWorkingDirectory());

        TraceLog(LOG_INFO, TextFormat("Models loaded in %.3f ms", t1 - t0));
    }

    void UnloadAllModels()
    {
        UnloadModel(Road);

        for (int i = 0; i < Util::CarVariantsNb; i++)
        {
            UnloadModel(Util::CarModels[i]);
        }

        TraceLog(LOG_INFO, "Unloaded all models");
    }
    void DrawOBB(Vector3 pos, Vector3 size, float angleY, Color color)
    {
        Vector3 max = size / 2;
        Vector3 min = Vector3Zeros - size / 2;

        Vector3 end1 = pos + Vector3RotateByAxisAngle({max.x, max.y, max.z}, Vector3UnitY, angleY),
                end2 = pos + Vector3RotateByAxisAngle({max.x, max.y, min.z}, Vector3UnitY, angleY),
                end3 = pos + Vector3RotateByAxisAngle({max.x, min.y, min.z}, Vector3UnitY, angleY),
                end4 = pos + Vector3RotateByAxisAngle({max.x, min.y, max.z}, Vector3UnitY, angleY),
                front1 = pos + Vector3RotateByAxisAngle({min.x, max.y, max.z}, Vector3UnitY, angleY),
                front2 = pos + Vector3RotateByAxisAngle({min.x, max.y, min.z}, Vector3UnitY, angleY),
                front3 = pos + Vector3RotateByAxisAngle({min.x, min.y, min.z}, Vector3UnitY, angleY),
                front4 = pos + Vector3RotateByAxisAngle({min.x, min.y, max.z}, Vector3UnitY, angleY);

        // End face
        DrawLine3D(end1, end2, color);
        DrawLine3D(end3, end2, color);
        DrawLine3D(end3, end4, color);
        DrawLine3D(end1, end4, color);

        // Front face
        DrawLine3D(front1, front2, color);
        DrawLine3D(front3, front2, color);
        DrawLine3D(front3, front4, color);
        DrawLine3D(front1, front4, color);

        // Sides
        DrawLine3D(end1, front1, color);
        DrawLine3D(end2, front2, color);
        DrawLine3D(end3, front3, color);
        DrawLine3D(end4, front4, color);
    }
}

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
    int randomRange(int max, int min)
    {
        return rand() % (max - min + 1) + min;
    }
    float lerpRadians(float a, float b, float lerpFactor) // Lerps from angle a to b (both between 0.f and PI_TIMES_TWO), taking the shortest path
    {
        float result;
        float diff = b - a;
        if (diff < -PI)
        {
            // lerp upwards past PI_TIMES_TWO
            b += 2 * PI;
            result = Lerp(a, b, lerpFactor);
            if (result >= 2 * PI)
            {
                result -= 2 * PI;
            }
        }
        else if (diff > PI)
        {
            // lerp downwards past 0
            b -= 2 * PI;
            result = Lerp(a, b, lerpFactor);
            if (result < 0.f)
            {
                result += 2 * PI;
            }
        }
        else
        {
            // straight lerp
            result = Lerp(a, b, lerpFactor);
        }

        return result;
    }
    bool boxCollision(Vector2 posA, Vector2 sizeA, Vector2 ax, Vector2 ay, Vector2 posB, Vector2 sizeB, float rotB)
    {
        for (int i = 0; i < 4; i++)
        {
            Vector2 diff = {posA.x - posB.x, posA.y - posB.y},
                    bx = Vector2Normalize(Vector2Rotate(Vector2UnitX, -rotB)),
                    bz = Vector2Normalize(Vector2Rotate(Vector2UnitY, -rotB)),
                    axes[4] = {ax, ay, bx, bz};

            // Projecting the half-sizes of the boxes onto the given axis, as well as the vector difference between those boxes
            float projA = abs(Vector2DotProduct(ax * sizeA.x / 2, axes[i])) + abs(Vector2DotProduct(ay * sizeA.y / 2, axes[i])),
                  projB = abs(Vector2DotProduct(bx * sizeB.x / 2, axes[i])) + abs(Vector2DotProduct(bz * sizeB.y / 2, axes[i])),
                  dist = abs(Vector2DotProduct(diff, axes[i]));

            // If the projected distance is more than the projected half-sizes, then the boxes can't possibly touch
            if (dist > projA + projB)
                return false;
        }

        return true;
    }
}