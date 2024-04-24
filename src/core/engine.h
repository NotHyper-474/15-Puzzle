#pragma once

#include <iostream>
#include <memory>

#include "globals.h"

class Game;
struct SDL_cond;
struct SDL_mutex;

class Engine
{
public:
	Engine();
	~Engine();

	void Start(Game* game);

	static Engine* getInstance()
	{
		static Engine engine;
		return &engine;
	}

	static Game* getGame()
	{
		return getInstance()->game.get();
	}
	
	// PUBLIC VARS
	uint32_t maxFPS = DEFAULT_MAXFPS;
private:
	std::unique_ptr<Game> game;
	SDL_cond* canRender;
	SDL_mutex* renderLock;

	size_t frameCount;
	void GameLoop();
	int RenderLoop();

	static int RenderLoopWrapper(void*)
	{
		return Engine::getInstance()->RenderLoop();
	}

	void operator=(const Engine& other) = delete;
	bool operator==(const Engine& other) const = delete;
};
