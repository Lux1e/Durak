#include "Game.h"
#include <fstream>
#include "scenes/LobbyScene.h"
#include "events/domain/ExitEvent.h"
#include "events/domain/WelcomeEvent.h"
#include "events/requests/CreateGameRequestEvent.h"
#include "events/requests/JoinGameRequestEvent.h"



void Game::init(std::optional<int> profileIndex)
{
	Random::get();
	token = loadOrCreateToken(profileIndex);

	subscribeAll();


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//FOR TESTS:
	TEST_clientContext.localId = 0;
	TEST_clientContext.localRole = ClientRole::Host;

	TEST_clientLobbyState.setPlayersPerGame(4);
	TEST_clientLobbyState.addClientPlayerData(ClientPlayerData(0, "Player", ClientRole::Host), 0);
	TEST_clientLobbyState.addClientPlayerData(ClientPlayerData(1, "Player1", ClientRole::Regular), 1);
	TEST_clientLobbyState.addClientPlayerData(ClientPlayerData(2, "Player2", ClientRole::Regular), -1);
	TEST_clientLobbyState.addClientPlayerData(ClientPlayerData(3, "Player3", ClientRole::Regular), -1);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//FOR TESTS:
	m_nickname = "Player";
	if (profileIndex)
		m_nickname += std::to_string(*profileIndex);

	m_clientModel = std::make_unique<ClientModel>(ES, loadOrCreateToken(profileIndex), m_nickname, TEST_clientLobbyState);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	uiController.setGameContext(m_gameContext);
	uiController.setWindowSize(_window.getView().getSize());

	//uiController.setScene(std::make_unique<TestMenuScene>(m_gameContext, uiController.getInputCapture()));
	uiController.setScene(std::make_unique<LobbyScene>(m_gameContext, uiController.getInputCapture(), TEST_clientContext, TEST_clientLobbyState));

	m_server = nullptr;
	//m_clientModel = nullptr;
}

void Game::subscribeAll()
{
	ES.subscribe<ExitEvent, Game>(this, &Game::onExitEvent);
	ES.subscribe<CreateGameRequestEvent, Game>(this, &Game::onCreateGameRequestEvent);
	ES.subscribe<JoinGameRequestEvent, Game>(this, &Game::onJoinGameRequestEvent);
	ES.subscribe<WelcomeEvent, Game>(this, &Game::onWelcomeEvent);
}


void Game::processReceivingPacket()
{
	if (m_server)
		m_server->processAllReceivingPacket();
	if (m_clientModel)
		m_clientModel->processReceivingPackets();
}

void Game::updateMouseInput()
{
	inputController.update(_window);
	uiController.mouseInputUpdate();
}

void Game::dispatchEvents()
{
	ES.dispatch();
}

void Game::handleEvents(const sf::Event& event)
{
	uiController.handleEvents(event);
}

void Game::update(float dt)
{
	uiController.update(dt);

	if (m_server)
		m_server->update(dt);
}

void Game::defer()
{
}

void Game::cleanUp()
{
	if (m_server)
		m_server->cleanUp();

	if (m_clientModel)
		m_clientModel->cleanUp();

	uiController.cleanUp();
}

void Game::processSendingPackets()
{
	if (m_server)
		m_server->processAllSendingPackets();
	if (m_clientModel)
		m_clientModel->processSendingPackets();
}

void Game::render()
{
	uiController.render(_window);
}


uint64_t Game::loadOrCreateToken(std::optional<int> profileIndex)
{
	uint64_t token;
	namespace fs = std::filesystem;

	fs::path base = fs::current_path() / "profiles";
	fs::path dir = base / "default";
	if (profileIndex)
		dir = base / std::to_string(*profileIndex);

	fs::path tokenPath = dir / "token.txt";

	fs::create_directories(dir);

	if (!fs::exists(tokenPath))
	{
		std::ofstream out(tokenPath);
		assert(out);
		out << Random::get()();
	}

	std::ifstream in(tokenPath);
	in >> token;

	if (!in)
		throw std::runtime_error("Failed to open token file.");

	return token;
}


void Game::onExitEvent(const ExitEvent& event, const EventInitiator& initiator)
{
	_window.close();
}


void Game::onCreateGameRequestEvent(const CreateGameRequestEvent& event, const EventInitiator& initiator)
{
	assert(!m_server);
	assert(!m_clientModel);

	m_server = std::make_unique<Server>();
	m_clientModel = std::make_unique<ClientModel>(ES, token, m_nickname, TEST_clientLobbyState);
}

void Game::onJoinGameRequestEvent(const JoinGameRequestEvent& event, const EventInitiator& initiator)
{
	assert(!m_server);
	assert(!m_clientModel);

	m_clientModel = std::make_unique<ClientModel>(ES, token, m_nickname, TEST_clientLobbyState);
}


void Game::onWelcomeEvent(const WelcomeEvent& event, const EventInitiator& initiator)
{
	//uiController.setScene(std::make_unique<TestLobbyScene>(ES, m_clientModel->getClientContext(), *m_clientModel->getClientLobbyState()));
}