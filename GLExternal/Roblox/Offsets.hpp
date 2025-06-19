#pragma once

#include "../Includes.h"

extern uintptr_t HyperionBase;

namespace Offsets
{
	static uintptr_t Name = 0x78;
	static uintptr_t Children = 0x80;
	static uintptr_t ClassDescriptor = 0x18;
	static uintptr_t Size = 0x8;

	static uintptr_t RequireBypass = 0x6E0;

	static uintptr_t IsCoreScript = 0x1a0;
	static uintptr_t ModuleDetection = 0x1a0 - 0x4;

	static uintptr_t FakeDataModelPointer = 0x675AA38;
	static uintptr_t FakeDataModelToDataModel = 0x1B8;

	static uintptr_t ModuleScriptByteCode = 0x158;

	static uintptr_t ValueBase = 0xD8;
}