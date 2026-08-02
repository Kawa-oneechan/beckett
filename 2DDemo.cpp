#include "engine/SpriteRenderer.h"
#include "engine/NineSlicer.h"
#include "engine/Particles.h"
#include "engine/InputsMap.h"
#include "Game.h"
#include "Camera.h"
#include "DemoUI.h"
#include "2DDemo.h"
#include "ExtraParts.h"

TwoDDemo::TwoDDemo()
{
	ID = "2D Demo Scene";

	bgm = new Stream("examples/2dscene/midnightcrew.ogg");
	bgm->SetLoop(true);
	//bgm->Play(false, false);

	AddChild(std::make_shared<Background>());

	auto panel = std::make_shared<NineSlicer>("ui/panels/panel1.png", 212, 16, 240, 64);
	panel->ID = "Test Panel";
	auto panelText = std::make_shared<TextLabel>("...", glm::vec2(16));
	panel->AddChild(panelText);
	AddChild(panel);

	auto rory = std::make_shared<RoryNite>();
	rory->position = glm::vec2(480, 32);
	AddChild(rory);

	auto testButton = std::make_shared<Button>("Click me?", glm::vec2(0), glm::vec2(128, -1));
	testButton->OnClick = []()
	{
		//TODO: replace effect, this makes no effect without a 3D camera.
		MainCamera->Angles(MainCamera->Angles() + glm::vec3(0, 0, 1));
	};
	testButton->AbsolutePosition = testButton->Position;

	auto bgmButton = std::make_shared<Button>("Play music", glm::vec2(0), glm::vec2(128, -1));
	bgmButton->OnClick = [&, bgmButton]()
	{
		if (bgm->IsPlaying())
		{
			bgm->Stop();
			bgmButton->Text = "Play music";
		}
		else
		{
			bgm->Play(false, false);
			bgmButton->Text = "Stop music";
		}
	};

	auto testPanel = std::make_shared<FlowPanelV>(glm::vec2(16));
	testPanel->AddChild(testButton);
	testPanel->AddChild(std::make_shared<Button>("Not me!", glm::vec2(0), glm::vec2(128, -1)));
	testPanel->AddChild(bgmButton);
	testPanel->Reflow();
	AddChild(testPanel);

	auto particles = std::make_shared<ParticleEmitter>();
	particles->prototype.position = glm::vec3(width * 0.5, 0, height * 0.5);
	AddChild(particles);

	labelTest = std::make_shared<DropLabel>("kek", &testFont, 100.0f, UI::themeColors["white"], DropLabel::Style::Outline);
	auto subtitle = std::make_shared<Subtitle>(labelTest);
	AddChild(subtitle);
	bgm->RegisterListener(subtitle.get());
}

TwoDDemo::~TwoDDemo()
{
	delete bgm;
	//delete testFont;
}
