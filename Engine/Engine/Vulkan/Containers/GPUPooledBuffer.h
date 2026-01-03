#pragma once
#include <vector>
#include <limits>
#include "GPUList.h" // Assuming your GPUList is defined here

// NOTE: THIS STRUCTURE HAS NOT BEEN TESTED YET. ONLY CREATED AND THEN MOVED OVER TO OTHER STUFF.

template<typename ElementType>
class GPUPooledBuffer : public IGPUBuffer
{
public:
    struct Allocation
    {
        uint myStartOffset = 0;
        uint mySize = 0;
        
        bool IsValid() const { return mySize > 0; }
    };

    // explicit constructor to ensure we bind to a specific GPUList
    explicit GPUPooledBuffer(GPUList<ElementType>& inUnderlyingList) 
        : myList(inUnderlyingList)
    {
    }
    
    VulkanBuffer* GetBuffer() const override
    {
        return myList.GetBuffer();
    }
    
    MulticastDelegate<void()>* GetOnBufferResized() const override
    {
        return myList.GetOnBufferResized();
    }

    /**
     * Allocates a sub-region in the GPU buffer.
     * Strategy: Best-Fit. Tries to find the smallest free block that fits the size.
     * If no block is found, it grows the underlying GPUList.
     */
    Allocation Allocate(const uint inSize)
    {
        if (inSize == 0)
        {
            return Allocation{};
        }

        // 1. Try to find a free block using Best-Fit strategy
        int bestBlockIndex = -1;
        uint minSizeDiff = std::numeric_limits<uint>::max();

        for (int i = 0; i < myFreeBlocks.size(); ++i)
        {
            const FreeBlock& block = myFreeBlocks[i];
            
            // Check if block can hold the request
            if (block.mySize >= inSize)
            {
                uint diff = block.mySize - inSize;
                
                // If this is a tighter fit than what we found before, pick it
                if (diff < minSizeDiff)
                {
                    minSizeDiff = diff;
                    bestBlockIndex = i;
                    
                    // Perfect fit, stop searching
                    if (diff == 0) 
                        break;
                }
            }
        }

        // 2. If a valid block was found, use it
        if (bestBlockIndex != -1)
        {
            FreeBlock& block = myFreeBlocks[bestBlockIndex];
            
            Allocation allocation;
            allocation.myStartOffset = block.myStart;
            allocation.mySize = inSize;

            // Reduce the free block size. 
            // If the block is now empty (perfect fit), remove it from the list.
            block.myStart += inSize;
            block.mySize -= inSize;

            if (block.mySize == 0)
            {
                // Remove fast by swapping with last (order doesn't matter for the search, 
                // but we sort on Deallocate anyway)
                myFreeBlocks[bestBlockIndex] = myFreeBlocks.back();
                myFreeBlocks.pop_back();
            }

            return allocation;
        }

        // 3. If no free block found, grow the underlying GPUList (High Water Mark)
        Allocation newAllocation;
        newAllocation.myStartOffset = myList.Grow(inSize);
        newAllocation.mySize = inSize;

        return newAllocation;
    }

    /**
     * Returns an allocation to the pool and coalesces adjacent free blocks.
     */
    void Deallocate(const Allocation& inAllocation)
    {
        if (!inAllocation.IsValid())
        {
            return;
        }

        // 1. Insert the new free block
        FreeBlock newBlock = { inAllocation.myStartOffset, inAllocation.mySize };
        
        // We need to insert it in a sorted manner to make coalescing easy (O(N) insert/merge).
        // Find the position to insert based on Start Offset.
        auto it = std::lower_bound(myFreeBlocks.begin(), myFreeBlocks.end(), newBlock, 
            [](const FreeBlock& a, const FreeBlock& b) 
            {
                return a.myStart < b.myStart;
            });

        auto insertedIt = myFreeBlocks.insert(it, newBlock);

        // 2. Coalesce with right neighbor (Next)
        // If the current block ends exactly where the next one starts, merge them.
        auto nextIt = std::next(insertedIt);
        if (nextIt != myFreeBlocks.end())
        {
            if (insertedIt->myStart + insertedIt->mySize == nextIt->myStart)
            {
                insertedIt->mySize += nextIt->mySize;
                myFreeBlocks.erase(nextIt);
            }
        }

        // 3. Coalesce with left neighbor (Prev)
        // If the previous block ends exactly where the current one starts, merge them.
        if (insertedIt != myFreeBlocks.begin())
        {
            auto prevIt = std::prev(insertedIt);
            if (prevIt->myStart + prevIt->mySize == insertedIt->myStart)
            {
                prevIt->mySize += insertedIt->mySize;
                myFreeBlocks.erase(insertedIt);
            }
        }
    }
    
    // Optional: Debug helper to see fragmentation
    uint GetFreeBlockCount() const { return (uint)myFreeBlocks.size(); }

private:
    struct FreeBlock
    {
        uint myStart;
        uint mySize;
    };

    GPUList<ElementType>& myList;
    std::vector<FreeBlock> myFreeBlocks;
};