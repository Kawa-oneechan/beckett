#include "Game.h"
#include "engine/Console.h"
#include "engine/Cursor.h"
#include "engine/Text.h"
#include "engine/JsonUtils.h"
#include "engine/InputsMap.h"
#include "engine/Model.h"
#include "engine/Tilemap.h"
#include "engine/DropLabel.h"
#include "engine/NineSlicer.h"

//Wouldn't need this here if the camera were a proper
//class in its own file like in PSK.
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_inverse.hpp>

#ifdef BECKETT_EXTRASAVEDIRS
#include <filesystem>
#ifdef _MSC_VER
namespace fs = std::experimental::filesystem;
#else
namespace fs = std::filesystem;
#endif
#endif

__declspec(noreturn)
extern void FatalError(const std::string& message);

extern "C" int glfwGetKeyScancode(int key);

constexpr int ScreenWidth = SCREENWIDTH;
constexpr int ScreenHeight = SCREENHEIGHT;

extern Texture* whiteRect;

class Background : public Tickable
{
public:
	void Draw(float dt)
	{
		dt;
		Sprite::DrawSprite(Shaders["background"], *whiteRect, glm::vec2(0), glm::vec2(width, height), glm::vec4(0, 0, width, height));
	}
};

class BoingBall : public Tickable
{
public:
	void Draw(float dt)
	{
		dt;
		Sprite::DrawSprite(Shaders["boingball"], *whiteRect, glm::vec2(0), glm::vec2(width, height), glm::vec4(0, 0, width, height));
	}
};

class Farrah : public Tickable
{
private:
	Texture sprite{ "example/farrah.png" };
	Texture stage{ "example/stage.png" };

public:
	void Draw(float dt)
	{
		dt;
		auto time = commonUniforms.TotalTime * 10.0f;

		auto& frame = stage[0];
		Sprite::DrawSprite(stage,
			glm::vec2(ScreenWidth - frame.z, ScreenHeight + (frame.w * 0.5)) / 2.0f,
			glm::vec2(frame.z, frame.w),
			frame,
			0.0f,
			glm::vec4(glm::vec3(0.5f + glm::abs(glm::sin(time * 0.1f) * 0.5f)), 1.0f));

		frame = sprite[(int)time / 2 % sprite.Frames()];
		Sprite::DrawSprite(sprite,
			glm::vec2(ScreenWidth - frame.z, ScreenHeight - (frame.w * 0.5)) / 2.0f,
			glm::vec2(frame.z, frame.w),
			frame,
			0.0f,
			glm::vec4(1.0f));

	}
};

class Teapot : public Tickable
{
private:
	Model model{ "example/teapot.fbx" };

public:
	void Draw(float dt)
	{
		dt;
		auto time = commonUniforms.TotalTime * 10.0f;

		//Have to flush here so the draw order becomes boingball, teapot, stage.
		Sprite::FlushBatch();
		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		model.Draw(glm::vec3(0), time);
		MeshBucket::Flush();

		glDisable(GL_DEPTH_TEST);
	}
};

class TestScreen : public Tickable
{
private:
	TilemapP tilemapMgr;
	std::string text;
	DropLabel* labelTest;

public:
	TestScreen()
	{
		//ChildTickables.push_back(std::make_shared<Background>());
		//ChildTickables.push_back(std::make_shared<Teapot>());
		//ChildTickables.push_back(std::make_shared<Farrah>());

		tilemapMgr = std::make_shared<Tilemap>("maps/test3.json");
		tilemapMgr->Scale = 3.0f;
		//tilemapMgr->Camera = glm::vec2(-(tilemapMgr->GetPixelSize().x / 5), 0);
		//tilemapMgr->SetTile(0, 1, { -2, 118 });
		ChildTickables.push_back(tilemapMgr);
		ChildTickables.push_back(tilemapMgr->GetLayer(0));
		//ChildTickables.push_back(std::make_shared<BoingBall>());
		//ChildTickables.push_back(std::make_shared<Teapot>());
		//ChildTickables.push_back(std::make_shared<Farrah>());
		ChildTickables.push_back(tilemapMgr->GetLayer(1));

		ChildTickables.push_back(std::make_shared<BoingBall>());

		ChildTickables.push_back(std::make_shared<NineSlicer>("example/panel1.png", 8, 8, 240, 64));
		
		labelTest = new DropLabel("Does this have a blurry\noutline?\n    ... yes yes it do", 2, 100, DropLabel::Style::Blur);
	}

	bool Tick(float dt)
	{
		//tilemapMgr->Camera = glm::vec2(glm::sin(commonUniforms.TotalTime * 10.0f) * 5, 0);

		text = fmt::format("{}x{} --> tile {}",
			Inputs.MousePosition.x, Inputs.MousePosition.y,
			tilemapMgr->GetTile(0, Inputs.MousePosition)
		);

		RevAllTickables(ChildTickables, dt);
		return true;
	}

	void Draw(float dt)
	{
		DrawAllTickables(ChildTickables, dt);


		Sprite::DrawText(2,
			text, //"Hello, Beckett Engine!",
			glm::vec2(24), glm::vec4(1), 50.0f);

		Sprite::DrawSprite(labelTest->Texture(), glm::vec2(32));

		//Sprite::DrawText(2, "Is this red?\nRenderDoc HELP!", glm::vec2(32 + 8), glm::vec4(1, 1, 1, 0.5));
	}

