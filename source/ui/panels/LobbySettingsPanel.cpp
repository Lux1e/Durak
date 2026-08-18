#include "LobbySettingsPanel.h"
#include "../../core/ClientContext.h"
#include "../../game/GameContext.h"
#include "../elements/RectangleElement.h"
#include "../elements/TextElement.h"
#include "../../utils/MathUtils.h"
#include "../../utils/UIUtils.h"
#include "../widgets/Button.h"
#include "../ButtonBuilder.h"



void LobbySettingsPanel::init(const ClientLobbyState& clientLobbyState, GameContext& gameContext, const ClientContext& clientContext)
{
	assert(m_inputCapture);

	setTransparentToInput(true);

	m_gameModeDropdown = std::make_unique<Dropdown>(gameContext);
	m_gameSpeedDropdown = std::make_unique<Dropdown>(gameContext);
	m_deckDropdown = std::make_unique<Dropdown>(gameContext);

	RectangleElement& background = static_cast<RectangleElement&>(addChild(std::make_unique<RectangleElement>(m_size)));
	background.shape().setFillColor({ 20, 20, 35, 255 });

	sf::Vector2f localOffset = { background.getSize().x * 0.075f, background.getSize().y * 0.075f };

	TextElement& settings = static_cast<TextElement&>(addChild(std::make_unique<TextElement>(Fonts::ArialBD, "Settings")));
	settings.setSize({ background.getSize().x * 0.6f, background.getSize().y * 0.06f });
	settings.setOrigin({ settings.getSize().x * 0.5f, 0.0f });
	settings.setPosition({ background.getSize().x * 0.5f, localOffset.y });

	m_topSettingsPanel = &static_cast<Panel&>(addChild(std::make_unique<Panel>()));
	m_topSettingsPanel->setSize({ m_size.x - localOffset.x * 2, m_size.y - settings.getBottomLeftPosition().y - localOffset.y * 2 });
	m_topSettingsPanel->setOrigin({ m_topSettingsPanel->getSize().x * 0.5f, 0.0f });
	m_topSettingsPanel->setPosition({ m_size.x * 0.5f, settings.getBottomLeftPosition().y + localOffset.y });

	initTopSettingsPanel(clientLobbyState, gameContext, clientContext);
}


