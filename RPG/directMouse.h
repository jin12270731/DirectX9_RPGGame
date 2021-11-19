#pragma once
#include "direct.h"
class directMouse
{
public:
	LPDIRECTINPUT8			g_pDInput = NULL;	// DirectInputオブジェクト
	LPDIRECTINPUTDEVICE8	g_pDIMouse = NULL;	// マウスデバイス
	DIMOUSESTATE2			g_zdiMouseState;			// マウス状態
	directMouse();
	~directMouse();
	bool create(HWND hWnd, HINSTANCE hInstance);
	void GetMouseState(HWND hWnd);
};

