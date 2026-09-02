#include "engine/SpriteRenderer.h"
#include "engine/InputsMap.h"
#include "engine/NineSlicer.h"
#include "engine/Font.h"
#include "engine/Audio.h"
#include "DemoUI.h"

extern Texture* whiteRect;

static std::shared_ptr<Sound> clickSound;
static UIControl* focused{ nullptr };
static UIControl* trapped{ nullptr }; //so we can drag slider thumbs without staying precisely in the box

static void FrameDrawer(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color, int flags)
{
	static auto texture = Texture{ "ui/button.png" };
	NineSlicer::Draw(texture, pos, const_cast<glm::vec2&>(size), 1.0f, color, flags * 9);
}

UIControl::~UIControl()
{
	if (focused == this)
		focused = nullptr;
}

bool UIControl::Scancode(unsigned int sc)
{
	if (!IsFocused())
		return Tickable::Scancode(sc);
	if (sc == 15 && navTarget[4])
	{
		navTarget[4]->SetFocus();
		return true;
	}
	return false;
}

bool UIControl::IsHovered() const
{
	return
		Inputs.MousePosition.x > AbsolutePosition.x &&
		Inputs.MousePosition.y > AbsolutePosition.y &&
		Inputs.MousePosition.x < AbsolutePosition.x + Size.x &&
		Inputs.MousePosition.y < AbsolutePosition.y + Size.y;
}

bool UIControl::IsClicked() const
{
	return
		Inputs.LastClickLeft.x > AbsolutePosition.x &&
		Inputs.LastClickLeft.y > AbsolutePosition.y &&
		Inputs.LastClickLeft.x < AbsolutePosition.x + Size.x &&
		Inputs.LastClickLeft.y < AbsolutePosition.y + Size.y;
}

bool UIControl::IsFocused() const
{
	return focused == this;
}

void UIControl::SetFocus()
{
	focused = this;
}

UIControl* UIControl::GetNavTarget(int direction)
{
	if (direction < 0 || direction > 4) return nullptr;
	return navTarget[direction];
}

void UIControl::SetNavTarget(int direction, UIControl* target)
{
	if (direction < 0 || direction > 4) return;
	navTarget[direction] = target;
}

Button::Button(const std::string& text, glm::vec2 position, glm::vec2 size) : Text(text)
{
	parent = nullptr;
	Position = position;
	Size = size;
	UIControl::TabStop = true;

	if (!clickSound)
		clickSound = std::make_shared<Sound>("ui/click5.ogg");

	auto minSize = Sprite::MeasureText(Font, Text, TextSize, Raw) + glm::vec2(16, 8);
	if (Size.x == -1)
		Size.x = minSize.x;
	if (Size.y == -1)
		Size.y = minSize.y;
}

bool Button::Tick(float dt)
{
	if (!Mutex && CanRun())
	{
		Call();
		return false;
	}

	if (IsClicked())
	{
		SetFocus();
		if (!ScriptID.empty())
		{
			Inputs.LastClickLeft = glm::vec2(-1000);
			clickSound->Play(true);
			Execute("onClick", nullptr);
			return true;
		}
		if (!OnClick)
			return true;
		Inputs.LastClickLeft = glm::vec2(-1000);
		clickSound->Play(true);
		OnClick(*this);
		return false;
	}
	return true;
}

void Button::Draw(float)
{
	auto frame = 0;
	if (!Enabled)
	{
		frame = 4;
	}
	else
	{
		if (IsFocused())
			frame = 2;
		if (IsHovered())
		{
			frame = 3;
			if (Inputs.MouseHoldLeft)
				frame = 1;
		}
	}

	if (!OnDraw)
		OnDraw = FrameDrawer;
	OnDraw(AbsolutePosition, Size, BackColor, frame);

	auto size = Sprite::MeasureText(Font, Text, TextSize, Raw);
	auto center = (Size * 0.5f) - (size * 0.5f);
	Sprite::DrawText(Font, Text, AbsolutePosition + center, Color, TextSize, Angle, Raw);
}

bool Button::Scancode(unsigned int sc)
{
	if (!IsFocused())
		return Tickable::Scancode(sc);
	if (sc == 28 && OnClick)
	{
		clickSound->Play(true);
		OnClick(*this);
		return true;
	}
	return UIControl::Scancode(sc);
}

glm::vec2 Button::GetMinimalSize()
{
	return Sprite::MeasureText(Font, Text, TextSize, Raw) + glm::vec2(16, 8);
}

Panel::Panel(glm::vec2 position, glm::vec2 size)
{
	parent = nullptr;
	Position = position;
	Size = size;

	if (Size.x == -1) Size.x = 32;
	if (Size.y == -1) Size.y = 32;

	Size += glm::vec2(8);
}

void Panel::Draw(float dt)
{
	if (!OnDraw)
		OnDraw = FrameDrawer;
	OnDraw(AbsolutePosition, Size, BackColor, 4);

	Tickable2D::Draw(dt);
}

void FlowPanelV::Reflow()
{
	UpdatePosition();
	Tick(0.0f);
	auto pos = glm::vec2(Margin);
	for (int i = 0; i < ChildTickables.size(); i++)
	{
		if (auto t2D = std::dynamic_pointer_cast<Tickable2D>(ChildTickables[i]))
		{
			t2D->Position = pos;
			t2D->UpdatePosition();
			pos.y += t2D->GetSize().y + Spacing;
		}
	}
	Size = GetMinimalSize() + glm::vec2(Margin);
}

void FlowPanelH::Reflow()
{
	UpdatePosition();
	Tick(0.0f);
	auto pos = glm::vec2(Margin);
	for (int i = 0; i < ChildTickables.size(); i++)
	{
		if (auto t2D = std::dynamic_pointer_cast<Tickable2D>(ChildTickables[i]))
		{
			t2D->Position = pos;
			t2D->UpdatePosition();
			pos.x += t2D->GetSize().x + Spacing;
		}
	}
	Size = GetMinimalSize() + glm::vec2(Margin);
}

