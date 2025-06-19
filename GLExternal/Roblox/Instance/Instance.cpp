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
    for (std::int32_t Index = 0; Code = Driver->ReadPointer<std::uint8_t>(Address + Index); Index++)
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
    if (!ChildrenPointer.Valid(false))
        return ChildrenList;

    auto ChildrenStart = Driver->ReadPointer<RobloxInstance>(ChildrenPointer.Self);
    if (!ChildrenStart.Valid(false))
        return ChildrenList;

    auto ChildrenEnd = Driver->ReadPointer<RobloxInstance>(ChildrenPointer.Self + Offsets::Size);
    if (!ChildrenEnd.Valid(false))
        return ChildrenList;

    for (uintptr_t FetchedAddress = ChildrenStart.Self; FetchedAddress < ChildrenEnd.Self; FetchedAddress += 16)
    {
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
    if (!this->Valid(true))
        Logger()->Error("Invalid address on Name");

    const auto RealName = Driver->ReadPointer<uintptr_t>(this->Self + Offsets::Name);
    return RealName ? ConvertString(RealName) : "None";
}

std::string RobloxInstance::ClassName()
{
    if (!this->Valid(true))
        Logger()->Error("Invalid address on ClassName");

    auto ClassDescriptor = Driver->ReadPointer<uint64_t>(this->Self + Offsets::ClassDescriptor);
    auto ClassName = Driver->ReadPointer<uint64_t>(ClassDescriptor + Offsets::Size);

    if (ClassName)
        return ConvertString(ClassName);

    return "Nonee";
}

RobloxInstance RobloxInstance::FindFirstChild(std::string InstanceName)
{
    if (!this->Valid(true))
        Logger()->Error("Invalid address on FindFirstChild");

    for (auto& FetchedObject : this->GetChildren())
    {
        if (FetchedObject.Name() == InstanceName)
        {
            return static_cast<RobloxInstance>(FetchedObject);
        }
    }
    return RobloxInstance(0);
}

RobloxInstance RobloxInstance::WaitForChild(std::string InstanceName, int Maxtime) {
    RobloxInstance Found = FindFirstChild(InstanceName);
    if (Found.Valid(false))
        return Found;

    std::chrono::steady_clock::time_point Start = std::chrono::high_resolution_clock::now();

    auto Timeout = std::chrono::seconds(Maxtime);

    while (std::chrono::high_resolution_clock::now() - Start <= Timeout) {
        if (FindFirstChild(InstanceName).Valid(false))
            return FindFirstChild(InstanceName);
        Sleep(100);
    }

    return RobloxInstance(0);
}

RobloxInstance RobloxInstance::FindFirstChildOfClass(std::string InstanceClass)
{
    if (!this->Valid(true))
        Logger()->Error("Invalid address on FindFirstChildOfClass");

    for (auto& FetchedObject : this->GetChildren())
    {
        if (FetchedObject.ClassName() == InstanceClass)
        {
            return static_cast<RobloxInstance>(FetchedObject);
        }
    }
    return RobloxInstance(0);
}

void RobloxInstance::SetBytecode(std::string Bytecode) { 
    if (!this->Valid(true))
        Logger()->Error("Invalid address on SetBytecode");

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

        uintptr_t NewBytecode = OldBytecode;

        for (size_t Bytes = 0; Bytes < Bytecode.size(); Bytes++) {
            Driver->WritePointer<BYTE>(OldBytecode + Bytes, static_cast<BYTE>(Bytecode[Bytes]));
        }

        Driver->WritePointer<uintptr_t>(ModuleBytecodePointer + 0x20, Bytecode.size());
    }
}

void RobloxInstance::RemoveCoreDetections()
{
    if (!this->Valid(true))
        Logger()->Error("Invalid address on RemoveCoreDetections");

    Driver->WritePointer(this->Self + Offsets::ModuleDetection, 0x100000000);
    Driver->WritePointer(this->Self + Offsets::IsCoreScript, 0x1);
}

RobloxInstance RobloxInstance::ObjectValue()
{
    return Driver->ReadPointer<RobloxInstance>(this->Self + Offsets::ValueBase);
}
