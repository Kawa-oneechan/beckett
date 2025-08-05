#include "Game.h"
#include "engine/Console.h"
#include "engine/Cursor.h"
#include "engine/Text.h"
#include "engine/JsonUtils.h"
#include "engine/InputsMap.h"
#include "engine/Model.h"

//Wouldn't need this here if the camera were a proper
//class in its own file like in PSK.
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_inverse.hpp>

__declspec(noreturn)
extern void FatalError(const std::string& message);

extern "C" int glfwGetKeyScancode(int key);

constexpr int ScreenWidth = SCREENWIDTH;
constexpr int ScreenHeight = SCREENHEIGHT;

class TestScreen : public Tickable
{
	Texture sprite{ "example/farrah.png" };
	Texture stage{ "example/stage.png" };
	Model model{ "example/teapot.fbx" };

	bool Tick(float dt)
	{
		RevAllTickables(tickables, dt);
		return true;
	}

	void Draw(float dt)
	{
		DrawAllTickables(tickables, dt);

		auto time = commonUniforms.TotalTime * 10.0f;

		//--------------------
		// 3D integration
		//--------------------
		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		model.Draw(glm::vec3(0), time);
		MeshBucket::Flush();

		glDisable(GL_DEPTH_TEST);
		//--------------------

		auto& frame = stage[0];
		Sprite::DrawSprite(stage,
			glm::vec2(((width / scale) - frame.z) * 0.5f, ((height / scale) + (frame.w * 0.5)) * 0.5f) * scale,
			glm::vec2(frame.z, frame.w) * scale,
			frame,
			0.0f,
			glm::vec4(glm::vec3(0.5f + glm::abs(glm::sin(time * 0.1f) * 0.5f)), 1.0f));

		frame = sprite[(int)time / 2 % sprite.Frames()];
		Sprite::DrawSprite(sprite,
			glm::vec2(((width / scale) - frame.z) * 0.5f, ((height / scale) - (frame.w * 0.5)) * 0.5f) * scale,
			glm::vec2(frame.z, frame.w) * scale,
			frame);

		Sprite::DrawText(1,
			PreprocessBJTS("Hello, Beckett Engine!"),
			glm::vec2(16), glm::vec4(1));
	}
};

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

void GameImGui()
{
	//Put this function in its own module and #include <ImGUI/imgui.h>.
	//Rendering is handled by the engine, just handle windows here.
}

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
}

void GameLoopStart()
{
	cursor->SetScale((int)(scale * 100));
}

void GamePreDraw(float dt)
{
	dt;
	glClearColor(0.0f, 0.1f, 0.2f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void GameQuit()
{
}
