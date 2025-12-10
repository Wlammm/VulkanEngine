#pragma once

#include <stack>
#include "GPUList.h"

/*
 * A wrapper around GPUList that provides stable CPU identifiers (Handles).
 * This allows removing elements without invalidating the indices of other elements.
 * 
 * To read this on the gpu create the following structure:
 * 
 * struct ObjectBuffer
 * {
 *      int myNumElements;
 *      ElementType[] myElements;
 * };
 */
template<typename ElementType>
class GPUSparseDenseBuffer : public IGPUList
{
public:
    GPUSparseDenseBuffer(const vk::BufferCreateInfo& inCreateInfo, const std::string& inBufferName, const VmaMemoryUsage inMemoryUsage, const uint inCapacity = 4)
        : myDenseData(inCreateInfo, inBufferName, inMemoryUsage, inCapacity)
    { }
    
    VulkanBuffer* GetBuffer() const override { return myDenseData.GetBuffer(); }
    MulticastDelegate<void()>& GetOnBufferResized() const override { return myDenseData.GetOnBufferResized();}
    
    /*
     * Adds data to the GPU buffer and returns a stable CPU handle.
     * This handle remains valid even if other elements are removed.
     */
    uint Add(const ElementType& inData)
    {
        uint handle;

        // Recycle an old handle if available, otherwise create a new one
        if (!myFreeHandles.empty())
        {
            handle = myFreeHandles.top();
            myFreeHandles.pop();
        }
        else
        {
            handle = static_cast<uint>(mySparseIndices.size());
            mySparseIndices.Add(0); // Placeholder, is being set below
        }

        // Determine where this data will live in the Dense array (at the end)
        // We can track size via our CPU reverse map, as it mirrors the GPU list size.
        uint denseIndex = static_cast<uint>(myDenseToHandle.size());

        // Add to the underlying GPU List
        myDenseData.Add(inData);

        // Update mappings
        mySparseIndices[handle] = denseIndex;
        myDenseToHandle.Add(handle);

        return handle;
    }
    
    /*
     * Updates the data on the GPU using the stable handle.
     */
    void Update(const ElementType& inData, const uint inHandle)
    {
        if (inHandle >= static_cast<uint>(mySparseIndices.size()))
        {
            check(false && "Invalid handle.");
            return;
        }

        uint denseIndex = mySparseIndices[inHandle];
        
        // Sanity check: Ensure this handle is actually pointing to a valid dense index
        if (denseIndex == INVALID_INDEX) 
        {
            check(false && "Invalid dense index.");
            return;
        }

        myDenseData.SetDataAtIndex(inData, denseIndex);
    }
    
    /*
     * Removes data using the stable handle.
     * This performs a "Swap and Pop" on the GPU buffer to maintain contiguous memory.
     */
    void Remove(const uint inHandle)
    {
        if (inHandle >= static_cast<uint>(mySparseIndices.size()))
        {
            check(false && "Invalid handle");
            return;
        }

        uint denseIndexToRemove = mySparseIndices[inHandle];
        if (denseIndexToRemove == INVALID_INDEX) return;

        uint lastDenseIndex = static_cast<uint>(myDenseToHandle.size()) - 1;

        // If the element to remove is NOT the last element, we must swap.
        if (denseIndexToRemove != lastDenseIndex)
        {
            // Identify who is currently at the end of the dense array
            uint lastHandle = myDenseToHandle[lastDenseIndex];

            // Perform the GPU move. 
            // GPUList::RemoveIndex() copies the last element's data into 'denseIndexToRemove'
            // and decrements the GPU size.
            myDenseData.RemoveIndex(denseIndexToRemove);

            // Update the mappings to reflect the swap
            // The handle that used to be at the end (lastHandle) is now at 'denseIndexToRemove'
            mySparseIndices[lastHandle] = denseIndexToRemove;
            myDenseToHandle[denseIndexToRemove] = lastHandle;
        }
        else
        {
            // If we are removing the last item, no swap is needed.
            myDenseData.RemoveLast();
        }

        // Cleanup CPU tracking
        myDenseToHandle.RemoveLast();       // Remove the last entry (size decreased)
        mySparseIndices[inHandle] = INVALID_INDEX; // Invalidate the removed handle
        myFreeHandles.push(inHandle);     // Recycle the handle
    }
    
private:
    static constexpr uint INVALID_INDEX = UINT_MAX;
    
    GPUList<ElementType> myDenseData;
    
    // If mySparseIndices[h] == i, it means Handle 'h' is stored at myDenseData[i]
    List<uint> mySparseIndices;

    // Map: Dense Index -> Stable Handle
    // If myDenseToHandle[i] == h, it means the data at myDenseData[i] belongs to Handle 'h'
    // This allows us to know which Handle to update when we move the last element during a Remove.
    List<uint> myDenseToHandle;

    // Stack of handles that have been removed and can be reused
    std::stack<uint> myFreeHandles;
};
