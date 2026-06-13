#include <ctime>
#include <cctype>
#include <glm/gtc/matrix_transform.hpp>
#include "engine/Utilities.h"
#include "engine/InputsMap.h"
#include "engine/Tickable.h"
#include "engine/JsonUtils.h"
#include "engine/Console.h"
#include "engine/Text.h"
#include "engine/VFS.h"
#include "Game.h"
#include "Camera.h"
#include "Utilities.h"

extern int width, height;

bool Project(const glm::vec3& in, glm::vec2& out)
{
	//TODO: account for the curve.
	auto res = glm::project(in, commonUniforms.View, commonUniforms.Projection, glm::vec4(0.0f, 0, width, height));
	out.x = res.x;
	out.y = height - res.y;
	return res.z >= 0.0f;
}

extern "C" { const char* glfwGetKeyName(int key, int scancode); }
std::string GetKeyName(int scancode)
{
	if (scancode == 1 || scancode == 14 || scancode == 15 || scancode == 28 || scancode == 57 ||
		(scancode >= 71 && scancode <= 83) || scancode == 284 || scancode == 309)
		return Text::Get(fmt::format("keys:scan:{}", scancode));

	auto glfw = glfwGetKeyName(-1, scancode);
	if (glfw[0] == '\0')
		return Text::Get(fmt::format("keys:scan:{}", scancode));
	else
		return std::string(glfw);
}

static jsonValue constants;
static jsonValue getConstant(const std::string& key)
{
	if (!constants.is_object())
		constants = VFS::ReadJSON("constants.json");
	auto constObj = constants.as_object();
	auto it = constObj.find(key);
	if (it != constObj.end())
		return it->second;
	return nullptr;
}

int GetJSONVal(const jsonValue& jsonVal, int default)
{
	if (jsonVal.is_integer())
		return jsonVal.as_integer();
	if (jsonVal.is_string())
	{
		auto str = jsonVal.as_string();
		if (str[0] == '#')
			return GetJSONVal(getConstant(str.substr(1)), default);
		else
			throw std::exception("Found string in JSON where int was expected.");
	}
	return default;
}

float GetJSONVal(const jsonValue& jsonVal, float default)
{
	if (jsonVal.is_number())
		return jsonVal.as_number();
	if (jsonVal.is_string())
	{
		auto str = jsonVal.as_string();
		if (str[0] == '#')
			return GetJSONVal(getConstant(str.substr(1)), default);
		else
			throw std::exception("Found string in JSON where number was expected.");
	}
	return default;
}

const std::string& GetJSONVal(const jsonValue& jsonVal, const std::string& default)
{
	if (jsonVal.is_string())
	{
		auto str = jsonVal.as_string();
		if (str[0] == '#')
			return GetJSONVal(getConstant(str.substr(1)), default);
		else
			return jsonVal.as_string();
	}
	else if (jsonVal.is_null())
		return default;
	else
		throw std::exception("Found non-string in JSON where string was expected.");
}

bool GetJSONBool(const jsonValue& jsonVal, bool default)
{
	if (jsonVal.is_boolean())
		return jsonVal.as_boolean();
	if (jsonVal.is_string())
	{
		auto str = jsonVal.as_string();
		if (str[0] == '#')
			return GetJSONBool(getConstant(str.substr(1)), default);
		else
			throw std::exception("Found string in JSON where bool was expected.");
	}
	return default;
}
