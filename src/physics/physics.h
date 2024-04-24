#pragma once

#include "core/math/vectormath.hpp"
#include "core/math/rect.hpp"
#include <format>
#include <array>
#include <vector>

typedef struct {
	Rect aabb;
	Vector2f velocity;
	Vector2f acceleration;
} Body;

typedef struct {
	//std::array<Node, 4> children;

	std::vector<Body> bodies;
} Node;

std::unique_ptr<Node> root;

void add_body(Body* body)
{
	if (root == nullptr)
	{
		//root = std::make_unique<Node>();
		//root->body = body;
		return;
	}
}