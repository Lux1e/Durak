#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>



class TextureManager
{
public:
	TextureManager()
	{
		init();
	}


	const sf::Texture& getTexture(const std::filesystem::path& path);

private:
	void init();

	std::unordered_map<std::filesystem::path, sf::Texture> m_textureByPath;
};