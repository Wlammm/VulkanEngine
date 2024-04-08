#pragma once

class LinearColor;

class Color
{
public:
    Color();
    Color(const Color& inOther);
    Color(const int inR, const int inG, const int inB, const int inA);

    LinearColor ToLinearColor() const;

    std::string ToString() const;

    static Color White();
    static Color Black();
    
private:
    uint8_t myR;
    uint8_t myG;
    uint8_t myB;
    uint8_t myA;
};
