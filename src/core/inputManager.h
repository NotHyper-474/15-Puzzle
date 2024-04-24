#pragma once

#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "game.h"
#include "input.h"

class InputManager
{
public:
	static void Initialize(Game& gameInstance)
	{
		game = &gameInstance;
	}

	static bool GetKey(int keyCode)
	{
		if (InitCheck("GetKey"))
			return game->GetInput()->GetKey(keyCode);
		return false;
	}

	static bool GetKeyDown(int keyCode)
	{
		if (InitCheck("GetKeyDown"))
			return game->GetInput()->GetKeyDown(keyCode);
		return false;
	}

	static bool GetKeyUp(int keyCode)
	{
		if (InitCheck("GetKeyUp"))
			return game->GetInput()->GetKeyUp(keyCode);
		return false;
	}

	static bool GetMouse(int keyCode)
	{
		if (InitCheck("GetMouse"))
			return game->GetInput()->GetMouse(keyCode);
		return false;
	}

private:
	static Game* game;

	static bool InitCheck(const char* funcName)
	{
		if (game == nullptr)
		{
			std::cerr << "\033[31mCouldn't " << funcName << " because provided game was null, " <<
				"please make sure you've used Initialize() before trying to access controls\033[0m" << std::endl;
			return false;
		}
		return true;
	}
};

#endif