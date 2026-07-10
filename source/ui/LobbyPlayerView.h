#pragma once
#include <SFML/Graphics.hpp>
#include "core/UIInteractive.h"
#include "../resources/Fonts.h"
#include "../protocol/ClientRole.h"
#include "../core/animation/Animation.h"
#include "state/InteractionColorsState.h"
#include "elements/RoundedRectangleElement.h"
#include "elements/TextElement.h"



class LobbyPlayerView final : public UIInteractive
{
public:
	LobbyPlayerView(uint32_t playerId, const std::string& nickname = "Player", ClientRole role = ClientRole::Regular, const sf::Font& font = Fonts::ArialBI, sf::Vector2f size = { 160.0f, 90.0f }, sf::Vector2f position = { 0.0f, 0.0f }) :
		UIInteractive(size, position), m_id(playerId), m_role(role)
	{
		init(size, font, nickname);
	}

	void update(float dt) override;


	uint32_t getId() const;

	RoundedRectangleElement* getBackground();

	void setNickname(std::string nickname);
	TextElement* getNickname();

private:
	const uint32_t m_id;
	ClientRole m_role;

	RoundedRectangleElement* m_background = nullptr;
	TextElement* m_nickname = nullptr;

	void init(sf::Vector2f size, const sf::Font& font, const std::string& nickaname);
	void initBackground();
	void initNickname(const sf::Font& font, const std::string& nickname);
	void initOnHover();
};