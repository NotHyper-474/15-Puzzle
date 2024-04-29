#pragma once

#include <iostream>
#include <memory>

#include "globals.h"

class Game;

class Engine
{
	Engine();
	~Engine();

	std::unique_ptr<Game> game;

	size_t frameCount;
	void GameLoop();

	void operator=(const Engine& other) = delete;
	bool operator==(const Engine& other) const = delete;

public:
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
};
