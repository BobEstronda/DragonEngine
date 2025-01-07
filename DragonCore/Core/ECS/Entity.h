#pragma once

#include "Registry.h"

namespace DragonCore::ECS
{
	class Entity
	{
	private:
		Registry& m_Registry;
		entt::entity m_Entity;
		std::string m_Name, m_Group;

	public:
		Entity(Registry& registry);
		Entity(Registry& registry, const std::string& name = "", const std::string& group = "");
		~Entity() = default;

		inline std::uint32_t Kill() { return m_Registry.GetRegistry().destroy(m_Entity); }

		inline entt::entity& GetEntity() { return m_Entity; }
		inline entt::registry& GetREgistry() { return m_Registry.GetRegistry(); }

		template <typename TComponent, typename ...Args>
		TComponent& AddComponent(Args&& ...args);

		template <typename TComponent, typename ...Args>
		TComponent& ReplaceComponent(Args&& ...args);

		template <typename TComponent>
		const bool HasComponent() const;

		template <typename TComponent>
		TComponent& GetComponent();

		template <typename TComponent>
		void RemoveComponent();
	};
}

#include "Entity.inl"