#pragma once
#include <string>

// Example cheat commands.
//
// Any static method tagged with META(Cheat) is automatically discovered through the reflection
// system and registered with the CVarSystem. It can then be invoked from the console by its method
// name, with the arguments parsed from the command line (supported types: bool, int, unsigned int,
// float, double and std::string).
//
// Replace these with your own gameplay/debug cheats - the mechanism is driven entirely by the tag.
class CheatCommands
{
public:
    // > GodMode true
    META(Cheat)
    static void GodMode(bool enabled);

    // > NoClip
    META(Cheat)
    static void NoClip();

    // > SetTimeScale 0.25
    META(Cheat)
    static void SetTimeScale(float scale);

    // > GiveItem sword 3
    META(Cheat)
    static void GiveItem(std::string item, int count);
};
