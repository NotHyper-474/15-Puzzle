#pragma once

#include "../collision.hpp"
#include "core/event/iobserver.h"
#include "core/math/rect.hpp"
#include "ECS/ECS.h"

struct Collider : public Component, IPublisher
{
	FORCEINLINE const Rect GetRect() const
	{
		return { transform->position, size };
	}

	FORCEINLINE void HandleCollision()
	{
		transform->position = previousPos;
		Notify();
	}

	Vector2f size;

protected:
	Vector2f previousPos;

	void FixedUpdate(const float) override
	{
		previousPos = transform->position;
	}


};