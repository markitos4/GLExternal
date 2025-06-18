#include "../../Includes.h"

class RobloxInstance {
public:
	uintptr_t Self;

	RobloxInstance GetDataModel();
	RobloxInstance FindFirstChild(std::string InstanceName);
	RobloxInstance FindFirstChildOfClass(std::string InstanceClass);
	RobloxInstance ObjectValue();
	bool BoolValue();
	std::vector<RobloxInstance> GetChildren();
	std::string Name();
	std::string ClassName();
	void SetBytecode(const std::string& Bytecode);
	void BypassModules();
	void RestoreModules();
	bool Valid(bool SkipCheck);
};

static auto Instance = std::make_unique<RobloxInstance>;
