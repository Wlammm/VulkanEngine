#pragma once


/*
 * BufferType type;
 * type.myMemberVariable = 3;
 * RenderGraph::UpdateBuffer<BufferType>(someBufferData);
 * 
 * VulkanBuffer* buffer = RenderGraph::GetBuffer("SomeBufferTypeName"); // To bind, we also need a resizable version...
 */

class RenderGraph
{
public:
    template<typename BufferType>
    static void RegisterBuffer()
    {
        
    }

private:
    
};
