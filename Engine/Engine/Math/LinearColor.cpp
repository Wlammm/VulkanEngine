#include "EnginePch.h"
#include "LinearColor.h"

#include "Color.h"

LinearColor::LinearColor()
{
    myR = 0.0f;
    myG = 0.0f;
    myB = 0.0f;
    myA = 0.0f;
}

LinearColor::LinearColor(const LinearColor& inOther)
{
    myR = inOther.myR;
    myG = inOther.myG;
    myB = inOther.myB;
    myA = inOther.myA;
}

LinearColor::LinearColor(const float inR, const float inG, const float inB, const float inA)
{
    myR = inR;
    myG = inG;
    myB = inB;
    myA = inA;
}

Color LinearColor::ToColor() const
{
    return { static_cast<int>(myR * 255), static_cast<int>(myG * 255), static_cast<int>(myB * 255), static_cast<int>(myA * 255) };
}