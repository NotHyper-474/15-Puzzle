#pragma once

#include <deque>
#include <vector>
#include <string>

#include "timing.h"
#include "math/vectormath.hpp"

struct PuzzleState;

namespace ida {
	constexpr int FOUND = -2;
	constexpr int INF = INT_MAX;

	int search(std::deque<PuzzleState>& path, int g, int bound, std::vector<Vector2i>& dirs, float startingTime);

	void convertDirections(const std::vector<Vector2i>& dirs, std::string& result);

	std::string ida_star(PuzzleState root);
}