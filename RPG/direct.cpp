#include "direct.h"

// コンストラクタ
Direct3D::Direct3D() {
	pD3D9 = NULL;			// Direct3Dデバイス生成用オブジェクト
	pDevice3D = NULL;		// Direct3Dのデバイス
	lpDI = NULL;			// DirectInputオブジェクト
	lpKeyboard = NULL;      // キーボードデバイス
	g_pEffect = NULL;
}
// デストラクタ
Direct3D::~Direct3D() {
	// DirectXの変数は必ず後処理をすること
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

// デバイス作成
bool Direct3D::Create(HWND hWnd, int Width, int Height, HINSTANCE hInstance) {

	// Direct3D9オブジェクトの作成	
	pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);
	// ディスプレイ情報取得
	D3DDISPLAYMODE Display;
	pD3D9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &Display);

	//////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////


	// IDirectInput8の作成
	HRESULT ret = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&lpDI, NULL);
	if (FAILED(ret)) 
	{
		// 作成に失敗
		return false;
	}
	// IDirectInputDevice8の取得
	if (FAILED(lpDI->CreateDevice(GUID_SysKeyboard, &lpKeyboard, NULL)))
	{
		lpDI->Release();
		return false;
	}

	// 入力データ形式のセット
	ret = lpKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(ret)) {
		lpKeyboard->Release();
		lpDI->Release();
		return false;
	}

	// 排他制御のセット
	ret = lpKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	if (FAILED(ret)) {
		lpKeyboard->Release();
		lpDI->Release();
		return false;
	}

	

	//////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////


	//バックバッファの設定などに使う
	D3DPRESENT_PARAMETERS	D3DParam = {			// スワップチェイン設定
		Width,Height,Display.Format,1,D3DMULTISAMPLE_NONE,0,
		D3DSWAPEFFECT_DISCARD,hWnd,TRUE,TRUE,D3DFMT_D24S8,0,0,D3DPRESENT_INTERVAL_DEFAULT
	};

	// HALモードで3Dデバイス作成
	if (FAILED(pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DParam.hDeviceWindow,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &D3DParam, &pDevice3D)))
		if (FAILED(pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DParam.hDeviceWindow,
			D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &D3DParam, &pDevice3D)))
			// REFモードで3Dデバイス作成
			if (FAILED(pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, D3DParam.hDeviceWindow,
				D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &D3DParam, &pDevice3D)))
				if (FAILED(pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, D3DParam.hDeviceWindow,
					D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &D3DParam, &pDevice3D)))
				{
					// 3Dデバイス作成失敗(このグラフィックボードではDirectXが使えない)
					pD3D9->Release();
					return false;
				}
	//シェーダーファイルの読み込み
	/*if (FAILED(D3DXCreateEffectFromFile(pDevice3D, "Test.fx", NULL, NULL, 0, NULL, &g_pEffect, NULL)))
	{
		MessageBox(NULL, "シェーダーファイル読み込み失敗", "", MB_OK);  
		return E_FAIL;
	}*/
	return true;
}

void Direct3D::photo()
{
	// バックバファの取得
	LPDIRECT3DSURFACE9 pBackBuf;
	pDevice3D->GetRenderTarget(0, &pBackBuf);

	// スクショ出力
	D3DXSaveSurfaceToFile("screenshot.bmp", D3DXIFF_BMP, pBackBuf, NULL, NULL);

	// Get系で取得したサーフェイスはAddRefが呼ばれているので忘れずに解放する
	pBackBuf->Release();
}

