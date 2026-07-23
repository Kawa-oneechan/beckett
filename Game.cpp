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
#include "engine/Framebuffer.h"
#include "engine/Particles.h"
#include "engine/PanelLayout.h"
#include "Camera.h"
#include "DemoUI.h"
#include "ExtraParts.h"
#include "2DDemo.h"
#include "3DDemo.h"
#include "TrainDemo.h"

//Wouldn't need this here if the camera were a proper
//class in its own file like in PSK.
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_inverse.hpp>

constexpr int ScreenWidth = BECKETT_SCREENWIDTH;
constexpr int ScreenHeight = BECKETT_SCREENHEIGHT;

std::shared_ptr<Camera> MainCamera;
extern Tickable root;

namespace Scripting {
	extern void Setup();
}

extern float fieldOfView;
extern void RecalcProjections();

std::string testString;
glm::vec2 testVector;
glm::vec4 testColor;
extern bool debugPanelLayoutPolygons;

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
	RV("panels", CVar::Type::Bool, &debugPanelLayoutPolygons);

#undef RV

	Scripting::Setup();
}

void Game::LoadSettings(jsonObject& settings)
{
	//Ensure all settings you need exist with a proper default.
#define DS(K, V) if (!settings[K].is_null()) settings[K] = jsonValue(V)
#define DA(K, V) if (!settings[K].is_null()) settings[K] = json5pp::array(V)
#define DO(K, V) if (!settings[K].is_null()) settings[K] = json5pp::object(V)
	//...
	DS("example", "googly moogly");
#undef DO
#undef DA
#undef DS

	//Apply any loaded settings in whatever way you need.
	testString = settings["example"].as_string();
}

void Game::SaveSettings(jsonObject& settings)
{
	settings["example"] = testString;
}

#ifdef TESTLOADER
#include <future>
void loaderTest(float* progress)
{
	std::promise<bool> p;
	auto future = p.get_future();
	*progress = 0.0f;
	for (int i = 0; i < 100; i++)
	{
		future.wait_for(std::chrono::milliseconds(50));
		*progress += 0.01f;
	}
}
#endif

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

	commonUniforms.Fresnel = true;

#ifdef TESTLOADER
	ThreadedLoader(loaderTest, "cinematics/loader/loader.json");
#endif
}

#ifdef BECKETT_EXTRASAVEDIRS
void Game::PrepareSaveDirs()
{
	//Example:
	//VFS::MakeSaveDir("map/farm");
}
#endif

template<typename T>
static std::shared_ptr<Button> makeSceneButton(const std::string& caption)
{
	auto currentScene = root.GetChild<Tickable>("Current Scene");
	auto theButton = std::make_shared<Button>(caption, glm::vec2(0), glm::vec2(160, -1));
	theButton->OnClick = [currentScene]()
	{
		currentScene->RemoveAll();
		currentScene->AddChild(std::make_shared<T>());
	};
	theButton->ID = fmt::format("btn_{}", caption);
	return theButton;
}

void Game::Start()
{
	/*
	//Trying to convert [1,0,0] (red) from RGB to HSV
	auto red = glm::vec3(1, 0, 0);
	auto hsv = RGBtoHSV(red); //expected: 0, 1, 1
	auto hsl = RGBtoHSL(red); //expected: 0, 1, 0.5
	auto lab = LinearSRGBtoOkLab(red); //expected: 0.628 0.225 0.126, get more precise than that
	auto hsvtored = HSVtoRGB(hsv);
	auto hsltored = HSLtoRGB(hsl);
	auto labtored = OkLabToLinearSRGB(lab); //has infinitesimal non-zero results but OKAY!
	auto labtored2 = OkLabToLinearSRGB(0.628f, 0.225f, 0.126f); //same problem but CLOSE ENOUGH!
	*/

	MainCamera->ID = "Camera";

	root.ID = "Root";
	root.AddChild(MainCamera);
	
	//TODO: build a List control of sorts to switch between MapScreen, TestScreen, TrainScene, and TiledScreen.
	//root.AddChild(std::make_shared<TestScreen>());
	auto currentScene = std::make_shared<Tickable>();
	currentScene->ID = "Current Scene";
	currentScene->AddChild(std::make_shared<TestScreen>());
	root.AddChild(currentScene);

	MainCamera->FirstPerson(true);
	root.AddChild(std::make_shared<FirstPersonController>());

	auto menuPanel = std::make_shared<TestPanel>(glm::vec2(8));
	menuPanel->ID = "Menu";
	menuPanel->Spacing = 2.0f;
	menuPanel->AddChild(makeSceneButton<MapScene>("Map (3D)"));
	menuPanel->AddChild(makeSceneButton<TestScreen>("Test (2D)"));
	menuPanel->AddChild(makeSceneButton<TrainScene>("Train (3D)"));
	auto exitButton = std::make_shared<Button>("Exit", glm::vec2(0), glm::vec2(160, -1));
	exitButton->OnClick = [currentScene]() { Game::ShouldClose = true; };
	exitButton->BackColor = glm::vec4(1.0, 0.0, 0.0, 0.5);
	exitButton->ID = "btnExit";
	menuPanel->AddChild(exitButton);
	menuPanel->Reflow();
	menuPanel->Position.y = height - menuPanel->Size.y - 8;
	menuPanel->Reflow();
	root.AddChild(menuPanel);	
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
