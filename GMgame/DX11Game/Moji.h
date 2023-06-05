#pragma once
#pragma comment(lib,"d2d1.lib")//---★追加---
#pragma comment(lib,"dwrite.lib")//---★追加---
#include <directxcolors.h>//---★追加---
#include <d2d1.h>//---★追加---
#include <dwrite.h>//---★追加---

class Moji
{
public:
	Moji();
	~Moji();

	HRESULT Init(IDXGISwapChain* pSwapChain);
	void Draw(const wchar_t* str, unsigned int str_length, int left, int top, int right, int bottom);
	void SetFont(const wchar_t* font_name, int size);
	void SetBrushColor(float r, float g, float b, float a);
private:
	//--------------------------★追加↓--------------------------
	ID2D1Factory* pD2DFactory;	//2D表示の初期化
	IDWriteFactory* pDWriteFactory;	//文字表示の初期化
	IDWriteTextFormat* pTextFormat;	//フォント情報
	ID2D1RenderTarget* pRT;	//Draw関係
	ID2D1SolidColorBrush* pSolidBrush;	//ブラシ情報
	IDXGISurface* pDXGISurface;	//描画を2Dへ変換
	int		Size;
	//--------------------------★追加↑--------------------------
};

