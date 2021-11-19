#include "DirectText.h"

// コンストラクタ
DirectXText::DirectXText()
{
	pFont = NULL;
	m_pTexture = NULL;
}

// デストラクタ
DirectXText::~DirectXText()
{
	if (pFont != NULL)
		pFont->Release();
}

// DirectXフォント作成
bool DirectXText::Create(IDirect3DDevice9* pD3DDevice, int FontHeight)
{
	HFONT	hFont = NULL;	// フォントハンドル
	LOGFONT	LogFont = {};	// フォント構造体

	// デフォルトフォント取得
	hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	 //デフォルトフォント情報取得
	GetObject(hFont, sizeof(LOGFONT), &LogFont);
	 //DirectXフォント作成
	if (FAILED(D3DXCreateFont(pD3DDevice,
		FontHeight,					// 文字高さ
		LogFont.lfWidth,			// 文字幅
		LogFont.lfWeight,			// フォントの太さ
		0,							// ミップマップレベル
		LogFont.lfItalic,			// イタリックフラグ
		LogFont.lfCharSet,			// 文字セット
		LogFont.lfOutPrecision,		// 実際のフォントと目的のフォントの特性の一致方法
		LogFont.lfQuality,			// クォリティ
		LogFont.lfPitchAndFamily,	// ピッチとファミリインデックス
		LogFont.lfFaceName,			// フォント名
		&pFont)))
	{
		return false;
	}

	return true;
}

void DirectXText::TextTexture(IDirect3DDevice9 * pDevice3D, HDC hdc,const string& text, int w, int h)
{
	HRESULT hr;
	hr = D3DXCreateTexture(pDevice3D, w, h,
		0, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pTexture);
	if (FAILED(hr)) {
		return;
	}

	// フォントを作成
	LOGFONT lf;
	HFONT font = CreateFontIndirect(&lf);
	SelectObject(hdc, font);

	// カレント座標
	int x = 0;
	int y = 0;

	D3DLOCKED_RECT lock;

	TEXTMETRIC tm;
	GetTextMetrics(hdc, &tm);

	if (FAILED(m_pTexture->LockRect(0, &lock, NULL, 0))) {
		return;
	}
	for (int i = 0; i < text.length(); i += 2) {
		LPBYTE pData; // データ
		GLYPHMETRICS gm; // 情報
		UINT uChar = (text[i] << 8) | (text[i + 1] & 0xff); // 文字
		uChar &= 0x0000ffff;
		MAT2 mat = { {0,1}, {0,0}, {0,0}, {0,1} }; // 行列
		DWORD size;
		// まずは必要なバッファサイズを取得
		size = GetGlyphOutline(hdc, uChar, GGO_GRAY8_BITMAP, &gm, 0, NULL, &mat);

		// 右や下にはみ出ないかチェック
		if (x + gm.gmCellIncX >= w) {
			// 右にはみ出るので次の行へ
			y += tm.tmHeight;
			x = 0;
			if (y + tm.tmHeight >= h) {
				// 下にはみ出るのでこれ以上無理
				break;
			}
		}

		// バッファを確保
		pData = (LPBYTE)GlobalAlloc(GPTR, size);

		// 実際に取得する
		GetGlyphOutline(hdc, uChar, GGO_GRAY8_BITMAP, &gm, size, pData, &mat);

		// ピッチ
		int pitch = (gm.gmBlackBoxX + 3) & 0xfffc;

		BYTE src;
		LPDWORD pDest;
		for (int v = 0; v < gm.gmBlackBoxY; v++) {
			for (int u = 0; u < gm.gmBlackBoxX; u++) {
				// α値の取得
				src = pData[u + v * pitch];
				src = src * 255 / 65;
				// 描画対象地点
				int xx = x + gm.gmptGlyphOrigin.x + u;
				int yy = y + tm.tmAscent - gm.gmptGlyphOrigin.y + v;
				pDest = (LPDWORD)lock.pBits + xx + yy * (lock.Pitch / 4);
				*pDest = src << 24 | 0x00ffffff;
			}
		}

		x += gm.gmCellIncX;

		// バッファを開放
		GlobalFree(pData);
	}

	m_pTexture->UnlockRect(0);
	DeleteObject(font);
	DeleteDC(hdc);
}

// 文字列の描画
void DirectXText::Draw(DWORD Color, int x, int y, const TCHAR* Str, ...)
{
	va_list args;
	va_start(args, Str);					// 可変引数の最初の要素を格納する
	int len = _vsctprintf(Str, args) + 1;	// 文字数カウント、ヌル文字分加える
	if (Buf.size() < (UINT)len)
		Buf.resize(len);						// 文字サイズ分動的にメモリ確保
	_vstprintf_s(&Buf[0], Buf.size(), Str, args);			// 文字を整形する

	SetRect(&Rect, 0, 0, 0, 0);
	// 描画領域サイズ取得
	pFont->DrawText(NULL, &Buf[0], -1, &Rect, DT_CENTER | DT_CALCRECT, Color);
	// 本描画
	Rect.left += x;
	Rect.right += x;
	Rect.top += y;
	Rect.bottom += y;
	pFont->DrawText(NULL, &Buf[0], -1, &Rect, DT_CENTER, Color);
}
