#include "spriteRenderer.h"
#include "core/globals.h"
#include "../src/core/inputManager.h"
#include "../src/core/textureManager.h"
#include "rendering/sprite.h"
#include "core/game.h"

SpriteRenderer::SpriteRenderer(Sprite* sprite)
{
	SDL_assert(sprite != nullptr);
	this->sprite = std::move(sprite);
	width = (float)sprite->getWidth() / sprite->FrameCount, height = sprite->getHeight();
	this->sprite->drawSrcRect = { 0, 0, static_cast<int>(width), static_cast<int>(height) };
	this->sprite->drawRect = { 0.f, 0.f, width, height };
}

SpriteRenderer::SpriteRenderer(const char* textureSheet) :
	SpriteRenderer(new Sprite(textureSheet, 0))
{
}

SpriteRenderer::~SpriteRenderer()
{
	if (sprite == nullptr) return;
	sprite->IsActive = false;
	sprite = nullptr;
}

void SpriteRenderer::Start()
{
	SDL_assert(sprite != nullptr);
	Game::GetSorter()->Add(sprite);
}

void SpriteRenderer::Draw()
{
	if (!owner->IsSelfActive())
		return;	

	sprite->sortOrder = transform->position.y;
	sprite->drawRect.position = transform->position - Vector2f(width, height).Scale(sprite->Pivot);
	sprite->drawRect.setSize(sprite->drawRect.size.Scale(transform->lossyScale));
}

void SpriteRenderer::SetSprite(Sprite* newSprite) {
	SDL_assert(newSprite != nullptr);
	sprite->IsActive = false;
	sprite = newSprite;
}

Sprite& SpriteRenderer::GetSprite() const
{
	return *sprite;
}