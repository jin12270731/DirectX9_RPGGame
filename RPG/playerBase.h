#pragma once
#include <string>
#include <d3d9.h>
#include <d3dx9.h>
#include "iniClass.h"

using namespace std;
class playerBase
{	
public:
	int getPlayers() { return 0; }
	void setConditionSuu(int);
	errno_t errorcode;
	playerBase();
	~playerBase();
	void setEquipment(char FileName[]);
	IDirect3DTexture9* FaceTexture;
	IDirect3DTexture9* playerTexture;
	//名前
	string name;
	//状態異常（0=無,1=毒,2=猛毒,3=火傷,4=睡眠,5=麻痺,6=弱体,7=凍結,8=石化,9=出血,10=洗脳）
	short* condition;
	//プレイヤーが選んだもの
	short choseEnemy[10], chosePlayer[5], choseItem, choseSkill, choseMagic;
	//フラグ
	bool combat_Stand_By_Flag, death_Flag, chosePhysical;
	//装備データ
	short headEquipment, neckEquipment, bodyEquipment, armEquipment, fingerEquipment[10], legEquipment, footEquipment, ankleEquipment[2];
	//属性適正
	float fire, water, soil, wind, naught, shine, darkness;
	//魔力操作（魔法の威力が上がる）、魔力感知（敵の情報がわかる）
	float magicOperation, magicPerception;
	//そのプレイヤーを持っているか
	bool havingPlayerFlag;
	//プレイヤーの顔データ
	short playerFaceNumberU;
	short playerFaceNumberV;
	//HP,SP,MP
	int MP, SP, MAX_HP, MAX_MP, MAX_SP;
	//プレイヤーの状態
	UINT level, skillLevel, skillPoint, magicLevel, HP, total_Experience, critical_Rate;
	double OffensivePower, magicOffensivePower, DefensePower, magicDefensePower, speed;
	//アイテム、スキル、魔法を持っているかどうか
	UINT haveItem[999];
	bool haveSkill[999];
	bool haveMagic[999];
	//使ったターン数を格納しておく
	struct skillState
	{
		short skillTrun, targetPeople, choseEnemy[10], chosePlayer[5];

	}skillState[999];
	bool playerLoad(IDirect3DDevice9 * pDevice3D, const char * textureName);
	bool FaceLoad(IDirect3DDevice9 * pDevice3D, const char * textureName);
};

