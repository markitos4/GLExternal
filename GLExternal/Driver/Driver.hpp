#include "../Includes.h"

class CDriver
{
private:
    HANDLE RobloxHandle = nullptr;
    HMODULE NtdllHandle = nullptr;
public:
    void Initialize(DWORD pid);

    HMODULE GetNtdllHandle() const {
        return this->NtdllHandle;
    }

    HANDLE GetRobloxHandle() const {
        return this->RobloxHandle;
    }

    bool IsMemoryValid(uintptr_t Address)
    {
        MEMORY_BASIC_INFORMATION MemoryInfo;
        if (VirtualQuery((void*)Address, &MemoryInfo, sizeof(MEMORY_BASIC_INFORMATION)) == 0) {
            return false;
        }

        return true;
    }


    LPVOID AllocateVirtualMemory(size_t Size, DWORD Type = MEM_COMMIT | MEM_RESERVE, DWORD Protected = PAGE_READWRITE) {
        if (!RobloxHandle) {
            return 0;
        }

        auto AllocatedMemory = VirtualAllocEx(RobloxHandle, nullptr, Size, Type, Protected);

        return AllocatedMemory ? AllocatedMemory : 0;
    }

    template <class T>
    T ReadPointer(const uintptr_t Pointer)
    {
        T Value = {};
        if (!RobloxHandle) {
            return Value;
        }

        for (auto Attempts = 0; Attempts < 5;)
        {
            if (!this->IsMemoryValid(Pointer))
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                Attempts++;
            }
            else
            {
                break;
            }
        }

        SIZE_T Readed = 0;

        BOOL Success = ReadProcessMemory(RobloxHandle, reinterpret_cast<LPCVOID>(Pointer), &Value, sizeof(T), &Readed);
        if (!Success || Readed != sizeof(T)) {
            return Value;
        }

        return Value;
    }

    template <class T>
    void WritePointer(const uintptr_t OldPointer, const T& ReplacePointer)
    {
        if (!RobloxHandle) {
            return;
        }

        for (auto Attempts = 0; Attempts < 5;)
        {
            if (!this->IsMemoryValid(OldPointer))
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                Attempts++;
            }
            else
            {
                break;
            }
        }

        SIZE_T Written = 0;

        BOOL Success = WriteProcessMemory(RobloxHandle, reinterpret_cast<LPVOID>(OldPointer), &ReplacePointer, sizeof(T), &Written);
        if (!Success || Written != sizeof(T)) {
            return;
        }

        return;
    }
};

inline auto Driver = std::make_unique<CDriver>();
