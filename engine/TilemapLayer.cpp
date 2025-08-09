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
			height = lo["height"].as_integer();
			
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
	std::string here;
	{
		auto slashPos = source.rfind('/');
		if (slashPos != std::string::npos)
			here = source.substr(0, slashPos + 1);
	}
	auto ts = obj["tilesets"].as_array()[0].as_object();
	if (ts["source"].is_string())
	{
		auto tsp = here + ts["source"].as_string();
		tsp = ResolvePath(tsp);
		ts = VFS::ReadJSON(tsp).as_object();
		auto slashPos = tsp.rfind('/');
		if (slashPos != std::string::npos)
			here = tsp.substr(0, slashPos + 1);
	}
	{
		tileWidth = ts["tilewidth"].as_integer();
		tileHeight= ts["tileheight"].as_integer();

		tileGridWidth = tileWidth;
		tileGridHeight = tileHeight;

		auto& src = ts["image"].as_string();
		auto tsp = here + src;
		tsp = ResolvePath(tsp);
		tilesetTexture = std::make_shared<Texture>(tsp, GL_CLAMP, 0, true);

		tilesPerLine = tilesetTexture->width / tileWidth;

		if (isometric)
		{
			auto grid = ts["grid"].as_object();
			tileGridWidth = grid["width"].as_integer();
			tileGridHeight = grid["height"].as_integer();
			auto offset = ts["tileoffset"].as_object();
			tileOffset = glm::vec2(offset["x"].as_integer(), offset["y"].as_integer());
		}
	}
}

void TilemapLayer::Draw(float dt)
{
	dt;
	if (!data)
		return;

	float s = Scale > 0 ? Scale : scale;
	
	auto top = (int)(Camera.y / tileGridHeight);
	if (top < 0) top = 0;
	auto bottom = top + (int)((SCREENHEIGHT / tileGridHeight) / s) + 1;
	if (bottom > height) bottom = height;
	auto left = (int)(Camera.x / tileGridWidth);
	if (left < 0) left = 0;
	auto right = left + (int)((SCREENWIDTH / tileGridWidth) / s) + 1;
	if (right > width) right = width;


	const auto tileSize = glm::vec2(tileWidth, tileHeight) * s;
	for (auto row = top; row < bottom; row++)
	{
		for (auto col = left; col < right; col++)
		{
			auto tile = data[row * width + col];
			if (tile == -1)
				continue;

			auto srcX = (tile % tilesPerLine) * tileWidth;
			auto srcY = (tile / tilesPerLine) * tileHeight;
			auto srcRect = glm::vec4(srcX, srcY, tileWidth, tileHeight);

			auto dest = !isometric ?
				glm::vec2(col * tileWidth, row * tileHeight) :
				glm::vec2(
					((col - row) * (tileGridWidth / 2)) - (tileWidth / 2),
					((row + col) * (tileGridHeight / 2))
				);

			Sprite::DrawSprite(*tilesetTexture, (dest - Camera - tileOffset) * s, tileSize, srcRect);
		}
	}
}

void TilemapLayer::SetTile(int row, int col, int tile)
{
	col = glm::clamp(col, 0, width - 1);
	row = glm::clamp(row, 0, height - 1);
	data[(row * width) + col] = tile;
}

const int TilemapLayer::GetTile(int row, int col) const
{
	col = glm::clamp(col, 0, width - 1);
	row = glm::clamp(row, 0, height - 1);
	return data[(row * width) + col];
}

glm::vec2 TilemapLayer::GetPixelSize()
{
	auto ret = glm::vec2(width * tileGridWidth, height * tileGridHeight);
	return ret;
}

glm::vec2 TilemapLayer::GetTileSize()
{
	auto ret = glm::vec2(width, height);
	return ret;
}

TilemapManager::TilemapManager(const std::string& source)
{
	auto doc = VFS::ReadJSON(source);
	auto obj = doc.as_object();
	for (auto& l : obj["layers"].as_array())
	{
		auto lo = l.as_object();
		if (lo["type"].as_string() != "tilelayer")
			continue;
		auto ln = lo["name"].as_string();
		layers.push_back(std::make_shared<TilemapLayer>(doc, source, ln));
	}

	Camera = glm::vec2(0);
}

bool TilemapManager::Tick(float dt)
{
	dt;
	for (auto& l : layers)
	{
		l->Scale = Scale;
		l->Camera = Camera;
	}
	return true;
}

TilemapLayerP TilemapManager::operator[](size_t i)
{
	if (i < layers.size()) return layers[i];
	return layers[0];
}

TilemapLayerP TilemapManager::GetLayer(size_t i)
{
	if (i < layers.size()) return layers[i];
	return layers[0];
}

void TilemapManager::SetTile(int row, int col, int tile)
{
	for (auto& l : layers)
		l->SetTile(row, col, tile);
}

void TilemapManager::SetTile(int row, int col, std::initializer_list<int> tiles)
{
	for (int i = 0; i < layers.size() && i < tiles.size(); i++)
	{
		auto t = *(tiles.begin() + i);
		if (t == -2)
			continue;
		layers[i]->SetTile(row, col, t);
	}
}

const int TilemapManager::GetTile(int layer, int row, int col) const
{
	return layers[layer]->GetTile(row, col);
}

glm::vec2 TilemapManager::GetPixelSize()
{
	auto ret = layers[0]->GetPixelSize();
	return ret;
}

glm::vec2 TilemapManager::GetTileSize()
{
	auto ret = layers[0]->GetTileSize();
	return ret;
}
