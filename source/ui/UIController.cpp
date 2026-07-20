#include "UIController.h"
#include "../game/scenes/TestMenuScene.h"
#include "../game/scenes/TestLobbyScene.h"



void UIController::subscribeAll()
{

}

void UIController::cleanSceneToDelete()
{
	if (m_sceneToDelete)
		m_sceneToDelete.reset();
}


void UIController::mouseInputUpdate()
{
	currentScene->mouseInputUpdate();
}

void UIController::handleEvents(const sf::Event& event)
{
	currentScene->handleEvents(event);
}

void UIController::update(float dt)
{
	currentScene->update(dt);
}

void UIController::defer()
{

}

void UIController::cleanUp()
{
	currentScene->cleanUp();
	cleanSceneToDelete();
}

void UIController::render(sf::RenderWindow& window)
{
	sf::RenderStates states = sf::RenderStates::Default;
	currentScene->render(window, states);
}


void UIController::setWindowSize(const sf::Vector2f& windowSize)
{
	m_windowSize = windowSize;
}

sf::Vector2f UIController::getWindowSize() const
{
	return m_windowSize;
}


void UIController::setScene(std::unique_ptr<IScene> scene)
{
	assert(!m_sceneToDelete);
	m_sceneToDelete = std::move(currentScene);
	currentScene = std::move(scene);
	currentScene->init(m_windowSize);
}

const IScene& UIController::getCurrentScene() const
{
	return *currentScene.get();
}