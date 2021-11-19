
#include <vector>
#include <Windows.h>
#include <tchar.h>
#include <string>
#include "direct.h" 

using namespace std;
// テキスト描画クラス
class DirectXText
{
private:
	IDirect3DTexture9*  m_pTexture;
	ID3DXFont*			pFont;	// Direct3Dフォント
	RECT				Rect;	// 描画領域
	std::vector<TCHAR>	Buf;	// 文字列バッファ
public:

	// コンストラクタ
	DirectXText();
	// デストラクタ
	virtual ~DirectXText();
	// DirectXフォント作成
	bool Create(IDirect3DDevice9* pD3DDevice, int FontHeight = 16);
	void TextTexture(IDirect3DDevice9 * pDevice3D, HDC hdc, const string & text, int w, int h);
	// 文字列の描画
	void Draw(DWORD Color, int x, int y, const TCHAR* Str, ...);
};