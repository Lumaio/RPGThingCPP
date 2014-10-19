#pragma once
#include <SFML/Graphics.hpp>

struct Player
{
	int x, y, health, max_health, gold, turns_since_attack;
	Player() { x = 1; y = 1; health = 5; max_health = 5; gold = 0; turns_since_attack = 0; }
};

struct TILE
{
	int gold;
	int type;
	bool visible;
	sf::RectangleShape bounds;
	TILE() {
		bounds.setSize(sf::Vector2f(10, 10));
		visible = false;
		gold = 0;
	}
};
