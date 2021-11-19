#pragma once
// Direct3Dの各種ヘッダーのインクルードが必要なため
// すでにまとめてあるヘッダーをインクルードする
#include "direct.h"



// テクスチャクラス
class Texture
{
public:
	IDirect3DTexture9* pTexture;
	float pictureWidth;
	float pictureHeight;
	int width;
	// コンストラクタ
	Texture();
	// デストラクタ
	~Texture();
	// 画像データ読み込み
	bool Load(IDirect3DDevice9* pDevice3D,const char* textureName);
	bool LoadEX(IDirect3DDevice9* pDevice3D, const char* textureName);
	void setpicture(int Width, int Height);
};