void LobbySettingsPanel::initTopSettingsPanel(const ClientLobbyState& clientLobbyState, GameContext& gameContext, const ClientContext& clientContext)
{
	RectangleElement& test = static_cast<RectangleElement&>(m_topSettingsPanel->addChild(std::make_unique<RectangleElement>()));
	test.setSize(m_topSettingsPanel->getSize());
	test.setPosition({ 0.0f, 0.0f });
	test.shape().setFillColor({ 255, 255, 255, 50 });

	sf::Vector2f panelSize = m_topSettingsPanel->getSize();


	sf::Vector2f dropDownOffset = MathUtils::round({ panelSize.x * 0.02f, panelSize.y * 0.025f });
	sf::Vector2f buttonsSize = MathUtils::round({ panelSize.x * 0.5f - dropDownOffset.x * 2.0f, panelSize.y * 0.1f });
	sf::Vector2f textElementSize = MathUtils::round({ buttonsSize.x, buttonsSize.y });

	TextElement& gameModeText = static_cast<TextElement&>(m_topSettingsPanel->addChild(std::make_unique<TextElement>(Fonts::ArialI, "Game Mode:")));
	TextElement& gameSpeedText = static_cast<TextElement&>(m_topSettingsPanel->addChild(std::make_unique<TextElement>(Fonts::ArialI, "Game Speed:")));
	TextElement& deckText = static_cast<TextElement&>(m_topSettingsPanel->addChild(std::make_unique<TextElement>(Fonts::ArialI, "Cards:")));

	gameModeText.setTextSizeMode(TextElement::TextSizeMode::Fixed);
	gameSpeedText.setTextSizeMode(TextElement::TextSizeMode::Fixed);
	deckText.setTextSizeMode(TextElement::TextSizeMode::Fixed);

	gameModeText.setTextAlign(TextElement::TextAlign::Right);
	gameSpeedText.setTextAlign(TextElement::TextAlign::Right);
	deckText.setTextAlign(TextElement::TextAlign::Right);

	gameModeText.setSize(textElementSize);
	gameSpeedText.setSize(textElementSize);
	deckText.setSize(textElementSize);

	gameModeText.setCharacterSize(buttonsSize.y * 0.5f);
	gameSpeedText.setCharacterSize(buttonsSize.y * 0.5f);
	deckText.setCharacterSize(buttonsSize.y * 0.5f);

	gameModeText.setOrigin({ gameModeText.getSize().x, 0.0f });
	gameSpeedText.setOrigin({ gameSpeedText.getSize().x, 0.0f });
	deckText.setOrigin({ deckText.getSize().x, 0.0f });

	gameModeText.setPosition({ panelSize.x * 0.5f - dropDownOffset.x, 0.0f });
	gameSpeedText.setPosition({ panelSize.x * 0.5f - dropDownOffset.x, gameModeText.getBottomRightPosition().y + dropDownOffset.y });
	deckText.setPosition({ panelSize.x * 0.5f - dropDownOffset.x, gameSpeedText.getBottomRightPosition().y + dropDownOffset.y });


	ButtonStyle style = ButtonStyleFactory::makeDropdownStyle();

	ButtonBuilder builder(gameContext.textureManager, "");
	builder.setButtonType(ButtonType::Rectangle);
	builder.setSize(buttonsSize);
	builder.setStyle(style);
	builder.setButtonType(ButtonType::Dropdown);
	builder.setFont(Fonts::ArialBD);

	Button& gameModeButton = static_cast<Button&>(m_topSettingsPanel->addChild(builder.build()));
	builder.setText("No Timer");
	Button& gameSpeedButton = static_cast<Button&>(m_topSettingsPanel->addChild(builder.build()));
	builder.setText("52");
	Button& deckButton = static_cast<Button&>(m_topSettingsPanel->addChild(builder.build()));

	float buttonPositionX = panelSize.x * 0.5f + dropDownOffset.x;

	gameModeButton.setPosition({ buttonPositionX, 0.0f });
	gameSpeedButton.setPosition({ buttonPositionX, gameModeButton.getBottomRightPosition().y + dropDownOffset.y });
	deckButton.setPosition({ buttonPositionX, gameSpeedButton.getBottomRightPosition().y + dropDownOffset.y });


	ListBox& gameModeList = static_cast<ListBox&>(m_topSettingsPanel->addChild(std::make_unique<ListBox>(buttonsSize)));
	ListBox& gameSpeedList = static_cast<ListBox&>(m_topSettingsPanel->addChild(std::make_unique<ListBox>(buttonsSize)));
	ListBox& deckList = static_cast<ListBox&>(m_topSettingsPanel->addChild(std::make_unique<ListBox>(buttonsSize)));

	gameModeList.setPosition(gameModeButton.getBottomLeftPosition());
	gameModeList.setVisible(false);

	gameSpeedList.setPosition(gameSpeedButton.getBottomLeftPosition());
	gameSpeedList.setVisible(false);

	deckList.setPosition(deckButton.getBottomLeftPosition());
	deckList.setVisible(false);


	builder.setButtonType(ButtonType::Rectangle);
	style = ButtonStyleFactory::makeStandardStyle();
	builder.setStyle(style);

	builder.setText("Classic");
	Button& classicButton = static_cast<Button&>(gameModeList.addChild(builder.build()));
	builder.setText("Passing");
	Button& passingButton = static_cast<Button&>(gameModeList.addChild(builder.build()));
	builder.setText("Teams");
	Button& teamsButton = static_cast<Button&>(gameModeList.addChild(builder.build()));

	builder.setText("No Timer");
	Button& noTimerButton = static_cast<Button&>(gameSpeedList.addChild(builder.build()));
	builder.setText("Slow");
	Button& slowButton = static_cast<Button&>(gameSpeedList.addChild(builder.build()));
	builder.setText("Standard");
	Button& standardButton = static_cast<Button&>(gameSpeedList.addChild(builder.build()));
	builder.setText("Fast");
	Button& fastButton = static_cast<Button&>(gameSpeedList.addChild(builder.build()));

	builder.setText("36");
	Button& firstDeckButton = static_cast<Button&>(deckList.addChild(builder.build()));
	builder.setText("52");
	Button& secondDeckButton = static_cast<Button&>(deckList.addChild(builder.build()));


	m_gameModeDropdown->setMainElement(gameModeButton);
	m_gameModeDropdown->setListBox(gameModeList);

	m_gameSpeedDropdown->setMainElement(gameSpeedButton);
	m_gameSpeedDropdown->setListBox(gameSpeedList);

	m_deckDropdown->setMainElement(deckButton);
	m_deckDropdown->setListBox(deckList);


	m_gameModeDropdown->setOnListBoxChildAction([this](UIInteractive& element)
		{
			Button* mainButton = static_cast<Button*>(m_gameModeDropdown->getMainElement());
			TextElement* mainText = static_cast<TextElement*>(mainButton->getTextElement());
			Button& listButton = static_cast<Button&>(element);
			TextElement* listText = static_cast<TextElement*>(listButton.getTextElement());

			assert(mainButton);
			assert(mainText);
			assert(listText);

			mainText->setString(listText->getString());
			for (auto& child : m_gameModeDropdown->getListBox()->getAllChildren())
				child->setVisible(true);

			listButton.setVisible(false);

			//event
		});

	m_gameSpeedDropdown->setOnListBoxChildAction([this](UIInteractive& element)
		{
			Button* mainButton = static_cast<Button*>(m_gameSpeedDropdown->getMainElement());
			TextElement* mainText = static_cast<TextElement*>(mainButton->getTextElement());
			Button& listButton = static_cast<Button&>(element);
			TextElement* listText = static_cast<TextElement*>(listButton.getTextElement());

			assert(mainButton);
			assert(mainText);
			assert(listText);

			mainText->setString(listText->getString());
			for (auto& child : m_gameSpeedDropdown->getListBox()->getAllChildren())
				child->setVisible(true);

			listButton.setVisible(false);

			//event
		});

	m_deckDropdown->setOnListBoxChildAction([this](UIInteractive& element)
		{
			Button* mainButton = static_cast<Button*>(m_deckDropdown->getMainElement());
			TextElement* mainText = static_cast<TextElement*>(mainButton->getTextElement());
			Button& listButton = static_cast<Button&>(element);
			TextElement* listText = static_cast<TextElement*>(listButton.getTextElement());

			assert(mainButton);
			assert(mainText);
			assert(listText);

			mainText->setString(listText->getString());
			for (auto& child : m_deckDropdown->getListBox()->getAllChildren())
				child->setVisible(true);

			listButton.setVisible(false);

			//event
		});


	updateSettings(clientLobbyState, clientContext);
}


