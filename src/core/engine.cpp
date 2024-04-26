#include "engine.h"

#include "game.h"
#include "timing.h"
#include "math/mathf.h"

#include <math.h>
#include <SDL_thread.h>

Engine::Engine() : frameCount()
{
	renderLock = SDL_CreateMutex();
	canRender = SDL_CreateCond();
}

Engine::~Engine()
{
	SDL_DestroyMutex(renderLock);
	SDL_DestroyCond(canRender);
}

void Engine::Start(Game* game)
{
	if (game == nullptr)
	{
		Logger::LogError("Provided game shouldn't be null");
		return;
	}

	if (this->game && this->game->Running())
	{
		Logger::LogError("Attempted to start engine with other game instance when current one is still running, operation not allowed.");
		return;
	}

	this->game.reset(game);
	SDL_Thread* renderThread = SDL_CreateThread(RenderLoopWrapper, "Render Loop", NULL);
	if (renderThread == NULL)
		Logger::LogErrorFormat("Could not create render thread: %s", SDL_GetError());

	GameLoop();

	SDL_WaitThread(renderThread, NULL);

	this->game->isRunning = false;
	this->game.reset();
}

void Engine::GameLoop()
{
	double lastTime = Time::GetTimeAsDouble();
	double updateTimer = 0.;
	float deltaClock = 0.f;
	int framerate = 60;
	double averageFrameTimeMs = (1.0 / 60.0) * 1000.0;

	while (game->Running())
	{
		double currentTime = Time::GetTimeAsDouble();
		double passedTime = (currentTime - lastTime) * Time::timeScale;

		lastTime = currentTime;

		updateTimer += passedTime;
		deltaClock += passedTime;

		game->HandleEvents();

		SDL_mutexP(renderLock);

		Time::deltaTime = Time::fixedDeltaTime; // Convenience
		// Fixed Update accumulator
		while (updateTimer > Time::fixedDeltaTime) {
			game->FixedTick(Time::fixedDeltaTime);
			updateTimer -= Time::fixedDeltaTime;
		}
		Time::deltaTime = fminf(float(passedTime), Time::maximumDeltaTime);

		game->PreUpdate();
		game->Tick(Time::deltaTime);

		game->GetManager()->Refresh();

		SDL_CondSignal(canRender);
		SDL_mutexV(renderLock);

		SDL_Delay(0);
#if _DEBUG
		if (deltaClock >= 1.f)
		{
			framerate = (float)frameCount * 0.5f + framerate * 0.5f;
			frameCount = 0;
			deltaClock -= 1.f;
			averageFrameTimeMs = 1000.0 / (framerate == 0 ? 0.001 : framerate);
			printf("CPU time was %lf;\nFrame rate: %d\n", averageFrameTimeMs, framerate);
			printf("DT FPS: %f\n", 1.f / Time::deltaTime);
		}
#endif
	}
}

int Engine::RenderLoop()
{
	while (game.get() != nullptr && game->Running())
	{
		SDL_mutexP(renderLock);
		SDL_CondWait(canRender, renderLock);
		game->Render();
		frameCount++;
		Uint32 delay = maxFPS ? 1000u / maxFPS : 0u;
		SDL_Delay(std::max(int(delay) - 1, 0));
		SDL_mutexV(renderLock);
	}
	return 0;
}
