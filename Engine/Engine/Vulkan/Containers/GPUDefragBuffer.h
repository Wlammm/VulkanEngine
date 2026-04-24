#pragma once
#include "GPUList.h"
#include "Engine/Vulkan/ResizableBuffer.h"
#include "Engine/Vulkan/VulkanAllocator.h"

/*
 * Default sparse entry type. SparseEntryType must expose myByteOffset.
 * Users can define their own struct with additional shader fields (e.g. element count,
 * stride) as long as myByteOffset is present.
 * NOTE: myByteSize is NOT required here — allocation sizes are tracked CPU-only in
 * AllocatedBlock, so the GPU-facing struct stays as small as possible.
 */
struct SparseBufferEntry
{
    uint myByteOffset = 0;
};

/*
 * A GPU buffer with stable handles and defragmentation support.
 *
 * Internally manages two GPU buffers:
 *   DENSE  (ResizableBuffer) — raw byte storage; all actual data (vertices, indices, etc.)
 *   SPARSE (GPUList<SparseEntryType>) — fixed-size entries indexed by stable uint handles;
 *                                       each stores a byte offset+size into the dense buffer.
 *
 * On the GPU, shaders read:
 *   SparseEntryType entry = sparseBuffer[handle.mySparseIndex];
 *   // then access denseBuffer at entry.myByteOffset with entry.myByteSize bytes
 *
 * Defrag moves dense allocations left to close holes and only patches sparse entries.
 * All handles remain valid across dealloc and defrag — the indirection absorbs movement.
 *
 * --- Non-overlapping copy constraint in Defrag ---
 * vkCmdCopyBuffer does NOT allow src/dst regions to overlap within the same buffer.
 * When filling a free block at freeOffset with an allocation at srcOffset (> freeOffset),
 * the aligned destination is: dstOffset = AlignUp(freeOffset, alloc.alignment).
 * The copy is safe when: dstOffset + allocSize <= srcOffset
 * Defrag also checks the allocation fits within the free block:
 *   dstOffset + allocSize <= freeOffset + freeBlock.size
 *
 * --- SparseEntryType requirements ---
 * Must be default-constructible and expose:
 *   uint myByteOffset;
 * myByteSize is NOT required — sizes are tracked CPU-only inside AllocatedBlock.
 */
template<typename SparseEntryType = SparseBufferEntry>
class GPUDefragBuffer : public IGPUBuffer
{
public:
    struct Handle
    {
        uint mySparseIndex = static_cast<uint>(-1);
        bool IsValid() const { return mySparseIndex != static_cast<uint>(-1); }
    };

    static Handle InvalidHandle() { return Handle{}; }

    /*
     * inDenseCreateInfo  — BufferCreateInfo for the dense (data) buffer.
     *                      Must include eTransferSrc | eTransferDst for defrag moves,
     *                      plus eVertexBuffer or eIndexBuffer as needed.
     * inSparseCreateInfo — BufferCreateInfo for the sparse buffer.
     *                      Must include eStorageBuffer | eTransferDst.
     */
    GPUDefragBuffer(
        const vk::BufferCreateInfo& inDenseCreateInfo,
        const std::string&          inDenseBufferName,
        const vk::BufferCreateInfo& inSparseCreateInfo,
        const std::string&          inSparseBufferName,
        const VmaMemoryUsage        inMemoryUsage,
        const uint                  inInitialSparseCapacity = 4)
    {
        myDenseBuffer = new ResizableBuffer(
            VulkanAllocator::AllocateBuffer_TS(inDenseBufferName, inDenseCreateInfo, inMemoryUsage, false));
        
        mySparseBuffer = new GPUList<SparseEntryType>(inSparseCreateInfo, inSparseBufferName, inMemoryUsage, inInitialSparseCapacity);
    }

    ~GPUDefragBuffer() override
    {
        delete myDenseBuffer;
        myDenseBuffer = nullptr;
        delete mySparseBuffer;
        mySparseBuffer = nullptr;
    }

    GPUDefragBuffer(const GPUDefragBuffer&) = delete;
    void operator=(const GPUDefragBuffer&) = delete;

    // IGPUBuffer — returns the dense buffer (bind as vertex / index buffer)
    VulkanBuffer* GetBuffer() const override { return myDenseBuffer->GetBuffer(); }
    MulticastDelegate<void()>* GetOnBufferResized() const override { return myDenseBuffer->GetOnBufferResized(); }

    // Returns the sparse buffer (bind as storage buffer for shader reads)
    IGPUBuffer* GetSparseBuffer() { return mySparseBuffer; }

