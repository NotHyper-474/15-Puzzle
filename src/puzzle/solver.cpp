#include "solver.hpp"

#include "ida_ai.h"

#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>


std::string solver::solvePuzzle(PuzzleState initialState) {
	if (initialState.isGoal()) {
		Logger::Log("Current puzzle state is already solved.");
		return "";
	}

	if (PatternDB::getInstance().groups.size() == 0) {
		Logger::Log("Loading patterns, hang tight...");
		std::ifstream patternFile(patternFilePath.c_str());
		if (patternFile.is_open() == false) {
			Logger::LogError("Pattern file not found. Make sure it's in the same directory as the executable");
			throw;
		}

		boost::archive::text_iarchive test(patternFile);

		test >> PatternDB::getInstance().groups;
		test >> PatternDB::getInstance().maps;

		if (PatternDB::getInstance().groups.size() != PatternDB::getInstance().maps.size()) {
			Logger::LogError("Groups and maps count doesn't match. Was the pattern file generated correctly?");
			throw;
		}

		Logger::Log("Pattern Data loaded successfully");
		for (size_t i = 0; i < PatternDB::getInstance().groups.size(); i++) {
			setlocale(LC_ALL, "");
			Logger::LogFormat("Group %d, permutations: %d", i, PatternDB::getInstance().maps[i].size());
		}
	}

#if USE_ASTAR
	std::priority_queue<MoveState> openSet;
	std::unordered_set<std::array<int, 16>, CustomHasher> visited;
	openSet.push((MoveState)initialState);
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