#pragma once
// DirectSound�ɕK�v�ȃw�b�_�[�ƃ��C�u����

#define DIRECTSOUND_VERSION 0x800	// DirectSound�̃o�[�W����

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "d3dxof.lib")
#pragma comment(lib, "dxguid.lib")

#include <comdef.h>
#include <dsound.h>
#include <tchar.h>


// DirectSound�N���X
class DirectSound
{
public:
	IDirectSound8*	pDirectSound8; // DirectSound�f�o�C�X


	// �R���X�g���N�^
	DirectSound();
	// �f�X�g���N�^
	~DirectSound();

	bool Create(HWND hWnd);
};