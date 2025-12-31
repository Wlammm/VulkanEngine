#pragma once

class Color;

class LinearColor
{
public:
    LinearColor();
    LinearColor(const LinearColor& inOther);
    LinearColor(const float inR, const float inG, const float inB, const float inA);

    Color ToColor() const;

    float R;
    float G;
    float B;
    float A;
};
