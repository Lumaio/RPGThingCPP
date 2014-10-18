#pragma once

#include <iostream>
#include "Structs.h"

class Enemy
{
private:
	int x, y;
	int health;
public:
	Enemy();
	~Enemy() = default;

	void SetPosition(int xx, int yy);
	void SetHP(int hp);
	bool FoundPlayer(int px, int py);
	void Update(Player &ply, TILE map[48][32]);
	int GetX();
	int GetY();
	int GetHP();

};
