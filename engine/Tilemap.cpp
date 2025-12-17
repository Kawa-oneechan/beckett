#include "Tilemap.h"
#include "TextUtils.h"
#include "VFS.h"
#include "SpriteRenderer.h"
#include "../Game.h"

extern float scale;

Tilemap::MapLayer::MapLayer(jsonValue& doc, Tilemap* owner) : owner(owner)
{
	auto lo = doc.as_object();

	layerName = lo["name"].as_string();
	width = lo["width"].as_integer();
	height = lo["height"].as_integer();

	Parallax = glm::vec2(1);
	if (lo["parallaxx"].is_number())
		Parallax.x = lo["parallaxx"].as_number();
	if (lo["parallaxy"].is_number())
		Parallax.y = lo["parallaxy"].as_number();

	if (lo["tintcolor"].is_string())
		Tint = GetJSONColor(lo["tintcolor"]);
	if (lo["opacity"].is_number())
		Tint.a *= lo["opacity"].as_number();

	auto totalSize = this->width * this->height;

	data = std::make_unique<int[]>(totalSize);
	int i = 0;
	for (const auto& di : lo["data"].as_array())
	{
		data[i++] = di.as_integer() - 1;
	}
}

void Tilemap::MapLayer::Draw(float dt)
{
	(void)(dt);
	if (!data)
		return;

	float s = Scale > 0 ? Scale : scale;

	auto cam = Camera * Parallax;

	const auto& tiles = owner->tileset;

	auto top = (int)(cam.y / tiles.tileGridHeight);
	if (top < 0) top = 0;
	auto bottom = top + (int)((BECKETT_SCREENHEIGHT / tiles.tileGridHeight) / s) + 1;
	if (bottom > height) bottom = height;
	auto left = (int)(cam.x / tiles.tileGridWidth);
	if (left < 0) left = 0;
	auto right = left + (int)((BECKETT_SCREENWIDTH / tiles.tileGridWidth) / s) + 1;
	if (right > width) right = width;

	const auto tileSize = glm::vec2(tiles.tileWidth, tiles.tileHeight) * s;
	for (auto row = top; row < bottom; row++)
	{
		for (auto col = left; col < right; col++)
		{
			auto tile = data[row * width + col];
			if (tile == -1)
				continue;

			auto anim = tiles.animations.find(tile);
			if (anim != tiles.animations.end())
				tile = anim->second.tile[anim->second.currentFrame];

			auto srcX = (tile % tiles.tilesPerLine) * tiles.tileWidth;
			auto srcY = (tile / tiles.tilesPerLine) * tiles.tileHeight;
			auto srcRect = glm::vec4(srcX, srcY, tiles.tileWidth, tiles.tileHeight);

			auto dest = !owner->isometric ?
				glm::vec2(col * tiles.tileWidth, row * tiles.tileHeight) :
				glm::vec2(
				((col - row) * (tiles.tileGridWidth / 2)) - (tiles.tileWidth / 2),
					((row + col) * (tiles.tileGridHeight / 2))
				);

			Sprite::DrawSprite(*tiles.texture, ((dest - cam - tiles.tileOffset) * s) + owner->Position, tileSize, srcRect, 0.0, Tint);
		}
	}
}

void Tilemap::MapLayer::SetTile(int row, int col, int tile)
{
	col = glm::clamp(col, 0, width - 1);
	row = glm::clamp(row, 0, height - 1);
	data[(row * width) + col] = tile;
}

const int Tilemap::MapLayer::GetTile(int row, int col) const
{
	col = glm::clamp(col, 0, width - 1);
	row = glm::clamp(row, 0, height - 1);
	return data[(row * width) + col];
}

const int Tilemap::MapLayer::GetTile(const glm::vec2& position) const
{
	auto pos = position / Scale;
	pos += Camera;
	pos.x /= owner->tileset.tileWidth;
	pos.y /= owner->tileset.tileHeight;
	return GetTile((int)pos.y, (int)pos.x);
}

glm::vec2 Tilemap::MapLayer::GetPixelSize()
{
	auto ret = glm::vec2(width * owner->tileset.tileGridWidth, height * owner->tileset.tileGridHeight);
	return ret;
}

glm::vec2 Tilemap::MapLayer::GetTileSize()
{
	auto ret = glm::vec2(width, height);
	return ret;
}

glm::vec4 Tilemap::MapLayer::GetCollision(int row, int col)
{
	col = glm::clamp(col, 0, width - 1);
	row = glm::clamp(row, 0, height - 1);
	auto tile = owner->tileset.collisions.find(data[(row * width) + col]);
	if (tile != owner->tileset.collisions.end())
		return tile->second.rectP;
	return glm::vec4(-1);
}

