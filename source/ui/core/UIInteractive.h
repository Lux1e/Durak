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


	bool isInteractive() const override;
	UIInteractive* asInteractive() override;

	void captureInput();
	void releaseInput();
	bool isCaptured() const;

	void resetInteractive();

	void setOnInputUpdate(std::function<void(UIInteractive&, const InputState&)> func);
	void setOnHandleEvents(std::function<void(UIInteractive&, const InputState&, const sf::Event&)> func);

	void inputUpdate(const InputState& inputState);

	void handleEvents(const InputState& inputState, const sf::Event& event);

	void setOnCaptureEnd(std::function<void(UIInteractive&)> func);
	void onCaptureEnd();

	void setOnMouseDown(std::function<void(UIInteractive&)> func);
	void setOnMouseUp(std::function<void(UIInteractive&, bool)> func);

	void onMouseDown();
	void onMouseUp(bool isHit);

	void setOnHover(std::function<void(UIInteractive&)> func);
	void setOnHoverEnd(std::function<void(UIInteractive&)> func);
	void setOnPressed(std::function<void(UIInteractive&)> func);
	void setOnReleased(std::function<void(UIInteractive&, bool isHit)> func);

	void setOnClick(std::function<void()> func);
	void onClick();

	void setHovered(bool value);
	void setDragged(bool value);

	bool isHovered() const;
	bool isPressed() const;
	bool isDragged() const;

	void blockHover();
	void blockPress();
	void blockClick();
	void blockDrag();

	void blockInteraction();

	void unblockHover();
	void unblockPress();
	void unblockClick();
	void unblockDrag();

	void unblockInteraction();

	bool canBeHovered() const;
	bool canBePressed() const;
	bool canBeClicked() const;
	bool canBeDragged() const;

protected:
	void setPressed(bool value, bool isHit = true);

	void applyDrag(const InputState& inputState);

private:
	ControlState control;

	std::function<void(UIInteractive&, const InputState&)> m_onInputUpdate;
	std::function<void(UIInteractive&, const InputState&, const sf::Event& event)> m_onHandleEvents;

	std::function<void(UIInteractive&)> m_onCaptureEnd;

	std::function<void(UIInteractive&)> m_onMouseDown;
	std::function<void(UIInteractive&, bool)> m_onMouseUp;

	std::function<void(UIInteractive&)> m_onHover;
	std::function<void(UIInteractive&)> m_onHoverEnd;
	std::function<void(UIInteractive&)> m_onPressed;
	std::function<void(UIInteractive&, bool isHit)> m_onReleased;

	std::function<void()> m_onClick;
};


inline UIInteractive::~UIInteractive() {}