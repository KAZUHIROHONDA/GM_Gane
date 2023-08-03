//=============================================================================
//
// 背景表示処理 [tex.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "tex.h"
#include "polygon.h"
#include "Texture.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// テクスチャファイル名
#define TEX_TEXTURENAME	_T("data/texture/tex.png")
#define TEX1_TEXTURENAME	_T("data/texture/tex1.png")

#define TEX_POS_X	(0)					//初期位置X
#define TEX_POS_Y	(0)					//初期位置Y
#define TEX_SIZE_X	(SCREEN_WIDTH)		//横幅
#define TEX_SIZE_Y	(SCREEN_HEIGHT)		//縦幅


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTexture[2];				// テクスチャ

//=============================================================================
// 背景表示の初期化処理
//=============================================================================
HRESULT InitTex()
{
	HRESULT hr;

	// テクスチャ読み込み
	hr = CreateTextureFromFile(GetDevice(), TEX_TEXTURENAME, &g_pTexture[0]);
	if (FAILED(hr))return hr;

	// テクスチャ読み込み
	hr = CreateTextureFromFile(GetDevice(), TEX1_TEXTURENAME, &g_pTexture[1]);
	if (FAILED(hr))return hr;
	
	return hr;
}

//=============================================================================
// 背景表示の終了処理
//=============================================================================
void UninitTex()
{
	// テクスチャ開放
	SAFE_RELEASE(g_pTexture[0]);
	SAFE_RELEASE(g_pTexture[1]);
}

//=============================================================================
// 背景表示の更新処理
//=============================================================================
void UpdateTex()
{
	// (何もしない)
}

//=============================================================================
// 背景表示処理
//=============================================================================
void DrawTex()
{
	// 背景描画
	//ポリゴン情報設定
	SetPolygonPos(TEX_POS_X, TEX_POS_Y);			//座標
	SetPolygonSize(TEX_SIZE_X, TEX_SIZE_Y);		//大きさ
	SetPolygonAngle(0.0f);				//角度
	SetPolygonColor(1.0f, 1.0f, 1.0f);	//色(RGB)
	SetPolygonAlpha(1.0f);				//α値(透明度)

	SetPolygonUV(0.0f, 0.0f);			//0番のテクスチャ
	SetPolygonFrameSize(1.0f, 1.0f);	//テクスチャ
	SetPolygonTexture(g_pTexture[0]);		//テクスチャ

	//ポリゴンの描画処理
	DrawPolygon(GetDeviceContext());

}

void DrawTex1()
{
	// 背景描画
	//ポリゴン情報設定
	SetPolygonPos(TEX_POS_X, TEX_POS_Y);			//座標
	SetPolygonSize(TEX_SIZE_X, TEX_SIZE_Y);		//大きさ
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
// ① tex.h にtex.cppにある関数のプロトタイプ宣言を書く
// ② main.cpp でtex.cppの初期化、終了、更新、描画を呼び出す
// ③ tex.cpp に座標とサイズとテクスチャーファイル名の定数を書く
// ④ InitTEX()関数を実装する
// ⑤ DrawTEX()関数を実装する
