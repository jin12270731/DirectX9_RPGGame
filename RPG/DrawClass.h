#pragma once
#include "direct.h"
#include "texture.h"
#include "sprite.h"
#include "directsound.h"
#include "wave.h"
#include "soundbuffer.h"
#include "DirectText.h"
#include "enemy.h"
#include "player.h"
#include "directMouse.h"
class DrawClass
{
public:

	Direct3D direct3d;
	directMouse directmouse;

	DrawClass();
	~DrawClass();
	bool crate3D(HWND hWnd, HINSTANCE hInstance);
};

