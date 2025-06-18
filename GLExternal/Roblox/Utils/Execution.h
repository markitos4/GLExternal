#pragma once

#include "../Instance/Instance.hpp"
#include "Compressing.h"


static void Execute(std::string Source) {
    auto DataModel = Instance()->GetDataModel();
    if (!DataModel.Valid(true))
    {
        Logger()->Error("Error getting DataModel on execution!");
        Logger()->WaitAndClose();
    }

    //Logger()->Debug("DataModel: " + std::to_string(DataModel.Self));

    auto ScriptContext = DataModel.FindFirstChildOfClass("ScriptContext");
    if (!ScriptContext.Valid(false))
    {
        Logger()->Error("Error getting ScriptContext!");
        Logger()->WaitAndClose();
    }

    //Logger()->Debug("ScriptContext: " + std::to_string(ScriptContext.Self));

    auto CoreGui = DataModel.FindFirstChildOfClass("CoreGui");
    if (!CoreGui.Valid(true))
    {
        Logger()->Error("Error getting CoreGui on execution!");
        Logger()->WaitAndClose();
    }

    //Logger()->Debug("CoreGui: " + std::to_string(CoreGui.Self));

    auto ScriptsHolder = CoreGui.FindFirstChild("GLExternal_Holder").ObjectValue();
    if (!ScriptsHolder.Valid(true))
    {
        Logger()->Error("Error getting ScriptsHolder on execution!");
        Logger()->WaitAndClose();
    }

    ScriptsHolder.SetBytecode(LuauCompress::ZstdCompress(LuauCompress::Compile("return {Arctic = function(...)\n" + Source + "\nend}")));

    ScriptContext.BypassModules();

    Sleep(500);

    ScriptContext.RestoreModules();
}
