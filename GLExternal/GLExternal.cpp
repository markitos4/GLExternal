#include "Includes.h"

#include "Driver/Driver.hpp"
#include "Roblox/Processes.hpp"
#include "Roblox/Offsets.hpp"
#include "Roblox/Utils/Execution.h"
#include "Dependencies/EnvironmentInclude.h"
#include "Roblox/Communication/Communication.hpp"

HWND ReturnedHwnd = NULL;
BOOL CALLBACK WindowsProcess(HWND hwnd, LPARAM lParam) {
    DWORD windowProcessId;
    GetWindowThreadProcessId(hwnd, &windowProcessId);
    if (windowProcessId == (DWORD)lParam) {
        ReturnedHwnd = hwnd;
        return FALSE;
    }
    return TRUE;
}

HWND GetHandleFromProcessId(DWORD processId) {
    EnumWindows(WindowsProcess, processId);
    return ReturnedHwnd;
}

std::string Alternative = R"(
    warn('Error getting Environment.lua!')
)";

HMODULE GetResourceModule() {
    HMODULE Module;
    GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)GetResourceModule, &Module);
    return Module;
}

uintptr_t HyperionBase;

int main()
{
    system("cls");

    SetConsoleTitleA("GLExecutor: Injecting...");

    DWORD RobloxProcessId = Processes()->GetRobloxPID();
    while (!RobloxProcessId)
    {
        system("cls");
        Logger()->Error("Roblox isn't open!");
        RobloxProcessId = Processes()->GetRobloxPID();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    HWND RobloxHWND;

    while (!(RobloxHWND = GetHandleFromProcessId(RobloxProcessId))) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    Logger()->Debug("Roblox PID: " + std::to_string(RobloxProcessId));

    uintptr_t HyperionAddress = Processes()->GetHyperionBase(RobloxProcessId);
    if (!HyperionAddress)
    {
        Logger()->Error("Error getting Roblox hyperion base!");
        Logger()->WaitAndClose();
        return 0;
    }

    HyperionBase = HyperionAddress;

    Logger()->Debug("Roblox Hyperion Address: " + std::to_string(HyperionAddress));

    Driver->Initialize(RobloxProcessId);

    while (!Driver->GetRobloxHandle()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    {
        auto DataModel = Instance()->GetDataModel();
        if (!DataModel.Valid(false))
        {
            Logger()->Error("Error getting DataModel!");
            Logger()->WaitAndClose();
            return 0;
        }

        Logger()->Debug("DataModel: " + std::to_string(DataModel.Self)); // Self is the address of the Instance.

        auto ScriptContext = DataModel.FindFirstChildOfClass("ScriptContext");
        if (!ScriptContext.Valid(false))
        {
            Logger()->Error("Error getting ScriptContext!");
            Logger()->WaitAndClose();
            return 0;
        }

        Logger()->Debug("ScriptContext: " + std::to_string(ScriptContext.Self));

        ScriptContext.RemoveContextDetections();

        auto StarterPlayer = DataModel.FindFirstChildOfClass("StarterPlayer");
        if (!StarterPlayer.Valid(false))
        {
            Logger()->Error("Error getting StarterPlayer!");
            Logger()->WaitAndClose();
            return 0;
        }

        Logger()->Debug("StarterPlayer: " + std::to_string(StarterPlayer.Self));

        auto StarterPlayerScripts = StarterPlayer.FindFirstChildOfClass("StarterPlayerScripts");
        if (!StarterPlayerScripts.Valid(false))
        {
            Logger()->Error("Error getting StarterPlayerScripts!");
            Logger()->WaitAndClose();
            return 0;
        }

        Logger()->Debug("StarterPlayerScripts: " + std::to_string(StarterPlayerScripts.Self));

        auto PlayerModule = StarterPlayerScripts.FindFirstChild("PlayerModule");
        if (!PlayerModule.Valid(false))
        {
            Logger()->Error("Error getting PlayerModule!");
            Logger()->WaitAndClose();
            return 0;
        }

        Logger()->Debug("PlayerModule: " + std::to_string(PlayerModule.Self));

        auto ControlModule = PlayerModule.FindFirstChild("ControlModule");
        if (!ControlModule.Valid(false))
        {
            Logger()->Error("Error getting ControlModule!");
            Logger()->WaitAndClose();
            return 0;
        }

        Logger()->Debug("ControlModule: " + std::to_string(ControlModule.Self));

        auto VRNavigation = ControlModule.FindFirstChild("VRNavigation");
        if (!VRNavigation.Valid(false))
        {
            Logger()->Error("Error getting VRNavigation!");
            Logger()->WaitAndClose();
            return 0;
        }

        Logger()->Debug("VRNavigation: " + std::to_string(VRNavigation.Self));

        auto CoreGui = DataModel.FindFirstChildOfClass("CoreGui");
        if (!CoreGui.Valid(false))
        {
            Logger()->Error("Error getting CoreGui!");
            Logger()->WaitAndClose();
            return 0;
        }

        Logger()->Debug("CoreGui: " + std::to_string(CoreGui.Self));

        auto RobloxGui = CoreGui.FindFirstChild("RobloxGui");
        if (!RobloxGui.Valid(false))
        {
            Logger()->Error("Error getting RobloxGui!");
            Logger()->WaitAndClose();
            return 0;
        }

        Logger()->Debug("RobloxGui: " + std::to_string(RobloxGui.Self));

        auto Modules = RobloxGui.FindFirstChild("Modules");
        if (!Modules.Valid(false))
        {
            Logger()->Error("Error getting Modules!");
            Logger()->WaitAndClose();
            return 0;
        }

        Logger()->Debug("Modules: " + std::to_string(Modules.Self));

        auto PlayerList = Modules.FindFirstChild("PlayerList");
        if (!PlayerList.Valid(false))
        {
            Logger()->Error("Error getting PlayerList!");
            Logger()->WaitAndClose();
            return 0;
        }

        Logger()->Debug("PlayerList: " + std::to_string(PlayerList.Self));

        auto PlayerListManager = PlayerList.FindFirstChild("PlayerListManager");
        if (!PlayerListManager.Valid(false))
        {
            Logger()->Error("Error getting PlayerListManager!");
            Logger()->WaitAndClose();
            return 0;
        }

        Logger()->Debug("PlayerListManager: " + std::to_string(PlayerListManager.Self));

        Driver->WritePointer<uintptr_t>(PlayerListManager.Self + Offsets::Size, VRNavigation.Self);

        HMODULE Module = GetResourceModule();
        if (Module == NULL)
        {
            Logger()->Error("Error relocating Init module!");
            Logger()->WaitAndClose();
            return 0;
        }

        HRSRC Resource = FindResourceW(Module, MAKEINTRESOURCE(Lua), MAKEINTRESOURCE(Init));
        if (Resource == NULL)
        {
            Logger()->Error("Error relocating Init module-2!");
            Logger()->WaitAndClose();
            return 0;
        }

        HGLOBAL Data = LoadResource(Module, Resource);
        if (Data == NULL)
        {
            Logger()->Error("Error relocating Init module-3!");
            Logger()->WaitAndClose();
            return 0;
        }

        void* LockedData = LockResource(Data);
        if (LockedData == NULL)
        {
            Logger()->Error("Error relocating Init module-4!");
            Logger()->WaitAndClose();
            return 0;
        }

        // This lua code will avoid an error that roblox throws as the new vrnavigation's bytecode doesn't returns anything
        VRNavigation.SetBytecode(LuauCompress::ZstdCompress(LuauCompress::Compile("getfenv(0)['script'] = nil coroutine['wrap'](function(...)" + std::string(static_cast<char*>(LockedData), SizeofResource(Module, Resource)) + "\nend)() while task['wait'](9e9) do task['wait'](9e9) end")));

        while (GetForegroundWindow() != RobloxHWND) { // Roblox could kick you sometimes if u dont implement this (yes it happened to me)
            SetForegroundWindow(RobloxHWND);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        SendMessageW(RobloxHWND, WM_CLOSE, NULL, NULL);

        Sleep(100);

        Driver->WritePointer<uintptr_t>(PlayerListManager.Self + Offsets::Size, PlayerListManager.Self);

        ScriptContext.RestoreContextDetections();

        Bridge()->Initialize();

        system("cls");

        SetConsoleTitleA("GLExecutor: Injected!");

        Logger()->Debug("Ready to use GLExecutor!");

        Execute("warn('I will add loadstring and more bridge functions on next update!')");
    }

    while (Processes()->GetRobloxPID()) {
        Sleep(5000);
    }

    Logger()->WaitAndClose();

    return 0;
}
