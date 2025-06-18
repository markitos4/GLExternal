#pragma once

#include "../Includes.h"

class CProcesses 
{
public:
	DWORD GetRobloxPID();
	uintptr_t GetHyperionBase(DWORD RobloxProcessId);
};

static auto Processes = std::make_unique<CProcesses>;