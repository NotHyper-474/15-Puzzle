#pragma once

#include <iostream>
#include <memory>

#include "globals.h"

class Game;

class Engine
{
	Engine();
	~Engine();

	Game* currentGame;

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

	static const Game* getGame()
	{
		return getInstance()->currentGame;
	}
	
	// PUBLIC VARS
	uint32_t maxFPS = DEFAULT_MAXFPS;
};
