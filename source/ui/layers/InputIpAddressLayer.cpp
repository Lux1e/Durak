#include "InputIpAddressLayer.h"
#include "../../core/EventSystem.h"
#include "../../core/Constants.h"
#include "../elements/RectangleElement.h"
#include "../widgets/TextBox.h"
#include "../widgets/Button.h"
#include "../ButtonBuilder.h"
#include "../../utils/NetworkUtils.h"
#include "../../game/events/requests/JoinGameRequestEvent.h"



void InputIpAddressLayer::init()
{
	setVisible(false);
	setModal(true);

	m_overlayPanel = &addPanel(std::make_unique<Panel>());
	m_ipPanel = &addPanel(std::make_unique<Panel>());

	initOverlayPanel(*m_overlayPanel);
	initIpPanel(*m_ipPanel);
}


void InputIpAddressLayer::initOverlayPanel(Panel& overlayPanel)
{
	overlayPanel.setSize(m_size);

	RectangleElement& overlay = static_cast<RectangleElement&>(overlayPanel.addChild(std::make_unique<RectangleElement>(m_size)));
	overlay.shape().setFillColor(Constants::UI::OverlayColor);
}

void InputIpAddressLayer::initIpPanel(Panel& ipPanel)
{
	ipPanel.setSize({ m_size.x / 3.0f, m_size.y / 8.0f });
	ipPanel.setOrigin(ipPanel.getGeometricCenter());
	ipPanel.setPosition({ m_size.x / 2.0f, m_size.y / 2.0f });
	sf::Vector2f ipPanelSize = ipPanel.getSize();

	m_ipBox = &static_cast<TextBox&>(ipPanel.addChild(std::make_unique<TextBox>("", Fonts::Arial, "Enter IP address...", Fonts::ArialI)));
	m_ipBox->setStringRatio({ 0.8f, 0.5f });
	m_ipBox->setPlaceholderRatio({ 0.8f, 0.4f });
	m_ipBox->setStringColor(sf::Color::Yellow);
	m_ipBox->setPlaceholderColor({ 35,35,35,255 });


	sf::Color passiveButtonColorBG = { 20, 20, 20, 255 };
	sf::Color passiveButtonColorString = { 100, 100, 100, 255 };

	ButtonBuilder builder("CONNECT");
	builder.setButtonType(ButtonType::Rectangle);
	auto connectButtonStyle = ButtonStyleFactory::makeStandardStyle(sf::Color{ 20, 120, 20, 255 });
	connectButtonStyle.background.outline.width = 0.0f;
	builder.setStyle(connectButtonStyle);
	m_connectButton = &static_cast<Button&>(ipPanel.addChild(builder.build()));

	m_connectButton->blockHovered();
	m_connectButton->blockPressed();
	m_connectButton->blockClick();

	switchConnectButtonColors = [this, connectButtonStyle, passiveButtonColorBG, passiveButtonColorString](bool state)
		{
			assert(m_connectButton);

			if (auto* backgroundPtr = m_connectButton->getBackgroundElement())
			{
				RectangleElement& bg = static_cast<RectangleElement&>(*backgroundPtr);
				if (state)
					bg.shape().setFillColor(connectButtonStyle.background.colors.normal);
				else
					bg.shape().setFillColor(passiveButtonColorBG);
			}

			if (auto* stringPtr = m_connectButton->getTextElement())
			{
				TextElement& string = static_cast<TextElement&>(*stringPtr);
				if (state)
					string.text().setFillColor(connectButtonStyle.text.colors.normal);
				else
					string.text().setFillColor(passiveButtonColorString);
			}
		};

	switchConnectButtonColors(false);


	builder.setText("CANCEL");
	auto cancelButtonStyle = ButtonStyleFactory::makeStandardStyle(sf::Color{ 120, 20, 20, 255 });
	cancelButtonStyle.background.outline.width = 0.0f;
	builder.setStyle(cancelButtonStyle);
	Button& cancelButton = static_cast<Button&>(ipPanel.addChild(builder.build()));

	m_ipBox->setSize({ ipPanelSize.x, ipPanelSize.y / 1.5f });
	m_connectButton->setSize({ ipPanelSize.x / 2.0f, ipPanelSize.y - m_ipBox->getSize().y });
	cancelButton.setSize(m_connectButton->getSize());

	m_connectButton->setOrigin({ m_connectButton->getSize().x / 2.0f, 0.0f });
	cancelButton.setOrigin({ cancelButton.getSize().x / 2.0f, 0.0f });

	m_connectButton->setPosition({ ipPanelSize.x * 0.25f, m_ipBox->getSize().y });
	cancelButton.setPosition({ ipPanelSize.x * 0.75f, m_ipBox->getSize().y });

	m_connectButton->setOnClick([this]() { m_gameContext.ES.publish<JoinGameRequestEvent>(m_ipBox->getString()); });
	cancelButton.setOnClick([this]() { m_ipBox->clearString(); setVisible(false); });
}

