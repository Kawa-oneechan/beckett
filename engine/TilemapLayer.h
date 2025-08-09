#pragma once

#include "Tickable.h"
#include "Texture.h"
#include "JsonUtils.h"

class TilemapLayer : public Tickable
{
private:
	std::shared_ptr<Texture> tilesetTexture{ nullptr };
	int width{ 0 };
	int height{ 0 };
	int tileWidth{ 0 };
	int tileHeight{ 0 };
	int tileGridWidth{ 0 };
	int tileGridHeight{ 0 };
	int tileOffsetX{ 0 };
	int tileOffsetY{ 0 };
	int tilesPerLine{ 0 };
	bool isometric{ false };
	std::string layerName;
	std::unique_ptr<int[]> data{ nullptr };

public:
	TilemapLayer() = default;
	TilemapLayer(jsonValue& doc, const std::string& source, const std::string& layer);
	void Draw(float dt);
	void SetTile(int row, int col, int tile);

	float Scale{ -1.0f };
};

using TilemapLayerP = std::shared_ptr<TilemapLayer>;

//Provides as easier means to load a multi-layer map.
//To properly use, place this somewhere in the tickable tree, then add the component layers
//to the tree in their proper order so you can render sprites and such inbetween.
class TilemapManager : public Tickable
{
private:
	std::vector<TilemapLayer> layers;

public:
	glm::vec2 Camera;
	float Scale{ -1.0f };

	TilemapManager(const std::string& source);
	void Draw(float dt) {}
	bool Tick(float);
	
	TilemapLayer& operator[](size_t i);
	void TilemapManager::SetTile(int row, int col, int tile);
	void TilemapManager::SetTile(int row, int col, std::initializer_list<int> tiles);
};
