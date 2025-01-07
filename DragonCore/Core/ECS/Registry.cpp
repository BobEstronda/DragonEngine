#include "Registry.h"

namespace DragonCore::ECS
{
	Registry::Registry() : m_pRegistry{nullptr}
	{
		m_pRegistry = std::make_unique<entt::registry>();
	}
}
