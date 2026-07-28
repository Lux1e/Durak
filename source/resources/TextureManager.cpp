#include "TextureManager.h"



void TextureManager::init()
{
	m_textureByPath["assets/textures/ui/arrowLeft.png"] = sf::Texture{ std::filesystem::path("assets/textures/ui/arrowLeft.png"), true };
	m_textureByPath["assets/textures/ui/arrowLeft2.png"] = sf::Texture{ std::filesystem::path("assets/textures/ui/arrowLeft2.png"), true };
	m_textureByPath["assets/textures/ui/arrow_decorative_w.png"] = sf::Texture{ std::filesystem::path("assets/textures/ui/arrow_decorative_w.png"), true };
	m_textureByPath["assets/textures/ui/arrow_basic_w.png"] = sf::Texture{std::filesystem::path("assets/textures/ui/arrow_basic_w.png"), true};
	m_textureByPath["assets/textures/ui/arrow_rotate.png"] = sf::Texture{ std::filesystem::path("assets/textures/ui/arrow_rotate.png"), true };

	m_textureByPath["assets/textures/ui/exitLeft.png"] = sf::Texture{ std::filesystem::path("assets/textures/ui/exitLeft.png"), true };
	m_textureByPath["assets/textures/ui/gear.png"] = sf::Texture{ std::filesystem::path("assets/textures/ui/gear.png"), true };

	m_textureByPath["assets/textures/ui/locked.png"] = sf::Texture{ std::filesystem::path("assets/textures/ui/locked.png"), true };
	m_textureByPath["assets/textures/ui/unlocked.png"] = sf::Texture{ std::filesystem::path("assets/textures/ui/unlocked.png"), true };


	for (auto& [key, texture] : m_textureByPath)
		texture.setSmooth(true);
}


const sf::Texture& TextureManager::getTexture(const std::filesystem::path& path)
{
	auto it = m_textureByPath.find(path);
	assert(it != m_textureByPath.end());

	return it->second;
}