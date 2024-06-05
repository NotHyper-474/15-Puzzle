#include "solver.hpp"

#include "ida_ai.h"

std::string solver::solvePuzzle(PuzzleState initialState) {
	if (initialState.isGoal()) {
		Logger::Log("Current puzzle state is already solved.");
		return "";
	}

	if (PatternDB::getInstance().groups.size() == 0 && PatternDB::getInstance().LoadFromFile(patternFilePath.c_str()) == 0) {
		return "";
	}

	Logger::Log("Solving puzzle...");

#if USE_ASTAR
	std::priority_queue<MoveState> openSet;
	std::unordered_set<std::array<int, 16>, CustomHasher> visited;
	openSet.push(MoveState{ initialState });
	float t1 = Time::GetTime();

	while (!openSet.empty()) {
		MoveState currentState = openSet.top();
		openSet.pop();

		if (currentState.isGoal()) {
			std::cout << "Solution found in " << (Time::GetTime() - t1) << " seconds!" << std::endl
			<< openSet.size() << " states were iterated and " << visited.size() << " were visited to find it." << std::endl
			<< "Moves to solution:" << std::endl
			<< currentState.moves << std::endl;

			return currentState.moves;
		}


		if (visited.contains(currentState.puzzle)) {
			continue;
		}

		visited.insert(currentState.puzzle);

		std::vector<MoveState> successors = currentState.successors();
		for (const auto& successor : successors) {
			if (visited.contains(successor.puzzle) == false) {
				openSet.push(successor);
			}
		}
	}

	// Generally never happens and if the puzzle is not solvable it'll get stuck and never reach this part
	std::cout << "No solution found for this puzzle." << std::endl;
#else 
	return ida::ida_star(initialState);
#endif
}