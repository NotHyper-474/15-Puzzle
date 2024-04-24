#pragma once

#include "../core/common.h"
#include "../core/logger.hpp"
#include "../core/math/vectormath.hpp"
#include <SDL.h>

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>
#include <queue>

#pragma warning ( push )
#pragma warning ( disable : 4100)

class Component;
class Transform;

using ComponentID = std::size_t;

inline ComponentID GetComponentTypeID()
{
	static ComponentID lastID = 0;
	return lastID++;
}

template<typename T>
ComponentID GetComponentTypeID() noexcept
{
	static ComponentID typeID = GetComponentTypeID();
	return typeID;
}

constexpr std::size_t maxComponents = 32U;

using ComponentBitSet = std::bitset<maxComponents>;
using ComponentArray = std::array<Component*, maxComponents>;

/// <summary>
/// Entity is the main object used during a level
/// </summary>
class Entity
{
	friend class Component;
	friend class Manager;
	bool destroyed = false;
	bool active = true;
	std::vector<std::unique_ptr<Component>> components;
	Transform* m_transform;

	ComponentArray componentArray;
	ComponentBitSet componentBitSet;
public:
	std::string name;
	__property(getTransform) Transform* const transform;

	Entity(const char* name, const Vector2f& position = Vector2f::zero());

	void Start();
	void Update(const float _deltaTime);
	void FixedUpdate(const float _fixedDeltaTime);
	void Draw();

	FORCEINLINE Transform* getTransform() const
	{
		return m_transform;
	}

	bool IsSelfActive() const { return active; }
	void Destroy() { destroyed = true; }

	template<typename T> requires std::derived_from<T, Component>
	bool HasComponent() const
	{
		return componentBitSet.test(GetComponentTypeID<T>());
	}

	template<typename T, typename... TArgs> requires std::derived_from<T, Component>
	T& AddComponent(TArgs&&... args)
	{
		return AddComponent(new T(std::forward<TArgs>(args)...));
	}

	template <typename T> requires std::derived_from<T, Component>
	T& AddComponent(T* instance)
	{
		if (instance == nullptr)
		{
			throw std::invalid_argument("Instance shouldn't be null");
		}
		T* c(std::move(instance));
		c->m_owner = this;
		c->m_transform = m_transform;
		std::unique_ptr<Component> uPtr(c);
		components.emplace_back(std::move(uPtr));

		const ComponentID id = GetComponentTypeID<T>();

		componentArray[id] = c;
		componentBitSet[id] = true;
		return *c;
	}

	// Gets an entity's component, returns NULL if component is not found
	template<typename T>
	T* GetComponent() const
	{
		static_assert(std::is_base_of<Component, T>::value, "Type must inherit Component");
		const ComponentID id = GetComponentTypeID<T>();
		if (!componentBitSet.test(id)) {
			return nullptr;
		}
		auto ptr(componentArray[id]);
		return static_cast<T*>(ptr);
	}

	template<typename T>
	bool TryGetComponent(OUT T** result) const
	{
		if (!result)
			return false;

		return (*result = GetComponent<T>()) != nullptr;
	}

protected:
	Entity() = default;
	void operator=(const Entity& other) = delete;
};

class Component
{
	friend class Entity;
	friend class Manager;
	Entity* m_owner;
	Transform* m_transform;

protected:
	virtual void Start() {}
	virtual void Update(const float _deltaTime) {}
	virtual void FixedUpdate(const float _fixedDeltaTime) {}
	virtual void Draw() {}

public:
	__property(getOwnerEntity) Entity* owner;
	__property(getTransform) Transform* transform;

	virtual ~Component()
	{
		m_owner = nullptr;
		m_transform = nullptr;
	}

	FORCEINLINE Entity* getOwnerEntity() const
	{
		return m_owner;
	}

	FORCEINLINE Transform* getTransform() const
	{
		return m_transform;
	}

	template <typename T>
	FORCEINLINE bool HasComponent() const requires std::derived_from<T, Component>
	{
		return m_owner->HasComponent<T>();
	}

	template <typename T>
	FORCEINLINE T* GetComponent() const requires std::derived_from<T, Component>
	{
		return m_owner->GetComponent<T>();
	}
};

#pragma warning ( pop )

// Give definition after or else other scripts will also treat it as incomplete
#include "../core/transform.h"

class Manager
{
	friend class Game;

	std::vector<std::unique_ptr<Entity>> entities;
	std::queue<Entity*> pendingEntities;

public:

	void PreUpdate();

	void Update(const float _deltaTime);

	void FixedUpdate(const float _fixedDeltaTime);

	void Draw();

	/// <summary>
	/// Removes destroyed entities
	/// </summary>
	void Refresh();

	Entity& AddEntity(const std::string& name = nullptr)
	{
		return AddEntity(name.c_str());
	}

	Entity& AddEntity(const char* name = nullptr)
	{
		return AddEntity(new Entity(name));
	}

	template<typename T>
	T& AddEntity(T* instance)
	{
		entities.emplace_back(std::move(instance));
		pendingEntities.push(instance);
		return *instance;
	}

	std::vector<Entity*> GetEntities()
	{
		std::vector<Entity*> result;
		for (const auto& ent : entities)
		{
			result.emplace_back(ent.get());
		}
		return result;
	}

	FORCEINLINE void RemoveChild(Transform* child);
};
