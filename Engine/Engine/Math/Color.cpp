#include "EnginePch.h"
#include "Color.h"

#include "LinearColor.h"

Color::Color()
{
    myR = 0;
    myG = 0;
    myB = 0;
    myA = 0;
}

Color::Color(const Color& inOther)
{
    myR = inOther.myR;
    myG = inOther.myG;
    myB = inOther.myB;
    myA = inOther.myA;
}

Color::Color(const int inR, const int inG, const int inB, const int inA)
{
    myR = static_cast<uint8_t>(inR);
    myG = static_cast<uint8_t>(inG);
    myB = static_cast<uint8_t>(inB);
    myA = static_cast<uint8_t>(inA);
}

LinearColor Color::ToLinearColor() const
{
    return {myR / 255.0f, myG / 255.0f, myB / 255.0f, myA / 255.0f};
}

std::string Color::ToString() const
{
    std::string val =
            "R: " + myR;
    val += " G: " + myG;
    val += " B: " + myB;
    val += " A: " + myA;
    return val;
}

Color Color::White()
{
    return Color(255, 255, 255, 255);
}

Color Color::Black()
{
    return Color(0, 0, 0, 255);
}

