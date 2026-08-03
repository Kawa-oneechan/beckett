#include "engine/Tilemap.h"
#include "engine/InputsMap.h"
#include "Game.h"
#include "Camera.h"
#include "DemoUI.h"
#include "MapDemo.h"

MapDemo::MapDemo()
{
	ID = "Tiled Map Demo Scene";

	tilemapMgr = std::make_shared<Tilemap>("examples/tiled/maps/farm.json");
	tilemapMgr->Scale = 4.0f;
	tilemapMgr->Position = glm::vec2(0);
	//tilemapMgr->Camera = glm::vec2(-(tilemapMgr->GetPixelSize().x / 5), 0);
	//tilemapMgr->SetTile(0, 1, { -2, 118 });
	AddChild(tilemapMgr);

	auto testLabel = std::make_shared<TextLabel>(u8"Test: \uE004 \uE024 \uE028", glm::vec2(16), 2);
	AddChild(testLabel);
}

/*
bool MapDemo::Tick(float dt)
{
	//tilemapMgr->Camera = glm::vec2(glm::sin(commonUniforms.TotalTime * 10.0f) * 5, 0);
	return Tickable::Tick(dt);
}
*/
