#pragma once
#include "direct.h"
class directMouse
{
public:
	LPDIRECTINPUT8			g_pDInput = NULL;	// DirectInput�I�u�W�F�N�g
	LPDIRECTINPUTDEVICE8	g_pDIMouse = NULL;	// �}�E�X�f�o�C�X
	DIMOUSESTATE2			g_zdiMouseState;			// �}�E�X���
	directMouse();
	~directMouse();
	bool create(HWND hWnd, HINSTANCE hInstance);
	void GetMouseState(HWND hWnd);
};

