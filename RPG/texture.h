#pragma once
// Direct3D�̊e��w�b�_�[�̃C���N���[�h���K�v�Ȃ���
// ���łɂ܂Ƃ߂Ă���w�b�_�[���C���N���[�h����
#include "direct.h"



// �e�N�X�`���N���X
class Texture
{
public:
	IDirect3DTexture9* pTexture;
	float pictureWidth;
	float pictureHeight;
	int width;
	// �R���X�g���N�^
	Texture();
	// �f�X�g���N�^
	~Texture();
	// �摜�f�[�^�ǂݍ���
	bool Load(IDirect3DDevice9* pDevice3D,const char* textureName);
	bool LoadEX(IDirect3DDevice9* pDevice3D, const char* textureName);
	void setpicture(int Width, int Height);
};