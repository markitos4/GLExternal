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
