#include "UIInteractive.h"
#include "../../core/input/InputCapture.h"



bool UIInteractive::isInteractive() const
{
	return true;
}

UIInteractive* UIInteractive::asInteractive()
{
	return this;
}


void UIInteractive::captureInput()
{
	if (m_inputCapture)
		m_inputCapture->set(*this);
}

void UIInteractive::releaseInput()
{
	if (m_inputCapture)
		m_inputCapture->release();
}

bool UIInteractive::isCaptured() const
{
	if (m_inputCapture)
		return m_inputCapture->get() == this;
	else
		return false;
}


void UIInteractive::resetInteractive()
{
	if (isHovered()) setHovered(false);
	if (isPressed()) setPressed(false, false);
	if (isDragged()) setDragged(false);
}


void UIInteractive::setOnInputUpdate(std::function<void(UIInteractive&, const InputState&)> func)
{
	m_onInputUpdate = std::move(func);
}

void UIInteractive::setOnHandleEvents(std::function<void(UIInteractive&, const InputState&, const sf::Event&)> func)
{
	m_onHandleEvents = std::move(func);
}

void UIInteractive::inputUpdate(const InputState& inputState)
{
	if (isDragged())
		applyDrag(inputState);

	if (m_onInputUpdate)
		m_onInputUpdate(*this, inputState);
};

void UIInteractive::handleEvents(const InputState& inputState, const sf::Event& event)
{
	if (m_onHandleEvents)
		m_onHandleEvents(*this, inputState, event);
};


void UIInteractive::setOnCaptureEnd(std::function<void(UIInteractive&)> func)
{
	m_onCaptureEnd = std::move(func);
};

void UIInteractive::onCaptureEnd()
{
	setPressed(false, false);
	setDragged(false);
	if (m_onCaptureEnd)
		m_onCaptureEnd(*this);
};


void UIInteractive::setOnMouseDown(std::function<void(UIInteractive&)> func)
{
	m_onMouseDown = std::move(func);
}

void UIInteractive::setOnMouseUp(std::function<void(UIInteractive&, bool)> func)
{
	m_onMouseUp = std::move(func);
}

void UIInteractive::onMouseDown()
{
	if (control.locks.press > 0)
		return;

	setPressed(true);
	if (m_onMouseDown)
		m_onMouseDown(*this);
};

void UIInteractive::onMouseUp(bool isHit)
{
	if (control.locks.press > 0)
		return;

	bool wasPressed = control.interaction.pressed;
	setPressed(false, isHit);

	if (wasPressed && isHit)
		onClick();

	if (m_onMouseUp)
		m_onMouseUp(*this, isHit);
};


void UIInteractive::setOnHover(std::function<void(UIInteractive&)> func)
{
	m_onHover = std::move(func);
}

void UIInteractive::setOnHoverEnd(std::function<void(UIInteractive&)> func)
{
	m_onHoverEnd = std::move(func);
}

void UIInteractive::setOnPressed(std::function<void(UIInteractive&)> func)
{
	m_onPressed = std::move(func);
}

void UIInteractive::setOnReleased(std::function<void(UIInteractive&, bool isHit)> func)
{
	m_onReleased = std::move(func);
}

void UIInteractive::setOnClick(std::function<void()> func)
{
	m_onClick = std::move(func);
};

void UIInteractive::onClick()
{
	if (!canBeClicked())
		return;

	if (m_onClick)
		m_onClick();
}


void UIInteractive::setOnDisabled(std::function<void(UIInteractive&, bool)> func)
{
	m_onDisabled = std::move(func);
}


void UIInteractive::setHovered(bool value)
{
	if (value == control.interaction.hovered || control.locks.hover != 0 || isDisabled())
		return;

	control.interaction.hovered = value;

	if (control.interaction.hovered)
	{
		if (m_onHover)
			m_onHover(*this);
	}

	else
	{
		if (m_onHoverEnd)
			m_onHoverEnd(*this);
	}
}

void UIInteractive::setDragged(bool value)
{
	if (value == control.interaction.dragged || control.locks.drag != 0 || isDisabled())
		return;

	control.interaction.dragged = value;
}


void UIInteractive::setDisabled(bool value)
{
	if (value == control.interaction.disabled)
		return;

	control.interaction.disabled = value;

	if (m_onDisabled)
		m_onDisabled(*this, value);
}


bool UIInteractive::isHovered() const
{
	return control.interaction.hovered;
}

bool UIInteractive::isPressed() const
{
	return control.interaction.pressed;
}

bool UIInteractive::isDragged() const
{
	return control.interaction.dragged;
}

bool UIInteractive::isDisabled() const
{
	return control.interaction.disabled;
}


void UIInteractive::blockHover()
{
	setHovered(false);
	++control.locks.hover;
}

void UIInteractive::blockPress()
{
	if (isCaptured())
		releaseInput();
	else
		setPressed(false, false);

	++control.locks.press;
}

void UIInteractive::blockClick()
{
	++control.locks.click;
}

void UIInteractive::blockDrag()
{
	setDragged(false);
	++control.locks.drag;
}

void UIInteractive::blockInteraction()
{
	blockHover();
	blockPress();
	blockClick();
	blockDrag();
	setDisabled(true);
}


void UIInteractive::unblockHover()
{
	if (control.locks.hover > 0)
		--control.locks.hover;
}

void UIInteractive::unblockPress()
{
	if (control.locks.press > 0)
		--control.locks.press;
}

void UIInteractive::unblockClick()
{
	if (control.locks.click > 0)
		--control.locks.click;
}

void UIInteractive::unblockDrag()
{
	if (control.locks.drag > 0)
		--control.locks.drag;
}

void UIInteractive::unblockInteraction()
{
	unblockHover();
	unblockPress();
	unblockClick();
	unblockDrag();
	setDisabled(false);
}


bool UIInteractive::canBeHovered() const
{
	return control.locks.hover == 0;
}

bool UIInteractive::canBePressed() const
{
	return control.locks.press == 0;
}

bool UIInteractive::canBeClicked() const
{
	return control.locks.click == 0;
}

bool UIInteractive::canBeDragged() const
{
	return control.locks.drag == 0;
}


void UIInteractive::setPressed(bool value, bool isHit)
{
	if (value == control.interaction.pressed || control.locks.press != 0 || isDisabled())
		return;

	control.interaction.pressed = value;

	if (control.interaction.pressed)
	{
		if (m_onPressed)
			m_onPressed(*this);
	}

	else
	{
		if (m_onReleased)
			m_onReleased(*this, isHit);
	}
}


void UIInteractive::applyDrag(const InputState& inputState)
{
	sf::Transform inverse = m_parent ? m_parent->getGlobalTransform().getInverse() : sf::Transform::Identity;
	sf::Vector2f delta = inverse.transformPoint(inputState.mouseDeltaWorld) - inverse.transformPoint({ 0.0f, 0.0f });

	setPosition(getPosition() + delta);
}