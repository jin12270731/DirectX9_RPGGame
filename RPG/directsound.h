#pragma once
// DirectSoundに必要なヘッダーとライブラリ

#define DIRECTSOUND_VERSION 0x800	// DirectSoundのバージョン

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "d3dxof.lib")
#pragma comment(lib, "dxguid.lib")

#include <comdef.h>
#include <dsound.h>
#include <tchar.h>


// DirectSoundクラス
class DirectSound
{
public:
	IDirectSound8*	pDirectSound8; // DirectSoundデバイス


	// コンストラクタ
	DirectSound();
	// デストラクタ
	~DirectSound();

	bool Create(HWND hWnd);
};