void LobbySettingsPanel::updateSettings(const ClientLobbyState& clientLobbyState, const ClientContext& clientContext)
{
	//to do

	Button& gameModeMainButton = static_cast<Button&>(*m_gameModeDropdown->getMainElement());
	TextElement* gameModeString = static_cast<TextElement*>(gameModeMainButton.getTextElement());

	gameModeString->setString("Classic");

	for (auto& child : m_gameModeDropdown->getListBox()->getAllChildren())
	{
		Button& button = static_cast<Button&>(*child.get());
		TextElement& text = *static_cast<TextElement*>(button.getTextElement());

		if (text.getString() == gameModeString->getString())
			button.setVisible(false);
	}


	Button& gameSpeedMainButton = static_cast<Button&>(*m_gameSpeedDropdown->getMainElement());
	TextElement* gameSpeedString = static_cast<TextElement*>(gameSpeedMainButton.getTextElement());

	gameSpeedString->setString("No Timer");

	for (auto& child : m_gameSpeedDropdown->getListBox()->getAllChildren())
	{
		Button& button = static_cast<Button&>(*child.get());
		TextElement& text = *static_cast<TextElement*>(button.getTextElement());

		if (text.getString() == gameSpeedString->getString())
			button.setVisible(false);
	}

	
	Button& deckMainButton = static_cast<Button&>(*m_deckDropdown->getMainElement());
	TextElement* deckString = static_cast<TextElement*>(deckMainButton.getTextElement());

	deckString->setString("52");

	for (auto& child : m_deckDropdown->getListBox()->getAllChildren())
	{
		Button& button = static_cast<Button&>(*child.get());
		TextElement& text = *static_cast<TextElement*>(button.getTextElement());

		if (text.getString() == deckString->getString())
			button.setVisible(false);
	}


	if (clientContext.localRole == ClientRole::Regular)
	{
		gameModeMainButton.blockInteraction();
		gameSpeedMainButton.blockInteraction();
		deckMainButton.blockInteraction();
	}
}