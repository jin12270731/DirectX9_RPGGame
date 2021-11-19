#pragma once

// Direct3Dの各種ヘッダーのインクルードが必要なため
// すでにまとめてあるヘッダーをインクルードする
#include "direct.h"
#include "enemy.h"
#include "player.h"
#include "texture.h"
#include "playerBase.h"

// ウィンドウ幅、高さはディスプレイに依存する。普通は4:3
const int WINDOW_WIDTH = 790;
const int WINDOW_HEIGHT = 600;
const int PICTURE_SIZE = 32;
const float PICTURE_WIDTH = 0.125;
const float PICTURE_HEIGHT = 0.0625;
const int HANI = 24;
const int FACE_SIZE = 100;
const int COMBAT_ENEMY_SIZE = 50;
const int MAP_WIDTH = 100;//33
const int MAP_HEIGHT = 100;//25
const int STOP_MAP1 = 60;
const int STOP_MAP2 = 50;
const int STOP_MAP3 = 30;
const int STOP_MAP4 = 40;
const int STOP_MAP5 = 40;
const int STOP_MAP6 = 40;
const int STOP_MAP7 = 40;
const int STOP_MAP[7] = {60,50,30,40,40,40,40};
struct AnimationNum {
	unsigned int numU;
	unsigned int numV;
};

//マップデータ
static int mapChip1[MAP_HEIGHT]/*縦*/[MAP_WIDTH]/*横*/;
static int backmapChip1[MAP_HEIGHT]/*縦*/[MAP_WIDTH];
static int stopMap1[STOP_MAP1] =
{16, 17, 18, 32, 33, 34, 35, 36, 37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,62,63,67,68,69,70,
71,75,76,78,79,80,81,86,87,88,89,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111};

static int stopMap2[STOP_MAP2] =
{12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,138,139,140,168,169,170,108,110,111,113};

static int stopMap3[STOP_MAP3] = {

};
static int stopMap4[STOP_MAP4] = {
86,87,102,103,118,119,132,133,150,219,220,221,235,237,251,252,253
};
static int stopMap5[STOP_MAP5] = {
123,124,125,153,154,155,183,184,185,213,214,215,138,139,140,168,169,170,198,199,200,477,478
};
static int stopMap6[STOP_MAP6] = {

};
static int stopMap7[STOP_MAP7] = {

};
// スプライトクラス（2D板ポリゴン）
class Sprite
{
public:
	
	// 板ポリゴン頂点情報
	struct Vertex {
		float x, y, z;// 3次元座標
		float rhw;	// 2D変換済みフラグ
		float u, v;	// UV座標
		D3DCOLOR color;
	};
	// FVF（柔軟な頂点構造体宣言）フラグ
	static const DWORD SPRITE_FVF = D3DFVF_XYZRHW | D3DFVF_TEX1;
	static const DWORD D3DFVF_2DVERTEX = D3DFVF_XYZRHW | D3DFVF_TEX1 | D3DFVF_DIFFUSE;
	// スプライト位置
	D3DXVECTOR2 pos;

	IDirect3DVertexBuffer9* g_pVB;
	// スプライトサイズ
	int width;
	int height;
	// UVの分割数
	unsigned int divU;
	unsigned int divV;

	

	// コンストラクタ
	Sprite();
	// デストラクタ
	~Sprite();

	void SetPos(float x, float y);
	void SetWidth(int Width, int Height);
	void DrawTitleBar(IDirect3DDevice9* pDevice3D, IDirect3DTexture9* pTexture);
	void DrawMap(IDirect3DDevice9* pDevice3D, Texture* MapChip, int o , int xPoint, int yPoint, int mapdate, player* player, int mapMoveX = 0, int mapMoveY = 0);
	void DrawPlayer(IDirect3DDevice9* pDevice3D, IDirect3DTexture9* pTexture, player* player);
	void DrawEnemy(IDirect3DDevice9* pDevice3D, IDirect3DTexture9* pTexture, enemy* eneMy, int mapPointX, int mapPointY);
	void DrawCombatEnemy(IDirect3DDevice9* pDevice3D, IDirect3DTexture9* pTexture, enemy* eneMy, int enemysuu, int i);
	void DrawCombatEnemyState(IDirect3DDevice9 * pDevice3D, IDirect3DTexture9 * pTexture, int players, int i);
	void DrawCombatEnemyBar(IDirect3DDevice9 * pDevice3D, IDirect3DTexture9 * pTexture, int players, int i, int j, float k);
	void DrawCombatMap(IDirect3DDevice9* pDevice3D, IDirect3DTexture9* pTexture);
	void DrawDeath(IDirect3DDevice9 * pDevice3D, IDirect3DTexture9 * pTexture, playerBase * player, int players, int i);
	void DrawPlayerFace(IDirect3DDevice9 * pDevice3D, IDirect3DTexture9 * pTexture, playerBase * player, int players, int i);
	void DrawResultFace(IDirect3DDevice9 * pDevice3D, IDirect3DTexture9 * pTexture, playerBase * player, int players, int i);
	void DrawCombatText(IDirect3DDevice9 * pDevice3D, IDirect3DTexture9 * pTexture);
	void DrawCombatLog(IDirect3DDevice9 * pDevice3D, IDirect3DTexture9 * pTexture);
	void DrawCombatPlayerState(IDirect3DDevice9 * pDevice3D, IDirect3DTexture9 * pTexture, int players, int i);
	void DrawCombatPlayerBar(IDirect3DDevice9 * pDevice3D, IDirect3DTexture9 * pTexture, int players, int i, int j, float k);
	void SetDivide(unsigned int DivU, unsigned int DivV);
	bool playerComperMap(int mapPointX, int mapPointY, int mspdate);
	int comperMapChip(int map);
};