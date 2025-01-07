#pragma once

#include <entt.hpp>

namespace DragonCore::ECS
{
	class Registry
	{
	private:
		std::unique_ptr<entt::registry> m_pRegistry;
	public:
		Registry();
		~Registry() = default;

		inline entt::registry& GetRegistry() const { return *m_pRegistry; }
		inline entt::entity CreateEntity() { return m_pRegistry->create(); }

		template <typename TContext>
		TContext AddToContext(TContext context);

		template <typename TContext>
		TContext& GetContext() const;
	};

	template<typename TContext>
	inline TContext Registry::AddToContext(TContext context)
	{
		return m_pRegistry->ctx().emplace<TContext>(context);
	}

	template<typename TContext>
	inline TContext& Registry::GetContext() const
	{
		return m_pRegistry->ctx().get<TContext>();
	}
}