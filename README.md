# 15-Puzzle
An attempt of using my incomplete 2D engine to make a simple game,
includes two solving implementations, an A* and an IDA* algorithm.

## Controls

- X to show the original/solved image
- R to scramble the puzzle
- H to solve the puzzle

### Choosing Algorithm
`src/puzzle/solver.hpp` has a "USE_ASTAR" macro. It is 0 by default.  
A* might be a bit slower and use more memory than compared to IDA*, but either can be used freely

### Changing Puzzle Size
`src/puzzle/puzzleState.hpp` has a constexpr "puzzleSize" variable you can set to whatever width you want  
However you'll need to replace the image and, if you need the AI solver, generate a new pattern database for that width. See [Generating Pattern Database](#generating-pattern-database) below  
You can use it with a renamed "patternDb_4.dat", it will resort to using Manhattan distance


### Generating Pattern Database
You must include `src/puzzle/patternGen.hpp` in a file (can be main) and call `patternDb::threadedGen()`  
If you changed the width of the puzzle you must create new groups for the new puzzle  
Take for instance the already existing pattern groups for a 4x4 board:  
```cpp
// 555 - 5,765,760 iterations for each group of 5, smallest time and lowest memory usage (~2 GB)
const vector<vector<int>>groups = { {1, 2, 3, 4, 7}, { 5,6,9,10,13 }, { 8,11,12,14,15 } };

// 663 - 57,657,600 iterations for each group of 6, medium time and high memory usage (>10 GB)
const vector<vector<int>> groups = { {1, 5, 6, 9, 10, 13}, { 7,8,11,12,14,15 }, { 2,3,4 } };

// 78 - 4,151,347,200 iterations for group of 8, highest time and memory usage (?? GB)
const vector<vector<int>> groups = { {1, 2, 3, 4, 5, 6, 7, 8}, { 9,10,11,12,13,14,15 } };
```

## Building
W.I.P