    /*
     * Allocates inByteSize bytes in the dense buffer, uploads inData, and returns a stable Handle.
     * inAlignment is the required byte alignment for the start of the allocation.
     */
    Handle Allocate(const void* inData, const uint inByteSize, const uint inAlignment = 4)
    {
        check(inByteSize > 0);

        const uint denseOffset = FindOrGrowDense(inByteSize, inAlignment);

        // Acquire a sparse index — recycle a freed slot or expand the sparse buffer
        uint sparseIndex;
        if (!myFreeSparseIndices.IsEmpty())
        {
            sparseIndex = myFreeSparseIndices.Last();
            myFreeSparseIndices.RemoveLast();
        }
        else
        {
            sparseIndex = static_cast<uint>(mySparseEntries_CPU.size());
            mySparseEntries_CPU.Emplace();       // grow CPU mirror
            mySparseBuffer->Add(SparseEntryType{}); // grow GPU list
        }

        // Write sparse entry (only myByteOffset; size is tracked CPU-only)
        SparseEntryType& entry = mySparseEntries_CPU[sparseIndex];
        entry.myByteOffset = denseOffset;
        mySparseBuffer->SetDataAtIndex(entry, sparseIndex);

        // Upload dense data (ResizableBuffer::SetData handles capacity growth)
        myDenseBuffer->SetData(inData, inByteSize, denseOffset);

        // Track allocation (unsorted; Defrag uses linear scans)
        myAllocatedBlocks.Add({ denseOffset, inByteSize, sparseIndex, inAlignment });

        return Handle{ sparseIndex };
    }

    /*
     * Same as Allocate but copies from a GPU staging buffer instead of CPU memory.
     * Use this when the source data is already in a VulkanBuffer (e.g. from the StagingSystem).
     */
    Handle AllocateFromStagingBuffer(VulkanBuffer* inStagingBuffer, const uint inByteSize, const uint inAlignment = 4)
    {
        check(inByteSize > 0);

        const uint denseOffset = FindOrGrowDense(inByteSize, inAlignment);

        uint sparseIndex;
        if (!myFreeSparseIndices.IsEmpty())
        {
            sparseIndex = myFreeSparseIndices.Last();
            myFreeSparseIndices.RemoveLast();
        }
        else
        {
            sparseIndex = static_cast<uint>(mySparseEntries_CPU.size());
            mySparseEntries_CPU.Emplace();
            mySparseBuffer->Add(SparseEntryType{});
        }

        SparseEntryType& entry = mySparseEntries_CPU[sparseIndex];
        entry.myByteOffset = denseOffset;
        mySparseBuffer->SetDataAtIndex(entry, sparseIndex);

        myDenseBuffer->CopyDataFromBuffer(inStagingBuffer, inByteSize, denseOffset);

        myAllocatedBlocks.Add({ denseOffset, inByteSize, sparseIndex, inAlignment });

        return Handle{ sparseIndex };
    }

    /*
     * Frees the allocation associated with the handle.
     * The handle becomes invalid and must not be used again.
     */
    void Deallocate(const Handle inHandle)
    {
        check(inHandle.IsValid());

        // Find and remove the allocated block — get byte size from it (not the sparse entry)
        uint offset = 0;
        uint size   = 0;
        for (int i = 0; i < static_cast<int>(myAllocatedBlocks.size()); ++i)
        {
            if (myAllocatedBlocks[i].mySparseIndex == inHandle.mySparseIndex)
            {
                offset = myAllocatedBlocks[i].myByteOffset;
                size   = myAllocatedBlocks[i].myByteSize;
                myAllocatedBlocks.RemoveIndex(i);
                break;
            }
        }
        check(size > 0 && "Deallocate called with a handle not found in allocated blocks");

        // Return dense region to the free pool (coalesced on insert)
        InsertFreeBlock({ offset, size });

        // Invalidate the GPU sparse entry (zero sentinel)
        mySparseEntries_CPU[inHandle.mySparseIndex] = SparseEntryType{};
        mySparseBuffer->SetDataAtIndex(SparseEntryType{}, inHandle.mySparseIndex);

        myFreeSparseIndices.Add(inHandle.mySparseIndex);
    }

