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
        return rand()%(max-min + 1) + min;
    }
}