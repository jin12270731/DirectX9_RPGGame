#include "player.h"

player::player()
{
	playerFaceNumberU = 0;
	playerFaceNumberV = 0;
	movePointerX = 0;
	movePointerY = 0;
	playerNumber = 0;
	placeNumber = 0;
	playerPointX = 0;
	playerPointY = 0;
	mapPointX = 0;
	mapPointY = 0;
}

player::~player()
{
}
void player::SetXandY(int x, int y)
{
	playerPointX = x;
	playerPointY = y;
	mapPointX = x - 16;
	mapPointY = y - 12;
}

int player::getPlayers()
{
	return players;
}

void player::setPlayers(int Players)
{
	players = Players;
}
