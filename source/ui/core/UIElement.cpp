#include "UIElement.h"
#include "UIInteractive.h"



void UIElement::update(float dt)
{
	updateAnimations(dt);

	for (auto& child : m_children)
		child->update(dt);
}

sf::FloatRect UIElement::getLocalBounds() const
{
	return { { 0.0f, 0.0f }, m_size };
}

sf::FloatRect UIElement::getGlobalBounds() const
{
	return getGlobalTransform().transformRect(getLocalBounds());
}

sf::Vector2f UIElement::getGeometricCenter() const
{
	return { m_size.x / 2.0f, m_size.y / 2.0f };
}

sf::Vector2f UIElement::getGlobalGeometricCenter() const
{
	return getGlobalTransform().transformPoint(getGeometricCenter());
}

void UIElement::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!m_isVisible) return;

	states.transform *= getTransform();

	drawSelf(target, states);
	for (const auto& child : m_children)
	{
		if (child)
			child->draw(target, states);
	}
}


void UIElement::setParent(UIElement& parent)
{
	m_parent = &parent;
}

UIElement* UIElement::getParent()
{
	return m_parent;
}

const UIElement* UIElement::getParent() const
{
	return m_parent;
}


bool UIElement::isInteractive() const
{
	return false;
}

UIInteractive* UIElement::asInteractive()
{
	return nullptr;
}

UIElement* UIElement::hitTest(sf::Vector2f globalPoint)
{
	if (!m_isVisible)
		return nullptr;

	sf::Vector2f localPoint = getGlobalTransform().getInverse().transformPoint(globalPoint);

	if (!getLocalBounds().contains(localPoint))
		return nullptr;

	if (m_hitTestChildren)
	{
		for (auto it = m_children.rbegin(); it != m_children.rend(); ++it)
		{
			if (it->get())
			{
				if (auto hit = it->get()->hitTest(globalPoint))
					return hit;
			}
		}
	}

	if (m_isTransparentToInput)
		return nullptr;

	if (!resolveHit(localPoint))
		return nullptr;

	return this;
}

void UIElement::setResolveHit(std::function<bool(sf::Vector2f localPoint)> func)
{
	m_resolveHit = std::move(func);
}


sf::Transform UIElement::getGlobalTransform() const
{
	if (m_parent)
		return m_parent->getGlobalTransform() * getTransform();

	return getTransform();
}

sf::Vector2f UIElement::getGlobalPosition() const
{
	return getGlobalTransform().transformPoint({ 0.0f ,0.0f });
}


sf::Vector2f UIElement::getTopLeftPosition() const
{
	const sf::Vector2f& position = getPosition();
	const sf::Vector2f& origin = getOrigin();

	return { position.x - origin.x, position.y - origin.y };
}

sf::Vector2f UIElement::getTopRightPosition() const
{
	const sf::Vector2f& position = getPosition();
	const sf::Vector2f& origin = getOrigin();

	return { position.x - origin.x + m_size.x, position.y - origin.y };
}

sf::Vector2f UIElement::getBottomLeftPosition() const
{
	const sf::Vector2f& position = getPosition();
	const sf::Vector2f& origin = getOrigin();

	return { position.x - origin.x, position.y - origin.y + m_size.y };
}

sf::Vector2f UIElement::getBottomRightPosition() const
{
	const sf::Vector2f& position = getPosition();
	const sf::Vector2f& origin = getOrigin();

	return { position.x - origin.x + m_size.x, position.y - origin.y + m_size.y };
}


sf::Vector2f UIElement::getGlobalScale() const
{
	if (m_parent)
		return { m_parent->getGlobalScale().x * getScale().x, m_parent->getGlobalScale().y * getScale().y };
	return getScale();
}


sf::Vector2f UIElement::toParentSpace(sf::Vector2f globalPoint)
{
	if (m_parent)
		return m_parent->getGlobalTransform().getInverse().transformPoint(globalPoint);

	return globalPoint;
}


void UIElement::setSize(sf::Vector2f size)
{
	assert(size.x >= 0.0f);
	assert(size.y >= 0.0f);

	sf::Vector2f ratio = {
		m_size.x > 0 ? size.x / m_size.x : 1.0f,
		m_size.y > 0 ? size.y / m_size.y : 1.0f
	};

	sf::Vector2f originRatio = {
		m_size.x > 0 ? getOrigin().x / m_size.x : 0.0f,
		m_size.y > 0 ? getOrigin().y / m_size.y : 0.0f
	};

	m_size = size;
	setOrigin({ size.x * originRatio.x, size.y * originRatio.y });

	for (auto& child : m_children)
	{
		if (child)
		{
			child->setPosition({ child->getPosition().x * ratio.x, child->getPosition().y * ratio.y });
			child->setSize({ child->getSize().x * ratio.x, child->getSize().y * ratio.y });
		}
	}

	onSizeChanged(ratio);
}

sf::Vector2f UIElement::getSize() const
{
	return m_size;
}

sf::Vector2f UIElement::getGlobalSize() const
{
	return { m_size.x * getGlobalScale().x, m_size.y * getGlobalScale().y };
}


UIElement& UIElement::addChild(std::unique_ptr<UIElement> child)
{
	child->m_parent = this;
	m_children.push_back(std::move(child));
	onChildAdded(*m_children.back());

	return *m_children.back();
}

void UIElement::eraseChild(UIElement& child)
{
	m_childrenToDelete.emplace_back(&child);
}

void UIElement::cleanUp()
{
	for (auto& child : m_children)
		child->cleanUp();

	for (const auto& childToDelete : m_childrenToDelete)
	{
		auto it = std::find_if(m_children.begin(), m_children.end(), [childToDelete](const auto& child)
			{
				return child.get() == childToDelete;
			});

		if (it != m_children.end())
			m_children.erase(it);
	}

	m_childrenToDelete.clear();
}


const std::vector<std::unique_ptr<UIElement>>& UIElement::getAllChildren() const
{
	return m_children;
}

std::vector<std::unique_ptr<UIElement>>& UIElement::getAllChildren()
{
	return m_children;
}


void UIElement::setVisible(bool value)
{
	m_isVisible = value;
	if (m_parent)
		m_parent->onChildVisibilityChanged(*this);
}

bool UIElement::isVisible() const
{
	return m_isVisible;
}

void UIElement::setChildrenHitTest(bool value)
{
	m_hitTestChildren = value;
}

bool UIElement::isChildrenHitTest() const
{
	return m_hitTestChildren;
}


void UIElement::setTransparentToInput(bool value)
{
	m_isTransparentToInput = value;
}

bool UIElement::isTransparentToInput() const
{
	return m_isTransparentToInput;
}

void UIElement::blockDraggable()
{
	++m_dragLocks;
}

void UIElement::unblockDraggable()
{
	if (m_dragLocks > 0)
		--m_dragLocks;
}


bool UIElement::isDraggable() const
{
	return m_dragLocks == 0;
}


bool UIElement::isAnimating() const
{
	return !animationState.animations.empty();
}


bool UIElement::resolveHit(sf::Vector2f localPoint) const
{
	if (m_resolveHit)
		return m_resolveHit(localPoint);

	return true;
}