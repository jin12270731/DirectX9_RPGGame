#include "sprite.h"

// コンストラクタ
Sprite::Sprite()
{
	pos.x = pos.y = 0.0f;
	width = 0;
	height = 0;
	divU = 1;
	divV = 1;
	g_pVB = NULL;
}
// デストラクタ
Sprite::~Sprite() {}

void Sprite::SetPos(float x, float y)
{
	pos.x = x;
	pos.y = y;
}
void Sprite::SetWidth(int Width, int Height)
{
	width = Width;
	height = Height;
}

void Sprite::DrawTitleBar(IDirect3DDevice9 * pDevice3D, IDirect3DTexture9 * pTexture)
{
	Vertex vtr[4] = {
			{500, 300, 0.0f, 1.0f ,1.0f ,0.0f},//右上
			{500, 500 ,0.0f,1.0f, 1.0f ,1.0f},//
			{270 ,300 , 0.0f, 1.0f, 0.0f ,0.0f},//左上
			{270, 500, 0.0f, 1.0f, 0.0f ,1.0f}//左下
	};
	// テクスチャセット
	pDevice3D->SetTexture(0, pTexture);
	// 頂点構造体宣言セット
	pDevice3D->SetFVF(SPRITE_FVF);
	// スプライト描画
	pDevice3D->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vtr, sizeof(Vertex));
}
void Sprite::DrawMap(IDirect3DDevice9 * pDevice3D, Texture* MapChip ,int o, int xPoint, int yPoint, int mapdate, player* player, int mapMoveX, int mapMoveY)
{
	int MapDate = mapdate - 100000 * (o + 1);
	int trueMapDate = comperMapChip(MapDate);
	Vertex vtr[4]{
		{HANI * (xPoint - mapMoveX) + HANI, HANI * (yPoint - mapMoveY), 0.0f,1.0f, (float)MapChip->pictureWidth * (trueMapDate % MapChip->width) + MapChip->pictureWidth, (float)MapChip->pictureHeight * ((int)(trueMapDate / MapChip->width))},
		{HANI * (xPoint - mapMoveX) + HANI,HANI * (yPoint - mapMoveY) + HANI, 0.0f, 1.0f, (float)MapChip->pictureWidth * (trueMapDate % MapChip->width) + MapChip->pictureWidth, (float)MapChip->pictureHeight * ((int)(trueMapDate / MapChip->width)) + MapChip->pictureHeight},
		{HANI * (xPoint - mapMoveX), HANI * (yPoint - mapMoveY), 0.0f, 1.0f, (float)MapChip->pictureWidth * (trueMapDate % MapChip->width), (float)MapChip->pictureHeight * ((int)(trueMapDate / MapChip->width))},
		{HANI * (xPoint - mapMoveX), HANI * (yPoint - mapMoveY) + HANI, 0.0f, 1.0f, (float)MapChip->pictureWidth * (trueMapDate % MapChip->width), (float)MapChip->pictureHeight * ((int)(trueMapDate / MapChip->width)) + MapChip->pictureHeight}
	};
	// テクスチャセット
	pDevice3D->SetTexture(0, MapChip->pTexture);
	// 頂点構造体宣言セット
	pDevice3D->SetFVF(SPRITE_FVF);
	// スプライト描画
	pDevice3D->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vtr, sizeof(Vertex));
}

void Sprite::DrawPlayer(IDirect3DDevice9 * pDevice3D, IDirect3DTexture9 * pTexture, player* player)
{
	Vertex vtr[4] = {
	{408, 288, 0.0f, 1.0f,(float)1 / divU * (player->playerNumber + 1), (float)1 / divV * player->placeNumber, D3DCOLOR_RGBA(255,255,255,128)},//右上
	{408, 312, 0.0f, 1.0f,(float)1 / divU * (player->playerNumber + 1), (float)1 / divV * (player->placeNumber + 1), D3DCOLOR_RGBA(255,255,255,128)},//右下
	{384, 288, 0.0f, 1.0f,(float)1 / divU * player->playerNumber, (float)1 / divV * player->placeNumber, D3DCOLOR_RGBA(255,255,255,128)},////左上
	{384, 312, 0.0f, 1.0f, (float)1 / divU * player->playerNumber, (float)1 / divV * (player->placeNumber + 1), D3DCOLOR_RGBA(255,255,255,128)}//左下
	};
	// テクスチャセット
	pDevice3D->SetTexture(0, pTexture);
	// 頂点構造体宣言セット
	pDevice3D->SetFVF(SPRITE_FVF);
	// スプライト描画
	pDevice3D->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vtr, sizeof(Vertex));
}

