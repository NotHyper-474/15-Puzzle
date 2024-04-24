#pragma once

#include <SDL.h>
#include <vector>

#include "../ECS/ECS.h"
#include "window.h"
#include "input.h"
#include "engine.h"

struct Collider;

class Game
{
	friend class Engine;

public:
	virtual ~Game();

	void Create(const char* title, int width, int height, float scale = DEFAULT_PIXELSCALE, bool fullscreen = false, bool vsync = false, uint32_t windowFlags = 0);

	void HandleEvents();
	void PreUpdate();

	void Tick(const float _deltaTime);
	void FixedTick(const float _fixedDeltaTime);
	void Render();

	FORCEINLINE Entity& Instantiate(Entity* instance, const Vector2f& position = Vector2f());
	FORCEINLINE Entity& Instantiate(const char* name, const Vector2f& position = Vector2f()) const;

	template<typename T> requires std::is_base_of<Component, T>::value
	T* FindEntityOfType() const
	{
		auto it = std::find_if(ecsManager.entities.begin(), ecsManager.entities.end(), [](const std::unique_ptr<Entity>& e) { return e->HasComponent<T>(); });

		if (it != ecsManager.entities.end())
		{
			return (*it->get()).template GetComponent<T>();
		}

		return nullptr;
	}

	template<typename T> requires std::derived_from<T, Component>
	std::vector<T*> FindEntitiesOfType() const
	{
		std::vector<T*> result;
		for (auto& ent : ecsManager.entities)
		{
			T* c = nullptr;
			if (ent->TryGetComponent<T>(&c))
				result.push_back(c);
		}
		return result;
	}

	template<typename T, size_t L> requires std::derived_from<T, Component>
	int FindEntitiesOfTypeNonAlloc(std::array<T*, L>& result)
	{
		size_t i = 0;
		for (auto& ent : ecsManager.entities)
		{
			T* c = nullptr;
			if (ent->TryGetComponent<T>(&c))
			{
				result[++i] = c;
				if (i == L) break;
			}
		}
		return i;
	}

	// @return First entity with specified name, might return nullptr if not found
	Entity* FindWithName(const char* name)
	{
		for (auto& ent : ecsManager.entities) {
			if (ent->name == name)
				return ent.get();
		}
		return nullptr;
	}

	bool Running() const;
	FORCEINLINE static Window* GetWindow() { return Engine::getGame()->window.get(); }
	FORCEINLINE static DrawSorter* GetSorter() { return Engine::getGame()->window->GetSorter(); }
	Input* GetInput() { return &input; }

	FORCEINLINE Manager* GetManager() { return &ecsManager; }

protected:
	virtual void Start() {}
	virtual void Update(const float _deltaTime) {}
	virtual void FixedUpdate(const float _fixedDeltaTime) {}
	virtual void Draw() {}

	bool isRunning = false;
	std::unique_ptr<Window> window;
	Input input;

	Manager ecsManager;
	std::array<Collider*, 32> colliders;

};
