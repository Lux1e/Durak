#pragma once
#include <SFML/Graphics.hpp>
#include "../core/UIElement.h"



class SpriteElement : public UIElement
{
public:
	SpriteElement(const sf::Texture& texture, sf::Vector2f position = { 0.0f, 0.0f }) : m_sprite(texture),
		UIElement(sf::Vector2f({ static_cast<float>(texture.getSize().x), static_cast<float>(texture.getSize().y) }), position) {}

	sf::Sprite& sprite() { return m_sprite; }

	void setSprite(const sf::Texture& texture) { m_sprite.setTexture(texture); m_size = { static_cast<float>(texture.getSize().x), static_cast<float>(texture.getSize().y) }; }
	void setTextureRect(sf::IntRect rect) { m_sprite.setTextureRect(rect); m_sprite.setScale({ m_size.x / rect.size.x, m_size.y / rect.size.y }); }

	void fitToSize(const sf::Vector2f& size)
	{
		assert(size.x >= 0.0f);
		assert(size.y >= 0.0f);

		const auto& rect = m_sprite.getTextureRect();

		float ratioX = size.x != 0.0f ? size.x / rect.size.x : 0.0f;
		float ratioY = size.y != 0.0f ? size.y / rect.size.y : 0.0f;

		float ratio = std::min(ratioX, ratioY);

		m_size = { m_size.x * ratio, m_size.y * ratio };
		onSizeChanged({ ratio, ratio });
	}

private:
	sf::Sprite m_sprite;

	void onSizeChanged(sf::Vector2f ratio) override { m_sprite.setScale({ ratio.x * m_sprite.getScale().x, ratio.y * m_sprite.getScale().y }); }
	void drawSelf(sf::RenderTarget& target, sf::RenderStates states) const override { target.draw(m_sprite, states); }
};