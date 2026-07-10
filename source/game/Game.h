#pragma once
#include "../core/Trackable.h"
#include "../core/EventSystem.h"
#include "../core/input/InputController.h"
#include "../core/scene/IScene.h"
#include "../resources/TextureManager.h"
#include "../utils/RandomUtils.h"
#include "../ui/UIController.h"
#include "../network/Server.h"
#include "../network/ClientModel.h"
#include "scenes/TestMenuScene.h"
#include "GameContext.h"


class EventInitiator;
struct ExitEvent;
struct CreateGameRequestEvent;
struct JoinGameRequestEvent;
struct WelcomeEvent;



class Game final : public Trackable
{
public:
	Game(std::optional<int> profileIndex, sf::RenderWindow& target) : _window(target), m_gameContext(ES, m_textureManager, inputController.getInputState())
	{
		init(profileIndex);
	}

	void run() {}

	void processReceivingPacket();
	void updateMouseInput();
	void handleEvents(const sf::Event& event);
	void dispatchEvents();
	void update(float dt);
	void defer();
	void cleanUp();
	void processSendingPackets();
	void render();

	//FOR TESTS:
	TextureManager& getTextureManager() { return m_textureManager; }

private:
	sf::RenderWindow& _window;
	EventSystem ES;

	TextureManager m_textureManager;
	InputController inputController;
	UIController uiController;

	GameContext m_gameContext;

	std::unique_ptr<Server> m_server;
	std::unique_ptr<ClientModel> m_clientModel;

	//FOR TESTS:
	uint64_t token;
	std::string m_nickname;
	ClientLobbyState TEST_clientLobbyState;
	ClientContext TEST_clientContext;


	void init(std::optional<int> profileIndex);
	void subscribeAll();


	uint64_t loadOrCreateToken(std::optional<int> profileIndex);


	void onExitEvent(const ExitEvent& event, const EventInitiator& initiator);

	void onCreateGameRequestEvent(const CreateGameRequestEvent& event, const EventInitiator& initiator);
	void onJoinGameRequestEvent(const JoinGameRequestEvent& event, const EventInitiator& initiator);

	void onWelcomeEvent(const WelcomeEvent& event, const EventInitiator& initiator);
};