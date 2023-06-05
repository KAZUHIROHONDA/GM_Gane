#include "Moji.h"
#include "main.h"
#include <cstdio>
#include <tchar.h>

#pragma warning(disable:4996)

Moji::Moji()
{
	//--------------------------★追加↓--------------------------
	pD2DFactory = nullptr;
	pDWriteFactory = nullptr;
	pTextFormat = nullptr;
	pRT = nullptr;
	pSolidBrush = nullptr;
	pDXGISurface = nullptr;
	Size = 30;
	//--------------------------★追加↓--------------------------
}

Moji::~Moji()
{	//--------------------------★追加↓--------------------------
	if (pDXGISurface) pDXGISurface->Release();
	if (pSolidBrush) pSolidBrush->Release();
	if (pRT) pRT->Release();
	if (pTextFormat) pTextFormat->Release();
	if (pDWriteFactory) pDWriteFactory->Release();
	if (pD2DFactory) pD2DFactory->Release();
	//--------------------------★追加↑--------------------------

}

HRESULT Moji::Init(IDXGISwapChain * pSwapChain)
{
	HRESULT hr = S_OK;

	//--------------------------★追加↓--------------------------
	// Direct2D,DirectWriteの初期化
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
	if (FAILED(hr))
		return hr;

	hr = pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pDXGISurface));
	if (FAILED(hr))
		return hr;

	FLOAT dpiX;
	FLOAT dpiY;
	pD2DFactory->GetDesktopDpi(&dpiX, &dpiY);//<-今は推奨されない記述(ウィンドウのDpi取得関数)
	//今はDisplayInformationクラスに変更されているが、取得方法が分からない

	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED), dpiX, dpiY);

	hr = pD2DFactory->CreateDxgiSurfaceRenderTarget(pDXGISurface, &props, &pRT);
	if (FAILED(hr))
		return hr;

	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown * *>(&pDWriteFactory));
	if (FAILED(hr))
		return hr;

	//関数CreateTextFormat()
	//第1引数：フォント名（L"メイリオ", L"Arial", L"Meiryo UI"等）
	//第2引数：フォントコレクション（nullptr）
	//第3引数：フォントの太さ（DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_WEIGHT_BOLD等）
	//第4引数：フォントスタイル（DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STYLE_OBLIQUE, DWRITE_FONT_STYLE_ITALIC）
	//第5引数：フォントの幅（DWRITE_FONT_STRETCH_NORMAL,DWRITE_FONT_STRETCH_EXTRA_EXPANDED等）
	//第6引数：フォントサイズ（20, 30等）
	//第7引数：ロケール名（L""）
	//第8引数：テキストフォーマット（&g_pTextFormat）
											//"文字変更"
	hr = pDWriteFactory->CreateTextFormat(L"メイリオ", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, Size, L"", &pTextFormat);
	if (FAILED(hr))
		return hr;

	//関数SetTextAlignment()
	//第1引数：テキストの配置（DWRITE_TEXT_ALIGNMENT_LEADING：前, DWRITE_TEXT_ALIGNMENT_TRAILING：後, DWRITE_TEXT_ALIGNMENT_CENTER：中央）
	hr = pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	if (FAILED(hr))
		return hr;

	//関数CreateSolidColorBrush()
	//第1引数：フォント色（D2D1::ColorF(D2D1::ColorF::Black)：黒, D2D1::ColorF(D2D1::ColorF(0.0f, 0.2f, 0.9f, 1.0f))：RGBA指定）
	hr = pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pSolidBrush);
	if (FAILED(hr))
		return hr;

	pSolidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));//フォント色変更：黒

	//--------------------------★追加↑--------------------------
	return hr;
}

void Moji::Draw(const wchar_t* str, unsigned int str_length, int left, int top, int right, int bottom)
{	//--------------------------★追加↓--------------------------
	// テキストの描画
	//WCHAR wcText1[] = L"　親譲りの無鉄砲で小供の時から損ばかりしている。小学校に居る時分学校の二階から";
	//WCHAR wcText2[] = L"飛び降りて一週間ほど腰を抜かした事がある。なぜそんな無闇をしたと聞く人があるか";
	//WCHAR wcText3[] = L"も知れぬ。別段深い理由でもない。新築の二階から首を出していたら、同級生の一人が";
	//WCHAR wcText4[] = L"冗談に、いくら威張っても、そこから飛び降りる事は出来まい。弱虫やーい。と囃した";
	//WCHAR wcText5[] = L"からである。小使に負ぶさって帰って来た時、おやじが大きな眼をして二階ぐらいから";
	//WCHAR wcText6[] = L"飛び降りて腰を抜かす奴があるかと云ったから、この次は抜かさずに飛んで見せますと";
	//WCHAR wcText7[] = L"答えた。";

	HWND hwnd = FindWindow(_T("AppClass"), _T("Othello（AI「AO」）"));
	HDC hdc = GetDC(hwnd);

	pRT->BeginDraw();
	//TEXTMETRIC tm;
	//GetTextMetrics(hdc, &tm);
	pRT->DrawText(str, str_length, pTextFormat, D2D1::RectF(left, top, right, bottom), pSolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
	
	pSolidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));//フォント色変更：青
	//pRT->DrawText(wcText1, ARRAYSIZE(wcText1) - 1, pTextFormat, D2D1::RectF(0, 0, 800, 20), pSolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
	//pRT->DrawText(wcText2, ARRAYSIZE(wcText2) - 1, pTextFormat, D2D1::RectF(0, tm.tmHeight, 800, 40), pSolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
	//pSolidBrush->SetOpacity(0.7f);//フォントの透明度変更：70％
	//pRT->DrawText(wcText3, ARRAYSIZE(wcText3) - 1, pTextFormat, D2D1::RectF(0, tm.tmHeight * 2, 800, 60), pSolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
	//pSolidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));//フォント色変更：黒
	//pRT->DrawText(wcText4, ARRAYSIZE(wcText4) - 1, pTextFormat, D2D1::RectF(0, tm.tmHeight * 3, 800, 80), pSolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
	//pRT->DrawText(wcText5, ARRAYSIZE(wcText5) - 1, pTextFormat, D2D1::RectF(0, tm.tmHeight * 4, 800, 100), pSolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
	//pSolidBrush->SetOpacity(1.0f);//フォントの透明度変更：100％
	//pRT->DrawText(wcText6, ARRAYSIZE(wcText6) - 1, pTextFormat, D2D1::RectF(0, tm.tmHeight * 5, 800, 120), pSolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
	//pRT->DrawText(wcText7, ARRAYSIZE(wcText7) - 1, pTextFormat, D2D1::RectF(0, tm.tmHeight * 6, 800, 140), pSolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
	//--------------------------★追加↑--------------------------
	
	ReleaseDC(hwnd, hdc);
	pRT->EndDraw();
}

void Moji::SetFont(const wchar_t * font_name, int size)
{
	pDWriteFactory->CreateTextFormat(font_name, nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, size, L"", &pTextFormat);
}

void Moji::SetBrushColor(float r, float g, float b, float a)
{
	//RGB変換の処理をまだ書いてない
	//pSolidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Blue));
	pSolidBrush->SetOpacity(a);
}
