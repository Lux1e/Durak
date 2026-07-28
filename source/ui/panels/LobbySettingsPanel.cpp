#include "LobbySettingsPanel.h"
#include "../../core/ClientContext.h"
#include "../../game/GameContext.h"
#include "../elements/RectangleElement.h"
#include "../elements/TextElement.h"
#include "../../utils/MathUtils.h"
#include "../../utils/UIUtils.h"
#include "../widgets/Button.h"
#include "../ButtonBuilder.h"



void LobbySettingsPanel::init(const ClientLobbyState& clientLobbyState, const GameContext& gameContext, const ClientContext& clientContext)
{
	setTransparentToInput(true);

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


void LobbySettingsPanel::initTopSettingsPanel(const ClientLobbyState& clientLobbyState, const GameContext& gameContext, const ClientContext& clientContext)
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

	gameModeText.setCharacterSize(18);
	gameSpeedText.setCharacterSize(18);
	deckText.setCharacterSize(18);

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
	//builder.setText("No Timer");
	Button& gameSpeedButton = static_cast<Button&>(m_topSettingsPanel->addChild(builder.build()));
	//builder.setText("52");
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

	gameModeList.setOnChildClick([gameModeListPtr = &gameModeList, gameModeButtonPtr = &gameModeButton](UIElement& element)
		{
			for (auto& child : gameModeListPtr->getAllChildren())
				child->setVisible(true);

			auto& button = static_cast<Button&>(element);
			button.setVisible(false);

			TextElement* buttonListText = static_cast<TextElement*>(button.getTextElement());
			TextElement* mainButtonText = static_cast<TextElement*>(gameModeButtonPtr->getTextElement());

			assert(buttonListText);
			assert(mainButtonText);

			mainButtonText->setString(buttonListText->getString());
			mainButtonText->setTextAlign(TextElement::TextAlign::Left);

			gameModeListPtr->setVisible(false);
		});

	gameModeButton.setOnClick([gameModeListPtr = &gameModeList]()
		{
			if (gameModeListPtr)
				gameModeListPtr->isVisible() ? gameModeListPtr->setVisible(false) : gameModeListPtr->setVisible(true);
		});


	gameSpeedList.setOnChildClick([gameSpeedListPtr = &gameSpeedList, gameSpeedButtonPtr = &gameSpeedButton](UIElement& element)
		{
			for (auto& child : gameSpeedListPtr->getAllChildren())
				child->setVisible(true);

			auto& button = static_cast<Button&>(element);
			button.setVisible(false);

			TextElement* buttonListText = static_cast<TextElement*>(button.getTextElement());
			TextElement* mainButtonText = static_cast<TextElement*>(gameSpeedButtonPtr->getTextElement());

			assert(buttonListText);
			assert(mainButtonText);

			mainButtonText->setString(buttonListText->getString());
			mainButtonText->setTextAlign(TextElement::TextAlign::Left);

			gameSpeedListPtr->setVisible(false);
		});

	gameSpeedButton.setOnClick([gameSpeedListPtr = &gameSpeedList]()
		{
			if (gameSpeedListPtr)
				gameSpeedListPtr->isVisible() ? gameSpeedListPtr->setVisible(false) : gameSpeedListPtr->setVisible(true);
		});


	builder.setButtonType(ButtonType::Rectangle);
	style = ButtonStyleFactory::makeStandardStyle();
	builder.setStyle(style);

	builder.setText("Classic");
	Button& classicButton = static_cast<Button&>(gameModeList.addChild(builder.build()));
	builder.setText("Passing");
	Button& passingButton = static_cast<Button&>(gameModeList.addChild(builder.build()));
	builder.setText("Teams");
	Button& teamsButton = static_cast<Button&>(gameModeList.addChild(builder.build()));

	m_gameModeDropdown.setMainElement(gameModeButton);
	m_gameModeDropdown.setListBox(gameModeList);


	builder.setText("No Timer");
	Button& noTimerButton = static_cast<Button&>(gameSpeedList.addChild(builder.build()));
	builder.setText("Slow");
	Button& slowButton = static_cast<Button&>(gameSpeedList.addChild(builder.build()));
	builder.setText("Standard");
	Button& standardButton = static_cast<Button&>(gameSpeedList.addChild(builder.build()));
	builder.setText("Fast");
	Button& fastButton = static_cast<Button&>(gameSpeedList.addChild(builder.build()));

	m_gameSpeedDropdown.setMainElement(gameSpeedButton);
	m_gameSpeedDropdown.setListBox(gameSpeedList);


	updateSettings(clientLobbyState, clientContext);
}


void LobbySettingsPanel::updateSettings(const ClientLobbyState& clientLobbyState, const ClientContext& clientContext)
{
	//to do

	Button& gameModeMainButton = static_cast<Button&>(m_gameModeDropdown.getMainElement());
	TextElement* gameModeString = static_cast<TextElement*>(gameModeMainButton.getTextElement());
	
	gameModeString->setString("Classic");
	gameModeString->setTextAlign(TextElement::TextAlign::Left);

	for (auto& child : m_gameModeDropdown.getListBox().getAllChildren())
	{
		Button& button = static_cast<Button&>(*child.get());
		TextElement& text = *static_cast<TextElement*>(button.getTextElement());
		
		if (text.getString() == gameModeString->getString())
			button.setVisible(false);
	}
	

	Button& gameSpeedMainButton = static_cast<Button&>(m_gameSpeedDropdown.getMainElement());
	TextElement* gameSpeedString = static_cast<TextElement*>(gameSpeedMainButton.getTextElement());

	gameSpeedString->setString("No Timer");
	gameSpeedString->setTextAlign(TextElement::TextAlign::Left);

	for (auto& child : m_gameSpeedDropdown.getListBox().getAllChildren())
	{
		Button& button = static_cast<Button&>(*child.get());
		TextElement& text = *static_cast<TextElement*>(button.getTextElement());

		if (text.getString() == gameSpeedString->getString())
			button.setVisible(false);
	}



	if (clientContext.localRole == ClientRole::Regular)
	{
		gameModeMainButton.blockFullActions();
		gameSpeedMainButton.blockFullActions();
	}
}