	bool Character(unsigned int ch)
	{
		labelTest->SetText(fmt::format("Ah! '{}'!", (char)ch));
		return false;
	}
};

std::string testString;
glm::vec2 testVector;
glm::vec4 testColor;

void ConsoleRegister(Console* console)
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

#undef RV

	//console->RegisterCCmd("reshade", CCmdReshade);
}

namespace UI
{
	std::map<std::string, glm::vec4> themeColors;
	std::vector<glm::vec4> textColors;

	std::shared_ptr<Texture> controls{ nullptr };

	jsonValue json;
	jsonValue settings;

	std::string initFile = "init.json";

	void Load()
	{
		UI::json = VFS::ReadJSON("ui/ui.json");
		if (!UI::json)
			FatalError("Could not read ui/ui.json. Something is very wrong.");
		auto json = UI::json.as_object();
		auto colors = json["colors"].as_object();
		for (auto& ink : colors["theme"].as_object())
		{
			themeColors[ink.first] = GetJSONColor(ink.second);
		}
		for (auto& ink : colors["text"].as_array())
		{
			textColors.push_back(GetJSONColor(ink));
		}

		try
		{
			UI::settings = VFS::ReadSaveJSON("options.json");
		}
		catch (std::runtime_error&)
		{
			UI::settings = json5pp::parse5("{}");
		}

		auto settings = UI::settings.as_object();

#define DS(K, V) if (!settings[K]) settings[K] = jsonValue(V)
#define DA(K, V) if (!settings[K]) settings[K] = json5pp::array(V)
		DS("screenWidth", ScreenWidth);
		DS("screenHeight", ScreenHeight);
		DA("keyBinds", {});
		DA("gamepadBinds", {});
#undef DA
#undef DS

		auto bluh = &width;
		width = settings["screenWidth"].as_integer();
		height = settings["screenHeight"].as_integer();

		auto keyBinds = settings["keyBinds"].as_array();
		if (keyBinds.size() != NumKeyBinds)
		{
			keyBinds.reserve(NumKeyBinds);
			for (auto &k : DefaultInputBindings)
				keyBinds.emplace_back(jsonValue(glfwGetKeyScancode(k)));
		}

		auto padBinds = settings["gamepadBinds"].as_array();
		if (padBinds.size() != NumKeyBinds)
		{
			padBinds.reserve(NumKeyBinds);
			for (auto &k : DefaultInputGamepadBindings)
				padBinds.emplace_back(jsonValue(k));
		}

		for (int i = 0; i < NumKeyBinds; i++)
		{
			Inputs.Keys[i].ScanCode = keyBinds[i].as_integer();
			Inputs.Keys[i].GamepadButton = padBinds[i].as_integer();
		}
	}

	void Save()
	{
		auto settings = UI::settings.as_object();
		settings["screenWidth"] = width;
		settings["screenHeight"] = height;

		auto binds = json5pp::array({});
		for (auto& k : Inputs.Keys)
			binds.as_array().push_back(k.ScanCode);
		settings["keyBinds"] = std::move(binds);

		auto binds2 = json5pp::array({});
		for (auto& k : Inputs.Keys)
			binds2.as_array().push_back(k.GamepadButton);
		settings["gamepadBinds"] = std::move(binds2);

		try
		{
			VFS::WriteSaveJSON("options.json", UI::settings);
		}
		catch (std::exception&)
		{
			conprint(2, "Couldn't save settings.");
		}
	}
};

#ifdef DEBUG
void GameImGui()
{
	//Put this function in its own module and #include <ImGUI/imgui.h>.
	//Rendering is handled by the engine, just handle windows here.
}
#endif

void GameInit()
{
	//Add extra loading steps here

	//Set up a camera and lights for the 3D example
	auto cameraPos = glm::vec3(0.0f, 5.0f, 15.0f);
	auto cameraTarget = glm::vec3(0.0f, 2.0f, 0.0f);
	commonUniforms.View = glm::lookAt(cameraPos, cameraTarget, glm::vec3(0.0f, 1.0f, 0.0f));
	commonUniforms.InvView = glm::affineInverse(commonUniforms.View);

	commonUniforms.Lights[0].color = glm::vec4(0.5);
	commonUniforms.Lights[0].pos = glm::vec4(0, 15, 20, 0);
	commonUniforms.Lights[1].color = glm::vec4(1, 0, 0, 0.25);
	commonUniforms.Lights[1].pos = glm::vec4(0, -15, 0, 0);
}

#ifdef BECKETT_EXTRASAVEDIRS
void GamePrepSaveDirs(const fs::path& savePath)
{
//	fs::create_directory(savePath / "map");
}
#endif

void GameStart(std::vector<TickableP>& tickables)
{
	tickables.push_back(std::make_shared<TestScreen>());
}

void GameMouse(double xPosIn, double yPosIn, float xoffset, float yoffset)
{
	xPosIn, yPosIn, xoffset, yoffset;
}

void GameResize()
{
	scale = 1.0f;
}

void GameLoopStart()
{
	//cursor->SetScale(scale);
}

void GamePreDraw(float dt)
{
	dt;
	glClearColor(0.0f, 0.0f, 0.25f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void GamePostDraw(float dt)
{
	dt;
}

void GameQuit()
{
}
