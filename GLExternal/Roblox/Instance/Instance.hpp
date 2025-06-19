#include "../../Includes.h"

class RobloxInstance {
public:
	uintptr_t Self;

	RobloxInstance GetDataModel();
	RobloxInstance FindFirstChild(std::string InstanceName);
	RobloxInstance FindFirstChildOfClass(std::string InstanceClass);
	RobloxInstance ObjectValue();
	RobloxInstance WaitForChild(std::string InstanceName, int Maxtime = 5);
	std::vector<RobloxInstance> GetChildren();
	std::string Name();
	std::string ClassName();
	void SetBytecode(std::string Bytecode);
	void RemoveCoreDetections();
	void RemoveContextDetections();
	void RestoreContextDetections();
	bool Valid(bool SkipCheck);
};

static auto Instance = std::make_unique<RobloxInstance>;
