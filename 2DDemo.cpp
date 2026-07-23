#include "engine/SpriteRenderer.h"
#include "engine/NineSlicer.h"
#include "engine/Particles.h"
#include "engine/InputsMap.h"
#include "Game.h"
#include "Camera.h"
#include "DemoUI.h"
#include "2DDemo.h"
#include "ExtraParts.h"

extern Texture* whiteRect;

TestScreen::TestScreen()
{
	ID = "Test Screen";

	//TODO: split all these into a TiledScreen demo, and maybe some others.

	//testFont = new TrueTypeFont("fonts/cc-pulp-fiction-medium.ttf", 32.0f);
	//testFont->SetPUAFallback(0xE0, "fonts/kenney_input_xbox_series.otf");
	//testFont = new TrueTypeFont(json5pp::parse5("{\"file\":\"fonts/cc-pulp-fiction-medium.ttf\",\"size\":16,\"e0\":\"fonts/kenney_input_xbox_series.otf\",\"lineHeight\":2.0}"));
	//testFont = new BitmapFont("fonts/unifont_{:02X}.png");
	testFont = new BitmapFont("fonts/doom.png");

	AddChild(std::make_shared<Background>());
	//AddChild(std::make_shared<RoryNite>());

	/*
	tilemapMgr = std::make_shared<Tilemap>("examples/tiled/maps/test3.json");
	tilemapMgr->Scale = 2.0f;
	tilemapMgr->Position = glm::vec2(16);
	//tilemapMgr->Camera = glm::vec2(-(tilemapMgr->GetPixelSize().x / 5), 0);
	//tilemapMgr->SetTile(0, 1, { -2, 118 });
	AddChild(tilemapMgr);
	AddChild(tilemapMgr->GetLayer(0));
	//AddChild(std::make_shared<BoingBall>());
	#ifndef BECKETT_NO3DMODELS
	//AddChild(std::make_shared<Teapot>());
	#endif
	AddChild(tilemapMgr->GetLayer(1));

	AddChild(std::make_shared<Farrah>());

	//AddChild(std::make_shared<BoingBall>());
	*/
	auto panel = std::make_shared<NineSlicer>("ui/panels/panel1.png", 212, 16, 240, 64);
	panel->ID = "Test Panel";
	auto panelText = std::make_shared<Subtitle>("...", glm::vec2(16));
	panel->AddChild(panelText);
	AddChild(panel);
	/*
	labelTest = std::make_shared<DropLabel>("Does this have a blurry\noutline?\n    ... yes yes it do", 2,75.0f, UI::themeColors["white"], DropLabel::Style::Blur);
	labelTest->Position = glm::vec2(32);
	AddChild(labelTest);
	//*/

	//AddChild(std::make_shared<Teapot>());
	//AddChild(std::make_shared<SimpleSprite>("examples/rorynite.png", 0, glm::vec2(480, 32)));
	auto rory = std::make_shared<RoryNite>();
	rory->position = glm::vec2(480, 32);
	AddChild(rory);

	bgm = new Audio("audio/music/retrobeat.ogg");
	bgm->SetLoop(true);
	bgm->RegisterListener(panelText.get());
	//bgm->Play(false, false);
	//bgm->SetPosition(glm::vec3(0.5, 0, .5));

	auto testButton = std::make_shared<Button>("Click me?", glm::vec2(0), glm::vec2(160, -1));
	testButton->OnClick = []()
	{
		//TODO: replace effect, this makes no effect without a 3D camera.
		MainCamera->Angles(MainCamera->Angles() + glm::vec3(0, 0, 1));
	};
	testButton->AbsolutePosition = testButton->Position;
	//AddChild(testButton);

	auto testPanel = std::make_shared<TestPanel>(glm::vec2(16));
	testPanel->AddChild(testButton);
	testPanel->AddChild(std::make_shared<Button>("Not me!", glm::vec2(0), glm::vec2(160, -1)));
	testPanel->Reflow();
	AddChild(testPanel);

	auto particles = std::make_shared<ParticleEmitter>();
	particles->prototype.position = glm::vec3(width * 0.5, 0, height * 0.5);
	AddChild(particles);


	//auto logoAnim = std::make_shared<PanelLayout>(VFS::ReadJSON("cinematics/logo/logo.json").as_object()["cinematic"]);
	//auto logoAnim = std::make_shared<PanelLayout>(VFS::ReadJSON("cinematics/godot/godot.json"));
	//logoAnim->Play("open");
	//AddChild(logoAnim);
}

TestScreen::~TestScreen()
{
	delete bgm;
	delete testFont;
}

bool TestScreen::Tick(float dt)
{
	//tilemapMgr->Camera = glm::vec2(glm::sin(commonUniforms.TotalTime * 10.0f) * 5, 0);

	/*
	if (Inputs.MouseHoldLeft)
	{
	auto panel = GetChild<Tickable2D>("Test Panel");
	panel->Position = Inputs.MousePosition / scale;
	panel->GetChild<TextLabel>(0)->Text = fmt::format("{}x{}", panel->Position.x, panel->Position.y);
	}
	*/

	//TODO: replace with another sound, since retrobeat isn't 3D-enabled.
	//Audio::SetListenerPosition(glm::vec3(Inputs.MousePosition.x / width, 0, Inputs.MousePosition.y / height));

	/*
	text = fmt::format("{}x{} --> tile {}",
	Inputs.MousePosition.x, Inputs.MousePosition.y,
	tilemapMgr->GetTile(0, Inputs.MousePosition)
	);
	*/
	/*
	text = fmt::format("{}, {} | {}, {}",
	Inputs.StickAngles[0], Inputs.StickDists[0],
	Inputs.StickAngles[1], Inputs.StickDists[1]);
	*/

	Tickable::Tick(dt);
	return true;
}

void TestScreen::Draw(float dt)
{
	Tickable::Draw(dt);
	//Sprite::DrawLine(glm::vec2(width * 0.5f, height * 0.5f), Inputs.MousePosition, glm::vec4(1, 0, 1, 1));
	Sprite::FlushBatch();

	//testFont->Draw("UMMM WAHOO?\n\naf42 ef16 bf1\n\nABCDEFGHIJKLM\nNOPQRSTUVWXYZ\n0123456789", glm::vec2(128), glm::vec4(1), 200.0f, 45.0f);
	Sprite::DrawText(3, "Jackdaws love my big <color:1>sphinx</color> of quartz\nYou know, as one does.\nABCDEFGHIJKLMNOPQRSTUVWXYZ", glm::vec2(128), glm::vec4(1), 200.0f, 0.0f);
}

bool TestScreen::Character(unsigned int ch)
{
	//labelTest->SetText(fmt::format("Ah! '{}'!", (char)ch));
	return false;
}
