#include "Includes.h"

#include "Driver/Driver.hpp"
#include "Roblox/Processes.hpp"
#include "Roblox/Offsets.hpp"
#include "Roblox/Instance/Instance.hpp"
#include "Luau/Compiler.h"
#include "Luau/BytecodeBuilder.h"
#include "Luau/BytecodeUtils.h"
#include "Zstd/xxhash.h"
#include "Zstd/zstd.h"
#include "Dependencies/EnvironmentInclude.h"

static std::string ZstdCompress(const std::string_view Bytecode)
{
    const auto MaxSize = ZSTD_compressBound(Bytecode.size());
    auto Buffer = std::vector<char>(MaxSize + 8);

    strcpy_s(&Buffer[0], Buffer.capacity(), "RSB1");

    const auto Size = Bytecode.size();

    memcpy_s(&Buffer[4], Buffer.capacity(), &Size, sizeof(Size));

    const auto compressed_size = ZSTD_compress(&Buffer[8], MaxSize, Bytecode.data(), Size, ZSTD_maxCLevel());
    if (ZSTD_isError(compressed_size))
        return "";

    const auto FinalSize = compressed_size + 8;
    const auto HashKey = XXH32(Buffer.data(), FinalSize, 42u);
    const auto Bytes = reinterpret_cast<const uint8_t*>(&HashKey);

    for (auto i = 0u; i < FinalSize; ++i)
        Buffer[i] ^= Bytes[i % 4] + i * 41u;

    return std::string(Buffer.data(), FinalSize);
}

class BytecodeEncoderClass : public Luau::BytecodeEncoder {
    inline void encode(uint32_t* data, size_t count) override {
        for (auto i = 0u; i < count;) {
            auto& opcode = *reinterpret_cast<uint8_t*>(data + i);
            i += Luau::getOpLength(LuauOpcode(opcode));
            opcode *= 227;
        }
    }
};

std::string Compile(const std::string& source)
{
    static BytecodeEncoderClass Encoder;
    return Luau::compile(source, {}, {}, &Encoder);
}

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

std::string GetInitScript() {
    HMODULE Module = GetResourceModule();
    if (Module == NULL)
    {
        Logger()->Error("Error relocating Init module!");
        return Alternative;
    }

    HRSRC Resource = FindResourceW(Module, MAKEINTRESOURCE(Lua), MAKEINTRESOURCE(Init));
    if (Resource == NULL)
    {
        Logger()->Error("Error relocating Init module-2!");
        return Alternative;
    }

    HGLOBAL Data = LoadResource(Module, Resource);
    if (Data == NULL)
    {
        Logger()->Error("Error relocating Init module-3!");
        return Alternative;
    }

    LockResource(Data);
    return std::string(static_cast<char*>(Data), SizeofResource(Module, Resource));
}

uintptr_t HyperionBase;

[[noreturn]] int main()
{
    system("cls");

    DWORD RobloxProcessId = Processes()->GetRobloxPID();
    while (!RobloxProcessId)
    {
        system("cls");
        Logger()->Error("Roblox isn't open!");
        RobloxProcessId = Processes()->GetRobloxPID();
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
    }

    HyperionBase = HyperionAddress;

    Logger()->Debug("Roblox Hyperion Address: " + std::to_string(HyperionAddress));

    Driver->Initialize(RobloxProcessId);

    while (!Driver->GetRobloxHandle()) {
        Sleep(100);
    }

    {
        auto DataModel = Instance()->GetDataModel();

        Logger()->Debug("DataModel: " + std::to_string(DataModel.Self)); // Self is the address of the Instance.

        auto ScriptContext = DataModel.FindFirstChildOfClass("ScriptContext");

        Logger()->Debug("ScriptContext: " + std::to_string(ScriptContext.Self));

        ScriptContext.BypassDetections();

        auto StarterPlayer = DataModel.FindFirstChildOfClass("StarterPlayer");

        Logger()->Debug("StarterPlayer: " + std::to_string(StarterPlayer.Self));

        auto StarterPlayerScripts = StarterPlayer.FindFirstChild("StarterPlayerScripts");

        Logger()->Debug("StarterPlayerScripts: " + std::to_string(StarterPlayerScripts.Self));

        auto PlayerModule = StarterPlayerScripts.FindFirstChild("PlayerModule");

        Logger()->Debug("PlayerModule: " + std::to_string(PlayerModule.Self));

        auto ControlModule = PlayerModule.FindFirstChild("ControlModule");

        Logger()->Debug("ControlModule: " + std::to_string(ControlModule.Self));

        auto VRNavigation = ControlModule.FindFirstChild("VRNavigation");

        Logger()->Debug("VRNavigation: " + std::to_string(VRNavigation.Self));

        auto CoreGui = DataModel.FindFirstChild("CoreGui");

        Logger()->Debug("CoreGui: " + std::to_string(CoreGui.Self));

        auto RobloxGui = CoreGui.FindFirstChild("RobloxGui");

        Logger()->Debug("RobloxGui: " + std::to_string(RobloxGui.Self));

        auto Modules = RobloxGui.FindFirstChild("Modules");

        Logger()->Debug("Modules: " + std::to_string(Modules.Self));

        auto PlayerList = Modules.FindFirstChild("PlayerList");

        Logger()->Debug("PlayerList: " + std::to_string(PlayerList.Self));

        auto PlayerListManager = PlayerList.FindFirstChild("PlayerListManager");

        Logger()->Debug("PlayerListManager: " + std::to_string(PlayerListManager.Self));

        PlayerListManager.BypassDetections();

        Driver->WritePointer<uintptr_t>(PlayerListManager.Self + 0x8, VRNavigation.Self);

        // This lua code will avoid an error that roblox throws as the new vrnavigation's bytecode doesn't returns anything
        VRNavigation.SetBytecode(ZstdCompress(Compile("getfenv(0)['script'].Parent = nil coroutine['wrap'](function(...)" + GetInitScript() + "\nend)() while task['wait'](9e9) do task['wait'](9e9) end")));

        SendMessageW(RobloxHWND, WM_CLOSE, NULL, NULL);

        Sleep(150);

        Driver->WritePointer<uintptr_t>(PlayerListManager.Self + 0x8, PlayerListManager.Self);

        Logger()->Debug("Execution will be added sooooooooooon!");
    }

    while (Processes()->GetRobloxPID()) {
        Sleep(5000);
    }

    exit(0);
}
