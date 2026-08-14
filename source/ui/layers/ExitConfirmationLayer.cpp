#include "ExitConfirmationLayer.h"
#include "../../core/Constants.h"
#include ".././../core/EventSystem.h"
#include "../../resources/Fonts.h"
#include "../ButtonBuilder.h"
#include "../widgets/Button.h"
#include "../elements/RectangleElement.h"
#include "../elements/RoundedRectangleElement.h"
#include "../elements/TextElement.h"
#include "../../game/events/domain/ExitEvent.h"
#include "InputIpAddressLayer.h"



void ExitConfirmationLayer::init()
{
	m_isVisible = false;
	m_isModal = true;

	m_overlayPanel = &addPanel(std::make_unique<Panel>(m_size));
	m_exitConfirmationPanel = &addPanel(std::make_unique<Panel>());

	initOverlayPanel(*m_overlayPanel);
	initExitConfirmationPanel(*m_exitConfirmationPanel);
}


void ExitConfirmationLayer::initOverlayPanel(Panel& overlayPanel)
{
	RectangleElement& element = static_cast<RectangleElement&>(overlayPanel.addChild(std::make_unique<RectangleElement>(m_size)));
	element.shape().setFillColor(Constants::UI::OverlayColor);
}

void ExitConfirmationLayer::initExitConfirmationPanel(Panel& exitConfirmationPanel)
{
	exitConfirmationPanel.setOrigin(exitConfirmationPanel.getGeometricCenter());
	exitConfirmationPanel.setPosition({ m_size.x / 2.0f, m_size.y / 2.0f });
	exitConfirmationPanel.setSize({ m_size.x / 3.0f, m_size.y / 4.0f });

	const sf::Vector2f& exitPanelSize = exitConfirmationPanel.getSize();

	RoundedRectangleElement& backgroundElement = static_cast<RoundedRectangleElement&>(exitConfirmationPanel.addChild(std::make_unique<RoundedRectangleElement>()));
	backgroundElement.setSize(exitPanelSize);
	backgroundElement.shape().setFillColor({ 50, 50, 60, 255 });
	backgroundElement.shape().setOutlineThickness(-2.0f);
	backgroundElement.shape().setOutlineColor({ 60, 60, 60, 255 });

	TextElement& textElement = static_cast<TextElement&>(exitConfirmationPanel.addChild(std::make_unique<TextElement>(Fonts::Arial, "Are you sure you want to quit?")));
	auto& textShape = textElement.text();
	textShape.setFillColor({ 185, 185, 185, 255 });
	textShape.setOutlineThickness(1.0f);
	textShape.setOutlineColor({ 40, 40, 40, 255 });
	textElement.setSize({ backgroundElement.getSize().x * 0.8f, backgroundElement.getSize().y * 0.7f });
	textElement.setOrigin(textElement.getGeometricCenter());
	textElement.setPosition({ exitPanelSize.x / 2.0f, exitPanelSize.y * 0.35f });

	ButtonBuilder buttonBuilder(m_gameContext.textureManager, "YES");
	buttonBuilder.setButtonType(ButtonType::Pilled);
	ButtonStyle style = ButtonStyleFactory::makeStandardStyle(sf::Color(75, 75, 85, 255));
	style.text.sizeFactor = { 0.8f, 0.5f };
	style.background.outline.color = { 90, 90, 90, 255 };
	buttonBuilder.setStyle(style);

	Button& yesButton = static_cast<Button&>(exitConfirmationPanel.addChild(buttonBuilder.build()));

	buttonBuilder.setText("CANCEL");
	Button& cancelButton = static_cast<Button&>(exitConfirmationPanel.addChild(buttonBuilder.build()));

	yesButton.setSize({ exitPanelSize.x / 4.0f, exitPanelSize.y / 5.0f });
	yesButton.setOrigin(yesButton.getGeometricCenter());
	yesButton.setPosition({ exitPanelSize.x * 0.25f, backgroundElement.getSize().y * 0.8f });

	cancelButton.setSize({ exitPanelSize.x / 4.0f, exitPanelSize.y / 5.0f });
	cancelButton.setOrigin(cancelButton.getGeometricCenter());
	cancelButton.setPosition({ exitPanelSize.x * 0.75f, backgroundElement.getSize().y * 0.8f });

	yesButton.setOnClick([this]() { m_gameContext.ES.publish(ExitEvent{}); });
	cancelButton.setOnClick([this]() { setVisible(false); });
}


bool ExitConfirmationLayer::handleEvents(const sf::Event& event)
{
	if (auto e = event.getIf<sf::Event::KeyReleased>())
	{
		if (e->scancode == sf::Keyboard::Scan::Escape)
		{
			setVisible(false);
			return true;
		}
	}

	if (auto e = event.getIf<sf::Event::KeyPressed>())
	{
		if (e->code == sf::Keyboard::Key::Enter)
		{
			m_gameContext.ES.publish(ExitEvent{});
			return true;
		}
	}

	return false;
}