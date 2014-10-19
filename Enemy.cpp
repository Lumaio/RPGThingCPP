#include "Enemy.h"

enum TYPE
{
	WALL,
	FLOOR,
	NODRAW,
};

Enemy::Enemy()
{
	health = 3;
}

void Enemy::SetPosition(int xx, int yy)
{
	x = xx;
	y = yy;
}
void Enemy::SetHP(int hp)
{
	health = hp;
}

bool Enemy::FoundPlayer(int px, int py)
{
	if ((px == x - 1 && py == y) || (px == x + 1 && py == y) || (py == y - 1 && px == x) || (py == y + 1 && px == x))
		return true;
	return false;
}

void Enemy::Update(Player &ply, TILE map[48][32])
{

	bool foundplayer = FoundPlayer(ply.x, ply.y);
	if (!foundplayer)
	{
		if (ply.x < x && x > -1 && map[x - 1][y].type == FLOOR && map[x - 1][y].bounds.getFillColor().r != 150 && (x - 1 != ply.x && ply.y != y))
		{
			x -= 1;
		}
		if (ply.x > x && x < 48 && map[x + 1][y].type == FLOOR && map[x + 1][y].bounds.getFillColor().r != 150 && (x + 1 != ply.x && ply.y != y))
		{
			x += 1;
		}
		if (ply.y < y && y > -0 && map[x][y - 1].type == FLOOR && map[x][y - 1].bounds.getFillColor().r != 150 && (y - 1 != ply.y && ply.x != x))
		{
			y -= 1;
		}
		if (ply.y > y && y < 32 && map[x][y + 1].type == FLOOR && map[x][y + 1].bounds.getFillColor().r != 150 && (y + 1 != ply.y && ply.x != x))
		{
			y += 1;
		}
	}
	else if (foundplayer)
	{
		ply.health -= 1;
		ply.turns_since_attack = 0;
	}
	else
	{
		std::cout << "Bang!" << std::endl;
	}
}

int Enemy::GetX()
{
	return x;
}

int Enemy::GetY()
{
	return y;
}

int Enemy::GetHP()
{
	return health;
}
