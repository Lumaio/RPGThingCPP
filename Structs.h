#pragma once
#include <SFML/Graphics.hpp>

struct Player
{
	int x, y, health;
	Player() { x = 1; y = 1; health = 5; }
};

struct TILE
{
	int type;
	bool visible;
	sf::RectangleShape bounds;
	TILE() {
		bounds.setSize(sf::Vector2f(10, 10));
		visible = false;
	}
};
