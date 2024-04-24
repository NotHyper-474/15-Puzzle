#pragma once

#include "ECS/ECS.h"
#include "core/inputManager.h"
#include "ECS/components/spriteRenderer.h"
#include "core/math/vectormath.hpp"
#include <box2d/box2d.h>

class Player : public Component
{
public:
	float dummy = -1.f;

	Player(b2Body* rigidbody) : rigidbody(rigidbody) {}

	virtual ~Player() override
	{
	}

	void Start() override
	{
		rigidbody->SetTransform(transform->position, 0.f);
		rigidbody->SetFixedRotation(true);
	}

	void Update(const float _deltaTime) override
	{
		const float hor = (InputManager::GetKey(Input::KEY_D) - InputManager::GetKey(Input::KEY_A));
		const float ver = (InputManager::GetKey(Input::KEY_S) - InputManager::GetKey(Input::KEY_W));
		const float speed = 200.f;

		Vector2f vec = Vector2f(hor, ver) * speed;
		vec.ClampMagnitude(vec, speed);
		if (dummy != -1.f)
		{
			for (size_t i = 0; i < 10000000; i++)
			{
				dummy += Vector2f::Distance(Vector2f(i, i * 2), transform->position);
			}
		}
		if (InputManager::GetKeyDown(Input::KEY_Q))
		{
			dummy = dummy != -1.f ? -1.f : 0.f;
		}
		rigidbody->SetLinearVelocity(vec * 10.f);
		transform->setPosition(Vector2f{ rigidbody->GetPosition() });
	}

private:
	b2Body* rigidbody;
};
