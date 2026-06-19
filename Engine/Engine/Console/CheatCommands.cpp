#include "EnginePch.h"
#include "Console/CheatCommands.h"

#include "Core/Console.h"
#include "Core/Time.h"

namespace
{
    bool ourGodModeEnabled = false;
    bool ourNoClipEnabled = false;
}

void CheatCommands::GodMode(bool enabled)
{
    ourGodModeEnabled = enabled;
    LOG("Cheat: god mode %s.", enabled ? "enabled" : "disabled");
}

void CheatCommands::NoClip()
{
    ourNoClipEnabled = !ourNoClipEnabled;
    LOG("Cheat: noclip %s.", ourNoClipEnabled ? "enabled" : "disabled");
}

void CheatCommands::SetTimeScale(float scale)
{
    Time::SetTimeScale(scale);
    LOG("Cheat: time scale set to %.3f.", scale);
}

void CheatCommands::GiveItem(std::string item, int count)
{
    LOG("Cheat: gave %d x '%s'.", count, item.c_str());
}
