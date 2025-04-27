#include "EnginePch.h"
#include "Heightfield.h"

Heightfield::Heightfield(const siv::PerlinNoise::seed_type inSeed)
{
    mySeed = inSeed;
    myPerlinNoise = siv::BasicPerlinNoise<float>(inSeed);
}

float Heightfield::GetHeight(const glm::vec2 inCoordinates) const
{
    return myPerlinNoise.octave2D_01((static_cast<float>(inCoordinates.x) * frequency), (static_cast<float>(inCoordinates.y) * frequency), 4) * amplitude;
}
