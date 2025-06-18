#pragma once

#include <Windows.h>

typedef DWORD(NtUnlockVirtualMemory_t)(HANDLE ProcessHandle, PVOID* BaseAddress, PSIZE_T NumberOfBytesToUnlock, ULONG LockType);
typedef DWORD(NtLockVirtualMemory_t)(HANDLE ProcessHandle, PVOID* BaseAddress, PSIZE_T NumberOfBytesToLock, ULONG LockOption);
typedef DWORD(NtSuspendProcess_t)(HANDLE ProcessHandle);
typedef DWORD(NtResumeProcess_t)(HANDLE ProcessHandle);
typedef DWORD(NtReadVirtualMemory_t)(HANDLE ProcessHandle, LPCVOID BaseAddress, PVOID Buffer, ULONG NumberOfBytesToRead, PSIZE_T NumberOfBytesRead);
typedef DWORD(NtWriteVirtualMemory_t)(HANDLE ProcessHandle, PVOID BaseAddress, PVOID Buffer, ULONG NumberOfBytesToWrite, PSIZE_T NumberOfBytesWritten);

extern NtUnlockVirtualMemory_t* NtUnlockVirtualMemory;
extern NtLockVirtualMemory_t* NtLockVirtualMemory;
extern NtSuspendProcess_t* NtSuspendProcess; // They have to be extern defs or else they will error
extern NtResumeProcess_t* NtResumeProcess;
extern NtReadVirtualMemory_t* NtReadVirtualMemory;
extern NtWriteVirtualMemory_t* NtWriteVirtualMemory;

void InitializeDll(HMODULE Ntdll);