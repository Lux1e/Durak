#include "LobbyTopLeftPanel.h"
#include "../../core/EventSystem.h"
#include "../../core/Constants.h"
#include "../../resources/TextureManager.h"
#include "../../game/GameContext.h"
#include "../../core/ClientContext.h"
#include "../../states/client/ClientLobbyState.h"
#include "../elements/RoundedRectangleElement.h"
#include "../elements/SpriteElement.h"
#include "../widgets/Button.h"
#include "../../utils/MathUtils.h"
#include "../../utils/UIUtils.h"
#include "../styles/VisualComponent.h"
#include "../../core/animation/RotateAnimation.h"
#include "../../game/events/requests/ExitRequestEvent.h"
#include "LobbySettingsPanel.h"



void LobbyTopLeftPanel::init(const ClientLobbyState& clientLobbyState, const GameContext& gameContext, const ClientContext& clientContext)
{
	RoundedRectangleElement& background = static_cast<RoundedRectangleElement&>(addChild(std::make_unique<RoundedRectangleElement>(m_size)));
	background.shape().setCornerRatio(0.5f);
	background.shape().setFillColor({ 10, 10, 10, 100 });

	sf::Vector2f&& backgroundSize = background.getSize();


	sf::Vector2f buttonsSize = MathUtils::round({ backgroundSize.y, backgroundSize.y });
	float buttonOffset = std::round(backgroundSize.x / 20.0f);
	VisualComponent buttonsComponents;
	buttonsComponents.colors.normal = { 175, 175, 200, 255 };
	m_buttonsNormalColor = buttonsComponents.colors.normal;
	buttonsComponents.colors.hovered = { 210, 210, 0, 255 };
	buttonsComponents.colors.pressed = UIUtils::scaleColor(buttonsComponents.colors.hovered, 0.75f);

	Button& settingsButton = static_cast<Button&>(addChild(std::make_unique<Button>()));
	settingsButton.setSize(buttonsSize);
	settingsButton.setOrigin({ settingsButton.getGeometricCenter().x, 0.0f });
	settingsButton.setPosition({ std::round(m_size.x * 0.5f), 0.0f });

	SpriteElement& settingsSprite = static_cast<SpriteElement&>(settingsButton.addChild(std::make_unique<SpriteElement>(gameContext.textureManager.getTexture("assets/textures/ui/gear.png"))));
	settingsSprite.setSize(buttonsSize);
	settingsSprite.setOrigin(settingsSprite.getGeometricCenter());
	settingsSprite.setPosition(settingsSprite.getGeometricCenter());
	settingsSprite.sprite().setColor(buttonsComponents.colors.normal);


	Button& exitButton = static_cast<Button&>(addChild(std::make_unique<Button>()));
	exitButton.setSize(buttonsSize);
	exitButton.setOrigin({ exitButton.getSize().x, 0.0f });
	exitButton.setPosition({ settingsButton.getTopLeftPosition().x - buttonOffset, 0.0f });

	SpriteElement& exitSprite = static_cast<SpriteElement&>(exitButton.addChild(std::make_unique<SpriteElement>(gameContext.textureManager.getTexture("assets/textures/ui/exitLeft.png"))));
	exitSprite.setSize(exitButton.getSize());
	exitSprite.setOrigin(exitSprite.getGeometricCenter());
	exitSprite.setPosition(exitButton.getGeometricCenter());
	exitSprite.sprite().setColor(buttonsComponents.colors.normal);


	exitButton.setOnHover([exitSpritePtr = &exitSprite, buttonsComponents](UIInteractive& element)
		{
			exitSpritePtr->sprite().setColor(buttonsComponents.colors.hovered);
		});

	exitButton.setOnHoverEnd([exitSpritePtr = &exitSprite, buttonsComponents](UIInteractive& element)
		{
			exitSpritePtr->sprite().setColor(buttonsComponents.colors.normal);
		});

	exitButton.setOnPressed([exitSpritePtr = &exitSprite, buttonsComponents](UIInteractive& element)
		{
			exitSpritePtr->sprite().setColor(buttonsComponents.colors.pressed);
		});

	exitButton.setOnReleased([exitSpritePtr = &exitSprite, buttonsComponents](UIInteractive& element, bool isHit)
		{
			if (isHit)
				exitSpritePtr->sprite().setColor(buttonsComponents.colors.hovered);
			else
				exitSpritePtr->sprite().setColor(buttonsComponents.colors.normal);
		});

	exitButton.setOnClick([&gameContext]()
		{
			gameContext.ES.publish(ExitRequestEvent{});
		});


	settingsButton.setOnHover([settingsSpritePtr = &settingsSprite, buttonsComponents](UIInteractive& element)
		{
			if (!settingsSpritePtr->isAnimating())
			{
				sf::Angle startAngle = settingsSpritePtr->getRotation();
				sf::Angle endAngle = startAngle + sf::degrees(180.0f);

				settingsSpritePtr->addAnimation<RotateAnimation>([settingsSpritePtr](sf::Angle angle) { settingsSpritePtr->setRotation(angle); },
					EaseType::InOutCubic, startAngle, endAngle, Constants::Animations::LongRotateAnimationTime);
			}

			settingsSpritePtr->sprite().setColor(buttonsComponents.colors.hovered);
		});

	settingsButton.setOnHoverEnd([settingsSpritePtr = &settingsSprite, buttonsComponents](UIInteractive& element)
		{
			settingsSpritePtr->sprite().setColor(buttonsComponents.colors.normal);
		});

	settingsButton.setOnPressed([settingsSpritePtr = &settingsSprite, buttonsComponents](UIInteractive& element)
		{
			settingsSpritePtr->sprite().setColor(buttonsComponents.colors.pressed);
		});

	settingsButton.setOnReleased([settingsSpritePtr = &settingsSprite, buttonsComponents](UIInteractive& element, bool isHit)
		{
			if (isHit)
				settingsSpritePtr->sprite().setColor(buttonsComponents.colors.hovered);
			else
				settingsSpritePtr->sprite().setColor(buttonsComponents.colors.normal);
		});

	settingsButton.setOnClick([]() {}); //to do


	sf::Vector2f lockButtonLocalPosition = { settingsButton.getTopRightPosition().x + buttonOffset, 0.0f };

	m_lockButton = &static_cast<Button&>(addChild(std::make_unique<Button>()));
	m_lockButton->setSize(buttonsSize);
	m_lockButton->setOrigin({ 0.0f, 0.0f });
	m_lockButton->setPosition(lockButtonLocalPosition);

	initLockButton(clientLobbyState, gameContext, clientContext);
}

