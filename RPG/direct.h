#pragma once
#define D3D_DEBUG_INFO	// Direct3Dデバックフラグ
#define DIRECTINPUT_VERSION		0x0800		// DirectInputのバージョン情報

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3dxof.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#include <comdef.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <mmsystem.h>
#include <dinput.h>
#include <time.h>
#include <dinput.h>


// Direct3Dクラス
class Direct3D
{
public:
	IDirect3D9*				pD3D9;			// Direct3Dデバイス生成用オブジェクト
	IDirect3DDevice9*		pDevice3D;		// Direct3Dのデバイス
	LPDIRECTINPUT8			lpDI;			// DirectInputオブジェクト
	LPD3DXEFFECT			g_pEffect;		// Direct3Dシェーダー
	LPDIRECTINPUTDEVICE8    lpKeyboard;     // キーボードデバイス
	// コンストラクタ
	Direct3D();
	// デストラクタ
	~Direct3D();

	// デバイス作成
	bool Create(HWND hWnd, int Width, int Height, HINSTANCE hInstance);
	//
	void photo();
};