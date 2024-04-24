#include "ECS/components/camera.h"
#include "game.h"
#include "math/mathf.h"
#include "patternGen.hpp"
#include "rendering/shapes/textShape.h"
#include "solver.hpp"
#include "textureManager.h"
#include "timing.h"

#include <algorithm>
#include <functional>
#include <iomanip>
#include <random>

constexpr int SECONDS_PER_HOUR = 3600;
constexpr int SECONDS_PER_MINUTE = 60;

class PuzzleGame : Game
{
	enum GameState : char {
		GAME_RUNNING,
		GAME_STOPPED,
		GAME_OVER
	};

	GameState state = GameState::GAME_STOPPED;

	size_t boardSize = 4;
	std::vector<int> board;
	size_t hoveredTile = -1;
	size_t emptyTile;

	SDL_Texture* puzzleImage = nullptr;
	Vector2<int> imageSize, tileSize;
	std::string pathToLoad;
	std::string steps;
	float scale = 1.f;
	bool showImage = false;

	std::vector<std::tuple<float, Vector2f, size_t>> tileLerpQueue;
	float interpSpeed = 10.f;
	TextShape* text = nullptr;
	float startingTime, finishingTime = 0;

protected:
#pragma region Game Loop

	void Start() override
	{
		int w, h;

		if (puzzleImage == nullptr && !pathToLoad.empty())
			SetPuzzleImage(pathToLoad.c_str());
		else
			Logger::LogError("Could not initialize board because image is null and no path to it was specified");

		SDL_QueryTexture(puzzleImage, 0, 0, &w, &h);
		imageSize.setValues(w, h);

		if (w % boardSize != 0 || h % boardSize != 0)
		{
			Logger::LogWarning("Size of image for puzzle is not divisible by boardSize, this might cause incorrect tile sizes");
		}

		const int width = w / static_cast<float>(boardSize);
		const int height = h / static_cast<float>(boardSize);

		tileSize.setValues(width, height);

		scale = sizeToFit / std::fminf(w, h);

		window->SetSize(w * scale, h * scale);

		LoopBoard([&](int i)
			{
				board.push_back(i + 1);
			});
		board[board.size() - 1] = 0;

		Scramble();
		emptyTile = FindEmptyIndex();

		Entity* cam = &GetManager()->AddEntity("Camera");
		cam->transform->setPosition(window->GetSize() * 0.5f);
		cam->AddComponent<Camera>();
	}

	void Update(const float _dt) override
	{
		HandleTileInterpolation(_dt);

		if (input.GetKeyDown(Input::KEY_H)) {
			if (state == GAME_STOPPED) {
				state = GAME_RUNNING;
				startingTime = Time::GetTime();
			}

			if (steps.length() == 0) {
				if (CheckIfSolved()) return;

				std::array<int, 16> copy{};
				std::copy(board.begin(), board.end(), copy.begin());
				PuzzleState inicial{ copy, (int)emptyTile };
				steps = solver::solvePuzzle(inicial);
			}
			else {
				const char& c = steps.front();
				Vector2zu blankPos = To2DIndex(emptyTile);
				Vector2zu newPos = blankPos;
				int newIndex = -1;

				if (c == 'U' && blankPos.y > 0) {
					newPos.y -= 1;
				}
				else if (c == 'D' && blankPos.y < boardSize - 1) {
					newPos.y += 1;
				}
				else if (c == 'L' && blankPos.x > 0) {
					newPos.x -= 1;
				}
				else if (c == 'R' && blankPos.x < boardSize - 1) {
					newPos.x += 1;
				}
				else {
					Logger::LogWarning("Invalid movement attempted, ignoring.");
				}
				newIndex = FlatIndex(newPos);

				steps.erase(steps.begin());

				if (newIndex > -1) {
					std::swap(board[emptyTile], board[newIndex]);
					emptyTile = newIndex;
					// TODO: Make this work :P
					/*Vector2i dir;
					dir.x = std::clamp(int(newPos.x) - int(blankPos.x), -1, 1);
					dir.y = std::clamp(int(newPos.y) - int(blankPos.y), -1, 1);

					tileLerpQueue.push_back(std::make_tuple(0.f, blankPos + dir, newIndex));*/
				}
			}
		}


		if (input.GetKeyDown(Input::KEY_R)) {
			Scramble();
			emptyTile = FindEmptyIndex();
			steps = "";
			state = GAME_STOPPED;
			finishingTime = 0.f;
			if (text) {
				text->autoDestroy = true;
				text = nullptr;
			}
		}

		showImage = input.GetKey(Input::KEY_X);

		if (hoveredTile == -1 || showImage || !input.GetMouseDown(Input::MOUSE_LEFT_BUTTON))
			return;

		// Solution is lost if board is changed
		steps = "";

		HandleTileClick();

		if (CheckIfSolved())
			state = GAME_OVER;
	}

