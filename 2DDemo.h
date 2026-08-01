#pragma once
#include "engine/Tickable.h"
#include "engine/Audio.h"
#include "engine/DropLabel.h"
#include "engine/Font.h"

class TwoDDemo : public Tickable
{
private:
	DropLabelP labelTest;

	Stream* bgm = nullptr;

	TrueTypeFont testFont{ "fonts/CinecavXUIBold.ttf", 40 };

public:
	TwoDDemo();
	~TwoDDemo() override;

	TwoDDemo(TwoDDemo&) = delete;
	TwoDDemo operator=(TwoDDemo) = delete;
};

