#include "TilemapLayer.h"
#include "TextUtils.h"
#include "SpriteRenderer.h"
#include "../support/format.h"
#include "../Game.h"

extern float scale;

TilemapLayer::TilemapLayer(jsonValue& doc, const std::string& source, const std::string& layer)
{
	auto obj = doc.as_object();

	isometric = obj["orientation"].as_string() == "isometric";

	for (auto& l : obj["layers"].as_array())
	{
		auto lo = l.as_object();
		if (lo["name"].as_string() == layer)
		{
			layerName = layer;
			width = lo["width"].as_integer();
			height = lo["width"].as_integer();
			
			auto totalSize = this->width * this->height;

			data = std::make_unique<int[]>(totalSize);
			int i = 0;
			for (auto& di : lo["data"].as_array())
			{
				data[i++] = di.as_integer() - 1;
			}
			break;
		}
	}

	if (data == nullptr)
		throw std::exception("Couldn't find specified layer.");

	//Now handle the tileset.
	//We only support the one, and it has to be "from sheet".
	//TODO: support external tileset files.
	auto ts = obj["tilesets"].as_array()[0].as_object();
	{
		tileWidth = ts["tilewidth"].as_integer();
		tileHeight= ts["tileheight"].as_integer();

		auto src = ts["image"].as_string();
		std::string here;
		auto slashPos = source.rfind('/');
		if (slashPos != std::string::npos)
			here = source.substr(0, slashPos + 1);
		auto tsp = here + src;
		tsp = ResolvePath(tsp);
		tilesetTexture = std::make_shared<Texture>(tsp);

		tilesPerLine = tilesetTexture->width / tileWidth;

		if (isometric)
		{
			auto grid = ts["grid"].as_object();
			tileGridWidth = grid["width"].as_integer();
			tileGridHeight = grid["height"].as_integer();
			auto offset = ts["tileoffset"].as_object();
			tileOffsetX = offset["x"].as_integer();
			tileOffsetY = offset["y"].as_integer();
		}
	}
}

void TilemapLayer::Draw(float dt)
{
	dt;
	if (!data)
		return;

	int i = 0;
	float s = Scale > 0 ? Scale : scale;
	
	const auto tileSize = glm::vec2(tileWidth, tileHeight) * s;
	for (auto row = 0; row < height; row++)
	{
		for (auto col = 0; col < width; col++)
		{
			auto tile = data[i++];
			if (tile == -1)
				continue;

			auto srcX = (tile % tilesPerLine) * tileWidth;
			auto srcY = (tile / tilesPerLine) * tileHeight;
			auto srcRect = glm::vec4(srcX, srcY, tileWidth, tileHeight);

			if (!isometric)
			{
				Sprite::DrawSprite(*tilesetTexture, glm::vec2(col * tileWidth, row * tileHeight) * s, tileSize, srcRect);
			}
			else
			{
				//TODO: use tileOffsetX/Y.
				const auto tgw2 = tileGridWidth / 2;
				const auto tgh2 = tileGridHeight / 2;

				Sprite::DrawSprite(*tilesetTexture,
					glm::vec2(
						((col - row) * tgw2) - (tileWidth / 2),
						(row + col) * tgh2
					) * s,
					tileSize, srcRect);
			}
		}
	}
}

void TilemapLayer::SetTile(int row, int col, int tile)
{
	col = glm::clamp(col, 0, width - 1);
	row = glm::clamp(row, 0, height - 1);
	data[(row * width) + col] = tile;
}

TilemapManager::TilemapManager(const std::string& source)
{
	auto doc = VFS::ReadJSON(source);
	auto obj = doc.as_object();
	for (auto& l : obj["layers"].as_array())
	{
		auto lo = l.as_object();
		auto ln = lo["name"].as_string();
		layers.emplace_back(doc, source, ln);
	}
}

bool TilemapManager::Tick(float dt)
{
	dt;
	for (auto& l : layers)
	{
		l.Scale = Scale;
		//l.Camera = Camera;
	}
	return true;
}

TilemapLayer& TilemapManager::operator[](size_t i)
{
	if (i < layers.size()) return layers[i];
	return layers[0];
}

void TilemapManager::SetTile(int row, int col, int tile)
{
	for (auto& l : layers)
		l.SetTile(row, col, tile);
}

void TilemapManager::SetTile(int row, int col, std::initializer_list<int> tiles)
{
	for (int i = 0; i < layers.size() && i < tiles.size(); i++)
	{
		auto t = *(tiles.begin() + i);
		if (t == -2)
			continue;
		layers[i].SetTile(row, col, t);
	}
}