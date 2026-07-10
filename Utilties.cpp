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

