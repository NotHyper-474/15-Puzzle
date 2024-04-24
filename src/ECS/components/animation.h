#pragma once

#include "core/common.h"
#include "rendering/sprite.h"
#include "core/timing.h"
#include "core/logger.hpp"
#include "ECS/ECS.h"
#include "spriteRenderer.h"

struct Animation final : public Component
{
	FORCEINLINE void Play()
	{
		isPlaying = true;
	}

	FORCEINLINE void Stop()
	{
		isPlaying = false;
	}

	FORCEINLINE bool IsPlaying() const { return isPlaying; }

	SpriteRenderer* renderer;
	int currentFrame = 0;
	int direction = 0;
	float speed = 1.f;

protected:
	void Start() override {
		if (!HasComponent<SpriteRenderer>()) {
			Logger::LogError("Animation requires a SpriteRenderer component assigned to the Entity");
			return;
		}
		renderer = GetComponent<SpriteRenderer>();
		this->renderer->sprite->drawSrcRect = { 0, 0, renderer->sprite->Width / renderer->sprite->FrameCount, renderer->sprite->Height };
		this->renderer->width = static_cast<float>(renderer->sprite->Width / renderer->sprite->FrameCount);
		this->renderer->height = renderer->sprite->Height;
	}

	void FixedUpdate(const float _dt) override
	{
		if (renderer == nullptr) return;
		timer += _dt;
		if (timer >= 0.1f)
		{
			currentFrame = (currentFrame + 1) % renderer->sprite->FrameCount;
			timer = 0.f;
		}
		renderer->sprite->drawSrcRect.x = currentFrame * (renderer->sprite->Width / renderer->sprite->FrameCount);
	}

private:
	bool isPlaying = false;
	float timer;
};