	void Draw() override
	{
		if (showImage)
		{
			auto rect = SDL_Rect{ 0, 0, Mathf::RoundToInt(imageSize.x * scale), Mathf::RoundToInt(imageSize.y * scale) };
			SDL_RenderCopy(window->GetRenderer(), puzzleImage, NULL, &rect);
		}
		else
			DrawTiles();

		if (state == GAME_OVER)
		{
			if (finishingTime == 0.f) finishingTime = Time::GetTime();

			float elapsedTime = finishingTime - startingTime;
			auto msg = ("You won!\nElapsed time: " + FormatTime(elapsedTime));
			if (text == nullptr)
			{
				text = GetSorter()->AddText(
					{ GetWindow()->GetSize() * 0.5f, { 0.f , 0.f } },
					msg,
					"Resources/arial.ttf",
					16,
					{ 255, 255, 0, 255 }
				);
				return;
			}

			text->text = msg;
		}
	}

#pragma endregion

#pragma region Board Methods
	void Scramble() {
		std::random_device rd;
		std::mt19937 engine(rd());

		do {
			Logger::LogSystem("Scrambling...");
			std::shuffle(board.begin(), board.end(), engine);
		} while (!IsSolvable());
	}

	// https://stackoverflow.com/questions/34570344/check-if-15-puzzle-is-solvable
	bool IsSolvable() {
		int parity = 0;
		int row = 0;
		int blankRow = 0;

		for (int i = 0; i < board.size(); i++)
		{
			if (i % boardSize == 0) { // advance to next row
				row++;
			}
			if (board[i] == 0) {
				blankRow = row; // save the row on which encountered
				continue;
			}
			for (int j = i + 1; j < board.size(); j++)
			{
				if (board[i] > board[j] && board[j] != 0)
				{
					parity++;
				}
			}
		}

		if (boardSize % 2 == 0) {
			if (blankRow % 2 == 0) { // blank on odd row; counting from bottom
				return parity % 2 == 0;
			}
			else { // blank on even row; counting from bottom
				return parity % 2 != 0;
			}
		}
		else {
			return parity % 2 == 0;
		}
	}

	bool CheckIfSolved() {
		for (int i = 0; i < board.size(); i++) {
			if (board[i] != i + 1 && board[i] != 0)
			{
				return false;
			}
		}
		return true;
	}
#pragma endregion

#pragma region Tile Methods

	bool IsValidMove(const Vector2zu& position) const
	{
		Vector2<size_t> emptyPos = To2DIndex(emptyTile);

		if (position.x == emptyPos.x || position.y == emptyPos.y) {
			return true;
		}

		return false;
	}

	void LoopBoard(std::function<void(int, int, bool&)> loop) const
	{
		bool shouldBreak = false;
		for (int y = 0; y < boardSize; y++) {
			for (int x = 0; x < boardSize; x++) {
				loop(x, y, shouldBreak);
				if (shouldBreak) return;
			}
		}
	}

	void LoopBoard(std::function<void(int)> loop) const
	{
		for (int i = 0; i < boardSize * boardSize; i++) {
			loop(i);
		}
	}

	size_t FindEmptyIndex() const
	{
		for (size_t i = 0; i < board.size(); i++)
		{
			if (board[i] == 0)
				return i;
		}
		// Should not happen
		return -1;
	}

	FORCEINLINE void HandleTileInterpolation(const float _dt)
	{
		if (tileLerpQueue.empty() == false)
		{
			for (size_t i = 0; i < tileLerpQueue.size(); i++)
			{
				std::get<0>(tileLerpQueue[i]) += _dt * interpSpeed;

				if (std::get<0>(tileLerpQueue[i]) >= 1.f) {
					tileLerpQueue.erase(tileLerpQueue.begin() + i);
				}
			}
		}
	}

