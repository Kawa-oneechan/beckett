#pragma once
#include "engine/Tickable.h"
#include "engine/Audio.h"
#include "engine/DropLabel.h"

class MapDemo : public Tickable
{
private:
	TilemapP tilemapMgr;
public:
	MapDemo();

	//MapDemo(MapDemo&) = delete;
	//MapDemo operator=(MapDemo) = delete;

	//bool Tick(float dt) override;
};