void InputIpAddressLayer::updateIPBox()
{
	bool valid = NetworkUtils::isValidIP(m_ipBox->getString());

	if (valid)
	{
		if (!m_connectButton->canBeClicked())
		{
			m_connectButton->unblockHovered();
			m_connectButton->unblockPressed();
			m_connectButton->unblockClick();

			switchConnectButtonColors(valid);
		}
	}

	else
	{
		if (m_connectButton->canBeClicked())
		{
			m_connectButton->setHovered(false);
			m_connectButton->setPressed(false, false);

			m_connectButton->blockHovered();
			m_connectButton->blockPressed();
			m_connectButton->blockClick();

			switchConnectButtonColors(valid);
		}
	}
}


bool InputIpAddressLayer::handleEvents(const sf::Event& event)
{
	if (const auto& e = event.getIf<sf::Event::MouseButtonPressed>())
	{
		if (e->button == sf::Mouse::Button::Left)
			return onMousePressedEvent();
	}

	if (const auto& e = event.getIf<sf::Event::MouseButtonReleased>())
	{
		if (e->button == sf::Mouse::Button::Left)
			return onMouseReleasedEvent();
	}

	if (const auto& e = event.getIf<sf::Event::TextEntered>())
	{
		if (e->unicode < 128)
		{
			if (m_ipBox->getString().size() >= 15)
				return false;

			char c = static_cast<char>(e->unicode);
			if (c == '.' || std::isdigit(c))
			{
				m_ipBox->enterSymbol(c);
				updateIPBox();

				return true;
			}
		}
	}

	if (const auto& key = event.getIf<sf::Event::KeyPressed>())
	{
		if (key->code == sf::Keyboard::Key::Backspace && key->control)
		{
			m_ipBox->clearString();
			updateIPBox();

			return true;
		}

		if (key->code == sf::Keyboard::Key::Backspace)
		{
			m_ipBox->deleteLastSymbol();
			updateIPBox();

			return true;
		}

		if (key->scancode == sf::Keyboard::Scan::V && key->control)
		{
			std::string clipboard = sf::Clipboard::getString();

			if (m_ipBox->getString().size() + clipboard.size() >= 15)
				return false;

			for (const char& c : clipboard)
			{
				if (c != '.' && !std::isdigit(c))
					return false;
			}

			m_ipBox->setString(m_ipBox->getString() + clipboard);
			updateIPBox();

			return true;
		}
	}

	if (const auto& key = event.getIf<sf::Event::KeyReleased>())
	{
		if (key->scancode == sf::Keyboard::Scan::Escape)
		{
			m_ipBox->clearString();
			setVisible(false);

			return true;
		}

		if (key->code == sf::Keyboard::Key::Enter)
		{
			if (m_connectButton->canBeClicked())
			{
				m_gameContext.ES.publish<JoinGameRequestEvent>(m_ipBox->getString());

				return true;
			}
		}
	}

	return false;
}