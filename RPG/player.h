#pragma once
#include <string>
#include "playerBase.h"
using namespace std;

class player:public playerBase
{
protected:
	//�퓬���̃v���C���[�̐l��
	int players;
public:
	player();
	~player();
	void SetXandY(int x, int y);
	int getPlayers();
	void setPlayers(int Players);
	//�v���C���[�̈ړ���
	int movePointerX;
	int movePointerY;
	//�}�b�v�̏ꏊ
	int mapPointX;
	int mapPointY;
	//�v���C���[�̊�f�[�^
	short playerFaceNumberU;
	short playerFaceNumberV;
	//�v���C���[�̓����Ă���
	short playerNumber;
	//�v���C���[�̌����Ă�������B�O�ŉE�A�P�ŉ��A�Q�ō��A�R�ŏ�
	short placeNumber;
	unsigned int playerPointX;
	unsigned int playerPointY;
};

