#include "Game.h"
#include <time.h>
#include <Windows.h>

using namespace System;

enum TYPE
{
	WALL,
	FLOOR,
	NODRAW,
};

enum DIRECT
{
	NORTH,
	EAST,
	SOUTH,
	WEST,
};

Game::Game(string t)
{
	title = t;
}

void Game::GenerateLevel(int passes)
{

	Console::Clear();

	int x = 24;
	int y = 16;
	int seed = GetTickCount();
	srand(seed);

	cout << "Map Seed: " << seed << endl;

	vector<DIRECT> directions;
	vector<sf::Vector2i> starts;
	starts.clear();

	int r;

	for (int i = 0; i < passes; i++)
	{
		x = rand() % 47;
		y = rand() % 31;
		starts.push_back(sf::Vector2i(x, y));
		for (int j = 1; j < 50; j++)
		{
			directions.clear();

			if (map[x][y - 1].type == WALL && y - 1 > 0) directions.push_back(NORTH);
			if (map[x + 1][y].type == WALL && x + 1 < 47) directions.push_back(EAST);
			if (map[x][y + 1].type == WALL && y + 1 < 31) directions.push_back(SOUTH);
			if (map[x - 1][y].type == WALL && x - 1 > 0) directions.push_back(WEST);

			if (directions.size() > 0)
				r = rand() % directions.size();
			else { x = rand() % 47; y = rand() % 31; continue; }

			if (directions[r] == NORTH)
			{
				y--;
				map[x][y].type = FLOOR;
			}
			else if (directions[r] == EAST)
			{
				x++;
				map[x][y].type = FLOOR;
			}
			else if (directions[r] == SOUTH)
			{
				y++;
				map[x][y].type = FLOOR;
			}
			else if (directions[r] == WEST)
			{
				x--;
				map[x][y].type = FLOOR;
			}
		}

		do
		{
			player.x = rand() % 47;
			player.y = rand() % 31;
		} while (map[player.x][player.y].type != FLOOR);

	}

	// Connect the dots motherfucker
	cout << "Pointes: " << endl;
	vector<sf::Vector2i> points;
	for (int i = 0; i < (signed)starts.size(); i++)
	{
		sf::Vector2i pos = starts[i];
		sf::Vector2i to;
		if (i == starts.size() - 1)
			to = starts[0];
		else
			to = starts[i + 1];

		cout << to.x << ":" << to.y << endl;

		while (pos.x != to.x)
		{
			if (to.x > pos.x && pos.x < 47)
			{
				pos.x++;
				map[pos.x][pos.y].type = FLOOR;
			}
			else if (to.x < pos.x && pos.x > -1)
			{
				pos.x--;
				map[pos.x][pos.y].type = FLOOR;
			}
		}
		while (pos.y != to.y)
		{
			if (to.y > pos.y && pos.y < 31)
			{
				pos.y++;
				map[pos.x][pos.y].type = FLOOR;
			}
			else if (to.y < pos.y && pos.y > 0)
			{
				pos.y--;
				map[pos.x][pos.y].type = FLOOR;
			}
		}

	}

	for (int i = 0; i < 3; i++)
	{
		int ex, ey;
		enemies.push_back(Enemy());

		do
		{
			ex = rand() % 47;
			ey = rand() % 31;
			enemies[i].SetPosition(ex, ey);
		} while (map[ex][ey].type != FLOOR);

	}

	int gold_sources = 5;
	int i = 0;
	do
	{
		int gx = rand() % 47;
		int gy = rand() % 31;

		if (map[gx][gy].type == FLOOR)
		{
			map[gx][gy].gold += rand() % 10+1;
			i++;
			cout << "GOOOOLLLD!!  " << map[gx][gy].gold << endl;
		}

	} while (i < gold_sources);

}

void Game::ResetLevel()
{
	for (int i = 0; i < 48; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			map[i][j].type = WALL;
		}
	}
}

int Game::DistanceFromPlayer(int x, int y)
{
	int dist = abs(player.x-x) + abs(player.y-y);
	return dist;
}

