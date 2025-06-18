#include "../../Includes.h"

class RobloxInstance {
public:
	uintptr_t Self;

	bool Valid(bool SkipCheck);
	RobloxInstance GetDataModel();
	std::vector<RobloxInstance> GetChildren();
	std::string Name();
	RobloxInstance FindFirstChild(std::string InstanceName);
	RobloxInstance FindFirstChildOfClass(std::string InstanceClass);
	void SetBytecode(const std::string& Bytecode);
	std::string ClassName();
	void BypassDetections();
};

static auto Instance = std::make_unique<RobloxInstance>;
