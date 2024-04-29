#include "ida_ai.h"

#include "solver.hpp"

int ida::search(std::deque<PuzzleState>& path, int g, int bound, std::vector<Vector2i>& dirs, float startingTime) {
	if (Time::GetTime() - startingTime > 20.f) {
		Logger::LogError(
			"Time limit exceeded, algorithm might have been fed an unsolvable puzzle\n\
If you're using Debug try using Release instead as it speeds up searching");
		return INF;
	}
	PuzzleState cur = path.back();
	int f = g + cur.heuristic();

	if (f > bound)
		return f;

	if (cur.isGoal()) {
		return FOUND;
	}

	int min = INF;

	// Successors
	for (auto& dir : solver::directions) {
		if (!dirs.empty() && -dir == dirs.back())
			continue;

		PuzzleState simPuzzle = cur;
		if (!simPuzzle.doMove(dir)
			|| std::count(std::begin(path), std::end(path), simPuzzle) > 0) {
			continue;
		}

		path.push_back(simPuzzle);
		dirs.push_back(dir);

		int t = ida::search(path, g + 1, bound, dirs, startingTime);
		if (t == FOUND) return FOUND;
		if (t < min) min = t;

		path.pop_back();
		dirs.pop_back();
	}
	return min;
}

void ida::convertDirections(const std::vector<Vector2i>& dirs, std::string& result) {
	for (auto& dir : dirs) {
		if (dir == solver::UP)
			result += 'U';
		else if (dir == solver::DOWN)
			result += 'D';
		else if (dir == solver::LEFT)
			result += 'L';
		else if (dir == solver::RIGHT)
			result += 'R';
	}
}

std::string ida::ida_star(PuzzleState root) {
	if (root.isGoal()) {
		return "";
	}

	float time = Time::GetTime();
	int bound = root.heuristic();
	std::deque <PuzzleState> path;
	std::vector<Vector2i> dirs;

	path.push_back(root);

	for (;;) {
		int t = ida::search(path, 0, bound, dirs, time);
		if (t == FOUND) {
			Logger::LogFormat("Took %f seconds to find a solution of %d moves", Time::GetTime() - time, dirs.size());
			std::string moves;
			ida::convertDirections(dirs, moves);
			Logger::LogFormat("Moves: %s", moves.c_str());
			return moves;
		}
		else if (t == INF) {
			return "";
		}
		bound = t;
	}
}