void Game::ToggleSound()
{
	muted = !muted;
	if (muted)
		soundtrack.stop();
	else
		soundtrack.play();
}

void Game::HideMap()
{
	for (int i = 0; i < 48; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			map[i][j].visible = false;
		}
	}
}

void Game::ShowMap()
{
	for (int i = 0; i < 48; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			map[i][j].visible = true;
		}
	}
}

void Game::UpdateEnemies()
{
	for (int i = 0; i < enemies.size(); i++)
	{
		Enemy &e = enemies[i];
		e.Update(player, map);
	}
}

void Game::FOV()
{
	float x, y;
	HideMap();
	for (int i = 0; i < 360; i++)
	{
		x = cos((float)i*0.01745f);
		y = sin((float)i*0.01745f);
		DoFOV(x, y);
	}
}

void Game::DoFOV(float x, float y)
{
	float ox, oy;
	ox = (float)player.x + 0.5f;
	oy = (float)player.y + 0.5f;
	for (int i = 0; i < 14; i+=2)
	{
		map[(int)ox][(int)oy].visible = true;
		if (map[(int)ox][(int)oy].type == WALL)
			return;
		ox += x;
		oy += y;
	}
}

void Game::Init()
{
	event_map["close"] = thor::Action(sf::Event::Closed);
	event_map["left"] = thor::Action(sf::Keyboard::Left, thor::Action::PressOnce);
	event_map["right"] = thor::Action(sf::Keyboard::Right, thor::Action::PressOnce);
	event_map["up"] = thor::Action(sf::Keyboard::Up, thor::Action::PressOnce);
	event_map["down"] = thor::Action(sf::Keyboard::Down, thor::Action::PressOnce);
	event_map["reset"] = thor::Action(sf::Keyboard::R, thor::Action::PressOnce);
	event_map["toggle"] = thor::Action(sf::Keyboard::T, thor::Action::PressOnce);
	event_map["mute"] = thor::Action(sf::Keyboard::M, thor::Action::PressOnce);
	event_map["clickl"] = thor::Action(sf::Mouse::Left, thor::Action::Hold);

	window.create(sf::VideoMode(480, 420, 32), title, sf::Style::Titlebar | sf::Style::Close);
	window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(false);
	window.setKeyRepeatEnabled(false);

	player = Player();

	font.loadFromFile("res/font.ttf");

	help.setString("Re-Seed:     R\n"
								 "Move Left:   LEFT\n"
								 "Move Right:  RIGHT\n"
								 "Move Up:     UP\n"
								 "Move Down:   DOWN\n"
								 "Toggle View: T\n"
								 "Mute Sound:  M\n");
	help.setFont(font);
	help.setCharacterSize(12);
	help.setPosition(10, 330);

	stats.setString(stats_s);
	stats.setFont(font);
	stats.setCharacterSize(12);
	stats.setPosition(250, 330);

	ui.setSize(sf::Vector2f(480, 100));
	ui.setPosition(sf::Vector2f(0, 320));
	ui.setFillColor(sf::Color(25, 25, 25, 255));

	view = true;
	muted = true;

	soundtrack.openFromFile("res/track.ogg");

	// Fill Array And Set Positions
	for (int i = 0; i < 48; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			map[i][j] = TILE();
			map[i][j].type = WALL;
			map[i][j].bounds.setPosition(sf::Vector2f((float)i*10, (float)j*10));
		}
	}

	GenerateLevel(10);

}

