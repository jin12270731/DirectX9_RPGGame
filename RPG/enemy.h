#pragma once
#include "player.h"
class enemy
{
public:
	enemy();
	~enemy();
	void setXandY(int enemyX, int enemyY);
	//�}�b�v�ł�AI�ݒ�
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
	//�\�����邩�ǂ��������߂�
	bool disPlayFlag;
	//�ǂ������邩�ǂ���
	bool chaseFlag;
	//�G�̑���
	UINT enemySpeed;
	int mapEnemyPointX;
	int mapEnemyPointY;
	//�v���C���[�̌����Ă�������B�O�ŉE�A�P�ŉ��A�Q�ō��A�R�ŏ�
	short placenumber;
	//�O�ŉ��A�P�ŏ�A�Q�ō��E
	short directionType;
	//���̉摜�ł̓G�̎��
	short enemyNumber;
	//�摜�f�[�^�̒l
	short pictureNumber;
	//
	short chosePlayer;
	//�G�̏��
	int HP, MP, SP, OffensivePower, magicOffensivePower,DefensePower, magicDefensePower,MAX_HP, MAX_MP, MAX_SP, speed, experience_point, critical_Rate;
};