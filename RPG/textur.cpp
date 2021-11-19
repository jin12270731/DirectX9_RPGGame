#include "texture.h"
#include <Windows.h>
#include <tchar.h>
#include "direct.h"

// �e�N�X�`���N���X

Texture::Texture() {
	pTexture = NULL;
	pictureWidth = 0.0f;
	pictureHeight = 0.0f;
	width = 1;
}
Texture::~Texture()
{
	// �ǂݍ��܂�Ă�����j��
	if (pTexture != NULL)
		pTexture->Release();
}
bool Texture::Load(IDirect3DDevice9* pDevice3D,const char* textureName)
{
	// �摜�ǂݍ���
	// DirextX��WindowsAPI�̊֐���HRESULT�����ʂɕԂ��֐�������
	// FAILED�}�N���Ŋ֐������s�������킩��
	// SUCEEDED�}�N���Ŋ֐��������������킩��
	if (FAILED(D3DXCreateTextureFromFile(pDevice3D, textureName, &pTexture)))
		return false;	// �摜�ǂݍ��ݎ��s�i�t�@�C�����Ȃ��\������j
	// �摜�ǂݍ��ݐ���
	return true;
}

bool Texture::LoadEX(IDirect3DDevice9 * pDevice3D, const char * textureName)
{
	D3DXCreateTextureFromFileEx(
		pDevice3D,
		textureName,              // �t�@�C����
		0,
		0,
		0,
		0,
		D3DFMT_A1R5G5B5,                // �F�������\��
		D3DPOOL_MANAGED,
		D3DX_FILTER_LINEAR,
		D3DX_FILTER_LINEAR,
		D3DCOLOR_ARGB(255, 0, 0, 255),  // �F�𓧉ߐF�Ƃ���
		NULL,
		NULL,
		&pTexture                   // �e�N�X�`����
	);

	return false;
}

void Texture::setpicture(int Width, int Height)
{
	width = Width;
	pictureWidth =(float) 1 / Width;
	pictureHeight = (float)1 / Height;
}
