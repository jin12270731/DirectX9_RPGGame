#include "directMouse.h"



directMouse::directMouse()
{
	g_pDInput = NULL;		// DirectInput�I�u�W�F�N�g
	g_pDIMouse = NULL;		// �}�E�X�f�o�C�X		
	// �}�E�X��Ԃ̏�����
	for (int i = 0; i < 8; i++)
		g_zdiMouseState.rgbButtons[i] = 0;
	g_zdiMouseState.lX = 0;
	g_zdiMouseState.lY = 0;
	g_zdiMouseState.lZ = 0;
}


directMouse::~directMouse()
{
	if (g_pDInput != NULL) // DirectInput�̃f�o�C�X���J��
		g_pDInput->Release();
	if (g_pDIMouse != NULL) // DirectInput�̃f�o�C�X���J��
		g_pDIMouse->Release();
}
bool directMouse::create(HWND hWnd, HINSTANCE hInstance)
{
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pDInput, NULL))) {
		return false;	// �쐬�Ɏ��s
	}
	HRESULT ret = S_FALSE;
	if (g_pDInput == NULL) {
		return false;
	}

	// �}�E�X�p�Ƀf�o�C�X�I�u�W�F�N�g���쐬
	ret = g_pDInput->CreateDevice(GUID_SysMouse, &g_pDIMouse, NULL);
	if (FAILED(ret)) {
		// �f�o�C�X�̍쐬�Ɏ��s
		return false;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	ret = g_pDIMouse->SetDataFormat(&c_dfDIMouse);	// �}�E�X�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
	if (FAILED(ret)) {
		// �f�[�^�t�H�[�}�b�g�Ɏ��s
		return false;
	}

	// ���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	ret = g_pDIMouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(ret)) {
		// ���[�h�̐ݒ�Ɏ��s
		return false;
	}

	// �f�o�C�X�̐ݒ�
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;	// ���Βl���[�h�Őݒ�i��Βl��DIPROPAXISMODE_ABS�j

	ret = g_pDIMouse->SetProperty(DIPROP_AXISMODE, &diprop.diph);
	if (FAILED(ret)) {
		// �f�o�C�X�̐ݒ�Ɏ��s
		return false;
	}
	// ���͐���J�n
	g_pDIMouse->Acquire();
	return true;

}

void directMouse::GetMouseState(HWND hWnd)
{
	// �ǎ�O�̒l��ێ����܂�
	DIMOUSESTATE g_zdiMouseState_bak;	// �}�E�X���(�ω����m�p)
	memcpy(&g_zdiMouseState_bak, &g_zdiMouseState, sizeof(g_zdiMouseState_bak));

		// �}�E�X�̏�Ԃ��擾���܂�
	HRESULT	hr = g_pDIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &g_zdiMouseState);
	if (hr == DIERR_INPUTLOST) {
		g_pDIMouse->Acquire();
		hr = g_pDIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &g_zdiMouseState);
	}
}
