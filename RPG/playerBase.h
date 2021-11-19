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
	//���O
	string name;
	//��Ԉُ�i0=��,1=��,2=�ғ�,3=�Ώ�,4=����,5=���,6=���,7=����,8=�Ή�,9=�o��,10=���]�j
	short* condition;
	//�v���C���[���I�񂾂���
	short choseEnemy[10], chosePlayer[5], choseItem, choseSkill, choseMagic;
	//�t���O
	bool combat_Stand_By_Flag, death_Flag, chosePhysical;
	//�����f�[�^
	short headEquipment, neckEquipment, bodyEquipment, armEquipment, fingerEquipment[10], legEquipment, footEquipment, ankleEquipment[2];
	//�����K��
	float fire, water, soil, wind, naught, shine, darkness;
	//���͑���i���@�̈З͂��オ��j�A���͊��m�i�G�̏�񂪂킩��j
	float magicOperation, magicPerception;
	//���̃v���C���[�������Ă��邩
	bool havingPlayerFlag;
	//�v���C���[�̊�f�[�^
	short playerFaceNumberU;
	short playerFaceNumberV;
	//HP,SP,MP
	int MP, SP, MAX_HP, MAX_MP, MAX_SP;
	//�v���C���[�̏��
	UINT level, skillLevel, skillPoint, magicLevel, HP, total_Experience, critical_Rate;
	double OffensivePower, magicOffensivePower, DefensePower, magicDefensePower, speed;
	//�A�C�e���A�X�L���A���@�������Ă��邩�ǂ���
	UINT haveItem[999];
	bool haveSkill[999];
	bool haveMagic[999];
	//�g�����^�[�������i�[���Ă���
	struct skillState
	{
		short skillTrun, targetPeople, choseEnemy[10], chosePlayer[5];

	}skillState[999];
	bool playerLoad(IDirect3DDevice9 * pDevice3D, const char * textureName);
	bool FaceLoad(IDirect3DDevice9 * pDevice3D, const char * textureName);
};

