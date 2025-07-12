#pragma once


class UniqueID
{
public:
    UniqueID() = default;
    UniqueID(const UniqueID& inOther) = default;
    UniqueID(UniqueID&& inOther) = default;
    UniqueID(const int inA, const int inB, const int inC, const int inD);

    UniqueID& operator=(const UniqueID& inOther) = default;
    
    static UniqueID GenerateUniqueID();

    bool operator==(const UniqueID& inOther) const;
    
private:
    META(SerializeField)
    int myA = 0;

    META(SerializeField)
    int myB = 0;

    META(SerializeField)
    int myC = 0;

    META(SerializeField)
    int myD = 0;

    // Give std::hash access to private members
    friend struct std::hash<UniqueID>;
};


inline void hash_combine(std::size_t& seed, int value) {
    seed ^= std::hash<int>()(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

namespace std {
    template<>
    struct hash<UniqueID> {
        std::size_t operator()(const UniqueID& id) const {
            std::size_t seed = 0;
            hash_combine(seed, id.myA);
            hash_combine(seed, id.myB);
            hash_combine(seed, id.myC);
            hash_combine(seed, id.myD);
            return seed;
        }
    };
}

namespace std {
    template<>
    struct hash<const UniqueID> : hash<UniqueID> {};
}