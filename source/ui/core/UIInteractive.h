#pragma once
#include <utility>
#include <functional>
#include "UIElement.h"
#include "../state/ControlState.h"



class UIInteractive : public UIElement
{
public:
	UIInteractive() = default;
	UIInteractive(sf::Vector2f size, sf::Vector2f position) : UIElement(size, position) {}

	UIInteractive(const UIInteractive&) = delete;
	UIInteractive& operator=(const UIInteractive&) = delete;
	UIInteractive(UIInteractive&&) = default;
	UIInteractive& operator=(UIInteractive&&) = default;

	virtual ~UIInteractive() = 0;


	bool isInteractive() const override { return true; }
	UIInteractive* asInteractive() override { return this; }

	void setOnHover(std::function<void(UIInteractive&)> func) { onHover = std::move(func); }
	void setOnHoverEnd(std::function<void(UIInteractive&)> func) { onHoverEnd = std::move(func); }
	void setOnPressed(std::function<void(UIInteractive&)> func) { onPressed = std::move(func); }
	void setOnReleased(std::function<void(UIInteractive&, bool isHit)> func) { onReleased = std::move(func); }

	void setOnClick(std::function<void()> func) { m_onClick = std::move(func); };
	void onClick() { if (m_onClick) m_onClick(); }

	void setHovered(bool value) { onHoverChanged(value); }
	void setPressed(bool value, bool isHit = true) { onPressedChanged(value, isHit); }

	bool isHovered() const { return control.interaction.hovered; }
	bool isPressed() const { return control.interaction.pressed; }


	void blockHovered()
	{
		if (control.locks.hover == 0)
			setHovered(false);

		++control.locks.hover;
	}

	void blockPressed()
	{
		if (control.locks.press == 0)
			setPressed(false);

		++control.locks.press;
	}

	void blockClick()
	{
		++control.locks.click;
	}

	void blockFullActions()
	{
		blockHovered();
		blockPressed();
		blockClick();
	}


	void unblockHovered()
	{
		if (control.locks.hover > 0)
			--control.locks.hover;
	}

	void unblockPressed()
	{
		if (control.locks.press > 0)
			--control.locks.press;
	}

	void unblockClick()
	{
		if (control.locks.click > 0)
			--control.locks.click;
	}

	void unblockFullActions()
	{
		unblockHovered();
		unblockPressed();
		unblockClick();
	}


	bool canBeHovered() const { return control.locks.hover == 0; }
	bool canBePressed() const { return control.locks.press == 0; }
	bool canBeClicked() const { return control.locks.click == 0; }

protected:
	void onHoverChanged(bool value)
	{
		if (value == control.interaction.hovered)
			return;

		if (!canBeHovered())
			return;

		control.interaction.hovered = value;

		if (control.interaction.hovered)
		{
			if (onHover)
				onHover(*this);
		}

		else
		{
			if (onHoverEnd)
				onHoverEnd(*this);
		}
	}

	void onPressedChanged(bool value, bool isHit)
	{
		if (value == control.interaction.pressed)
			return;

		if (!canBePressed())
			return;

		control.interaction.pressed = value;

		if (control.interaction.pressed)
		{
			if (onPressed)
				onPressed(*this);
		}

		else
		{
			if (onReleased)
				onReleased(*this, isHit);

			if (isHit && canBeHovered())
				setHovered(true);
		}
	}

private:
	ControlState control;

	std::function<void(UIInteractive&)> onHover;
	std::function<void(UIInteractive&)> onHoverEnd;
	std::function<void(UIInteractive&)> onPressed;
	std::function<void(UIInteractive&, bool isHit)> onReleased;

	std::function<void()> m_onClick;
};


inline UIInteractive::~UIInteractive() {}