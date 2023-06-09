//=============================================================================
//
// 背景表示処理 [bg.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "bg.h"
#include "polygon.h"
#include "Texture.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// テクスチャファイル名
#define BG_TEXTURENAME	_T("data/texture/aiueo.png")
#define BG1_TEXTURENAME	_T("data/texture/setumei.jpg")

#define BG_POS_X	(0)					//初期位置X
#define BG_POS_Y	(0)					//初期位置Y
#define BG_SIZE_X	(SCREEN_WIDTH)		//横幅
#define BG_SIZE_Y	(SCREEN_HEIGHT)		//縦幅


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTexture[2];				// テクスチャ

//=============================================================================
// 背景表示の初期化処理
//=============================================================================
HRESULT InitBg()
{
	HRESULT hr;

	// テクスチャ読み込み
	hr = CreateTextureFromFile(GetDevice(), BG_TEXTURENAME, &g_pTexture[0]);
	if (FAILED(hr))return hr;

	// テクスチャ読み込み
	hr = CreateTextureFromFile(GetDevice(), BG1_TEXTURENAME, &g_pTexture[1]);
	if (FAILED(hr))return hr;
	
	return hr;
}

//=============================================================================
// 背景表示の終了処理
//=============================================================================
void UninitBg()
{
	// テクスチャ開放
	SAFE_RELEASE(g_pTexture[0]);
	SAFE_RELEASE(g_pTexture[1]);
}

//=============================================================================
// 背景表示の更新処理
//=============================================================================
void UpdateBg()
{
	// (何もしない)
}

//=============================================================================
// 背景表示処理
//=============================================================================
void DrawBg()
{
	// 背景描画
	//ポリゴン情報設定
	SetPolygonPos(BG_POS_X, BG_POS_Y);			//座標
	SetPolygonSize(BG_SIZE_X, BG_SIZE_Y);		//大きさ
	SetPolygonAngle(0.0f);				//角度
	SetPolygonColor(1.0f, 1.0f, 1.0f);	//色(RGB)
	SetPolygonAlpha(1.0f);				//α値(透明度)

	SetPolygonUV(0.0f, 0.0f);			//0番のテクスチャ
	SetPolygonFrameSize(1.0f, 1.0f);	//テクスチャ
	SetPolygonTexture(g_pTexture[0]);		//テクスチャ

	//ポリゴンの描画処理
	DrawPolygon(GetDeviceContext());

}

void DrawBg1()
{
	// 背景描画
	//ポリゴン情報設定
	SetPolygonPos(BG_POS_X, BG_POS_Y);			//座標
	SetPolygonSize(BG_SIZE_X, BG_SIZE_Y);		//大きさ
	SetPolygonAngle(0.0f);				//角度
	SetPolygonColor(1.0f, 1.0f, 1.0f);	//色(RGB)
	SetPolygonAlpha(1.0f);				//α値(透明度)

	SetPolygonUV(0.0f, 0.0f);			//0番のテクスチャ
	SetPolygonFrameSize(1.0f, 1.0f);	//テクスチャ
	SetPolygonTexture(g_pTexture[1]);		//テクスチャ

	//ポリゴンの描画処理
	DrawPolygon(GetDeviceContext());

}


// 背景描画までの手順
// ① bg.h にbg.cppにある関数のプロトタイプ宣言を書く
// ② main.cpp でbg.cppの初期化、終了、更新、描画を呼び出す
// ③ bg.cpp に座標とサイズとテクスチャーファイル名の定数を書く
// ④ InitBG()関数を実装する
// ⑤ DrawBG()関数を実装する
