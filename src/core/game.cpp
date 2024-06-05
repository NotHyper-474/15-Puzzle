
#include "game.h"
#include "logger.hpp"
#include "textureManager.h"

#include "core/math/mathf.h"
#include "physics/collision.hpp"
#include "physics/components/collider.h"

Game::~Game()
{
	IMG_Quit();
	isRunning = false;
}

void Game::Create(const char* title, int width, int height, float scale, bool fullscreen, bool vsync, uint32_t windowFlags)
{	
	if (fullscreen)
	{
		windowFlags |= SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_VIDEO) == 0)
	{
		IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF);
		TTF_Init();
		Logger::LogSystem("Subsystems initialized.");
		SDL_Window* window = SDL_CreateWindow(
			title,
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			Mathf::RoundToInt(width * scale), Mathf::RoundToInt(height * scale),
			windowFlags);

		if (window)
		{
			Logger::LogSystem("Window created successfully.");
		}

		SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
		SDL_RenderSetVSync(renderer, vsync);
		if (renderer)
		{
			Logger::LogSystem("Renderer created.");
		}

		TextureManager::Instance().currentRender = renderer;
		this->window = std::make_unique<Window>(width, height, window, renderer, scale);
		isRunning = true;
	}
	else
	{
		Logger::LogErrorFormat("Game failed initializing! Error: %s", SDL_GetError());
		isRunning = false;
		throw std::runtime_error("SDL failed initializing");
	}

	Start();

	colliders.fill(nullptr);
	FindEntitiesOfTypeNonAlloc(colliders);
}

#pragma region Game Loop

void Game::HandleEvents()
{
	input.ClearMouseAndKeys();

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:

			isRunning = false;
			break;
		case SDL_WINDOWEVENT:
			window->HandleEvents(&event);
			break;
		case SDL_MOUSEMOTION:
			input.SetMouseX(event.motion.x);
			input.SetMouseY(event.motion.y);
			break;

		case SDL_KEYDOWN:
		{
			const int value = event.key.keysym.scancode;

			input.SetKey(value, true);
			input.SetKeyDown(value, true);
		}
		break;

		case SDL_KEYUP:
		{
			const int value = event.key.keysym.scancode;

			input.SetKey(value, false);
			input.SetKeyUp(value, true);
		}
		break;

		case SDL_MOUSEBUTTONDOWN:
		{
			const int value = event.button.button;

			input.SetMouse(value, true);
			input.SetMouseDown(value, true);
		}
		break;

		case SDL_MOUSEBUTTONUP:
		{
			const int value = event.button.button;

			input.SetMouse(value, false);
			input.SetMouseUp(value, true);
		}
		break;
		default:
			SDL_EventState(event.type, SDL_IGNORE);
			break;
		}
	}
}

void Game::PreUpdate()
{
	ecsManager.PreUpdate();
}

void Game::Tick(const float _deltaTime)
{
	ecsManager.Update(_deltaTime);
	Update(_deltaTime);
}

void Game::FixedTick(const float _fixedDeltaTime)
{
	for (size_t i = 0; i < colliders.size() - 1; i++)
	{
		if (colliders[i] == nullptr || colliders[i]->getTransform() == nullptr) continue;
		for (size_t j = i + 1; j < colliders.size(); j++)
		{
			if (colliders[j] == nullptr || colliders[j]->getTransform() == nullptr) continue;

			if (PhysicsUtils::AABB(colliders[i]->GetRect(), colliders[j]->GetRect()))
			{
				colliders[i]->HandleCollision();
				colliders[j]->HandleCollision();
				GetSorter()->DrawFillRect(colliders[i]->GetRect(), { 255, 255, 255, 255 });
			}
		}
	}

	ecsManager.FixedUpdate(_fixedDeltaTime);
	FixedUpdate(_fixedDeltaTime);
}

void Game::Render()
{
	const auto renderer = window->GetRenderer();
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
	SDL_RenderClear(renderer);

	Draw();
	ecsManager.Draw();
	window->GetSorter()->Paint();

	SDL_RenderPresent(renderer);
}

bool Game::Running() const
{
	return isRunning;
}
#pragma endregion

Entity* Game::FindWithName(const char* name)
{
	for (auto& ent : ecsManager.entities) {
		if (ent->name == name)
			return ent.get();
	}
	return nullptr;
}

Entity& Game::Instantiate(Entity* instance, const Vector2f& position)
{
	instance->transform->position = position;
	return ecsManager.AddEntity(instance);
}