void Sprite::DrawEnemy(IDirect3DDevice9 * pDevice3D, IDirect3DTexture9 * pTexture, enemy* eneMy, int mapPointX, int mapPointY)
{
	if (eneMy->directionType == 0 || eneMy->directionType == 3)
	{
		eneMy->enemyPointY += eneMy->enemyMovePointY;
		Vertex vtr[4] = {
			{eneMy->enemyPointerX[0] - HANI * mapPointX, HANI * eneMy->enemyPointY - HANI * mapPointY, 0.0f, 1.0f , (float)1 / divU * (eneMy->enemyNumber + 1 + eneMy->directionType), (float)1 / divV * eneMy->enemyNumber},//右上
			{eneMy->enemyPointerX[1] - HANI * mapPointX, HANI * (eneMy->enemyPointY + 1) - HANI * mapPointY, 0.0f, 1.0f,(float)1 / divU * (eneMy->enemyNumber + 1 + eneMy->directionType), (float)1 / divV * (eneMy->enemyNumber + 1)},//
			{eneMy->enemyPointerX[2] - HANI * mapPointX, HANI * eneMy->enemyPointY - HANI * mapPointY, 0.0f, 1.0f, (float)1 / divU * (eneMy->enemyNumber + eneMy->directionType), (float)1 / divV * eneMy->enemyNumber},
			{eneMy->enemyPointerX[3] - HANI * mapPointX, HANI * (eneMy->enemyPointY + 1) - HANI * mapPointY, 0.0f, 1.0f, (float)1 / divU * (eneMy->enemyNumber + eneMy->directionType), (float)1 / divV * (eneMy->enemyNumber + 1)}
		};
		/*eneMy->enemyPointerY[0] = HANI * eneMy->enemyPointY ;
		eneMy->enemyPointerY[1] = HANI * (eneMy->enemyPointY + 1);
		eneMy->enemyPointerY[2] = HANI * eneMy->enemyPointY;
		eneMy->enemyPointerY[3] = HANI * (eneMy->enemyPointY + 1);*/
		// テクスチャセット
		pDevice3D->SetTexture(0, pTexture);
		// 頂点構造体宣言セット
		pDevice3D->SetFVF(SPRITE_FVF);
		// スプライト描画
		pDevice3D->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vtr, sizeof(Vertex));
	}
	else if (eneMy->directionType == 1 || eneMy->directionType == 4)
	{
		eneMy->enemyPointY += eneMy->enemyMovePointY;
		Vertex vtr[4] = {
			{eneMy->enemyPointerX[0] - HANI * mapPointX, HANI * eneMy->enemyPointY - HANI * mapPointY, 0.0f, 1.0f , (float)1 / divU * (eneMy->enemyNumber + 1 + eneMy->directionType), (float)1 / divV * eneMy->enemyNumber},//右上
			{eneMy->enemyPointerX[1] - HANI * mapPointX, HANI * (eneMy->enemyPointY + 1) - HANI * mapPointY, 0.0f, 1.0f,(float)1 / divU * (eneMy->enemyNumber + 1 + eneMy->directionType), (float)1 / divV * (eneMy->enemyNumber + 1)},//
			{eneMy->enemyPointerX[2] - HANI * mapPointX, HANI * eneMy->enemyPointY - HANI * mapPointY, 0.0f, 1.0f, (float)1 / divU * (eneMy->enemyNumber + eneMy->directionType), (float)1 / divV * eneMy->enemyNumber},
			{eneMy->enemyPointerX[3] - HANI * mapPointX, HANI * (eneMy->enemyPointY + 1) - HANI * mapPointY, 0.0f, 1.0f, (float)1 / divU * (eneMy->enemyNumber + eneMy->directionType), (float)1 / divV * (eneMy->enemyNumber + 1)}
		};
		/*eneMy->enemyPointerY[0] = HANI * eneMy->enemyPointY;
		eneMy->enemyPointerY[1] = HANI * (eneMy->enemyPointY + 1);
		eneMy->enemyPointerY[2] = HANI * eneMy->enemyPointY;
		eneMy->enemyPointerY[3] = HANI * (eneMy->enemyPointY + 1);*/
		// テクスチャセット
		pDevice3D->SetTexture(0, pTexture);
		// 頂点構造体宣言セット
		pDevice3D->SetFVF(SPRITE_FVF);
		// スプライト描画
		pDevice3D->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vtr, sizeof(Vertex));
	}
	else if (eneMy->directionType == 2 || eneMy->directionType == 5)
	{
		eneMy->enemyPointX += eneMy->enemyMovePointX;
		Vertex vtr[4] = {
			{HANI *eneMy->enemyPointX + HANI - HANI * mapPointX, eneMy->enemyPointerY[0] - HANI * mapPointY, 0.0f, 1.0f ,(eneMy->enemyFlag ? (float)1 / divU * (eneMy->enemyNumber + eneMy->directionType) : (float)1 / divU * (eneMy->enemyNumber + 1 + eneMy->directionType)), (float)1 / divV * eneMy->enemyNumber},//右上
			{HANI * eneMy->enemyPointX + HANI - HANI * mapPointX, eneMy->enemyPointerY[1] - HANI * mapPointY, 0.0f,1.0f,(eneMy->enemyFlag ? (float)1 / divU * (eneMy->enemyNumber + eneMy->directionType) : (float)1 / divU * (eneMy->enemyNumber + 1 + eneMy->directionType)), (float)1 / divV * (eneMy->enemyNumber + 1)},//
			{HANI *eneMy->enemyPointX - HANI * mapPointX, eneMy->enemyPointerY[2] - HANI * mapPointY, 0.0f, 1.0f, (eneMy->enemyFlag ? (float)1 / divU * (eneMy->enemyNumber + 1 + eneMy->directionType) : (float)1 / divU * (eneMy->enemyNumber + eneMy->directionType)), (float)1 / divV * eneMy->enemyNumber},
			{HANI *eneMy->enemyPointX - HANI * mapPointX, eneMy->enemyPointerY[3] - HANI * mapPointY, 0.0f, 1.0f, (eneMy->enemyFlag ? (float)1 / divU * (eneMy->enemyNumber + 1 + eneMy->directionType) : (float)1 / divU * (eneMy->enemyNumber + eneMy->directionType)), (float)1 / divV * (eneMy->enemyNumber + 1)}
		};
		/*eneMy->enemyPointerX[0] = HANI * eneMy->enemyPointX + HANI;
		eneMy->enemyPointerX[1] = HANI * eneMy->enemyPointX + HANI;
		eneMy->enemyPointerX[2] = HANI * eneMy->enemyPointX;
		eneMy->enemyPointerX[3] = HANI * eneMy->enemyPointX;*/
		// テクスチャセット
		pDevice3D->SetTexture(0, pTexture);
		// 頂点構造体宣言セット
		pDevice3D->SetFVF(SPRITE_FVF);
		// スプライト描画
		pDevice3D->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vtr, sizeof(Vertex));
	}
}
void Sprite::DrawCombatEnemy(IDirect3DDevice9 * pDevice3D, IDirect3DTexture9 * pTexture, enemy* eneMy, int enemysuu, int i)
{
	int amari = enemysuu % 2;
	int syokiti = 370 - 25 * amari - 50 * (enemysuu / 2);
	Vertex vtr[4] =
	{
			{syokiti + 70 * (i + 1), 120, 0.0f, 1.0f , (float)1 / divU * (1 + eneMy->directionType), (float)1 / divV * 0},//右上
			{syokiti + 70 * (i + 1), 170, 0.0f, 1.0f,(float)1 / divU * (1 + eneMy->directionType), (float)1 / divV * 1},//
			{syokiti + 70 * i, 120, 0.0f, 1.0f, (float)1 / divU * eneMy->directionType, (float)1 / divV * 0},
			{syokiti + 70 * i, 170, 0.0f, 1.0f, (float)1 / divU * eneMy->directionType, (float)1 / divV * 1}
	};
	// テクスチャセット
	pDevice3D->SetTexture(0, pTexture);
	// 頂点構造体宣言セット
	pDevice3D->SetFVF(SPRITE_FVF);
	// スプライト描画
	pDevice3D->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vtr, sizeof(Vertex));
}
void Sprite::DrawCombatEnemyState(IDirect3DDevice9 * pDevice3D, IDirect3DTexture9 * pTexture, int enemysuu, int i)
{
	int amari = enemysuu % 2;
	int syokiti = 370 - 25 * amari - 50 * (enemysuu / 2);
	syokiti = syokiti + 40 * i;
	Vertex vtr[4] = {
	{syokiti + 25 * (i + 1) + i * 10, 180, 0.0f, 1.0f, 1.0f ,0.0f},//右上
	{syokiti + 25 * (i + 1) + i * 10, 230, 0.0f, 1.0f, 1.0f ,1.0f},//右下
	{syokiti + 25 * i + i * 10, 180, 0.0f, 1.0f, 0.0f, 0.0f},////左上
	{syokiti + 25 * i + i * 10, 230, 0.0f, 1.0f, 0.0f, 1.0f}//左下
	};
	// テクスチャセット
	pDevice3D->SetTexture(0, pTexture);
	// 頂点構造体宣言セット
	pDevice3D->SetFVF(SPRITE_FVF);
	// スプライト描画
	pDevice3D->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vtr, sizeof(Vertex));
}
void Sprite::DrawCombatEnemyBar(IDirect3DDevice9 * pDevice3D, IDirect3DTexture9 * pTexture, int enemysuu, int i, int j, float k)
{
	int amari = enemysuu % 2;
	int syokiti = 370 - 25 * amari - 50 * (enemysuu / 2);
	float kei = (float)40 * (1 - k);
	syokiti += 25;
	syokiti = syokiti + 25 * i;
	Vertex vtr[4] = {
	{(syokiti + 40 * (i + 1) + i * 10) - kei, 180 + 20 * j, 0.0f, 1.0f, 1.0f ,0.0f},//右上
	{(syokiti + 40 * (i + 1) + i * 10) - kei, 188 + 20 * j, 0.0f, 1.0f, 1.0f ,1.0f},//右下
	{syokiti + 40 * i + i * 10, 180 + 20 * j, 0.0f, 1.0f, 0.0f, 0.0f},////左上
	{syokiti + 40 * i + i * 10, 188 + 20 * j, 0.0f, 1.0f, 0.0f, 1.0f}//左下
	};
	// テクスチャセット
	pDevice3D->SetTexture(0, pTexture);
	// 頂点構造体宣言セット
	pDevice3D->SetFVF(SPRITE_FVF);
	// スプライト描画
	pDevice3D->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vtr, sizeof(Vertex));
}
void Sprite::DrawCombatMap(IDirect3DDevice9 * pDevice3D, IDirect3DTexture9 * pTexture)
{
	Vertex vtr[4] = {
			{WINDOW_WIDTH, 0, 0.0f, 1.0f ,1.0f ,0.0f},//右上
			{WINDOW_WIDTH, WINDOW_HEIGHT ,0.0f,1.0f, 1.0f ,1.0f},//
			{0 ,0 , 0.0f, 1.0f, 0.0f ,0.0f},
			{0, WINDOW_HEIGHT, 0.0f, 1.0f, 0.0f ,1.0f}
	};
	// テクスチャセット
	pDevice3D->SetTexture(0, pTexture);
	// 頂点構造体宣言セット
	pDevice3D->SetFVF(SPRITE_FVF);
	// スプライト描画
	pDevice3D->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vtr, sizeof(Vertex));
}
void Sprite::DrawDeath(IDirect3DDevice9 * pDevice3D, IDirect3DTexture9 * pTexture, playerBase * player, int players, int i)
{
	int amari = players % 2;
	int syokiti = 395 - 50 * amari - 100 * (players / 2);
	Vertex vtr[4] = {
	{syokiti + 100 * (i + 1) + i * 10, WINDOW_HEIGHT - FACE_SIZE - 43, 0.0f, 1.0f, 1.0f ,0.0f},//右上
	{syokiti + 100 * (i + 1) + i * 10, WINDOW_HEIGHT - 43, 0.0f, 1.0f, 1.0f ,1.0f},//右下
	{syokiti + 100 * i + i * 10, WINDOW_HEIGHT - FACE_SIZE - 43, 0.0f, 1.0f, 0.0f, 0.0f},////左上
	{syokiti + 100 * i + i * 10, WINDOW_HEIGHT - 43, 0.0f, 1.0f,  0.0f ,1.0f}//左下
	};
	// テクスチャセット
	pDevice3D->SetTexture(0, pTexture);
	// 頂点構造体宣言セット
	pDevice3D->SetFVF(SPRITE_FVF);
	// スプライト描画
	pDevice3D->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vtr, sizeof(Vertex));
}
void Sprite::DrawPlayerFace(IDirect3DDevice9 * pDevice3D, IDirect3DTexture9 * pTexture, playerBase * player, int players, int i)
{
	int amari = players % 2;
	int syokiti = 395 - 50 * amari - 100 * (players / 2);
	Vertex vtr[4] = {
	{syokiti + 100 * (i + 1) + i * 10, WINDOW_HEIGHT - FACE_SIZE - 43, 0.0f, 1.0f, (float)1 / divU * (player->playerFaceNumberU + 1), (float)1 / divV * player->playerFaceNumberV},//右上
	{syokiti + 100 * (i + 1) + i * 10, WINDOW_HEIGHT - 43, 0.0f, 1.0f, (float)1 / divU * (player->playerFaceNumberU + 1), (float)1 / divV * (player->playerFaceNumberV + 1)},//右下
	{syokiti + 100 * i + i * 10, WINDOW_HEIGHT - FACE_SIZE - 43, 0.0f, 1.0f, (float)1 / divU * player->playerFaceNumberU, (float)1 / divV * player->playerFaceNumberV},////左上
	{syokiti + 100 * i + i * 10, WINDOW_HEIGHT - 43, 0.0f, 1.0f, (float)1 / divU * player->playerFaceNumberU, (float)1 / divV * (player->playerFaceNumberV + 1)}//左下
	};
	// テクスチャセット
	pDevice3D->SetTexture(0, pTexture);
	// 頂点構造体宣言セット
	pDevice3D->SetFVF(SPRITE_FVF);
	// スプライト描画
	pDevice3D->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vtr, sizeof(Vertex));
}
void Sprite::DrawResultFace(IDirect3DDevice9 * pDevice3D, IDirect3DTexture9 * pTexture, playerBase * player, int players, int i)
{
	int amari = players % 2;
	int syokiti = 350 - 50 * amari - 100 * (players / 2);
	Vertex vtr[4] = {
	{syokiti + 100 * (i + 1) + i * 30, WINDOW_HEIGHT - FACE_SIZE - 100, 0.0f, 1.0f, (float)1 / divU * (player->playerFaceNumberU + 1), (float)1 / divV * player->playerFaceNumberV},//右上
	{syokiti + 100 * (i + 1) + i * 30, WINDOW_HEIGHT - 100, 0.0f, 1.0f, (float)1 / divU * (player->playerFaceNumberU + 1), (float)1 / divV * (player->playerFaceNumberV + 1)},//右下
	{syokiti + 100 * i + i * 30, WINDOW_HEIGHT - FACE_SIZE - 100, 0.0f, 1.0f, (float)1 / divU * player->playerFaceNumberU, (float)1 / divV * player->playerFaceNumberV},////左上
	{syokiti + 100 * i + i * 30, WINDOW_HEIGHT - 100, 0.0f, 1.0f, (float)1 / divU * player->playerFaceNumberU, (float)1 / divV * (player->playerFaceNumberV + 1)}//左下
	};
	// テクスチャセット
	pDevice3D->SetTexture(0, pTexture);
	// 頂点構造体宣言セット
	pDevice3D->SetFVF(SPRITE_FVF);
	// スプライト描画
	pDevice3D->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vtr, sizeof(Vertex));
}
void Sprite::DrawCombatPlayerState(IDirect3DDevice9 * pDevice3D, IDirect3DTexture9 * pTexture, int players, int i)
{
	int amari = players % 2;
	int syokiti = 395 - 50 * amari - 100 * (players / 2);
	syokiti = syokiti + 75 * i;
	Vertex vtr[4] = {
	{syokiti + 25 * (i + 1) + i * 10, WINDOW_HEIGHT - 40, 0.0f, 1.0f, 1.0f ,0.0f},//右上
	{syokiti + 25 * (i + 1) + i * 10, WINDOW_HEIGHT, 0.0f, 1.0f, 1.0f ,1.0f},//右下
	{syokiti + 25 * i + i * 10, WINDOW_HEIGHT - 40, 0.0f, 1.0f, 0.0f, 0.0f},////左上
	{syokiti + 25 * i + i * 10, WINDOW_HEIGHT, 0.0f, 1.0f, 0.0f, 1.0f}//左下
	};
	// テクスチャセット
	pDevice3D->SetTexture(0, pTexture);
	// 頂点構造体宣言セット
	pDevice3D->SetFVF(SPRITE_FVF);
	// スプライト描画
	pDevice3D->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vtr, sizeof(Vertex));
}
void Sprite::DrawCombatPlayerBar(IDirect3DDevice9 * pDevice3D, IDirect3DTexture9 * pTexture, int players, int i, int j, float k)
{
	int amari = players % 2;
	int syokiti = 395 - 50 * amari - 100 * (players / 2);
	float kei = (float)75 * (1 - k);
	syokiti += 25;
	syokiti = syokiti + 25 * i;
	Vertex vtr[4] = {
	{(syokiti + 75 * (i + 1) + i * 10) - kei, WINDOW_HEIGHT - 40 + 15 * j, 0.0f, 1.0f, 1.0f ,0.0f},//右上
	{(syokiti + 75 * (i + 1) + i * 10) - kei, WINDOW_HEIGHT - 32 + 15 * j, 0.0f, 1.0f, 1.0f ,1.0f},//右下
	{syokiti + 75 * i + i * 10, WINDOW_HEIGHT - 40 + 15 * j, 0.0f, 1.0f, 0.0f, 0.0f},////左上
	{syokiti + 75 * i + i * 10, WINDOW_HEIGHT - 32 + 15 * j, 0.0f, 1.0f, 0.0f, 1.0f}//左下
	};
	// テクスチャセット
	pDevice3D->SetTexture(0, pTexture);
	// 頂点構造体宣言セット
	pDevice3D->SetFVF(SPRITE_FVF);
	// スプライト描画
	pDevice3D->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vtr, sizeof(Vertex));
}
void Sprite::DrawCombatText(IDirect3DDevice9 * pDevice3D, IDirect3DTexture9 * pTexture)
{
	Vertex vtr[4] = {
	{500, 300, 0.0f, 1.0f, 1.0f ,0.0f},//右上
	{500, 450, 0.0f, 1.0f, 1.0f ,1.0f},//右下
	{300, 300, 0.0f, 1.0f, 0.0f, 0.0f},////左上
	{300, 450, 0.0f, 1.0f, 0.0f, 1.0f}//左下
	};
	// テクスチャセット
	pDevice3D->SetTexture(0, pTexture);
	// 頂点構造体宣言セット
	pDevice3D->SetFVF(SPRITE_FVF);
	// スプライト描画
	pDevice3D->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vtr, sizeof(Vertex));
}
void Sprite::DrawCombatLog(IDirect3DDevice9 * pDevice3D, IDirect3DTexture9 * pTexture)
{
	Vertex vtr[4] = {
	{300, 250, 0.0f, 1.0f, 1.0f ,0.0f},//右上
	{300, 350, 0.0f, 1.0f, 1.0f ,1.0f},//右下
	{0, 250, 0.0f, 1.0f, 0.0f, 0.0f},////左上
	{0, 350, 0.0f, 1.0f, 0.0f, 1.0f}//左下
	};
	// テクスチャセット
	pDevice3D->SetTexture(0, pTexture);
	// 頂点構造体宣言セット
	pDevice3D->SetFVF(SPRITE_FVF);
	// スプライト描画
	pDevice3D->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vtr, sizeof(Vertex));
}
void Sprite::SetDivide(unsigned int DivU, unsigned int DivV)
{
	if (DivU <= 0 || DivV <= 0)
		return;

	divU = DivU;
	divV = DivV;
}
bool Sprite::playerComperMap(int mapPointX, int mapPointY,int mapdate)
{
	mapPointY += 12;
	mapPointX += 16;
	int o = mapdate / 100000;
	o -= 1;
	int MapDate = mapdate - 100000 * (o + 1);
	int trueMapDate = comperMapChip(MapDate);
	if (mapPointY < 0 || mapPointX < 0 || mapPointY >= 100 || mapPointX >= 100)
		return true;
	else if (o == 0)
	{
		for (int i = 0; i < STOP_MAP[o]; i++)
			if (trueMapDate == stopMap1[i])
				return true;
	}
	else if (o == 1)
	{
		for (int i = 0; i < STOP_MAP[o]; i++)
		{
			if (trueMapDate == stopMap2[i])
			{
				return true;
			}
		}
	}
	else if (o == 2)
	{
		for (int i = 0; i < STOP_MAP[o]; i++)
		{
			if (trueMapDate == stopMap3[i])
			{
				return true;
			}
		}
	}
	else if (o == 3)
	{
		for (int i = 0; i < STOP_MAP[o]; i++)
		{
			if (trueMapDate == stopMap4[i])
			{         
				return true;
			}
		
		}
	}
	else if (o == 4)
	{
		for (int i = 0; i < STOP_MAP[o]; i++)
			if (trueMapDate == stopMap5[i])
				return true;
	}
	else if (o == 5)
	{
		for (int i = 0; i < STOP_MAP[o]; i++)
		{
			if (trueMapDate == stopMap6[i])
			{
				return true;
			}
		}
	}
	else if (o == 6)
	{
		for (int i = 0; i < STOP_MAP[o]; i++)
		{
			if (trueMapDate == stopMap7[i])
			{
				return true;
			}
		}
	}
	return false;
}
int Sprite::comperMapChip(int Map)
{
	if (Map >= 0 && Map <= 127)
	{
		return Map;
	}
	else if (Map >= 128 && Map <= 607)
	{
		Map -= 128;
		return Map;
	}
	else if (Map >= 608 && Map <= 735)
	{
		Map -= 608;
		return Map;
	}
	else if (Map >= 736 && Map <= 991)
	{
		Map -= 736;
		return Map;
	}
	else if (Map >= 992 && Map <= 1471)
	{
		Map -= 992;
		return Map;
	}
	else if (Map >= 1472 && Map <= 1591)
	{
		Map -= 1472;
		return Map;
	}
	else if (Map >= 1592 && Map <= 1736)
	{
		Map -= 1592;
		return Map;
	}
	else if (Map >= 1737 && Map <= 1991)
	{
		Map -= 1736;
		return Map;
	}
	else if (Map >= 1992 && Map <= 2319)
	{
		Map -= 1992;
		return Map;
	}
	else if (Map >= 2320 && Map <= 2679)
	{
		Map -= 2320;
		return Map;
	}
	return 0;
}

