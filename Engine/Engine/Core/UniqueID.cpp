#include "EnginePch.h"
#include "UniqueID.h"

#include "Random.h"

UniqueID::UniqueID(const int inA, const int inB, const int inC, const int inD)
    : myA(inA), myB(inB), myC(inC), myD(inD)
{ }

UniqueID UniqueID::GenerateUniqueID()
{
    constexpr int intMin = std::numeric_limits<int>::lowest();
    constexpr int intMax = std::numeric_limits<int>::max();
    return UniqueID(Random::StaticNum(intMin, intMax), Random::StaticNum(intMin, intMax), Random::StaticNum(intMin, intMax), Random::StaticNum(intMin, intMax));
}

bool UniqueID::operator==(const UniqueID& inOther) const
{
    return myA == inOther.myA && myB == inOther.myB && myC == inOther.myC && myD == inOther.myD;
}
