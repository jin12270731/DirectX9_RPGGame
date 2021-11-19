#include "direct.h"

// �R���X�g���N�^
Direct3D::Direct3D() {
	pD3D9 = NULL;			// Direct3D�f�o�C�X�����p�I�u�W�F�N�g
	pDevice3D = NULL;		// Direct3D�̃f�o�C�X
	lpDI = NULL;			// DirectInput�I�u�W�F�N�g
	lpKeyboard = NULL;      // �L�[�{�[�h�f�o�C�X
	g_pEffect = NULL;
}
// �f�X�g���N�^
Direct3D::~Direct3D() {
	// DirectX�̕ϐ��͕K���㏈�������邱��
	if (pDevice3D != NULL)
		pDevice3D->Release();
	if (pD3D9 != NULL)
		pD3D9->Release();
	if(lpKeyboard != NULL)
		lpKeyboard->Release();
	if(lpDI != NULL)
		lpDI->Release();
	if (g_pEffect != NULL)
		g_pEffect->Release();
}

// �f�o�C�X�쐬
bool Direct3D::Create(HWND hWnd, int Width, int Height, HINSTANCE hInstance) {

	// Direct3D9�I�u�W�F�N�g�̍쐬	
	pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);
	// �f�B�X�v���C���擾
	D3DDISPLAYMODE Display;
	pD3D9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &Display);

	//////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////


	// IDirectInput8�̍쐬
	HRESULT ret = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&lpDI, NULL);
	if (FAILED(ret)) 
	{
		// �쐬�Ɏ��s
		return false;
	}
	// IDirectInputDevice8�̎擾
	if (FAILED(lpDI->CreateDevice(GUID_SysKeyboard, &lpKeyboard, NULL)))
	{
		lpDI->Release();
		return false;
	}

	// ���̓f�[�^�`���̃Z�b�g
	ret = lpKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(ret)) {
		lpKeyboard->Release();
		lpDI->Release();
		return false;
	}

	// �r������̃Z�b�g
	ret = lpKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	if (FAILED(ret)) {
		lpKeyboard->Release();
		lpDI->Release();
		return false;
	}

	

	//////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////


	//�o�b�N�o�b�t�@�̐ݒ�ȂǂɎg��
	D3DPRESENT_PARAMETERS	D3DParam = {			// �X���b�v�`�F�C���ݒ�
		Width,Height,Display.Format,1,D3DMULTISAMPLE_NONE,0,
		D3DSWAPEFFECT_DISCARD,hWnd,TRUE,TRUE,D3DFMT_D24S8,0,0,D3DPRESENT_INTERVAL_DEFAULT
	};

	// HAL���[�h��3D�f�o�C�X�쐬
	if (FAILED(pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DParam.hDeviceWindow,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &D3DParam, &pDevice3D)))
		if (FAILED(pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DParam.hDeviceWindow,
			D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &D3DParam, &pDevice3D)))
			// REF���[�h��3D�f�o�C�X�쐬
			if (FAILED(pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, D3DParam.hDeviceWindow,
				D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &D3DParam, &pDevice3D)))
				if (FAILED(pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, D3DParam.hDeviceWindow,
					D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &D3DParam, &pDevice3D)))
				{
					// 3D�f�o�C�X�쐬���s(���̃O���t�B�b�N�{�[�h�ł�DirectX���g���Ȃ�)
					pD3D9->Release();
					return false;
				}
	//�V�F�[�_�[�t�@�C���̓ǂݍ���
	/*if (FAILED(D3DXCreateEffectFromFile(pDevice3D, "Test.fx", NULL, NULL, 0, NULL, &g_pEffect, NULL)))
	{
		MessageBox(NULL, "�V�F�[�_�[�t�@�C���ǂݍ��ݎ��s", "", MB_OK);  
		return E_FAIL;
	}*/
	return true;
}

void Direct3D::photo()
{
	// �o�b�N�o�t�@�̎擾
	LPDIRECT3DSURFACE9 pBackBuf;
	pDevice3D->GetRenderTarget(0, &pBackBuf);

	// �X�N�V���o��
	D3DXSaveSurfaceToFile("screenshot.bmp", D3DXIFF_BMP, pBackBuf, NULL, NULL);

	// Get�n�Ŏ擾�����T�[�t�F�C�X��AddRef���Ă΂�Ă���̂ŖY�ꂸ�ɉ������
	pBackBuf->Release();
}

