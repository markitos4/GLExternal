#include "Definitions.hpp"

NtUnlockVirtualMemory_t* NtUnlockVirtualMemory;
NtLockVirtualMemory_t* NtLockVirtualMemory;
NtSuspendProcess_t* NtSuspendProcess;
NtResumeProcess_t* NtResumeProcess;
NtReadVirtualMemory_t* NtReadVirtualMemory;
NtWriteVirtualMemory_t* NtWriteVirtualMemory;

void InitializeDll(HMODULE Ntdll) {
    NtUnlockVirtualMemory = reinterpret_cast<NtUnlockVirtualMemory_t*>(GetProcAddress(Ntdll, "NtUnlockVirtualMemory"));
    NtLockVirtualMemory = reinterpret_cast<NtLockVirtualMemory_t*>(GetProcAddress(Ntdll, "NtLockVirtualMemory"));
    NtSuspendProcess = reinterpret_cast<NtSuspendProcess_t*>(GetProcAddress(Ntdll, "NtSuspendProcess"));
    NtResumeProcess = reinterpret_cast<NtResumeProcess_t*>(GetProcAddress(Ntdll, "NtResumeProcess"));
    NtReadVirtualMemory = reinterpret_cast<NtReadVirtualMemory_t*>(GetProcAddress(Ntdll, "NtReadVirtualMemory"));
    NtWriteVirtualMemory = reinterpret_cast<NtWriteVirtualMemory_t*>(GetProcAddress(Ntdll, "NtWriteVirtualMemory"));
}