TrackBar::TrackBar(int value, int min, int max, int step, glm::vec2 position, float length, bool vertical) : vertical(vertical), Value(value), Min(min), Max(max), Step(step)
{
	parent = nullptr;
	Position = position;
	UIControl::TabStop = true;
	Size = vertical ? glm::vec2(Thickness, length) : glm::vec2(length, Thickness);
}

bool TrackBar::Tick(float dt)
{
	if (IsHovered())
	{
		if (!Inputs.MouseHoldLeft)
			return true;
		SetFocus();

		auto oldVal = Value;

		auto mp = !vertical ? Inputs.MousePosition.x : Inputs.MousePosition.y;
		auto ap = !vertical ? AbsolutePosition.x : AbsolutePosition.y;
		auto sz = !vertical ? Size.x : Size.y;

		auto x = glm::clamp(mp, ap, ap + sz);
		auto v = Min + ((x - ap) * (Max - Min)) / sz;
		Value = glm::clamp((int)(round(v / Step) * Step), Min, Max);

		if (Value != oldVal)
		{
			clickSound->Play(true);
			if (OnChange)
				OnChange(*this);
		}
		return false;
	}
	return true;
}

void TrackBar::Draw(float)
{
	auto frame = 0;
	if (!Enabled)
	{
		frame = 4;
	}
	else
	{
		if (IsFocused())
			frame = 2;
		if (IsHovered())
		{
			frame = 3;
			if (Inputs.MouseHoldLeft)
				frame = 1;
		}
	}

	if (!OnDraw)
		OnDraw = FrameDrawer;
	OnDraw(AbsolutePosition, Size, TrackColor, 4);

	auto range = Max - Min;
	auto ccur = glm::clamp(Value, Min, Max) - Min;
	if (!vertical)
	{
		auto thumbPos = ((ccur * (Size.x - (Size.y * 0.5f))) / range);
		OnDraw(AbsolutePosition + glm::vec2(thumbPos, 0), glm::vec2(Size.y * 0.5f, Size.y), Color, frame);
	}
	else
	{
		auto thumbPos = ((ccur * (Size.y - (Size.x * 0.5f))) / range);
		OnDraw(AbsolutePosition + glm::vec2(0, thumbPos), glm::vec2(Size.x, Size.x * 0.5f), Color, frame);
	}
}

bool TrackBar::Scancode(unsigned int sc)
{
	if (!IsFocused())
		return Tickable::Scancode(sc);
	auto oldVal = Value;
	if ((!vertical && sc == 331) || (sc == 328))
	{
		Value = glm::clamp(Value - Step, Min, Max);
	}
	else if ((!vertical && sc == 333) || (sc == 336))
	{
		Value = glm::clamp(Value + Step, Min, Max);
	}
	if (Value != oldVal)
	{
		clickSound->Play(true);
		if (OnChange)
			OnChange(*this);
		return true;
	}
	return UIControl::Scancode(sc);
}

CheckBox::CheckBox(const std::string& text, glm::vec2 position) : Text(text)
{
	parent = nullptr;
	Position = position;
	UIControl::TabStop = true;

	if (!clickSound)
		clickSound = std::make_shared<Sound>("ui/click5.ogg");

	Size = Sprite::MeasureText(Font, Text, TextSize, Raw) + glm::vec2(24);
}

bool CheckBox::Tick(float dt)
{
	if (IsClicked())
	{
		SetFocus();
		Checked = !Checked;
		if (!OnChange)
			return true;
		Inputs.LastClickLeft = glm::vec2(-1000);
		clickSound->Play(true);
		OnChange(*this);
		return false;
	}
	return true;

}

void CheckBox::Draw(float dt)
{
	static auto texture = Texture{ "ui/button.png" };

	auto frame = 0;
	if (!Enabled)
	{
		frame = 4;
	}
	else
	{
		if (IsFocused())
			frame = 2;
		if (IsHovered())
		{
			frame = 3;
			if (Inputs.MouseHoldLeft)
				frame = 1;
		}
	}
	if (Checked)
		frame += 5;

	frame += 45;

	Sprite::DrawSprite(texture, AbsolutePosition + glm::vec2(3), texture[frame]);
	auto fs = texture[frame].z;
	Sprite::DrawText(Font, Text, AbsolutePosition + glm::vec2(fs + 8, 0), Color, TextSize, Angle, Raw);
}

bool CheckBox::Scancode(unsigned int sc)
{
	if (!IsFocused())
		return Tickable::Scancode(sc);
	if (sc == 28 && OnChange)
	{
		clickSound->Play(true);
		OnChange(*this);
		return true;
	}
	return UIControl::Scancode(sc);
}

//---------------------

void SetupNavTargets(Tickable* root)
{
	std::vector<UIControl*> theBigList;
	theBigList.reserve(64);
	std::function<void(Tickable*)> worker = [&](const Tickable* from)
	{
		for (auto i = 0; i < from->size(); i++)
		{
			auto j = from->operator[](i);
			if (auto uic = dynamic_cast<UIControl*>(j))
			{
				if (uic->Enabled && uic->TabStop)
					theBigList.push_back(uic);
			}
			worker(j);
		}
	};
	worker(root);
	if (theBigList.size() < 2)
		return;

	//Simple tabs
	for (auto i = 0; i < theBigList.size() - 1; i++)
		theBigList[i]->SetNavTarget(4, theBigList[i + 1]);
	theBigList[theBigList.size() - 1]->SetNavTarget(4, theBigList[0]);

	//TODO: spatial
}
