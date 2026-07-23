#pragma once
#include "engine/Tickable.h"
#include "engine/Audio.h"
#include "engine/DropLabel.h"

class TestScreen : public Tickable
{
private:
	//TilemapP tilemapMgr;
	//std::string text;
	DropLabelP labelTest;

	Audio* bgm = nullptr;

	BeckettFont* testFont = nullptr;

public:
	TestScreen();
	~TestScreen() override;

	TestScreen(TestScreen&) = delete;
	TestScreen operator=(TestScreen) = delete;

	bool Tick(float dt) override;
	void Draw(float dt) override;
	bool Character(unsigned int ch) override;
};

