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

    auto CoreGui = DataModel.FindFirstChildOfClass("CoreGui");
    if (!CoreGui.Valid(true))
    {
        Logger()->Error("Error getting CoreGui on execution!");
        Logger()->WaitAndClose();
    }

    auto ScriptsHolder = CoreGui.WaitForChild("GLExternal_Holder");
    if (!ScriptsHolder.Valid(true))
    {
        Logger()->Error("Error getting ScriptsHolder on execution!");
        Logger()->WaitAndClose();
    }

    auto HolderValue = ScriptsHolder.ObjectValue();
    while (!HolderValue.Valid(true))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
        HolderValue = ScriptsHolder.ObjectValue();
    }

    HolderValue.SetBytecode(LuauCompress::ZstdCompress(LuauCompress::Compile("return {['GL-Execution'] = function(...)\n" + Source + "\nend}")));

    HolderValue.RemoveCoreDetections(); // We dont use 'RemoveContextDetections' because we have to restore it 
    // or else the modules will be required consistly and they would throw error, so this is way more easy lol
}
