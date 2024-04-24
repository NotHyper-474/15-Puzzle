#pragma once

#include <array>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

#include "core/math/vectormath.hpp"
#include "countMap.h"
#include "puzzleState.hpp"

#define USE_ASTAR 0

namespace solver {
	constexpr Vector2i UP	{  0, -1 };
	constexpr Vector2i DOWN {  0,  1 };
	constexpr Vector2i LEFT { -1,  0 };
	constexpr Vector2i RIGHT{  1,  0 };

	constexpr Vector2i directions[4] = { UP, DOWN, LEFT, RIGHT };

	const std::string patternFilePath = "Resources/patternDb_" + std::to_string(puzzleSize) + ".dat";

	struct PatternDB {
		void operator=(PatternDB& other) = delete;
		void operator==(PatternDB& other) = delete;

		static PatternDB& getInstance() {
			static PatternDB inst;
			return inst;
		}

		std::vector<std::vector<int>> groups;
		std::vector<countMap> maps;

	private:
		PatternDB() = default;
	};

	struct CustomHasher {
		size_t operator()(const std::array<int, 16>& arr) const {
			size_t hash = 0;
			for (int i = 0; i < 16; ++i) {
				hash ^= std::hash<int>{}(arr[i]) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
			}
			return hash;
		}
	};

	struct PuzzleEquals {
		bool operator()(const PuzzleState& state1, const PuzzleState& state2) const {
			for (int i = 0; i < puzzleSize * puzzleSize; ++i) {
				if (state1.puzzle[i] != state2.puzzle[i]) {
					return false;
				}
			}
			return true;
		}
	};

	std::string solvePuzzle(PuzzleState initialState);
}