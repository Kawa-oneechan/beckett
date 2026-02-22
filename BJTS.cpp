#include "engine/Console.h"
#include "engine/InputsMap.h"
#include "engine/Text.h"
#include "Game.h"

static const char* bindingNames[] = {
	"up", "down", "left", "right",
	"accept", "back", "pageup", "pagedown",
	"walkn", "walkw", "walks", "walke",
	"interact", "pickup",
	"cameracw", "cameraccw", "cameraup", "cameradown",
	"inventory", "map", "react",
	"hotbar1", "hotbar2", "hotbar3", "hotbar4", "hotbar5",
	"hotbar6", "hotbar7", "hotbar8", "hotbar9", "hotbar10",
	"console"
};

static void bjtsStr(std::string& data, BJTSParams)
{
	if (tags.size() < 2)
	{
		conprint(2, "Missing parameter in BJTS Str: {}", data.substr(start, len));
		return;
	}

	if (tags[1] == "player")
		data.replace(start, len, "Player");
}

static void bjtsKeyControl(std::string& data, BJTSParams)
{
	if (tags.size() < 2)
	{
		conprint(2, "Missing parameter in BJTS Key: {}", data.substr(start, len));
		return;
	}
	if (tags[1] == "arrows")
	{
		data.replace(start, len, fmt::format("{}/{}/{}/{}",
			Beck::Inputs.Keys[(int)Binds::WalkN].Name,
			Beck::Inputs.Keys[(int)Binds::WalkS].Name,
			Beck::Inputs.Keys[(int)Binds::WalkE].Name,
			Beck::Inputs.Keys[(int)Binds::WalkW].Name
		));
		return;
	}
	else  if (tags[1] == "updown")
	{
		data.replace(start, len, fmt::format("{}/{}",
			Beck::Inputs.Keys[(int)Binds::Up].Name,
			Beck::Inputs.Keys[(int)Binds::Down].Name
		));
		return;
	}
	else  if (tags[1] == "page")
	{
		data.replace(start, len, fmt::format("{}/{}",
			Beck::Inputs.Keys[(int)Binds::PageUp].Name,
			Beck::Inputs.Keys[(int)Binds::PageDown].Name
		));
		return;
	}
	for (int i = 0; i < NumKeyBinds; i++)
	{
		if (tags[1] == bindingNames[i])
		{
			data.replace(start, len, Beck::Inputs.Keys[i].Name);
			return;
		}
	}
}

static void bjtsGamepad(std::string& data, BJTSParams)
{
	if (tags.size() < 2)
	{
		conprint(2, "Missing parameter in BJTS Pad: {}", data.substr(start, len));
		return;
	}
	if (tags[1] == "dpad")
	{
		data.replace(start, len, fmt::format("{}/{}/{}/{}",
			GamepadPUAMap[Beck::Inputs.Keys[(int)Binds::WalkN].GamepadButton],
			GamepadPUAMap[Beck::Inputs.Keys[(int)Binds::WalkS].GamepadButton],
			GamepadPUAMap[Beck::Inputs.Keys[(int)Binds::WalkE].GamepadButton],
			GamepadPUAMap[Beck::Inputs.Keys[(int)Binds::WalkW].GamepadButton]
		));
		return;
	}
	else if (tags[1] == "updown")
	{
		data.replace(start, len, fmt::format("{}/{}",
			GamepadPUAMap[Beck::Inputs.Keys[(int)Binds::Up].GamepadButton],
			GamepadPUAMap[Beck::Inputs.Keys[(int)Binds::Down].GamepadButton]
		));
		return;
	}
	for (int i = 0; i < NumKeyBinds; i++)
	{
		if (tags[1] == bindingNames[i])
		{
			data.replace(start, len, GamepadPUAMap[Beck::Inputs.Keys[i].GamepadButton]);
			return;
		}
	}
}

const std::map<std::string, Beck::BJTSFunc> bjtsPhase1 = {
	{ "str", &bjtsStr },
	{ "key", &bjtsKeyControl },
	{ "pad", &bjtsGamepad },
};

std::map<std::string, std::string> bjtsPhase1X;
