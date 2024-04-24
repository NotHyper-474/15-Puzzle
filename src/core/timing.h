#ifndef TIME_H
#define TIME_H

#include "common.h"
#include "globals.h"

namespace Time
{
	float GetTime();
	double GetTimeAsDouble();
	
	/* How fast the game should run. This can be used for slow-motion effects*/
	static float timeScale = 1.f;
	/* Returns time since last frame in seconds */
	static float deltaTime;
	static float fixedDeltaTime = 1.f / DEFAULT_FIXEDFRAMERATE;
	/* Limits how big delta time can get (the bigger delta time is the lower the FPS) */
	static float maximumDeltaTime = 1.f / 3.f;
}

#endif