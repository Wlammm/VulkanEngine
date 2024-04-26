#pragma once

class MathUtils
{
public:
    // Returns the next (higher) size that is power of 2. 
    template<typename T>
    static T UpperPowerOfTwo(T inRequiredSize)
    {
        T a = static_cast<T>(log2(inRequiredSize));

        if (pow(2, a) == inRequiredSize)
            return inRequiredSize;

        return static_cast<T>(pow(2, a + 1));
    }
};