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

#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>

#include "core/math/vectormath.hpp"
#include "countMap.h"
#include "puzzleState.hpp"

#define USE_ASTAR 0

namespace solver {
	constexpr Vector2i UP	{  0, -1 };
	constexpr Vector2i DOWN {  0,  1 };
	constexpr Vector2i LEFT { -1,  0 };
	constexpr Vector2i RIGHT{  1,  0 };

	constexpr Vector2i directions[] = { UP, DOWN, LEFT, RIGHT };

	const std::string patternFilePath = "Resources/patternDb_" + std::to_string(puzzleSize) + ".dat";

	struct PatternDB {
		void operator=(PatternDB& other) = delete;
		void operator==(PatternDB& other) = delete;

		static PatternDB& getInstance() {
			static PatternDB inst;
			return inst;
		}

		/// <summary>
		/// Loads Pattern database from file
		/// </summary>
		/// <param name="fileName"></param>
		/// <returns>0 when an error occurs or 1 if succeded</returns>
		int LoadFromFile(const char* fileName) {
			Logger::Log("Loading patterns, hang tight...");
			std::ifstream patternFile(fileName);
			if (patternFile.is_open() == false) {
				Logger::LogError("Pattern file not found. Make sure it's in the same directory as the executable");
				return 0;
			}

			boost::archive::text_iarchive data(patternFile);

			data >> groups;
			data >> maps;

			if (groups.size() != maps.size()) {
				Logger::LogError("Groups and maps count doesn't match. Was the pattern file generated correctly?");
				return 0;
			}

			Logger::Log("Pattern Data loaded successfully");
			for (size_t i = 0; i < groups.size(); i++) {
				Logger::LogFormat("Group %d, permutations: %d", i, maps[i].size());
			}

			return 1;
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