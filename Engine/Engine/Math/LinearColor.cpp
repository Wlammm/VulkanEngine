#include "EnginePch.h"
#include "LinearColor.h"

#include "Color.h"

LinearColor::LinearColor()
{
    R = 0.0f;
    G = 0.0f;
    B = 0.0f;
    A = 0.0f;
}

LinearColor::LinearColor(const LinearColor& inOther)
{
    R = inOther.R;
    G = inOther.G;
    B = inOther.B;
    A = inOther.A;
}

LinearColor::LinearColor(const float inR, const float inG, const float inB, const float inA)
{
    R = inR;
    G = inG;
    B = inB;
    A = inA;
}

Color LinearColor::ToColor() const
{
    return { static_cast<int>(R * 255), static_cast<int>(G * 255), static_cast<int>(B * 255), static_cast<int>(A * 255) };
}