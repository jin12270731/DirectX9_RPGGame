#include "texture.h"
#include <Windows.h>
#include <tchar.h>
#include "direct.h"

// テクスチャクラス

Texture::Texture() {
	pTexture = NULL;
	pictureWidth = 0.0f;
	pictureHeight = 0.0f;
	width = 1;
}
Texture::~Texture()
{
	// 読み込まれていたら破棄
	if (pTexture != NULL)
		pTexture->Release();
}
bool Texture::Load(IDirect3DDevice9* pDevice3D,const char* textureName)
{
	// 画像読み込み
	// DirextXやWindowsAPIの関数はHRESULTを結果に返す関数が多い
	// FAILEDマクロで関数が失敗したかわかる
	// SUCEEDEDマクロで関数が成功したかわかる
	if (FAILED(D3DXCreateTextureFromFile(pDevice3D, textureName, &pTexture)))
		return false;	// 画像読み込み失敗（ファイルがない可能性あり）
	// 画像読み込み成功
	return true;
}

bool Texture::LoadEX(IDirect3DDevice9 * pDevice3D, const char * textureName)
{
	D3DXCreateTextureFromFileEx(
		pDevice3D,
		textureName,              // ファイル名
		0,
		0,
		0,
		0,
		D3DFMT_A1R5G5B5,                // 色抜きを可能に
		D3DPOOL_MANAGED,
		D3DX_FILTER_LINEAR,
		D3DX_FILTER_LINEAR,
		D3DCOLOR_ARGB(255, 0, 0, 255),  // 青色を透過色とする
		NULL,
		NULL,
		&pTexture                   // テクスチャ名
	);

	return false;
}

void Texture::setpicture(int Width, int Height)
{
	width = Width;
	pictureWidth =(float) 1 / Width;
	pictureHeight = (float)1 / Height;
}
