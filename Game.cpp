#include "Game.h"
#include "engine/Game.h"
#include "engine/Console.h"
#include "engine/Cursor.h"
#include "engine/Text.h"
#include "engine/JsonUtils.h"
#include "engine/InputsMap.h"
#include "engine/Model.h"
#include "engine/Tilemap.h"
#include "engine/DropLabel.h"
#include "engine/NineSlicer.h"
#include "engine/Audio.h"
#include "engine/Shader.h"
#include "engine//Random.h"
#include "Camera.h"
#include "engine/Framebuffer.h"

//Wouldn't need this here if the camera were a proper
//class in its own file like in PSK.
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_inverse.hpp>

constexpr int ScreenWidth = BECKETT_SCREENWIDTH;
constexpr int ScreenHeight = BECKETT_SCREENHEIGHT;

std::shared_ptr<Camera> MainCamera;

static void FrameDrawer(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color, int flags)
{
	Sprite::DrawSprite(*whiteRect, pos, size - glm::vec2(1), glm::vec4(0), 0.0f, color);

	auto diff = glm::vec4(0.25, 0.25, 0.25, 0.0);
	auto hi = color + diff;
	auto lo = color - diff;

	Sprite::DrawLine(pos, pos + glm::vec2(size.x - 1, 0), hi);
	Sprite::DrawLine(pos + glm::vec2(0, 1), pos + glm::vec2(0, size.y - 1), hi);
	Sprite::DrawLine(pos + glm::vec2(0, size.y - 1), pos + glm::vec2(size.x - 1, size.y - 1), lo);
	Sprite::DrawLine(pos + glm::vec2(size.x, 1), pos + glm::vec2(size.x, size.y - 1), lo);
}

class Button : public Tickable2D
{
public:
	std::string Text;
	glm::vec4 Color{ 1, 1, 1, 1 };
	glm::vec4 BackColor{ 0, 0, 0.75, 0.5 };
	glm::vec2 Size{ 128, 32 };
	float TextSize{ 100.0f };
	float Angle{ 0.0f };
	int Font{ 1 };
	bool Raw{ false };
	std::function<void()> OnClick{};
	std::function<void(const glm::vec2&, const glm::vec2&, const glm::vec4&, int)> OnDraw{ FrameDrawer };

	Button(const std::string& text, glm::vec2 position, glm::vec2 size = glm::vec2(-1)) : Text(text)
	{
		parent = nullptr;
		Position = position;
		Size = size;

		auto minSize = GetMinimalSize();
		if (Size.x == -1)
			Size.x = minSize.x;
		if (Size.y == -1)
			Size.y = minSize.y;
	}

	bool Tick(float) override
	{
		if (!Inputs.MouseLeft)
			return true;
		if (!OnClick)
			return true;
		if (Inputs.MousePosition.x > AbsolutePosition.x &&
			Inputs.MousePosition.y > AbsolutePosition.y &&
			Inputs.MousePosition.x < AbsolutePosition.x + Size.x &&
			Inputs.MousePosition.y < AbsolutePosition.y + Size.y)
		{
			OnClick();
			return false;
		}
		return true;
	}

	void Draw(float) override
	{
		auto color = BackColor;
		if (Inputs.MousePosition.x > AbsolutePosition.x &&
			Inputs.MousePosition.y > AbsolutePosition.y &&
			Inputs.MousePosition.x < AbsolutePosition.x + Size.x &&
			Inputs.MousePosition.y < AbsolutePosition.y + Size.y)
			color.a = 1.0f;
		//Sprite::DrawSprite(*whiteRect, AbsolutePosition, Size, glm::vec4(0), 0.0f, color);
		//Sprite::DrawRect(glm::vec4(AbsolutePosition, AbsolutePosition + Size), Color);
		FrameDrawer(AbsolutePosition, Size, color, 0);

		auto size = Sprite::MeasureText(Font, Text, TextSize, Raw);
		auto center = (Size * 0.5f) - (size * 0.5f);
		center.y += 4;
		Sprite::DrawText(Font, Text, AbsolutePosition + center, Color, TextSize, Angle, Raw);
	}

