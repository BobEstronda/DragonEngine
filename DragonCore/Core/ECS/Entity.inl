#pragma once

#include "Entity.h"

namespace DragonCore::ECS
{
	template <typename TComponent, typename ...Args>
	TComponent& Entity::AddComponent(Args&& ...args)
	{
		auto& registry = m_Registry.GetRegistry();
		return registry.emplace<TComponent>(m_Entity, std::forward<Args>(args) ...);
	}

	template <typename TComponent, typename ...Args>
	TComponent& Entity::ReplaceComponent(Args&& ...args)
	{
		auto& registry = m_Registry.GetRegistry();
		if (registry.all_of<TComponent>(m_Entity))
		{
			return registry.replace<TComponent>(m_Entity, std::forward<Args>(args) ...);
		}

		return registry.emplace<TComponent>(m_Entity, std::forward<Args>(args) ...);
	}

	template <typename TComponent>
	TComponent& Entity::GetComponent()
	{
		auto& registry = m_Registry.GetRegistry();
		return registry.get<TComponent>(m_Entity);
	}

	template <typename TComponent>
	const bool Entity::HasComponent() const
	{
		auto& registry = m_Registry.GetRegistry();
		return registry.all_of<TComponent>(m_Entity);
	}

	template <typename TComponent>
	void Entity::RemoveComponent()
	{
		auto& registry = m_Registry.GetRegistry();
		return registry.remove<TComponent>(m_Entity);
	}
}