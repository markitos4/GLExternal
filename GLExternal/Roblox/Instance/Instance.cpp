#include "Instance.hpp"

#include "../../Driver/Driver.hpp"
#include "../Offsets.hpp"

bool RobloxInstance::Valid(bool SkipCheck) {
    if (!this->Self) {
        return false;
    }

    if (this->Self <= 1000 || this->Self >= 0x7FFFFFFFFFFF) {
        return false;
    }

    if (!SkipCheck)
    {
        return Driver->IsMemoryValid(this->Self);
    }

    return true;
};

std::string ConvertString(uintptr_t Address)
{
    const auto Size = Driver->ReadPointer<size_t>(Address + 0x10);

    if (Size >= 16)
        Address = Driver->ReadPointer<uintptr_t>(Address);

    std::string FinalString;
    BYTE Code = 0;
    for (std::int32_t i = 0; Code = Driver->ReadPointer<std::uint8_t>(Address + i); i++)
        FinalString.push_back(Code);

    return FinalString;
}

RobloxInstance RobloxInstance::GetDataModel()
{
    auto FakeDataModelPointer = Driver->ReadPointer<uintptr_t>(HyperionBase + Offsets::FakeDataModelPointer);
    if (!FakeDataModelPointer)
        Logger()->Error("Error getting DataModel-1");

    auto RealDataModel = Driver->ReadPointer<RobloxInstance>(FakeDataModelPointer + Offsets::FakeDataModelToDataModel);
    if (!RealDataModel.Valid(false))
        Logger()->Error("Error getting DataModel-2");

    return RealDataModel;
};


std::vector<RobloxInstance> RobloxInstance::GetChildren()
{
    std::vector<RobloxInstance> ChildrenList;
    auto ChildrenPointer = Driver->ReadPointer<RobloxInstance>(this->Self + Offsets::Children);
    if (!ChildrenPointer.Valid(true))
        return std::vector<RobloxInstance>(0);

    auto ChildrenStart = Driver->ReadPointer<RobloxInstance>(ChildrenPointer.Self);
    if (!ChildrenStart.Valid(true))
        return std::vector<RobloxInstance>(0);

    auto ChildrenEnd = Driver->ReadPointer<RobloxInstance>(ChildrenPointer.Self + Offsets::Size);
    if (!ChildrenEnd.Valid(true))
        return std::vector<RobloxInstance>(0);

    for (uintptr_t FetchedAddress = ChildrenStart.Self; FetchedAddress < ChildrenEnd.Self + 1; FetchedAddress += 16) {
        RobloxInstance FetchedPointer = Driver->ReadPointer<RobloxInstance>(FetchedAddress);
        if (FetchedPointer.Valid(true))
        {
            ChildrenList.push_back(FetchedPointer);
        }
    }

    return ChildrenList;
}

std::string RobloxInstance::Name()
{
    if (!this->Self)
        Logger()->Error("Invalid address?");

    const auto RealName = Driver->ReadPointer<uintptr_t>(this->Self + Offsets::Name);
    return RealName ? ConvertString(RealName) : "None";
}

std::string RobloxInstance::ClassName()
{
    auto ClassDescriptor = Driver->ReadPointer<uint64_t>(this->Self + Offsets::ClassDescriptor);
    auto ClassName = Driver->ReadPointer<uint64_t>(ClassDescriptor + Offsets::Size);

    if (ClassName)
        return ConvertString(ClassName);

    return "Nonee";
}

RobloxInstance RobloxInstance::FindFirstChild(std::string InstanceName)
{
    for (auto& FetchedObject : this->GetChildren())
    {
        if (FetchedObject.Name() == InstanceName)
        {
            return static_cast<RobloxInstance>(FetchedObject);
            break;
        }
    }
    return RobloxInstance(0);
}

RobloxInstance RobloxInstance::FindFirstChildOfClass(std::string InstanceClass)
{
    for (auto& FetchedObject : this->GetChildren())
    {
        if (FetchedObject.ClassName() == InstanceClass)
        {
            return static_cast<RobloxInstance>(FetchedObject);
            break;
        }
    }
    return RobloxInstance(0);
}

void RobloxInstance::SetBytecode(const std::string& Bytecode) { // Credits to xeno for the method
    if (this->ClassName() == "ModuleScript")
    {
        uintptr_t ModuleBytecodePointer = Driver->ReadPointer<uintptr_t>(this->Self + Offsets::ModuleScriptByteCode);

        uintptr_t OldBytecode = Driver->ReadPointer<uintptr_t>(ModuleBytecodePointer + 0x10);
        uintptr_t OldSize = Driver->ReadPointer<uintptr_t>(ModuleBytecodePointer + 0x20);


        std::thread([ModuleBytecodePointer, OldBytecode, OldSize]() { // This will reset the bytecode after the bytecode is replaced by the new one (used for no detections)
            Sleep(1000);
            Driver->WritePointer<uintptr_t>(ModuleBytecodePointer + 0x10, OldBytecode);
            Driver->WritePointer<uintptr_t>(ModuleBytecodePointer + 0x20, OldSize);
            return;
            }).detach();

        auto BytecodeAllocated = Driver->AllocateVirtualMemory(Bytecode.size(), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        if (BytecodeAllocated)
        {
            DWORD OldProtect;

            VirtualProtectEx(Driver->GetRobloxHandle(), BytecodeAllocated, sizeof(Bytecode.c_str()), PAGE_READWRITE, &OldProtect);
            NtWriteVirtualMemory(Driver->GetRobloxHandle(), BytecodeAllocated, (PVOID)Bytecode.c_str(), (ULONG)Bytecode.size(), nullptr);

            VirtualProtectEx(Driver->GetRobloxHandle(), BytecodeAllocated, sizeof(Bytecode.c_str()), OldProtect, (PDWORD)0);
            Driver->WritePointer<uintptr_t>(ModuleBytecodePointer + 0x10, reinterpret_cast<uintptr_t>(BytecodeAllocated));
            Driver->WritePointer<uintptr_t>(ModuleBytecodePointer + 0x20, Bytecode.size());
            return;
        }
    }
}

void RobloxInstance::BypassModules()
{
    Driver->WritePointer(this->Self + Offsets::ModuleDetection, 0x100000000);
    Driver->WritePointer(this->Self + Offsets::IsCoreScript, 0x1);
}

void RobloxInstance::RestoreModules()
{
    Driver->WritePointer(this->Self + Offsets::ModuleDetection, 0x000000000);
    Driver->WritePointer(this->Self + Offsets::IsCoreScript, 0x0);
}

RobloxInstance RobloxInstance::ObjectValue()
{
    return Driver->ReadPointer<RobloxInstance>(this->Self + Offsets::ValueBase);
}

bool RobloxInstance::BoolValue()
{
    return Driver->ReadPointer<bool>(this->Self + Offsets::ValueBase);
}