	FORCEINLINE void HandleTileClick()
	{
		Vector2zu clickedPos = To2DIndex(hoveredTile);
		Vector2zu emptyPos = To2DIndex(emptyTile);
		if (IsValidMove(clickedPos))
		{
			if (state == GAME_STOPPED)
			{
				startingTime = Time::GetTime();
				state = GAME_RUNNING;
			}

			size_t axis = emptyPos.y == clickedPos.y ? 0 : 1; // 0: Horizontal; 1: Vertical
			int direction = std::clamp(int(clickedPos[axis]) - int(emptyPos[axis]), -1, 1);

			// Loop from the empty tile towards the clicked tile, swapping tiles in the way
			for (size_t i = emptyPos[axis]; i != clickedPos[axis] + direction; i += direction)
			{
				size_t idx;
				if (axis == 0)
					idx = i * boardSize + emptyPos.y;
				else
					idx = emptyPos.x * boardSize + i;

				std::swap(board[idx], board[emptyTile]);
				emptyTile = idx;
				emptyPos = To2DIndex(emptyTile);
				// We guess where it was based on direction
				Vector2zu previousPos = emptyPos;
				previousPos[axis] += direction;

				tileLerpQueue.emplace_back(std::make_tuple(0.f, previousPos, idx));
			}
		}
	}

	void DrawTiles()
	{
		bool hasHovered = false;
		LoopBoard([&](int i)
			{
				if (i == emptyTile) return;
				const int x = i / boardSize;
				const int y = i % boardSize;

				const int w = tileSize.x * scale;
				const int h = tileSize.y * scale;

				SDL_Rect slice{
					(board[i] - 1) / int(boardSize) * tileSize.x,
					(board[i] - 1) % int(boardSize) * tileSize.y,
					tileSize.x,
					tileSize.y
				};
				SDL_Rect rect{ x * w, y * h, w, h };

				bool needsInterp = false;
				for (size_t j = 0; j < tileLerpQueue.size(); j++)
				{
					const auto& tileToLerp = tileLerpQueue[j];
					if (std::get<2>(tileToLerp) == i)
					{
						rect.x = std::lerp(std::get<1>(tileToLerp).x * w, x * w, std::get<0>(tileLerpQueue[j]));
						rect.y = std::lerp(std::get<1>(tileToLerp).y * h, y * h, std::get<0>(tileLerpQueue[j]));
						needsInterp = true;
						break;
					}
				}

				const auto renderer = window->GetRenderer();

				SDL_RenderCopy(renderer,
					puzzleImage,
					&slice,
					&rect
				);

				if (!needsInterp && Rect::Contains(input.GetMousePosition(), rect))
				{
					hoveredTile = i;
					hasHovered = true;

					SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
					SDL_RenderDrawRect(renderer, &rect);
				}
			});

		if (hasHovered == false && hoveredTile != -1)
			hoveredTile = -1;
	}
#pragma endregion

#pragma region Utility
	FORCEINLINE size_t FlatIndex(const Vector2zu& pos) const noexcept
	{
		return pos.x * boardSize + pos.y;
	}

	FORCEINLINE size_t FlatIndex(size_t x, size_t y) const noexcept
	{
		return x * boardSize + y;
	}

	FORCEINLINE const Vector2zu To2DIndex(size_t i) const noexcept
	{
		return { i / boardSize, i % boardSize };
	}

	std::string FormatTime(float seconds)
	{
		int hours = seconds / SECONDS_PER_HOUR;
		seconds -= hours * SECONDS_PER_HOUR;

		int minutes = seconds / SECONDS_PER_MINUTE;
		seconds -= minutes * SECONDS_PER_MINUTE;

		std::ostringstream result;
		if (hours > 0) {
			result << hours << " hour" << (hours != 1 ? "s" : "") << ", ";
		}
		if (minutes > 0) {
			result << minutes << " minute" << (minutes != 1 ? "s" : "") << " and ";
		}
		seconds = std::floor(seconds);
		result << seconds << " second" << (seconds != 1 ? "s" : "");

		return result.str();
	}
#pragma endregion

public:
	int sizeToFit = 256;

	PuzzleGame(const char* puzzleImgPath)
	{
		pathToLoad = puzzleImgPath;
	}

	void SetPuzzleImage(const char* filePath)
	{
		puzzleImage = TextureManager::Instance().LoadFromFile(filePath);
	}
};