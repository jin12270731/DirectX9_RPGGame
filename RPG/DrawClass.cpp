#include "DrawClass.h"



DrawClass::DrawClass()
{
	
}


DrawClass::~DrawClass()
{
}

bool DrawClass::crate3D(HWND hWnd, HINSTANCE hInstance)
{
	
	if (false == direct3d.Create(hWnd, WINDOW_WIDTH, WINDOW_HEIGHT, hInstance)) 
	{
		MessageBox(NULL, "読み込み失敗", "", MB_OK);
		return false;
	}
	if (false == directmouse.create(hWnd, hInstance)) 
	{
		MessageBox(NULL, "読み込み失敗", "", MB_OK);
		return false;
	}
	return true;
}
