#include "puzzleState.hpp"

#include "solver.hpp"

#define PATTERN solver::PatternDB::getInstance()

int PuzzleState::heuristic() const
{
	int distance = 0;

	for (size_t k = 0; k < PATTERN.groups.size(); ++k) {
		std::string hash = hashGroup(PATTERN.groups[k]);
		if (PATTERN.maps[k].contains(hash)) {
			distance += PATTERN.maps[k][hash];
		}
		else {
			Logger::LogWarning("No pattern found in DB, resorting to Manhattan Distance");
			for (int i = 0; i < puzzleLength; ++i) {
				if (puzzle[i] != 0) {
					int goalRow = (puzzle[i] - 1) / puzzleSize;
					int goalCol = (puzzle[i] - 1) % puzzleSize;
					int currRow = i / puzzleSize;
					int currCol = i % puzzleSize;
					distance += abs(goalRow - currRow) + abs(goalCol - currCol);
				}
			}
		}
	}
	return distance;
}
