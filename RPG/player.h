#pragma once
#include <string>
#include "playerBase.h"
using namespace std;

class player:public playerBase
{
protected:
	//戦闘時のプレイヤーの人数
	int players;
public:
	player();
	~player();
	void SetXandY(int x, int y);
	int getPlayers();
	void setPlayers(int Players);
	//プレイヤーの移動量
	int movePointerX;
	int movePointerY;
	//マップの場所
	int mapPointX;
	int mapPointY;
	//プレイヤーの顔データ
	short playerFaceNumberU;
	short playerFaceNumberV;
	//プレイヤーの動いてる状態
	short playerNumber;
	//プレイヤーの向いている方向。０で右、１で下、２で左、３で上
	short placeNumber;
	unsigned int playerPointX;
	unsigned int playerPointY;
};

