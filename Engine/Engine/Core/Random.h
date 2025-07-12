#pragma once

#include <random>

class Random
{
public:
    template<typename NumericType>
    static NumericType StaticNum(NumericType inMin, NumericType inMax)
    {
        static std::random_device rd;
        static std::mt19937 engine(rd());
        return Generate(inMin, inMax, engine);
    }

    explicit Random(uint32_t seed)
        : myEngine(seed)
    {
    }

    template<typename NumericType>
    NumericType Num(NumericType inMin, NumericType inMax)
    {
        return Generate(inMin, inMax, myEngine);
    }

private:
    std::mt19937 myEngine;

    template<typename NumericType, typename Engine>
    static NumericType Generate(NumericType inMin, NumericType inMax, Engine& engine)
    {
        if constexpr (std::is_integral_v<NumericType>) {
            std::uniform_int_distribution<NumericType> dist(inMin, inMax);
            return dist(engine);
        } else if constexpr (std::is_floating_point_v<NumericType>) {
            std::uniform_real_distribution<NumericType> dist(inMin, inMax);
            return dist(engine);
        } else {
            static_assert(std::is_arithmetic_v<NumericType>, "Num only supports numeric types");
        }
    }
};