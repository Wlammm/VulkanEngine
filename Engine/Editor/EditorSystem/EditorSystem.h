#pragma once

class EditorSystem
{
public:
    virtual ~EditorSystem() = default;
    virtual void Tick() = 0;

private:
    friend class Editor;
    int myID = 0;
};