	glm::vec2 GetMinimalSize() override
	{
		return Sprite::MeasureText(Font, Text, TextSize, Raw) + glm::vec2(16, 8);
	}

	glm::vec2 GetSize() override
	{
		return Size;
	}
};

class TestPanel : public Tickable2D
{
public:
	glm::vec4 Color{ 1, 1, 1, 1 };
	glm::vec4 BackColor{ 0.75, 0, 0, 0.5 };
	glm::vec2 Size;
public:
	std::function<void(const glm::vec2&, const glm::vec2&, const glm::vec4&, int)> OnDraw{ FrameDrawer };

	TestPanel(glm::vec2 position, glm::vec2 size = glm::vec2(-1))
	{
		parent = nullptr;
		Position = position;
		Size = size;

		auto minSize = GetMinimalSize();
		if (Size.x == -1)
			Size.x = minSize.x;
		if (Size.y == -1)
			Size.y = minSize.y;

		Size += glm::vec2(8);
	}

	void Draw(float dt) override
	{
		//Sprite::DrawSprite(*whiteRect, AbsolutePosition, Size, glm::vec4(0), 0.0f, BackColor);
		//Sprite::DrawRect(glm::vec4(AbsolutePosition, AbsolutePosition + Size), Color);
		FrameDrawer(AbsolutePosition, Size, BackColor, 0);

		Tickable2D::Draw(dt);
	}

	void Reflow()
	{
		UpdatePosition();
		auto pos = glm::vec2(8);
		for (const auto& t : ChildTickables)
		{
			if (auto t2D = dynamic_cast<Tickable2D*>(t.get()))
			{
				t2D->Position = pos;
				t2D->UpdatePosition();
				pos.y += t2D->GetSize().y + 8;
			}
		}
		Size = GetMinimalSize();
	}
};

class Background : public Tickable
{
public:
	void Draw(float dt) override
	{
		(void)(dt);
		Sprite::DrawSprite(Shaders["background"], *whiteRect, glm::vec2(0), glm::vec2(width, height), glm::vec4(0, 0, width, height));
	}
};

class BoingBall : public Tickable
{
public:
	void Draw(float dt) override
	{
		(void)(dt);
		Sprite::DrawSprite(Shaders["boingball"], *whiteRect, glm::vec2(0), glm::vec2(width, height), glm::vec4(0, 0, width, height));
	}
};

class Farrah : public Tickable
{
private:
	Texture sprite{ "example/farrah.png" };
	Texture stage{ "example/stage.png" };

public:
	void Draw(float dt) override
	{
		(void)(dt);
		auto time = commonUniforms.TotalTime * 10.0f;
		auto pos = glm::vec2(ScreenWidth, ScreenHeight) * 0.5f;

		Sprite::DrawSprite(stage,
			pos, stage[0], 0.0f,
			glm::vec4(glm::vec3(0.5f + glm::abs(glm::sin(time * 0.1f) * 0.5f)), 1.0f),
			Sprite::SpriteFlags::CenterOrigin
		);
		Sprite::DrawSprite(sprite,
			pos, sprite[(int)time / 2 % sprite.Frames()], 0.0f,
			glm::vec4(1.0f),
			Sprite::SpriteFlags::MidBotOrigin
		);
	}
};

#ifndef BECKETT_NO3DMODELS
class Teapot : public Tickable
{
private:
	Model model{ "example/teapot.fbx" };

public:
	void Draw(float dt) override
	{
		(void)(dt);
		auto time = commonUniforms.TotalTime * 50.0f;

		//Have to flush here so the draw order becomes boingball, teapot, stage.
		Sprite::FlushBatch();
		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		model.Draw(glm::vec3(0), time);
		MeshBucket::Flush();

		glDisable(GL_DEPTH_TEST);
	}
};

