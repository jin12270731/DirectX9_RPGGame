#pragma once
#include "player.h"
class enemy
{
public:
	enemy();
	~enemy();
	void setXandY(int enemyX, int enemyY);
	//マップでのAI設定
	void mapAI(player* play);
	void chasePlayer(player * play);
	int enemyPointX;
	int enemyPointY;
	int enemyPointerX[4];
	int enemyPointerY[4];
	short enemyMovePointX;
	short enemyMovePointY;
	bool enemyFlag;
	//
	string name;
	//表示するかどうかを決める
	bool disPlayFlag;
	//追いかけるかどうか
	bool chaseFlag;
	//敵の速さ
	UINT enemySpeed;
	int mapEnemyPointX;
	int mapEnemyPointY;
	//プレイヤーの向いている方向。０で右、１で下、２で左、３で上
	short placenumber;
	//０で下、１で上、２で左右
	short directionType;
	//その画像での敵の種類
	short enemyNumber;
	//画像データの値
	short pictureNumber;
	//
	short chosePlayer;
	//敵の状態
	int HP, MP, SP, OffensivePower, magicOffensivePower,DefensePower, magicDefensePower,MAX_HP, MAX_MP, MAX_SP, speed, experience_point, critical_Rate;
};