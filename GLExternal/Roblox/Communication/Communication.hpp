#pragma once

#include "../Utils/Execution.h"

class CBridge
{
public:
	void Initialize();
};

static auto Bridge = std::make_unique<CBridge>;