#pragma once


#include "ECS/ECS.h"
#include "core/game.h"

struct Camera : public Component
{
	FORCEINLINE Vector2f getProjection() const
	{
		const Vector2f pos = transform->position;
		const Vector2f size = getCenterOffset();
		return Vector2f(pos.x - size.x, pos.y - size.y);
	}

	FORCEINLINE Vector2f getCenterOffset() const
	{
		return Engine::getGame()->GetWindow()->GetSize() * 0.5f;
	}

private:
	Vector2f viewport;
};