Tilemap::Tilemap(const std::string& source)
{
	auto doc = VFS::ReadJSON(source);
	auto obj = doc.as_object();

	glm::vec2 tileSize;

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
		tileset.tileWidth = ts["tilewidth"].as_integer();
		tileset.tileHeight = ts["tileheight"].as_integer();

		tileset.tileGridWidth = tileset.tileWidth;
		tileset.tileGridHeight = tileset.tileHeight;

		tileSize = glm::vec2(tileset.tileWidth, tileset.tileHeight);

		auto& src = ts["image"].as_string();
		auto tsp = here + src;
		tsp = ResolvePath(tsp);
		tileset.texture = std::make_shared<Texture>(tsp, GL_CLAMP, 0, true);

		tileset.tilesPerLine = tileset.texture->width / tileset.tileWidth;

		if (isometric)
		{
			auto grid = ts["grid"].as_object();
			tileset.tileGridWidth = grid["width"].as_integer();
			tileset.tileGridHeight = grid["height"].as_integer();
			auto offset = ts["tileoffset"].as_object();
			tileset.tileOffset = glm::vec2(offset["x"].as_integer(), offset["y"].as_integer());
		}

		if (ts["tiles"].is_array())
		{
			for (auto t : ts["tiles"].as_array())
			{
				auto id = t.as_object()["id"].as_integer();
				if (t.as_object()["animation"].is_array())
				{
					auto anim = t.as_object()["animation"].as_array();
					auto& newAnim = tileset.animations[id];
					newAnim.frameCt = glm::clamp((int)anim.size(), 0, MaxAnimFrames);
					for (int f = 0; f < newAnim.frameCt; f++)
					{
						newAnim.tile[f] = anim[f].as_object()["tileid"].as_integer();
						newAnim.duration[f] = anim[f].as_object()["duration"].as_integer();
					}
					newAnim.durationLeft = newAnim.duration[0];
				}
				if (t.as_object()["objectgroup"].is_object())
				{
					//only one object per tile for now please
					auto ob = t.as_object()["objectgroup"].as_object()["objects"].as_array()[0].as_object();
					auto& newObj = tileset.collisions[id];
					newObj.name = ob["name"].as_string();
					newObj.type = ob["type"].as_string();
					auto pos = glm::vec2(ob["x"].as_number(), ob["y"].as_number());
					auto size = glm::vec2(ob["width"].as_number(), ob["height"].as_number());
					newObj.rectP = glm::vec4(pos, pos + size);
					newObj.rectT = glm::vec4(pos / tileSize, (pos + size) / tileSize);
				}
			}
		}
	}

	for (auto& l : obj["layers"].as_array())
	{
		auto lo = l.as_object();
		if (lo["type"].as_string() == "tilelayer")
			layers.push_back(std::make_shared<MapLayer>(l, this));
		if (lo["type"].as_string() == "objectgroup")
		{
			for (auto ob : lo["objects"].as_array())
			{
				auto o = ob.as_object();
				auto pos = glm::vec2(o["x"].as_number(), o["y"].as_number());
				auto size = glm::vec2(o["width"].as_number(), o["height"].as_number());
				Shape shp = {
					glm::vec4(pos, pos + size),
					glm::vec4(pos / tileSize, (pos + size) / tileSize),
					o["name"].as_string(),
					o["type"].as_string()
				};
				shapes.push_back(shp);
			}
		}
	}

	Camera = glm::vec2(0);
}

bool Tilemap::Tick(float dt)
{
	Tickable2D::Tick(dt);

	for (auto& a : tileset.animations)
	{
		a.second.durationLeft -= (int)(dt * 1000);
		if (a.second.durationLeft <= 0)
		{
			a.second.currentFrame++;
			if (a.second.currentFrame >= a.second.frameCt)
				a.second.currentFrame = 0;
			a.second.durationLeft = a.second.duration[a.second.currentFrame];
		}
	}

	for (const auto& l : layers)
	{
		l->Scale = Scale;
		l->Camera = Camera;
	}
	return true;
}

std::shared_ptr<Tilemap::MapLayer> Tilemap::operator[](size_t i) const
{
	if (i < layers.size()) return layers[i];
	return layers[0];
}

std::shared_ptr<Tilemap::MapLayer> Tilemap::GetLayer(size_t i)
{
	if (i < layers.size()) return layers[i];
	return layers[0];
}

void Tilemap::SetTile(int row, int col, int tile)
{
	for (auto& l : layers)
		l->SetTile(row, col, tile);
}

void Tilemap::SetTile(int row, int col, std::initializer_list<int> tiles)
{
	for (int i = 0; i < layers.size() && i < tiles.size(); i++)
	{
		auto t = *(tiles.begin() + i);
		if (t == -2)
			continue;
		layers[i]->SetTile(row, col, t);
	}
}

const int Tilemap::GetTile(int layer, int row, int col) const
{
	return layers[layer]->GetTile(row, col);
}

const int Tilemap::GetTile(int layer, const glm::vec2& position) const
{
	return layers[layer]->GetTile(position);
}

glm::vec2 Tilemap::GetPixelSize()
{
	auto ret = layers[0]->GetPixelSize();
	return ret;
}

glm::vec2 Tilemap::GetTileSize()
{
	auto ret = layers[0]->GetTileSize();
	return ret;
}

glm::vec4 Tilemap::GetCollision(int layer, int row, int col)
{
	return layers[layer]->GetCollision(row, col);
}
