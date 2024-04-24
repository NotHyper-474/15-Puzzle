#pragma once

#include "core/globals.h"
#include "ECS/ECS.h"
#include "core/transform.h"
#include "ECS/components/camera.h"

#include <SDL.h>

struct Sprite;
struct C_Drawable;

struct SpriteRenderer : public Component
{
	friend struct Animation;

	SpriteRenderer() = default;
	SpriteRenderer(Sprite* sprite);
	SpriteRenderer(const char* textureSheet);
	virtual ~SpriteRenderer();

	void Start() override;
	void Draw() override;

	void SetSprite(Sprite* newSprite);
	Sprite& GetSprite() const;

	float width = 16.f;
	float height = 16.f;
private:
	Sprite* sprite;
};