class MapScene : public Tickable
{
private:
	Model model{ "example/scene.fbx" };

public:
	void Draw(float dt) override
	{
		(void)(dt);

		Sprite::FlushBatch();
		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		commonUniforms.Lights[0].pos.x = 20 * glm::cos(commonUniforms.TotalTime * 1.0f);
		commonUniforms.Lights[0].pos.y = 20 * glm::sin(commonUniforms.TotalTime * 1.0f);

		model.Draw(glm::vec3(0));
		MeshBucket::Flush();

		glDisable(GL_DEPTH_TEST);
	}
};

class TrainScene : public Tickable
{
private:
	Model model{ "example/train/train.fbx" };
	float bumpTimer{ 0.0f };
	Framebuffer* postFx;

	Model bob{ "example/bob/bob.fbx" };

public:
	TrainScene()
	{
		MainCamera->Target(glm::vec3(6, 12, -37));
		MainCamera->Angles(glm::vec3(1, 2, -3));
		MainCamera->Distance(50);
		MainCamera->FirstPerson(false);

		commonUniforms.Lights[0].color = glm::vec4(1.0, 1.0, 1.0, 0.0);
		commonUniforms.Lights[0].pos = glm::vec4(-50, 24, 35, 0);
		commonUniforms.Lights[1].color = glm::vec4(0.75, 0.75, 0.75, 0.04);
		commonUniforms.Lights[1].pos = glm::vec4(16, 22, -16, 0);
		commonUniforms.Lights[2].color = glm::vec4(0.75, 0.75, 0.75, 0.04);
		commonUniforms.Lights[2].pos = glm::vec4(16, 22, -40, 0);

		commonUniforms.Toon = false;

		//postFx = new Framebuffer(Shaders["postfx"], width, height);
	}

	~TrainScene()
	{
		//delete postFx;
	}

	bool Tick(float dt) override
	{
		//bumpTimer += dt;
		auto t = MainCamera->Target();
		//t.y = Random::GetFloat(12.0f, 12.1f);
		if (bumpTimer > 2.0f)
			t.y = 12.4f;
		MainCamera->Target(t);
		if (bumpTimer > 2.1f)
			bumpTimer = 0.0f;

		return Tickable::Tick(dt);
	}

	void Draw(float dt) override
	{
		(void)(dt);

		Sprite::FlushBatch();

		//postFx->Use();

		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		//commonUniforms.Lights[0].pos.x = 20 * glm::cos(commonUniforms.TotalTime * 1.0f);
		//commonUniforms.Lights[0].pos.y = 20 * glm::sin(commonUniforms.TotalTime * 1.0f);

		bob.Draw(glm::vec3(3, 4, -14));
		model.Draw(glm::vec3(0));
		MeshBucket::Flush();

		glDisable(GL_DEPTH_TEST);

		//postFx->Drop();
		//postFx->Draw(glm::vec2(0), glm::vec2(width, height));
	}
};

#endif

class RoryNite : public Tickable
{
private:
	Texture sprite{ "rorynite.png" };
	static constexpr int trailCt = 8;
public:
	glm::vec2 position = glm::vec2(ScreenWidth / 2, ScreenHeight / 2);

	void Draw(float dt) override
	{
		for (int i = trailCt - 1; i >= 0; i--)
		{
			auto p = position + glm::vec2(i * 8, glm::sin((commonUniforms.TotalTime * 8) - i) * 4); //* (trailCt / (float)i));
			Sprite::DrawSprite(sprite, p, sprite[0], 0.0f, glm::vec4(1, 1, 1, 1.0 - (i / (float)trailCt)));
		}
	}
};

class Subtitle : public TextLabel, AudioEventListener
{
public:
	void AudioEvent(float time, const std::string& text) override
	{
		this->Text = text;
	}
	Subtitle(const std::string& text, glm::vec2 position) : TextLabel(text, position) {}
};