void LobbyTopLeftPanel::initLockButton(const ClientLobbyState& clientLobbyState, const GameContext& gameContext, const ClientContext& clientContext)
{
	VisualComponent lockButtonComponents;
	VisualComponent unlockButtonComponents;

	lockButtonComponents.colors.normal = m_buttonsNormalColor;
	unlockButtonComponents.colors.normal = m_buttonsNormalColor;
	lockButtonComponents.colors.hovered = sf::Color::Red;
	unlockButtonComponents.colors.hovered = sf::Color::Green;
	lockButtonComponents.colors.pressed = UIUtils::scaleColor(lockButtonComponents.colors.hovered, 0.6f);
	unlockButtonComponents.colors.pressed = UIUtils::scaleColor(unlockButtonComponents.colors.hovered, 0.6f);

	SpriteElement& lockSprite = static_cast<SpriteElement&>(m_lockButton->addChild(std::make_unique<SpriteElement>(gameContext.textureManager.getTexture("assets/textures/ui/locked.png"))));
	lockSprite.setSize(m_lockButton->getSize());
	lockSprite.setOrigin(lockSprite.getGeometricCenter());
	lockSprite.setPosition(m_lockButton->getGeometricCenter());
	lockSprite.sprite().setColor(m_buttonsNormalColor);

	SpriteElement& unlockSprite = static_cast<SpriteElement&>(m_lockButton->addChild(std::make_unique<SpriteElement>(gameContext.textureManager.getTexture("assets/textures/ui/unlocked.png"))));
	unlockSprite.setSize(m_lockButton->getSize());
	unlockSprite.setOrigin(unlockSprite.getGeometricCenter());
	unlockSprite.setPosition(m_lockButton->getGeometricCenter());
	unlockSprite.sprite().setColor(m_buttonsNormalColor);

	clientLobbyState.isLobbyOpen() ? lockSprite.setVisible(false) : unlockSprite.setVisible(false);

	m_lockButton->setOnHover([this, lockSpritePtr = &lockSprite, unlockSpritePtr = &unlockSprite, lockButtonComponents, unlockButtonComponents](UIInteractive& element)
		{
			if (lockSpritePtr->isVisible())
			{
				lockSpritePtr->setVisible(false);
				unlockSpritePtr->setVisible(true);

				unlockSpritePtr->sprite().setColor(unlockButtonComponents.colors.hovered);
			}

			else
			{
				lockSpritePtr->setVisible(true);
				unlockSpritePtr->setVisible(false);

				lockSpritePtr->sprite().setColor(lockButtonComponents.colors.hovered);
			}
		});

	m_lockButton->setOnHoverEnd([lockSpritePtr = &lockSprite, unlockSpritePtr = &unlockSprite, lockButtonComponents, unlockButtonComponents](UIInteractive& element)
		{
			if (lockSpritePtr->isVisible())
			{
				lockSpritePtr->setVisible(false);
				unlockSpritePtr->setVisible(true);
			}

			else
			{
				lockSpritePtr->setVisible(true);
				unlockSpritePtr->setVisible(false);
			}

			lockSpritePtr->sprite().setColor(lockButtonComponents.colors.normal);
			unlockSpritePtr->sprite().setColor(unlockButtonComponents.colors.normal);
		});

	m_lockButton->setOnPressed([lockSpritePtr = &lockSprite, unlockSpritePtr = &unlockSprite, lockButtonComponents, unlockButtonComponents](UIInteractive& element)
		{
			if (lockSpritePtr->isVisible())
				lockSpritePtr->sprite().setColor(lockButtonComponents.colors.pressed);
			else
				unlockSpritePtr->sprite().setColor(unlockButtonComponents.colors.pressed);
		});

	m_lockButton->setOnReleased([lockSpritePtr = &lockSprite, unlockSpritePtr = &unlockSprite, lockButtonComponents, unlockButtonComponents](UIInteractive& element, bool isHit)
		{
			if (isHit)
			{
				if (lockSpritePtr->isVisible())
				{
					lockSpritePtr->setVisible(false);
					unlockSpritePtr->setVisible(true);

					lockSpritePtr->sprite().setColor(lockButtonComponents.colors.normal);
					unlockSpritePtr->sprite().setColor(unlockButtonComponents.colors.hovered);
				}

				else
				{
					lockSpritePtr->setVisible(true);
					unlockSpritePtr->setVisible(false);

					unlockSpritePtr->sprite().setColor(unlockButtonComponents.colors.normal);
					lockSpritePtr->sprite().setColor(lockButtonComponents.colors.hovered);
				}
			}

			else
			{
				lockSpritePtr->sprite().setColor(lockButtonComponents.colors.normal);
				unlockSpritePtr->sprite().setColor(unlockButtonComponents.colors.normal);
			}
		});

	m_lockButton->setOnClick([]() {}); // to do


	if (clientContext.localRole == ClientRole::Regular)
	{
		m_lockButton->blockInteraction();
	}
}