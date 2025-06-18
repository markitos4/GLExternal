#include "../Includes.h"
#include "Definitions.hpp"

class CDriver
{
private:
    HANDLE RobloxHandle = nullptr;
    HMODULE NtdllHandle;
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
        T value = {};

        if (!RobloxHandle) {
            Logger()->Error("Roblox handle not initialized!");
            return value;
        }

        for (auto Try = 0; Try < 6;)
        {
            if (!this->IsMemoryValid(Pointer))
            {
                Sleep(20);
                Try++;
            }
            else
            {
                break;
            }
        }

        MEMORY_BASIC_INFORMATION MemoryInfo;

        VirtualQueryEx(RobloxHandle, reinterpret_cast<LPCVOID>(Pointer), &MemoryInfo, sizeof(MemoryInfo));

        NtReadVirtualMemory(RobloxHandle, reinterpret_cast<LPCVOID>(Pointer), &value, sizeof(value), nullptr);

        NtUnlockVirtualMemory(RobloxHandle, &MemoryInfo.AllocationBase, &MemoryInfo.RegionSize, 1);

        return value;
    }

    template <class T>
    void WritePointer(const uintptr_t Pointer, const T& NewPointer)
    {
        if (!RobloxHandle) {
            return;
        }

        for (auto Try = 0; Try < 6;)
        {
            if (!this->IsMemoryValid(Pointer))
            {
                Sleep(20);
                Try++;
            }
            else
            {
                break;
            }
        }

        DWORD P;

        VirtualProtectEx(RobloxHandle, reinterpret_cast<LPVOID>(Pointer), sizeof(NewPointer), PAGE_READWRITE, &P);

        NtWriteVirtualMemory(RobloxHandle, reinterpret_cast<PVOID>(Pointer), (PVOID)&NewPointer, sizeof(NewPointer), 0);

        VirtualProtectEx(RobloxHandle, reinterpret_cast<LPVOID>(Pointer), sizeof(NewPointer), P, 0);
    }
};

inline auto Driver = std::make_unique<CDriver>();