class FirstPersonController : public Tickable
{
	bool Tick(float dt) override
	{
		if (Inputs.KeyDown(Binds::WalkN) || Inputs.KeyDown(Binds::WalkS))
		{
			auto speed = Inputs.KeyDown(Binds::WalkN) ? 0.05f : -0.05f;
			auto pos = MainCamera->GetTarget();
			auto yaw = MainCamera->GetAngles().z;
			pos.x -= glm::sin(glm::radians(yaw)) * speed;
			pos.z -= glm::cos(glm::radians(yaw)) * speed;
			MainCamera->Target(pos);
		}
		if (Inputs.KeyDown(Binds::WalkE) || Inputs.KeyDown(Binds::WalkW))
		{
			auto speed = Inputs.KeyDown(Binds::WalkW) ? 0.025f : -0.025f;
			auto pos = MainCamera->GetTarget();
			auto yaw = MainCamera->GetAngles().z + 90.0f;
			pos.x -= glm::sin(glm::radians(yaw)) * speed;
			pos.z -= glm::cos(glm::radians(yaw)) * speed;
			MainCamera->Target(pos);
		}

		return Tickable::Tick(dt);
	}
};

class TestScreen : public Tickable
{
private:
	TilemapP tilemapMgr;
	std::string text;
	DropLabelP labelTest;

	Audio* bgm = nullptr;

public:
	TestScreen()
	{
		//AddChild(std::make_shared<Background>());
		//AddChild(std::make_shared<RoryNite>());

		/*
		tilemapMgr = std::make_shared<Tilemap>("maps/test3.json");
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

		auto panel = std::make_shared<NineSlicer>("example/panel1.png", 8, 8, 240, 64);
		panel->ID = "Test Panel";
		auto panelText = std::make_shared<Subtitle>("gadunkadunk", glm::vec2(16));
		panel->AddChild(panelText);
		AddChild(panel);

		AddChild(std::make_shared<SimpleSprite>("rorynite.png", 0, glm::vec2(480, 32)));

		labelTest = std::make_shared<DropLabel>("Does this have a blurry\noutline?\n    ... yes yes it do", 2,75.0f, UI::themeColors["white"], DropLabel::Style::Blur);
		labelTest->Position = glm::vec2(32);
		AddChild(labelTest);
		//*/

		//AddChild(std::make_shared<Teapot>());

		//bgm = new Audio("example/dontforget.ogg");
		//bgm->RegisterListener((AudioEventListener*)panelText.get());
		//bgm->Play(false, false);
		//bgm->SetPosition(glm::vec3(0.5, 0, .5));

		AddChild(std::make_shared<TrainScene>());

		auto testButton = std::make_shared<Button>("Click me?", glm::vec2(8), glm::vec2(160, -1));
		testButton->OnClick = []()
		{
			//conprint(0, "Yowza!");
			MainCamera->Angles(MainCamera->Angles() + glm::vec3(0, 0, 1));
		};
		testButton->AbsolutePosition = testButton->Position;
		//AddChild(testButton);

		auto testPanel = std::make_shared<TestPanel>(glm::vec2(4));
		testPanel->AddChild(testButton);
		testPanel->AddChild(std::make_shared<Button>("Not me!", glm::vec2(8), glm::vec2(160, -1)));
		testPanel->Reflow();
		AddChild(testPanel);

		//MainCamera->FirstPerson(true);
		//AddChild(std::make_shared<FirstPersonController>());
	}

