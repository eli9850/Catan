#pragma once

#include <SFML/Graphics.hpp>

class NodeSprite
{
public:
	void set_sprite(const sf::Sprite& sprite) { m_structure = sprite; }
	const sf::Sprite& get_sprite() const { return m_structure; }

	void set_texture(const sf::Texture& texture) { m_structure.setTexture(texture); }
	void set_scale(const float factor_x, const float factor_y) { m_structure.setScale(factor_x, factor_y); }
	void set_position(const float x, const float y) { m_structure.setPosition(x, y); }

private:
	sf::Sprite m_structure;
};
