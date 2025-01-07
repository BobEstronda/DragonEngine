#include "Entity.h"

#include "Components/IdentificationComponent.h"

namespace DragonCore::ECS
{
	Entity::Entity(Registry& registry) : Entity(registry, "GameObject","")
	{

	}

	Entity::Entity(Registry& registry, const std::string& name, const std::string& group) : m_Registry(registry), m_Entity{ registry.CreateEntity()}, m_Name{name}, m_Group{group}
	{
		AddComponent<IdentificationComponent>(IdentificationComponent{
			.name = name,
			.group = group,
			.entityID = static_cast<uint32_t>(m_Entity) });
	}
}