	bool Tick(float dt) override
	{
		//tilemapMgr->Camera = glm::vec2(glm::sin(commonUniforms.TotalTime * 10.0f) * 5, 0);

		/*&
		if (Inputs.MouseHoldLeft)
		{
			auto panel = GetChild<Tickable2D>("Test Panel");
			panel->Position = Inputs.MousePosition / scale;
			panel->GetChild<TextLabel>(0)->Text = fmt::format("{}x{}", panel->Position.x, panel->Position.y);
		}
		*/

		Audio::SetListenerPosition(glm::vec3(Inputs.MousePosition.x / width, 0, Inputs.MousePosition.y / height));

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

	void Draw(float dt) override
	{
		Tickable::Draw(dt);
		//Sprite::DrawLine(glm::vec2(width * 0.5f, height * 0.5f), Inputs.MousePosition, glm::vec4(1, 0, 1, 1));
		Sprite::FlushBatch();
	}

	bool Character(unsigned int ch) override
	{
		//labelTest->SetText(fmt::format("Ah! '{}'!", (char)ch));
		return false;
	}
};

std::string testString;
glm::vec2 testVector;
glm::vec4 testColor;

void Game::RegisterConsole(Console* console)
{
#define RV console->RegisterCVar

	//RV("ai_disable", CVar::Type::Bool, &);
	//RV("cl_showpos", CVar::Type::Bool, &);
	//RV("noclip", CVar::Type::Bool, &, true);
	//RV("r_acredistance", CVar::Type::Int, &, false, 1, 6);
	//RV("r_drawgui", CVar::Type::Bool, &, true);
	//RV("r_drum", CVar::Type::Bool, &);
	//RV("r_drumexp", CVar::Type::Float, &);
	//RV("r_farz", CVar::Type::float, &, yes);

	RV("teststr", CVar::Type::String, &testString);
	RV("testvec", CVar::Type::Vec2, &testVector);
	RV("testcol", CVar::Type::Color, &testColor);
	RV("callback", CVar::Type::String, &testString, false, -1, -1, [](CVar* c) { StringToUpper(*c->asString); });

#undef RV
}

void Game::LoadSettings(jsonObject& settings)
{
	//Ensure all settings you need exist with a proper default.
#define DS(K, V) if (!settings[K]) settings[K] = jsonValue(V)
#define DA(K, V) if (!settings[K]) settings[K] = json5pp::array(V)
#define DO(K, V) if (!settings[K]) settings[K] = json5pp::object(V)
	//...
#undef DO
#undef DA
#undef DS

	//Apply any loaded settings in whatever way you need.
}

void Game::SaveSettings(jsonObject& settings)
{
}

void Game::Initialize()
{
	//Add extra loading steps here

	//Set up a camera and lights for the 3D example
	/*
	auto cameraPos = glm::vec3(0.0f, 5.0f, 15.0f);
	auto cameraTarget = glm::vec3(0.0f, 2.0f, 0.0f);
	commonUniforms.View = glm::lookAt(cameraPos, cameraTarget, glm::vec3(0.0f, 1.0f, 0.0f));
	commonUniforms.InvView = glm::affineInverse(commonUniforms.View);
	*/
	MainCamera = std::make_shared<Camera>();
	MainCamera->Distance(50);
	MainCamera->Angles(glm::vec3(0, 18, 140));
	MainCamera->Target(glm::vec3(0, 5, 0));

	commonUniforms.Lights[0].color = glm::vec4(1.0);
	commonUniforms.Lights[0].pos = glm::vec4(20, 15, 0, 0);
	//commonUniforms.Lights[1].color = glm::vec4(1, 0, 0, 0.25);
	//commonUniforms.Lights[1].pos = glm::vec4(0, -15, 0, 0);
	commonUniforms.Fresnel = true;
}

#ifdef BECKETT_EXTRASAVEDIRS
void Game::PrepareSaveDirs()
{
	//Example:
	//VFS::MakeSaveDir("map/farm");
}
#endif

void Game::Start(std::vector<TickableP>& tickables)
{
	tickables.push_back(MainCamera);
	tickables.push_back(std::make_shared<TestScreen>());
}

void Game::OnKey(int key, int scancode, int action, int mods)
{
	if (key == 301 && action == 1) //F12
	{
		Screenshot();
		return;
	}
}

void Game::OnMouse(double xPosIn, double yPosIn, float xoffset, float yoffset)
{
	xPosIn, yPosIn;
	if (Inputs.MouseHoldMiddle && !MainCamera->Locked)
	{
		auto angles = MainCamera->GetAngles();
		angles.z -= xoffset;
		angles.y -= yoffset;
		MainCamera->Angles(angles);
	}
}

void Game::OnResize()
{
	//scale = 1.0f;
}

void Game::LoopStart()
{
	//cursor->SetScale(scale);
}

void Game::PreDraw(float dt)
{
	(void)(dt);
	glClearColor(0.0f, 0.0f, 0.25f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Game::PostDraw(float dt)
{
	(void)(dt);
}

void Game::OnQuit()
{
}
