#include "Game.h"
#include "engine/Console.h"
#include "engine/Cursor.h"
#include "engine/Text.h"
#include "engine/JsonUtils.h"
#include "engine/InputsMap.h"

__declspec(noreturn)
extern void FatalError(const std::string& message);

extern "C" {
	const char* glfwGetKeyName(int key, int scancode);
	int glfwGetKeyScancode(int key);
}

constexpr int ScreenWidth = SCREENWIDTH;
constexpr int ScreenHeight = SCREENHEIGHT;

class TestScreen : public Tickable
{
	Texture sprite{ "succfarm.png" };

	bool Tick(float dt)
	{
		RevAllTickables(tickables, dt);
		return true;
	}

	void Draw(float dt)
	{
		DrawAllTickables(tickables, dt);

		Sprite::DrawSprite(sprite, glm::vec2(80) * scale, glm::vec2(16, 32) * scale, sprite[1]);
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
		DS("language", "USen");
		DA("keyBinds", {});
		DA("gamepadBinds", {});
#undef DA
#undef DS

		width = settings["screenWidth"].as_integer();
		height = settings["screenHeight"].as_integer();

		//constexpr Language opt2lan[] = { Language::USen, Language::JPja, Language::EUde, Language::EUes, Language::EUfr, Language::EUit, Language::EUhu, Language::EUnl, Language::EUen };
		//gameLang = opt2lan[settings["language"]->AsInteger()];
		gameLang = Text::GetLangCode(settings["language"].as_string());

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
