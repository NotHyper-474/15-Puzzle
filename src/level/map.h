#pragma once

#include <vector>
#include <SDL.h>
#include "ECS/components/camera.h"

struct Tile
{
	Tile() : mBox()
	{
	}

	Tile(int x, int y, int width, int height)
	{
		mBox = { float(x) * width, float(y) * height, float(width), (float)height };
	}

	int type;

	Rect& GetBox() { return mBox; }
private:
	Rect mBox;
};

class Map
{
public:
	Map(Uint32 width, Uint32 height);
	~Map();

	void LoadMap();
	void LoadMap(const char* file);
	void SaveMap(const char* file);
	void Draw();

	Camera* camera;

#pragma region Properties
	__property(GetWidth) Uint32 Width;
	__property(GetHeight) Uint32 Height;
#pragma endregion

private:
	std::vector<Tile> tiles;
	SDL_Texture* tilesetTexture;
	Uint32 width, height;

	FORCEINLINE Uint32 GetWidth() const { return width; }
	FORCEINLINE Uint32 GetHeight() const { return height; }
};