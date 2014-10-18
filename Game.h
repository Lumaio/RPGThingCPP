#pragma once

#include <iostream>
#include <vector>
#include <map>

#include <cmath>
#include <cstdlib>
#include <random>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Thor/Input.hpp>

#include "Enemy.h"
#include "Structs.h"

using namespace std;

class Game
{
private:
	string title;
	sf::RenderWindow window;
	thor::ActionMap<string> event_map;
	
	Player player;
	
	TILE map[48][32];

	sf::Font font;
	sf::Text help;
	sf::Text stats;

	string stats_s;

	sf::RectangleShape ui;

	sf::Music soundtrack;

	vector<Enemy> enemies;

	bool view;
	bool muted;

public:
	Game(string t);
	~Game() = default;
	
	void GenerateLevel(int passes);
	void ResetLevel();
	
	int DistanceFromPlayer(int x, int y);

	void ToggleSound();

	void HideMap();
	void ShowMap();

	void UpdateEnemies();

	void FOV();
	void DoFOV(float x, float y);

	void Init();
	void Start();

};
