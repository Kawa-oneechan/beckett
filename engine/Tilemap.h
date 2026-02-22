#pragma once
#include <map>
#include "Tickable.h"
#include "Texture.h"
#include "JsonUtils.h"

namespace Beck
{

	//Provides as easier means to load a multi-layer map.
	//To properly use, place this somewhere in the tickable tree, then add the component layers
	//to the tree in their proper order so you can render sprites and such inbetween.
	class Tilemap : public Tickable2D
	{
		static constexpr int MaxAnimFrames = 32;

		struct Animation
		{
			int frameCt{ 0 };
			int duration[MaxAnimFrames]{};
			int tile[MaxAnimFrames]{};
			int currentFrame{ 0 };
			int durationLeft{ 0 };
		};

		struct Shape
		{
			glm::vec4 rectP;
			glm::vec4 rectT;
			std::string type;
			std::string name;
		};

		struct Tileset
		{
			std::shared_ptr<Texture> texture{ nullptr };
			int tileWidth{ 0 };
			int tileHeight{ 0 };
			int tileGridWidth{ 0 };
			int tileGridHeight{ 0 };
			glm::vec2 tileOffset{ 0 };
			int tilesPerLine{ 0 };

			std::map<int, Animation> animations;
			std::map<int, Shape> collisions;
		};

		class MapLayer : public Tickable
		{
		private:
			int width{ 0 };
			int height{ 0 };
			glm::vec2 Parallax{ 1, 1 };
			glm::vec4 Tint{ 1, 1, 1, 1 };
			std::string layerName;
			std::unique_ptr<int[]> data{ nullptr };
			Tilemap* owner{ nullptr };

		public:
			MapLayer() = default;
			MapLayer(jsonValue& doc, Tilemap* owner);
			void Draw(float dt) override;
			void SetTile(int row, int col, int tile);
			const int GetTile(int row, int col) const;
			const int GetTile(const glm::vec2& position) const;
			glm::vec2 GetPixelSize();
			glm::vec2 GetTileSize();
			glm::vec4 GetCollision(int row, int col);

			glm::vec2 Camera;
			float Scale{ -1.0f };
		};

	private:
		std::vector<std::shared_ptr<MapLayer>> layers;
		std::vector<Shape> shapes;
		Tileset tileset;
		bool isometric{ false };

	public:
		glm::vec2 Camera;
		float Scale{ 1.0f };

		explicit Tilemap(const std::string& source);
		void Draw(float dt) override {}
		bool Tick(float) override;

		std::shared_ptr<MapLayer> operator[](size_t i) const;
		std::shared_ptr<MapLayer> GetLayer(size_t i);
		void Tilemap::SetTile(int row, int col, int tile);
		void Tilemap::SetTile(int row, int col, std::initializer_list<int> tiles);
		const int GetTile(int layer, int row, int col) const;
		const int GetTile(int layer, const glm::vec2& position) const;
		glm::vec2 GetPixelSize();
		glm::vec2 GetTileSize();
		glm::vec4 GetCollision(int layer, int row, int col);
	};

	using TilemapP = std::shared_ptr<Tilemap>;

}
