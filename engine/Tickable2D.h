#pragma once
#include <glm/glm.hpp>
#include "Tickable.h"
#include "SpriteRenderer.h"
#include "InputsMap.h"
#include "Texture.h"

extern float scale;

class Tickable2D;

class Tickable2D : public Tickable
{
protected:
	Tickable2D* parent{ nullptr }; 
public:
	glm::vec2 Position;
	glm::vec2 AbsolutePosition;

	virtual bool Tick(float dt) override
	{
		AbsolutePosition = (parent ? (parent->AbsolutePosition + Position) : Position);

		for (int i = 0; i < ChildTickables.size(); i++)
		{
			if (auto t2D = std::dynamic_pointer_cast<Tickable2D>(ChildTickables[i]))
				t2D->parent = this;
		}

		return Tickable::Tick(dt);
	}

	virtual glm::vec2 GetMinimalSize()
	{
		auto ret = glm::vec2(0);
		for (int i = 0; i < ChildTickables.size(); i++)
		{
			if (auto t2D = std::dynamic_pointer_cast<Tickable2D>(ChildTickables[i]))
			{
				auto tl = t2D->Position;
				auto br = tl + t2D->GetSize();
				if (br.x > ret.x)
					ret.x = br.x;
				if (br.y > ret.y)
					ret.y = br.y;
			}
		}
		return ret;
	}

	virtual glm::vec2 GetSize()
	{
		return GetMinimalSize();
	}

	void UpdatePosition()
	{
		if (parent)
		{
			AbsolutePosition = parent->AbsolutePosition + Position;
		}
		else
		{
			AbsolutePosition = Position;
		}
		for (int i = 0; i < ChildTickables.size(); i++)
		{
			if (auto t2D = std::dynamic_pointer_cast<Tickable2D>(ChildTickables[i]))
				t2D->UpdatePosition();
		}
	}
};

using Tickable2DP = std::shared_ptr<Tickable2D>;

class TextLabel : public Tickable2D
{
public:
	std::string Text;
	glm::vec4 Color{ 1, 1, 1, 1 };
	float Size{ 100.0f };
	float Angle{ 0.0f };
	int Font{ 1 };
	bool Raw{ false };

	TextLabel(const std::string& text, glm::vec2 position) : Text(text)
	{
		parent = nullptr;
		Position = position;
	}

	void Draw(float) override
	{
		Sprite::DrawText(Font, Text, AbsolutePosition, Color, Size, Angle, Raw);
	}
};

using TextLabelP = std::shared_ptr<TextLabel>;

class SimpleSprite : public Tickable2D
{
private:
	Texture* texture;
public:
	Sprite::SpriteFlags Flags{ Sprite::SpriteFlags::NoFlags };
	int Frame;

	SimpleSprite(const std::string& texture, int frame, glm::vec2 position)
	{
		parent = nullptr;
		this->texture = new Texture(texture);
		Position = position;
		Frame = frame;
	}

	SimpleSprite(Texture* texture, int frame, glm::vec2 position)
	{
		parent = nullptr;
		this->texture = texture;
		Position = position;
		Frame = frame;
	}

	~SimpleSprite() override
	{
		delete this->texture;
	}

	void Draw(float) override
	{
		auto frame = texture->operator[](Frame);
		auto size = glm::vec2(frame.z, frame.w);
		Sprite::DrawSprite(*texture, AbsolutePosition, size, frame, 0.0f, glm::vec4(1), Flags);
	}
};
