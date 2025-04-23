#pragma once
#include <PerlinNoise.hpp>

// This class is used for sampling height data on the landscape.
class Heightfield
{
public:
    Heightfield(const siv::PerlinNoise::seed_type inSeed = 321u);

    float GetHeight(const glm::vec2 inCoordinates);

private:
    siv::PerlinNoise::seed_type mySeed = 321u;
    siv::BasicPerlinNoise<float> myPerlinNoise;
    
    constexpr float frequency = 0.0001f;
    constexpr float amplitude = 3000.0f;
};