    /*
     * Moves up to inMaxMoves allocations leftward to fill holes in the dense buffer.
     * Each move issues a GPU buffer copy and a sparse entry update.
     * Returns the number of moves actually performed.
     *
     * Call once per frame with a small budget (e.g. 4-8) to amortize defrag cost.
     */
    uint Defrag(const uint inMaxMoves)
    {
        uint movesDone = 0;

        while (movesDone < inMaxMoves)
        {
            // Find the leftmost free block that has a fitting allocation after it.
            // The aligned destination must satisfy:
            //   alignedDst + allocSize <= srcOffset          (non-overlapping copy)
            //   alignedDst + allocSize <= freeBlock.end      (fits within free block)
            // We scan all free blocks and pick the leftmost valid one.
            int  bestFreeIdx    = -1;
            int  bestAllocIdx   = -1;
            uint bestFreeOffset = UINT_MAX;

            for (int freeBlockIndex = 0; freeBlockIndex < static_cast<int>(myFreeBlocks.size()); ++freeBlockIndex)
            {
                const FreeBlock& freeBlock = myFreeBlocks[freeBlockIndex];
                if (freeBlock.myByteOffset >= bestFreeOffset)
                    continue;

                const uint freeEnd = freeBlock.myByteOffset + freeBlock.myByteSize;

                // Find the leftmost allocation that starts after this free block and fits in it
                int  candidateAllocIdx    = -1;
                uint candidateAllocOffset = UINT_MAX;

                for (int allocationIndex = 0; allocationIndex < myAllocatedBlocks.size(); ++allocationIndex)
                {
                    const AllocatedBlock& allocation = myAllocatedBlocks[allocationIndex];
                    if (allocation.myByteOffset < freeEnd)
                        continue; // allocation starts inside or before this free block

                    const uint alignedDst = AlignUp(freeBlock.myByteOffset, allocation.myAlignment);
                    if (alignedDst + allocation.myByteSize > allocation.myByteOffset) // would overlap src
                        continue;
                    if (alignedDst + allocation.myByteSize > freeEnd) // doesn't fit in free block
                        continue;
                    if (allocation.myByteOffset < candidateAllocOffset)
                    {
                        candidateAllocOffset = allocation.myByteOffset;
                        candidateAllocIdx    = allocationIndex;
                    }
                }

                if (candidateAllocIdx != -1)
                {
                    bestFreeIdx    = freeBlockIndex;
                    bestAllocIdx   = candidateAllocIdx;
                    bestFreeOffset = freeBlock.myByteOffset;
                }
            }

            if (bestFreeIdx == -1)
                break; // No valid move found

            const FreeBlock      freeBlock  = myFreeBlocks[bestFreeIdx];      // copy before mutation
            const AllocatedBlock allocBlock = myAllocatedBlocks[bestAllocIdx]; // copy before mutation
            const uint srcOffset = allocBlock.myByteOffset;
            const uint dstOffset = AlignUp(freeBlock.myByteOffset, allocBlock.myAlignment);
            const uint moveSize  = allocBlock.myByteSize;
            const uint sparseIdx = allocBlock.mySparseIndex;

            // GPU: copy dense data into the free block (non-overlapping guaranteed)
            myDenseBuffer->MoveData(srcOffset, dstOffset, moveSize);

            // Update the sparse entry so the handle still resolves correctly
            SparseEntryType& entry = mySparseEntries_CPU[sparseIdx];
            entry.myByteOffset = dstOffset;
            mySparseBuffer->SetDataAtIndex(entry, sparseIdx);

            // Remove the consumed free block (swap-and-pop is fine — we re-search next iteration)
            myFreeBlocks.RemoveIndex(bestFreeIdx);

            // Alignment padding before dstOffset stays free
            if (dstOffset > freeBlock.myByteOffset)
                InsertFreeBlock({ freeBlock.myByteOffset, dstOffset - freeBlock.myByteOffset });

            // Tail of the free block after the move stays free
            const uint freeEnd   = freeBlock.myByteOffset + freeBlock.myByteSize;
            const uint remainder = freeEnd - (dstOffset + moveSize);
            if (remainder > 0)
                InsertFreeBlock({ dstOffset + moveSize, remainder });

            // The source location is now free
            InsertFreeBlock({ srcOffset, moveSize });

            // Update the allocated block's offset in place (no sort needed — list is unsorted)
            myAllocatedBlocks[bestAllocIdx].myByteOffset = dstOffset;

            ++movesDone;
        }

        return movesDone;
    }

    // CPU-side read of a sparse entry (e.g. for firstVertex / firstIndex in draw calls)
    const SparseEntryType& GetSparseEntry(const Handle inHandle) const
    {
        return mySparseEntries_CPU[inHandle.mySparseIndex];
    }

    // Total bytes currently reserved in the dense buffer (high-water mark)
    uint GetUsedByteSize() const { return myUsedByteSize; }

