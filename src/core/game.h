#pragma once

#include <SDL.h>
#include <vector>

#include "../ECS/ECS.h"
#include "window.h"
#include "input.h"
#include "engine.h"

struct Collider;

struct Game
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
	T* FindEntityOfType() const;

	template<typename T> requires std::derived_from<T, Component>
	std::vector<T*> FindEntitiesOfType() const;
	
	template<typename T, size_t L> requires std::derived_from<T, Component>
	int FindEntitiesOfTypeNonAlloc(std::array<T*, L>& result);

	// @return First entity with specified name, will return nullptr if not found
	Entity* FindWithName(const char* name);

	bool Running() const;
	FORCEINLINE static Window* GetWindow() { return Engine::getGame()->window.get(); }
	FORCEINLINE static DrawSorter* GetSorter() { return Engine::getGame()->window->GetSorter(); }
	Input* GetInput() { return &input; }

	FORCEINLINE EntityManager* GetManager() { return &ecsManager; }

protected:
	virtual void Start() {}
	virtual void Update(const float _deltaTime) {}
	virtual void FixedUpdate(const float _fixedDeltaTime) {}
	virtual void Draw() {}

	bool isRunning = false;
	std::unique_ptr<Window> window;
	Input input;

	EntityManager ecsManager;
	std::array<Collider*, 32> colliders;

};

#include "game.inl"