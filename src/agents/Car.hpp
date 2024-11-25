#include "Vehicle.hpp"

namespace solferino
{
        class Car : public Vehicle
        {
        public:
                constexpr static float _maxSpeed = 5.f;

                void test();
        };
}
