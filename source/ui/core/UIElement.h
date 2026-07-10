#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include "../../core/Concepts.h"
#include "../../core/input/InputState.h"
#include "../../core/animation/AnimationState.h"


class UIInteractive;



class UIElement : public sf::Transformable
{
public:
	UIElement() = default;
	UIElement(sf::Vector2f size = { 160.0f, 90.0f }, sf::Vector2f position = { 0.0f, 0.0f })
	{
		setSize(size);
		setPosition(position);
		m_isVisible = true;
		m_hitTestChildren = true;
		m_isTransparentToInput = false;
		m_dragLocks = 1;
	};

	UIElement(const UIElement&) = delete;
	UIElement& operator=(const UIElement&) = delete;
	UIElement(UIElement&&) = default;
	UIElement& operator=(UIElement&&) = default;

	virtual ~UIElement() = 0;


	virtual void update(float dt);
	sf::FloatRect getLocalBounds() const;
	sf::FloatRect getGlobalBounds() const;
	sf::Vector2f getGeometricCenter() const;
	sf::Vector2f getGlobalGeometricCenter() const;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	UIElement* getParent();
	const UIElement* getParent() const;

	virtual bool isInteractive() const;
	virtual UIInteractive* asInteractive();

	virtual UIElement* hitTest(sf::Vector2f globalPoint);
	void setResolveHit(std::function<bool(sf::Vector2f localPoint)> func);

	sf::Transform getGlobalTransform() const;
	sf::Vector2f getGlobalPosition() const;

	sf::Vector2f getTopLeftPosition() const;
	sf::Vector2f getTopRightPosition() const;
	sf::Vector2f getBottomLeftPosition() const;
	sf::Vector2f getBottomRightPosition() const;

	sf::Vector2f getGlobalScale() const;

	sf::Vector2f toParentSpace(sf::Vector2f globalPoint);

	void setSize(sf::Vector2f size);
	sf::Vector2f getSize() const;
	sf::Vector2f getGlobalSize() const;

	UIElement& addChild(std::unique_ptr<UIElement> child);
	void eraseChild(std::unique_ptr<UIElement>& child);
	void eraseChild(UIElement& child);

	std::vector<std::unique_ptr<UIElement>>& getAllChildren();
	const std::vector<std::unique_ptr<UIElement>>& getAllChildren() const;

	void setVisible(bool value);
	bool isVisible() const;

	void setChildrenHitTest(bool value);
	bool isChildrenHitTest() const;

	void setTransparentToInput(bool value);
	bool isTransparentToInput() const;

	void blockDraggable();
	void unblockDraggable();

	bool isDraggable() const;

	template<AnimationType T, typename... Args>
	T& addAnimation(Args&&... args)
	{
		std::unique_ptr<T> animationPtr = std::make_unique<T>(std::forward<Args>(args)...);
		T& ref = *animationPtr;
		animationState.animations.push_back(std::move(animationPtr));

		return ref;
	}

	bool isAnimating() const;

protected:
	sf::Vector2f m_size;

	UIElement* m_parent = nullptr;
	std::vector<std::unique_ptr<UIElement>> m_children;

	std::function<bool(sf::Vector2f localPoint)> m_resolveHit;


	void updateAnimations(float dt)
	{
		for (auto it = animationState.animations.begin(); it != animationState.animations.end(); )
		{
			if (it->get()->update(dt))
				it = animationState.animations.erase(it);
			else
				++it;
		}
	}

	virtual void drawSelf(sf::RenderTarget& target, sf::RenderStates states) const {}
	virtual bool resolveHit(sf::Vector2f localPoint) const;

	virtual void onSizeChanged(sf::Vector2f ratio) {};

private:
	AnimationState animationState;

	bool m_isVisible;
	bool m_hitTestChildren;
	bool m_isTransparentToInput;

	uint32_t m_dragLocks;
};