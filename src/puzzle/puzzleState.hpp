#pragma once

#include <array>
#include <vector>
#include <string>

#include "countMap.h"
#include "logger.hpp"
#include "math/vectormath.hpp"

constexpr int puzzleSize = 4;
constexpr int puzzleLength = puzzleSize * puzzleSize;

struct PuzzleState {
	std::array<int, puzzleLength> puzzle;
	int blankIndex;
	int g;
	int h;

	PuzzleState() : g(), h() {
		for (int i = 0; i < puzzle.size(); ++i) {
			puzzle[i] = i + 1;
		}
		puzzle[puzzle.size() - 1] = 0;
		blankIndex = puzzleLength - 1;
	}

	PuzzleState(std::array<int, puzzleLength>& newPuzzle, int blank) : puzzle(newPuzzle), blankIndex(blank), g(0) {
		if (blank == -1) {
			for (int i = 0; i < puzzleLength; ++i) {
				if (puzzle[i] == 0) {
					blankIndex = i;
					break;
				}
			}
		}
	}

	int heuristic() const;

	bool isGoal() const {
		for (int i = 0; i < puzzleLength - 1; ++i) {
			if (puzzle[i] != i + 1 && puzzle[i] != 0) {
				return false;
			}
		}
		return puzzle[puzzle.size() - 1] == 0;
	}

	/// <summary>
	/// Move blank space towards a direction and returns true if it was successful
	/// </summary>
	/// <param name="dir"></param>
	/// <returns></returns>
	bool doMove(const Vector2i& dir) {
		const int newX = blankIndex / puzzleSize + dir.x;
		const int newY = blankIndex % puzzleSize + dir.y;
		if (newX < 0 || newX >= puzzleSize || newY < 0 || newY >= puzzleSize) return false;

		const int newIndex = newX * puzzleSize + newY;
		std::swap(puzzle[newIndex], puzzle[blankIndex]);
		blankIndex = newIndex;

		return true;
	}

	std::string hashGroup(std::vector<int> group = {}) const {
		if (group.empty()) {
			group.resize(puzzleLength);
			for (int i = 0; i < puzzleLength; ++i) {
				group[i] = i;
			}
		}

		std::string hashString(2 * puzzleLength, '0');

		for (int i = 0; i < puzzleSize; ++i) {
			for (int j = 0; j < puzzleSize; ++j) {
				int idx = i * puzzleSize + j;
				if (count(group.begin(), group.end(), puzzle[idx]) > 0) {
					hashString[2 * puzzle[idx]] = std::to_string(i)[0];
					hashString[2 * puzzle[idx] + 1] = std::to_string(j)[0];
				}
				else {
					hashString[2 * puzzle[idx]] = 'x';
					hashString[2 * puzzle[idx] + 1] = 'x';
				}
			}
		}

		erase_if(hashString, [](char& c) { return c == 'x'; });

		return hashString;
	}

	// Operador de comparação para a fila de prioridade do A*
	bool operator<(const PuzzleState& other) const {
		return g + h > other.g + other.h; // Ordenando pelo menor custo total estimado
	}

	bool operator==(const PuzzleState& other) const {
		return puzzle == other.puzzle;
	}
};

struct MoveState : public PuzzleState {
	std::string moves;

	std::vector<MoveState> successors() const {
		std::vector<MoveState> succ;
		int row = blankIndex / puzzleSize;
		int col = blankIndex % puzzleSize;
		constexpr int dx[] = { -1, 1, 0, 0 };
		constexpr int dy[] = { 0, 0, -1, 1 };
		for (int d = 0; d < 4; ++d) {
			int newRow = row + dx[d];
			int newCol = col + dy[d];
			if (newRow >= 0 && newRow < puzzleSize && newCol >= 0 && newCol < puzzleSize) {
				MoveState newState = *this;
				const int newIndex = newRow * puzzleSize + newCol;
				std::swap(newState.puzzle[blankIndex], newState.puzzle[newIndex]);
				newState.blankIndex = newRow * puzzleSize + newCol;
				newState.g = g + 1;
				newState.h = heuristic();

				if (dx[d] < 0)
					newState.moves += 'L';
				else if (dx[d] > 0)
					newState.moves += 'R';
				else if (dy[d] < 0)
					newState.moves += 'U';
				else if (dy[d] > 0)
					newState.moves += 'D';
				succ.push_back(newState);
			}
		}
		return succ;
	}
};