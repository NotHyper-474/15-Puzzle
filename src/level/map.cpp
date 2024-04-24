#include "map.h"
#include "core/game.h"
#include "core/globals.h"
#include "core/textureManager.h"
#include "physics/collision.hpp"

#include <fstream>


SDL_Texture* grassTexture;

Map::Map(Uint32 width, Uint32 height) : camera(nullptr)
{
	this->width = width;
	this->height = height;
	tiles = std::vector<Tile>(width * height);
	for (Uint32 x = 0; x < width; x++)
	{
		for (Uint32 y = 0; y < height; y++)
		{
			tiles[((size_t)x * height) + y] = Tile(
				x, y,
				8, 8);
			tiles[((size_t)x * height) + y].type = rand() % 5;
		}
	}
	tilesetTexture = TextureManager::Instance().LoadFromFile("Resources/level01.png");
	grassTexture = TextureManager::Instance().LoadFromFile("Resources/grass.png");
}

Map::~Map() = default;

void Map::LoadMap()
{

}

void Map::SaveMap(const char* file)
{
	if (tiles.size() == 0)
	{
		Logger::LogWarning("Tile data is empty. Ignoring...");
		return;
	}
	std::ofstream stream{ file, std::ios_base::binary };
	if (stream.fail())
	{
		Logger::LogErrorFormat("Could not create file '%s' for map.", file);
		return;
	}
	std::vector<unsigned char> buf;
	
}

void Map::LoadMap(const char* file)
{
	std::ifstream f(file, std::ifstream::in | std::ifstream::binary);
	tiles.clear();
	if (!f.is_open())
	{
		printf("Could not open '%s'\n", file);
		return;
	}
	int i = 0;
	if (f.peek() == EOF)
	{
		printf("File '%s' is empty.\n", file);
		return;
	}

	char size[4];
	f.read(size, sizeof(size));
	width = static_cast<Uint32>(std::stoul(size));

	f.read(size, sizeof(size));
	height = static_cast<Uint32>(std::stoul(size));
	tiles = std::vector<Tile>(width * height);
	while (!f.eof())
	{
		char c = f.get();

		if (c == EOF)
			break;
		
		++i;
	}

	f.close();
}

void Map::Draw()
{
	Rect dst =
	{
		{10, 10},
		Game::GetWindow()->GetSize()
	};
	dst.xMax -= 10.f;
	dst.yMax -= 10.f;
	auto renderer = Game::GetWindow()->GetRenderer();
	for (size_t i = 0; i < tiles.size(); i++)
	{
		Rect r = tiles[i].GetBox();
		r.position -= camera->getProjection();
		if (SDL_HasIntersectionF(&r.getRectF(), &dst.getRectF()) == SDL_TRUE)
		{
			SDL_SetRenderDrawColor(renderer, tiles[i].type * 100, tiles[i].type * 128, tiles[i].type * 24, SDL_ALPHA_OPAQUE);
			SDL_RenderFillRectF(renderer, &r.getRectF());
		}
	}
	//SDL_RenderCopyF(Game::GetWindow()->GetRenderer(), tilesetTexture, NULL, reinterpret_cast<const SDL_FRect*>(&dst));
}