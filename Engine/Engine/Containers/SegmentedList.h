#pragma once
#include "Engine/EnginePch.h"

template<typename ElementType, uint ElementsPerSegment>
class SegmentedList
{
public:
    class Iterator
    {
    public:
        Iterator(SegmentedList& inList, uint inIndex)
            : myList(inList), myIndex(inIndex)
        {
#if DEBUG
            myInitCurrentSize = myList.myCurrentSize;
            myInitFreeIndicesSize = myList.myFreeIndices.size();
#endif
            SkipInvalidIndices();
        }

        ElementType& operator*() const
        {
            return myList[myIndex];
        }

        Iterator& operator++()
        {
#if DEBUG
            check(myInitCurrentSize == myList.myCurrentSize && myInitFreeIndicesSize == myList.myFreeIndices.size() && "List range was modified during range-based iteration.");
#endif
            myIndex++;
            SkipInvalidIndices();
            return *this;
        }

        bool operator!=(const Iterator& inOther) const
        {
            return myIndex != inOther.myIndex;
        }

        Iterator& SkipInvalidIndices()
        {
            while(myIndex < myList.myCurrentSize && myList.myFreeList[myIndex])
            {
                myIndex++;
            }
            return *this;
        }

    private:
        SegmentedList& myList;
        uint myIndex;
#if DEBUG
        uint myInitCurrentSize;
        uint myInitFreeIndicesSize;
#endif
    };

    class ConstIterator
    {
    public:
        ConstIterator(const SegmentedList& inList, uint inIndex)
            : myList(inList), myIndex(inIndex)
        {
#if DEBUG
            myInitCurrentSize = myList.myCurrentSize;
            myInitFreeIndicesSize = myList.myFreeIndices.size();
#endif
            
            SkipInvalidIndices();
        }

        const ElementType& operator*() const
        {
            return myList[myIndex];
        }

        ConstIterator& operator++()
        {
#if DEBUG
            check(myInitCurrentSize == myList.myCurrentSize && myInitFreeIndicesSize == myList.myFreeIndices.size() && "List range was modified during range-based iteration.");
#endif
            myIndex++;
            SkipInvalidIndices();
            return *this;
        }

        bool operator!=(const ConstIterator& inOther) const
        {
            return myIndex != inOther.myIndex;
        }

        ConstIterator& SkipInvalidIndices()
        {
            while(myIndex < myList.myCurrentSize && myList.myFreeList[myIndex])
            {
                myIndex++;
            }
            return *this;
        }

    private:
        const SegmentedList& myList;
        uint myIndex;

#if DEBUG
        uint myInitCurrentSize;
        uint myInitFreeIndicesSize;
#endif
    };

    Iterator begin() { return Iterator(*this, 0); }
    Iterator end() { return Iterator(*this, myCurrentSize); }
    
    ConstIterator begin() const { return ConstIterator(*this, 0); }
    ConstIterator end() const { return ConstIterator(*this, myCurrentSize); }

    ConstIterator cbegin() const { return ConstIterator(*this, 0); }
    ConstIterator cend() const { return ConstIterator(*this, myCurrentSize); }

    
    SegmentedList() = default;
    
    ~SegmentedList()
    {
        Clear();
        for(ElementType* segment : mySegments)
        {
            free(segment);
        }
        mySegments.Clear();
    }

    void Clear()
    {
        if constexpr(!std::is_trivially_destructible<ElementType>::value)
        {
            for (const auto& value : *this)
            {
                value.~ElementType();
            }
        }
        myCurrentSize = 0;
    }

    void Add(const ElementType& inValue)
    {
        Emplace(inValue);
    }

    void Add(ElementType&& inValue)
    {
        Emplace(std::move(inValue));
    }

    template<typename... ArgsType>
    ElementType& EmplaceAndGetIndex(uint& outIndex, ArgsType&&... InArgs)
    {
        if(!myFreeIndices.IsEmpty())
        {
            outIndex = myFreeIndices.Last();
            myFreeIndices.RemoveLast();
        }
        else
        {
            outIndex = myCurrentSize;
            myFreeList.push_back(false);
            myCurrentSize++;
        }
        myFreeList[outIndex] = false;

        const uint segmentIndex = GetSegmentIndex(outIndex);
        const uint blockIndex = GetBlockIndex(outIndex);

        if(segmentIndex == mySegments.size())
            mySegments.Add((ElementType*)calloc(ElementsPerSegment, sizeof(ElementType)));
        
        ElementType* segment = mySegments[segmentIndex];
        new (segment + blockIndex) ElementType(std::forward<ArgsType>(InArgs)...);
        return segment[blockIndex];
    }
    
    template<typename... ArgsType>
    ElementType& Emplace(ArgsType&&... InArgs)
    {
        uint index;
        return EmplaceAndGetIndex(index, std::forward<ArgsType>(InArgs)...);   
    }

    const ElementType& operator[] (const uint inIndex) const
    {
        check(IsValidIndex(inIndex));
        const uint segmentIndex = GetSegmentIndex(inIndex);
        const uint blockIndex = GetBlockIndex(inIndex);
        return mySegments[segmentIndex][blockIndex];
    }

    ElementType& operator[] (const uint inIndex)
    {
        check(IsValidIndex(inIndex));
        const uint segmentIndex = GetSegmentIndex(inIndex);
        const uint blockIndex = GetBlockIndex(inIndex);
        return mySegments[segmentIndex][blockIndex];
    }

    const ElementType& First() const
    {
        return *begin().SkipInvalidIndices();
    }

    ElementType& First()
    {
        return *begin().SkipInvalidIndices();
    }
    
    void RemoveIndex(const uint inIndex)
    {
        check(IsValidIndex(inIndex));
        
        myFreeIndices.Add(inIndex);
        myFreeList[inIndex] = true;

        const uint segmentIndex = GetSegmentIndex(inIndex);
        const uint blockIndex = GetBlockIndex(inIndex);
        
        ElementType* segment = mySegments[segmentIndex];
        
        if constexpr(!std::is_trivially_destructible<ElementType>::value)
        {
            segment[blockIndex].~ElementType();
        }
    }

    bool IsValidIndex(const uint inIndex) const
    {
        return inIndex >= 0 && inIndex < myCurrentSize && !myFreeList[inIndex];    
    }
    
    bool IsEmpty() const
    {
        return myFreeIndices.size() == myCurrentSize;
    }

    bool IsIndexOccupied(const uint inIndex) const
    {
        return !myFreeIndices[inIndex];
    }

private:
    uint GetSegmentIndex(const uint inIndex) const
    {
        return inIndex / ElementsPerSegment;
    }

    uint GetBlockIndex(const uint inIndex) const
    {
        return inIndex % ElementsPerSegment;
    }
    
private:
    List<ElementType*> mySegments;

    // Using vector here as it already have a specialized implementation for bools. Returns true whenever an index is free. 
    std::vector<bool> myFreeList;
    
    // An extra list of free indices for faster insertion.
    List<uint> myFreeIndices;

    uint myCurrentSize;
};
