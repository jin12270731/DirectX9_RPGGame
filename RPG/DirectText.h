
#include <vector>
#include <Windows.h>
#include <tchar.h>
#include <string>
#include "direct.h" 

using namespace std;
// �e�L�X�g�`��N���X
class DirectXText
{
private:
	IDirect3DTexture9*  m_pTexture;
	ID3DXFont*			pFont;	// Direct3D�t�H���g
	RECT				Rect;	// �`��̈�
	std::vector<TCHAR>	Buf;	// ������o�b�t�@
public:

	// �R���X�g���N�^
	DirectXText();
	// �f�X�g���N�^
	virtual ~DirectXText();
	// DirectX�t�H���g�쐬
	bool Create(IDirect3DDevice9* pD3DDevice, int FontHeight = 16);
	void TextTexture(IDirect3DDevice9 * pDevice3D, HDC hdc, const string & text, int w, int h);
	// ������̕`��
	void Draw(DWORD Color, int x, int y, const TCHAR* Str, ...);
};