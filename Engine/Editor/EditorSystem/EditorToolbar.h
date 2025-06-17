#pragma once
#include "EditorSystem.h"

class Method;

class EditorToolbar : public EditorSystem
{
public:
    EditorToolbar();
    
    void Tick() override;

private:
    void RenderMultipleParts(const Method* inMethod, const List<std::string>& inPathParts);
    void RenderSinglePart(const Method* inMethod, const List<std::string>& inPathParts);
    

private:
    List<const Method*> myToolbarMethods{};
};
