#pragma once

#include <string>

namespace DragonCore::ECS
{
	struct IdentificationComponent
	{
		std::string name{ "GameObject" }, group{ "" };
		uint32_t entityID;
	};
}