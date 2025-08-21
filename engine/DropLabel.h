#pragma once
#include <glm/glm.hpp>
#include "Texture.h"

class DropLabel
{
private:
	std::string text;
	glm::vec2 size;
	Texture canvas;
	
	void update();

public:
	DropLabel(const std::string& text);
	void SetText(const std::string& text);
	const glm::vec2 Size() const { return size; }
	Texture& Texture() { return canvas; }
};
