#pragma once
#include <vector>
#include <functional>
#include <glm/glm.hpp>
#include "engine/Types.h"
#include "engine/JsonUtils.h"

//Helper function to project a point in world space to screen space, according to the current camera.
//TODO: does it return true if it's in front of the camera plane or behind it?
extern bool Project(const glm::vec3& in, glm::vec2& out);

//Returns the name of a key for the given scancode, using glfwGetKeyName for printables and Text::Get for specials.
extern std::string GetKeyName(int scancode);

extern int GetJSONVal(const jsonValue& jsonVal, int default);
extern float GetJSONVal(const jsonValue& jsonVal, float default);
extern const std::string& GetJSONVal(const jsonValue& jsonVal, const std::string& default);
extern bool GetJSONBool(const jsonValue& jsonVal, bool default);

extern Tickable root;
