#include "timing.h"
#include <ctime>
#include <SDL.h>
#include <SDL_timer.h>

double Time::GetTimeAsDouble()
{
	static Uint64 g_freq;
	static bool g_timerInitialized = false;
	if (!g_timerInitialized)
	{
		Uint64 li = SDL_GetPerformanceFrequency();

		g_freq = li;
		g_timerInitialized = true;
	}

	Uint64 li = SDL_GetPerformanceCounter();

	return double(li) / g_freq;
}

float Time::GetTime()
{
	return static_cast<float>(Time::GetTimeAsDouble());
}