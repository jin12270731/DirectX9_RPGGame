#include "DirectText.h"

// �R���X�g���N�^
DirectXText::DirectXText()
{
	pFont = NULL;
	m_pTexture = NULL;
}

// �f�X�g���N�^
DirectXText::~DirectXText()
{
	if (pFont != NULL)
		pFont->Release();
}

// DirectX�t�H���g�쐬
bool DirectXText::Create(IDirect3DDevice9* pD3DDevice, int FontHeight)
{
	HFONT	hFont = NULL;	// �t�H���g�n���h��
	LOGFONT	LogFont = {};	// �t�H���g�\����

	// �f�t�H���g�t�H���g�擾
	hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	 //�f�t�H���g�t�H���g���擾
	GetObject(hFont, sizeof(LOGFONT), &LogFont);
	 //DirectX�t�H���g�쐬
	if (FAILED(D3DXCreateFont(pD3DDevice,
		FontHeight,					// ��������
		LogFont.lfWidth,			// ������
		LogFont.lfWeight,			// �t�H���g�̑���
		0,							// �~�b�v�}�b�v���x��
		LogFont.lfItalic,			// �C�^���b�N�t���O
		LogFont.lfCharSet,			// �����Z�b�g
		LogFont.lfOutPrecision,		// ���ۂ̃t�H���g�ƖړI�̃t�H���g�̓����̈�v���@
		LogFont.lfQuality,			// �N�H���e�B
		LogFont.lfPitchAndFamily,	// �s�b�`�ƃt�@�~���C���f�b�N�X
		LogFont.lfFaceName,			// �t�H���g��
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

	// �t�H���g���쐬
	LOGFONT lf;
	HFONT font = CreateFontIndirect(&lf);
	SelectObject(hdc, font);

	// �J�����g���W
	int x = 0;
	int y = 0;

	D3DLOCKED_RECT lock;

	TEXTMETRIC tm;
	GetTextMetrics(hdc, &tm);

	if (FAILED(m_pTexture->LockRect(0, &lock, NULL, 0))) {
		return;
	}
	for (int i = 0; i < text.length(); i += 2) {
		LPBYTE pData; // �f�[�^
		GLYPHMETRICS gm; // ���
		UINT uChar = (text[i] << 8) | (text[i + 1] & 0xff); // ����
		uChar &= 0x0000ffff;
		MAT2 mat = { {0,1}, {0,0}, {0,0}, {0,1} }; // �s��
		DWORD size;
		// �܂��͕K�v�ȃo�b�t�@�T�C�Y���擾
		size = GetGlyphOutline(hdc, uChar, GGO_GRAY8_BITMAP, &gm, 0, NULL, &mat);

		// �E�≺�ɂ͂ݏo�Ȃ����`�F�b�N
		if (x + gm.gmCellIncX >= w) {
			// �E�ɂ͂ݏo��̂Ŏ��̍s��
			y += tm.tmHeight;
			x = 0;
			if (y + tm.tmHeight >= h) {
				// ���ɂ͂ݏo��̂ł���ȏ㖳��
				break;
			}
		}

		// �o�b�t�@���m��
		pData = (LPBYTE)GlobalAlloc(GPTR, size);

		// ���ۂɎ擾����
		GetGlyphOutline(hdc, uChar, GGO_GRAY8_BITMAP, &gm, size, pData, &mat);

		// �s�b�`
		int pitch = (gm.gmBlackBoxX + 3) & 0xfffc;

		BYTE src;
		LPDWORD pDest;
		for (int v = 0; v < gm.gmBlackBoxY; v++) {
			for (int u = 0; u < gm.gmBlackBoxX; u++) {
				// ���l�̎擾
				src = pData[u + v * pitch];
				src = src * 255 / 65;
				// �`��Ώےn�_
				int xx = x + gm.gmptGlyphOrigin.x + u;
				int yy = y + tm.tmAscent - gm.gmptGlyphOrigin.y + v;
				pDest = (LPDWORD)lock.pBits + xx + yy * (lock.Pitch / 4);
				*pDest = src << 24 | 0x00ffffff;
			}
		}

		x += gm.gmCellIncX;

		// �o�b�t�@���J��
		GlobalFree(pData);
	}

	m_pTexture->UnlockRect(0);
	DeleteObject(font);
	DeleteDC(hdc);
}

// ������̕`��
void DirectXText::Draw(DWORD Color, int x, int y, const TCHAR* Str, ...)
{
	va_list args;
	va_start(args, Str);					// �ψ����̍ŏ��̗v�f���i�[����
	int len = _vsctprintf(Str, args) + 1;	// �������J�E���g�A�k��������������
	if (Buf.size() < (UINT)len)
		Buf.resize(len);						// �����T�C�Y�����I�Ƀ������m��
	_vstprintf_s(&Buf[0], Buf.size(), Str, args);			// �����𐮌`����

	SetRect(&Rect, 0, 0, 0, 0);
	// �`��̈�T�C�Y�擾
	pFont->DrawText(NULL, &Buf[0], -1, &Rect, DT_CENTER | DT_CALCRECT, Color);
	// �{�`��
	Rect.left += x;
	Rect.right += x;
	Rect.top += y;
	Rect.bottom += y;
	pFont->DrawText(NULL, &Buf[0], -1, &Rect, DT_CENTER, Color);
}
