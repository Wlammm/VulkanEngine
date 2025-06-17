#pragma once

class EditorSystem
{
public:
    virtual void Tick() = 0;

private:
    friend class Editor;
    int myID = 0;
};