    // Number of fragmented free regions (useful for deciding when to run Defrag)
    uint GetFreeBlockCount() const { return static_cast<uint>(myFreeBlocks.size()); }

private:
    struct FreeBlock
    {
        uint myByteOffset;
        uint myByteSize;
    };

    struct AllocatedBlock
    {
        uint myByteOffset;
        uint myByteSize;
        uint mySparseIndex;
        uint myAlignment;
    };

    /*
     * Finds a best-fit free block or appends at the high-water mark.
     * Updates myUsedByteSize if growing. Returns the byte offset for the allocation.
     * The actual dense buffer capacity is grown lazily inside the subsequent SetData call.
     */
    uint FindOrGrowDense(const uint inByteSize, const uint inAlignment)
    {
        int  bestIdx   = -1;
        uint bestWaste = UINT_MAX;

        for (int i = 0; i < static_cast<int>(myFreeBlocks.size()); ++i)
        {
            const FreeBlock& block       = myFreeBlocks[i];
            const uint       alignedStart = AlignUp(block.myByteOffset, inAlignment);
            const uint       blockEnd    = block.myByteOffset + block.myByteSize;

            if (alignedStart + inByteSize <= blockEnd)
            {
                const uint waste = blockEnd - (alignedStart + inByteSize);
                if (waste < bestWaste)
                {
                    bestWaste = waste;
                    bestIdx   = i;
                }
            }
        }

        if (bestIdx != -1)
        {
            const FreeBlock block = myFreeBlocks[bestIdx];
            myFreeBlocks.RemoveIndex(bestIdx);

            const uint alignedStart = AlignUp(block.myByteOffset, inAlignment);
            const uint blockEnd     = block.myByteOffset + block.myByteSize;

            // Padding before aligned start goes back to the free pool
            if (alignedStart > block.myByteOffset)
                InsertFreeBlock({ block.myByteOffset, alignedStart - block.myByteOffset });

            // Tail after the allocation goes back to the free pool
            const uint tail = blockEnd - (alignedStart + inByteSize);
            if (tail > 0)
                InsertFreeBlock({ alignedStart + inByteSize, tail });

            return alignedStart;
        }

        // No suitable free block — grow at the high-water mark.
        // Any padding bytes skipped for alignment are returned to the free pool.
        const uint alignedStart = AlignUp(myUsedByteSize, inAlignment);
        if (alignedStart > myUsedByteSize)
            InsertFreeBlock({ myUsedByteSize, alignedStart - myUsedByteSize });
        myUsedByteSize = alignedStart + inByteSize;
        return alignedStart;
    }

    // Inserts a free block and coalesces it with any touching neighbors.
    // The list is unsorted; coalescing uses a linear scan (at most one merge per side).
    void InsertFreeBlock(const FreeBlock& inBlock)
    {
        uint offset = inBlock.myByteOffset;
        uint size   = inBlock.myByteSize;

        // Coalesce with a left neighbor (a block whose end touches our start)
        for (int i = 0; i < static_cast<int>(myFreeBlocks.size()); ++i)
        {
            if (myFreeBlocks[i].myByteOffset + myFreeBlocks[i].myByteSize == offset)
            {
                offset = myFreeBlocks[i].myByteOffset;
                size  += myFreeBlocks[i].myByteSize;
                myFreeBlocks.RemoveIndex(i);
                break;
            }
        }

        // Coalesce with a right neighbor (a block whose start touches our end)
        for (int i = 0; i < static_cast<int>(myFreeBlocks.size()); ++i)
        {
            if (myFreeBlocks[i].myByteOffset == offset + size)
            {
                size += myFreeBlocks[i].myByteSize;
                myFreeBlocks.RemoveIndex(i);
                break;
            }
        }

        myFreeBlocks.Add({ offset, size });
    }

    static uint AlignUp(const uint inValue, const uint inAlignment)
    {
        return (inValue + inAlignment - 1) & ~(inAlignment - 1);
    }

    // GPU buffers
    ResizableBuffer*         myDenseBuffer  = nullptr;  // raw byte data (vertex/index)
    GPUList<SparseEntryType>* mySparseBuffer = nullptr; // stable-index sparse table

    // CPU-side state
    List<SparseEntryType> mySparseEntries_CPU;  // CPU mirror, indexed by sparse index
    List<uint>            myFreeSparseIndices;  // recycled sparse slots
    List<AllocatedBlock>  myAllocatedBlocks;    // unsorted; linear-scanned in Defrag
    List<FreeBlock>       myFreeBlocks;         // unsorted; coalesced on insert

    uint myUsedByteSize = 0; // high-water mark for the dense buffer
};