void Game::Start()
{
//	soundtrack.play();
	while (window.isOpen())
	{
		event_map.update(window);

		// Window Close Event
		if (event_map.isActive("close"))
			window.close();
		// Movement
		if (event_map.isActive("left") && player.x - 1 > -1 && map[player.x - 1][player.y].type != WALL)
		{
			for (Enemy &e : enemies)
			{
				if (player.x - 1 == e.GetX() && player.y == e.GetY())
					e.SetHP(e.GetHP() - 1);
			}
			if (map[player.x - 1][player.y].bounds.getFillColor().r != 150)
				player.x--;
			UpdateEnemies();
		}
		else if (event_map.isActive("right") && player.x < 47 && map[player.x + 1][player.y].type != WALL)
		{
			for (Enemy &e : enemies)
			{
				if (player.x + 1 == e.GetX() && player.y == e.GetY())
					e.SetHP(e.GetHP() - 1);
			}
			if (map[player.x + 1][player.y].bounds.getFillColor().r != 150)
				player.x++;
			UpdateEnemies();
		}
		else if (event_map.isActive("up") && player.y > 0 && map[player.x][player.y - 1].type != WALL)
		{
			for (Enemy &e : enemies)
			{
				if (player.y - 1 == e.GetY() && player.x == e.GetX())
					e.SetHP(e.GetHP() - 1);
			}
			if (map[player.x][player.y - 1].bounds.getFillColor().r != 150)
				player.y--;
			UpdateEnemies();
		}
		else if (event_map.isActive("down") && player.y < 31 && map[player.x][player.y + 1].type != WALL)
		{
			for (Enemy &e : enemies)
			{
				if (player.y + 1 == e.GetY() && player.x == e.GetX())
					e.SetHP(e.GetHP() - 1);
			}
			if (map[player.x][player.y + 1].bounds.getFillColor().r != 150)
				player.y++;
			UpdateEnemies();
		}

		// Mostly Debug Tools
		if (event_map.isActive("reset"))
		{
			ResetLevel();
			GenerateLevel(10);
		}
		if (event_map.isActive("toggle"))
			view = !view;
		if (event_map.isActive("mute"))
			ToggleSound();
		if (event_map.isActive("clickl"))
		{
			sf::Vector2i mpos = sf::Mouse::getPosition(window);
			for (int i = 0; i < 48; i++)
			{
				for (int j = 0; j < 32; j++)
				{
					if (map[i][j].bounds.getGlobalBounds().contains(mpos.x, mpos.y))
					{
						map[i][j].type = FLOOR;
					}
				}
			}
		}

		char buff[100];
		sprintf_s(buff, "HP: %d\nGOLD: %d\nX: %d\nY: %d\n", player.health, player.gold, player.x, player.y);
		stats_s = buff;
		stats.setString(stats_s);

		for (int i = 0; i < 48; i++)
		{
			for (int j = 0; j < 32; j++)
			{
				if (map[i][j].type == WALL)   // WALL Texture
					map[i][j].bounds.setFillColor(sf::Color(100, 100, 100, 255));
				if (map[i][j].type == FLOOR)  // FLOOR Texture
					map[i][j].bounds.setFillColor(sf::Color(50, 50, 50, 255));
				if (map[i][j].type == NODRAW) // NODRAW Texure
					map[i][j].bounds.setFillColor(sf::Color(200, 105, 180));

				if (map[i][j].gold > 0) // Found some gold
					map[i][j].bounds.setFillColor(sf::Color(155, 155, 0));
				if (i == player.x && j == player.y) // Found the player
					map[i][j].bounds.setFillColor(sf::Color(0, 0, 100, 255));

				for (int z = 0; z < enemies.size(); z++)
				{
					if (enemies[z].GetHP() <= 0)
					{
						enemies.erase(enemies.begin() + z);
					}
					if (i == enemies[z].GetX() && j == enemies[z].GetY()) // Found an enemy
					{
						map[i][j].bounds.setFillColor(sf::Color(150, 0, 0, 255));
					}
				}

			}
		}

		if (view)
			FOV();
		else
			ShowMap();

		window.clear(sf::Color(10,10,10,255));

		for (int i = 0; i < 48; i++)
		{
			for (int j = 0; j < 32; j++)
			{
				if (map[i][j].visible || (i==player.x && j==player.y))
				{
					window.draw(map[i][j].bounds);
				}
			}
		}

		if (muted)
			window.draw(sf::Text("MUTED", font, 15));

		window.draw(ui);
		window.draw(help);
		window.draw(stats);

		window.display();

	}
}
