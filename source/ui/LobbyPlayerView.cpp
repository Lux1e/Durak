#include "LobbyPlayerView.h"
#include "styles/VisualComponent.h"
#include "../utils/UIUtils.h"



void LobbyPlayerView::init(sf::Vector2f size, const sf::Font& font, const std::string& nickname)
{
	blockClick();
	setChildrenHitTest(false);
	unblockDrag();

	initBackground();
	initNickname(font, nickname);
	initColors();
	initInputActions();
}

void LobbyPlayerView::initBackground()
{
	m_background = &static_cast<RoundedRectangleElement&>(addChild(std::make_unique<RoundedRectangleElement>(m_size)));
	m_background->shape().setFillColor({ 60, 60, 70, 255 });
	m_background->shape().setOutlineThickness(1.0f);
	m_background->shape().setOutlineColor(m_background->shape().getFillColor());
}

void LobbyPlayerView::initNickname(const sf::Font& font, const std::string& nickname)
{
	m_nickname = &static_cast<TextElement&>(addChild(std::make_unique<TextElement>(font, nickname)));
	m_nickname->setSize({ m_size.x * 0.8f, m_size.y * 0.5f });
	m_nickname->setOrigin(m_nickname->getGeometricCenter());
	m_nickname->setPosition({ m_size.x * 0.5f, m_size.y * 0.5f });
	m_nickname->text().setFillColor(sf::Color::Yellow);
}

void LobbyPlayerView::initColors()
{
	VisualComponent backgroundComponent;
	backgroundComponent.colors.normal = m_background->shape().getFillColor();
	backgroundComponent.colors.hovered = UIUtils::scaleColor(backgroundComponent.colors.normal, 1.8f);
	backgroundComponent.colors.pressed = UIUtils::scaleColor(backgroundComponent.colors.normal, 1.8f);

	setOnHover([this, backgroundComponent](UIInteractive&)
		{
			m_background->shape().setFillColor(backgroundComponent.colors.hovered);
		});

	setOnHoverEnd([this, backgroundComponent](UIInteractive&)
		{
			m_background->shape().setFillColor(backgroundComponent.colors.normal);
		});

	setOnPressed([this, backgroundComponent](UIInteractive&)
		{
			m_background->shape().setFillColor(backgroundComponent.colors.pressed);
		});

	setOnReleased([this, backgroundComponent](UIInteractive&, bool isHit)
		{
			m_background->shape().setFillColor(backgroundComponent.colors.normal);
		});
}

void LobbyPlayerView::initInputActions()
{
	setOnMouseUp([](UIInteractive& element, bool isHit) { element.releaseInput(); });
	setOnHandleEvents([](UIInteractive& element, const InputState& inputState, const sf::Event& event)
		{
			if (const auto* e = event.getIf<sf::Event::MouseButtonReleased>())
			{
				if (e->button == sf::Mouse::Button::Left)
				{
					bool isHit = element.hitTest(inputState.mouseWorldPosition);
					element.onMouseUp(isHit);

					return;
				}
			}
		});
}


void LobbyPlayerView::update(float dt)
{
	updateAnimations(dt);
}


uint32_t LobbyPlayerView::getId() const
{
	return m_id;
}


RoundedRectangleElement* LobbyPlayerView::getBackground()
{
	return m_background;
}


void LobbyPlayerView::setNickname(std::string nickname)
{
	m_nickname->setString(std::move(nickname));
}

TextElement* LobbyPlayerView::getNickname()
{
	return m_nickname;
}