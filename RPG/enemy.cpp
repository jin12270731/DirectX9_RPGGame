#include "enemy.h"



enemy::enemy()
{
	for (int i = 0; i < 4; i++)
	{
		enemyPointerX[i] = 0;
		enemyPointerY[i] = 0;
	}
	disPlayFlag = true;
	enemySpeed = 500;
	chaseFlag = false;
	enemyPointX = 0;
	enemyPointY = 0;
	enemyMovePointX = 0;
	enemyMovePointY = 0;
	enemyFlag = false;
	mapEnemyPointX = 0;
	mapEnemyPointY = 0;
	directionType = 0;
	enemyNumber = 0;
	placenumber = 0;
	HP = 0;
	MP = 0;
	SP = 0;
	OffensivePower = 0;
	DefensePower = 0;
	MAX_HP = 0;
	MAX_MP = 0;
	MAX_SP = 0;
}


enemy::~enemy()
{
}

void enemy::setXandY(int enemyX, int enemyY)
{
	int HANI = 24;
	enemyPointerX[0] = HANI * (enemyX + 1);
	enemyPointerY[0] = HANI * enemyY;
	enemyPointerX[1] = HANI * (enemyX + 1);
	enemyPointerY[1] = HANI * (enemyY + 1);
	enemyPointerX[2] = HANI * enemyX;
	enemyPointerY[2] = HANI * enemyY;
	enemyPointerX[3] = HANI * enemyX;
	enemyPointerY[3] = HANI * (enemyY + 1);
	enemyPointX = enemyX;
	enemyPointY = enemyY;
	mapEnemyPointX = enemyX;
	mapEnemyPointY = enemyY;
}
//
void combatAI(playerBase* play)
{

}
//プレイヤーがそばにいるか調べる
void enemy::mapAI(player* play)
{
	switch (placenumber)
	{
	case 0://右を向いている場合
	{
		for (int i = -1; i < 6; i++)//右に三回
		{
			if (play->mapPointX + 16 == mapEnemyPointX + i && play->mapPointY + 12 == mapEnemyPointY)
				chaseFlag = true; return;
		}
		for (int j = 0; j < 2; j++)
		{
			for (int i = -1; i < 3; i++)
			{
				if (play->mapPointX + 16 == mapEnemyPointX + i && play->mapPointY + 12 == mapEnemyPointY + 1 && j == 0)
				{
					chaseFlag = true; return;
				}
				else if (play->mapPointX + 16 == mapEnemyPointX + i && play->mapPointY + 12 == mapEnemyPointY - 1 && j == 1)
				{
					chaseFlag = true; return;
				}
			}
		}
		for (int j = 0; j < 2; j++)
		{
			for (int i = 0; i < 2; i++)
			{
				if (play->mapPointX + 16 == mapEnemyPointX + i && play->mapPointY + 12 == mapEnemyPointY + 2 && j == 0)
				{
					chaseFlag = true; return;
				}
				else if (play->mapPointX + 16 == mapEnemyPointX + i && play->mapPointY + 12 == mapEnemyPointY - 2 && j == 0)
				{
					chaseFlag = true; return;
				}
			}
		}
	}
	break;
	case 1:
	{
		for (int  i = -1; i < 6; i++)
		{
			if (play->mapPointX + 16 == mapEnemyPointX && play->mapPointY + 12 == mapEnemyPointY + i)
			{
				chaseFlag = true; return;
			}
		}
		for (int  j = 0; j < 2; j++)
		{
			for (int  i = -1; i < 3; i++)
			{
				if (play->mapPointX + 16 == mapEnemyPointX + 1 && play->mapPointY + 12 == mapEnemyPointY + j && j == 0)
				{
					chaseFlag = true; return;
				}
				else if (play->mapPointX + 16 == mapEnemyPointX - 1 && play->mapPointY + 12 == mapEnemyPointY + j && j == 1)
				{
					chaseFlag = true; return;
				}
			}
		}
		for (int  j = 0; j < 2; j++)
		{
			for (int  i = 0; i < 2; i++)
			{
				if (play->mapPointX + 16 == mapEnemyPointX + 2 && play->mapPointY + 12 == mapEnemyPointY + i && j == 0)
				{
					chaseFlag = true; return;
				}
				else if (play->mapPointX + 16 == mapEnemyPointX - 2 && play->mapPointY + 12 == mapEnemyPointY + i && j == 0)
				{
					chaseFlag = true; return;
				}
			}
		}
	}
	break;
	case 2:
	{
		for (int i = -3; i < 2; i++)//に三回
		{
			if (play->mapPointX + 16 == mapEnemyPointX + i && play->mapPointY + 12 == mapEnemyPointY)
			{
				chaseFlag = true; return;
			}
		}
		for (int  j = 0; j < 2; j++)
		{
			for (int  i = -2; i < 2; i++)
			{
				if (play->mapPointX + 16 == mapEnemyPointX + i && play->mapPointY + 12 == mapEnemyPointY + 1 && j == 0)
				{
					chaseFlag = true; return;
				}
				else if (play->mapPointX + 16 == mapEnemyPointX + i && play->mapPointY + 12 == mapEnemyPointY - 1 && j == 1)
				{
					chaseFlag = true; return;
				}
			}
		}
		for (int j = 0; j < 2; j++)
		{
			for (int  i = -1; i < 1; i++)
			{
				if (play->mapPointX + 16 == mapEnemyPointX + i && play->mapPointY + 12 == mapEnemyPointY + 2 && j == 0)
				{
					chaseFlag = true; return;
				}
				else if (play->mapPointX + 16 == mapEnemyPointX + i && play->mapPointY + 12 == mapEnemyPointY - 2 && j == 0)
				{
					chaseFlag = true; return;
				}
			}
		}
	}
	break;
	case 3:
	{
		for (int  i = -3; i < 2; i++)
		{
			if (play->mapPointX + 16 == mapEnemyPointX && play->mapPointY + 12 == mapEnemyPointY + i)
			{
				chaseFlag = true; return;
			}
		}
		for (int  j = 0; j < 2; j++)
		{
			for (int  i = -2; i < 2; i++)
			{
				if (play->mapPointX + 16 == mapEnemyPointX + 1 && play->mapPointY + 12 == mapEnemyPointY + j && j == 0)
				{
					chaseFlag = true; return;
				}
				else if (play->mapPointX + 16 == mapEnemyPointX - 1 && play->mapPointY + 12 == mapEnemyPointY + j && j == 1)
				{
					chaseFlag = true; return;
				}
			}
		}
		for (int  j = 0; j < 2; j++)
		{
			for (int  i = -1; i < 1; i++)
			{
				if (play->mapPointX + 16 == mapEnemyPointX + 2 && play->mapPointY + 12 == mapEnemyPointY + i && j == 0)
				{
					chaseFlag = true; return;
				}
				else if (play->mapPointX + 16 == mapEnemyPointX - 2 && play->mapPointY + 12 == mapEnemyPointY + i && j == 0)
				{
					chaseFlag = true; return;
				}
			}
		}
	}
	break;
	}
	chaseFlag = false;
	return;
}
//プレイヤーを追いかける
void enemy::chasePlayer(player* play)
{
	if (play->mapPointX + 16> enemyPointX)
	{
		placenumber = 0;
		directionType = 2;
		mapEnemyPointX += 1;
	}
	else if (play->mapPointX + 16< enemyPointX)
	{
		placenumber = 2;
		enemyFlag = false;
		directionType = 2;
		mapEnemyPointX -= 1;
	}
	else if (play->mapPointY + 12> enemyPointY)
	{
		placenumber = 1;
		directionType = 0;
		mapEnemyPointY += 1;
	}
	else if (play->mapPointY + 12< enemyPointY)
	{
		placenumber = 3;
		enemyFlag = true;
		directionType = 1;
		mapEnemyPointY -= 1;
	}
}
