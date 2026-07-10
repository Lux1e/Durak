#pragma once
#include "../core/scene/IScene.h"



class UIController
{
public:
	UIController() = default;
	UIController(std::unique_ptr<IScene> scene, const sf::Vector2f& windowSize) : currentScene(std::move(scene)), m_windowSize(windowSize)
	{
		currentScene->init(m_windowSize);
	}

	UIController(UIController&&) = default;
	UIController& operator=(UIController&&) = default;

	UIController(const UIController&) = delete;
	UIController& operator=(const UIController&) = delete;

	void mouseInputUpdate();
	void handleEvents(const sf::Event& event);
	void update(float dt);
	void defer();
	void cleanUp();
	void render(sf::RenderWindow& window);

	void setWindowSize(const sf::Vector2f& windowSize);
	sf::Vector2f getWindowSize() const;

	void setScene(std::unique_ptr<IScene> scene);
	const IScene& getCurrentScene() const;

private:
	void subscribeAll();
	void cleanSceneToDelete();

	sf::Vector2f m_windowSize;

	std::unique_ptr<IScene> currentScene;
	std::unique_ptr<IScene> m_sceneToDelete;
};