#include "Driver.hpp"

void CDriver::Initialize(DWORD RobloxPid)
{
    if (this->RobloxHandle)
    {
        CloseHandle(this->RobloxHandle);
        this->RobloxHandle = nullptr;
    }

    if (RobloxPid != 0)
    {
        this->NtdllHandle = LoadLibraryA("ntdll.dll");
        if (!this->NtdllHandle) {
            return;
        }

        InitializeDll(this->NtdllHandle);

        HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, RobloxPid);
        if (!handle)
        {
            return;
        }
        this->RobloxHandle = handle;
    }
    else
    {
        return;
    }
}
