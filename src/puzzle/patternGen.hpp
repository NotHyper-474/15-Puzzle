#pragma once

#ifndef clang

#include <cmath>
#include <fstream>
#include <future>
#include <iostream>
#include <locale>
#include <queue>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/vector.hpp>

#include "logger.hpp"
#include "solver.hpp"
#include "timing.h"
#include "countMap.h"

namespace patternDb {
	using namespace std;
	using namespace boost;
	

	std::unordered_map<int, uint64_t> factCache;

	unsigned long long factorial(int n) {
		if (n <= 1) return 1;

		if (factCache.find(n) != factCache.end())
			return factCache[n];

		uint64_t result = n;
		for (int i = n - 1; i > 1; --i) {
			result *= i;
		}
		return factCache[n] = result;
	}

	uint64_t countPermutations(int p, int n) {
		return factorial(p) / factorial(p - n);
	}

	bool visitNode(PuzzleState& puzzle,
		unordered_set<string>& visited,
		countMap& closedSet,
		const vector<int>& groupWithBlank,
		const vector<int>& group
	) {
		string puzzleHashWithBlank = puzzle.hashGroup(groupWithBlank);
		if (visited.count(puzzleHashWithBlank))
			return false;

		visited.insert(puzzleHashWithBlank);

		string groupHash = puzzle.hashGroup(group);
		if (closedSet.contains(groupHash) == false)
			closedSet[groupHash] = puzzle.g;
		else if (closedSet[groupHash] > puzzle.g)
			closedSet[groupHash] = puzzle.g;

		return true;
	}

	countMap generateDb(int boardSize, const vector<int>& group, int groupNum) {
		const int boardLen = pow(boardSize, 2);

		PuzzleState puzzle;
		puzzle.g = 0;

		vector<int> groupWithBlank = group;
		groupWithBlank.emplace(groupWithBlank.begin(), 0);

		unordered_set<string> visited; // Permutation of group tiles + blank tile locations visited
		countMap closedSet; // Permutation of group tile locations with min move count so far
		deque<pair<PuzzleState, Vector2i>> openSet; // Next permutations to visit
		uint64_t i = 0;
		uint64_t max_iters = countPermutations(boardLen, groupWithBlank.size());
		float t1 = Time::GetTime();
		float totalTimeStart = t1;

		openSet.push_back(make_pair(puzzle, Vector2i()));

		while (not openSet.empty()) {
			auto [cur, prevMove] = openSet.front();
			openSet.pop_front();

			if (not visitNode(cur,
				visited,
				closedSet,
				groupWithBlank,
				group)) {
				continue;
			}

			for (auto& dir : solver::directions) {
				if (dir == prevMove) continue;

				PuzzleState simPuzzle = cur;
				if (not simPuzzle.doMove(dir)) {
					continue;
				}

				if (count(group.begin(), group.end(), simPuzzle.puzzle[cur.blankIndex]) > 0) {
					simPuzzle.g += 1;
				}

				openSet.push_back(make_pair(simPuzzle, -dir));
			}

			i++;

			if (i % 100000 == 0) {
				float t2 = Time::GetTime();
				float elapsed = t2 - t1;

				stringstream s;
				s.imbue(locale(""));
				s << "Group " << groupNum << "; Iteration " << fixed << i << " of " << fixed << max_iters << ", time elapsed: " << elapsed << " s" << endl;
				s << "Size of closed list: " << fixed << closedSet.size() << endl;
				s << "Size of open list: " << fixed << openSet.size() << endl;
				Logger::Log(s.str().c_str());

				t1 = t2;
			}
		}

		float totalTime = Time::GetTime() - totalTimeStart;
		Logger::LogFormat("Finished group %d in a total of %f seconds", groupNum, totalTime);

		return closedSet;
	}

	/// <summary>
	/// Call this function to generate pattern database file
	/// </summary>
	/// <param name="boardSize"></param>
	void threadedGen() {
		ofstream patternDbFile(solver::patternFilePath, ios::out);

		if (!patternDbFile.is_open()) {
			Logger::LogError("Could not open file for PatternDB generating.");
			return;
		}

		// If you have less than or 16 GB of memory use 555 grouping
		// Please close any programs as to increase space for memory allocation
		// Release build is recommended for faster generation times

		// 555 - 5,765,760 iterations for each group of 5, smallest time and lowest memory usage (~2 GB)
		const vector<vector<int>>groups = { {1, 2, 3, 4, 7}, { 5,6,9,10,13 }, { 8,11,12,14,15 } };

		// 663 - 57,657,600 iterations for each group of 6, medium time and high memory usage (>10 GB)
		//const vector<vector<int>> groups = { {1, 5, 6, 9, 10, 13}, { 7,8,11,12,14,15 }, { 2,3,4 } };

		// 78 - 4,151,347,200 iterations for group of 8, highest time and memory usage (?? GB)
		//const vector<vector<int>> groups = { {1, 2, 3, 4, 5, 6, 7, 8}, { 9,10,11,12,13,14,15 } };


		vector<future<countMap>> results;
		for (int i = 0; i < groups.size(); ++i) {
			results.push_back(async(launch::async, generateDb, puzzleSize, groups[i], i));
		}

		boost::archive::text_oarchive oa(patternDbFile);

		oa << groups;
		vector<countMap> closedList;
		for (auto& res : results) {
			closedList.push_back(res.get());
		}
		oa << closedList;

		for (size_t i = 0; i < closedList.size(); i++)
		{
			auto& map = closedList[i];
			stringstream ss;
			ss << "Group: [";
			int j = 0;
			for (size_t j = 0; j < groups[i].size(); ++j)
			{
				ss << groups[i][j];
				if (j != groups[i].size() - 1)
					ss << ", ";
			}
			ss << "] " << map.size() << " permutations";
			Logger::Log(ss.str().c_str());
		}

		patternDbFile.close();
	}
}

#endif