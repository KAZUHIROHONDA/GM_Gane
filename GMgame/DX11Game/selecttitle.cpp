//=============================================================================
//
// 背景表示処理 [seltitle.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "selecttitle.h"
#include "polygon.h"
#include "Texture.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// テクスチャファイル名
#define SETI_TEXTURENAME	_T("data/texture/gamesel.png")
#define SETI1_TEXTURENAME	_T("data/texture/stagesel.png")

#define SETI_POS_X	(-400)					//初期位置X
#define SETI_POS_Y	(300)					//初期位置Y
#define SETI_SIZE_X	(360)		//横幅
#define SETI_SIZE_Y	(80)		//縦幅


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTexture[2];				// テクスチャ

//=============================================================================
// 背景表示の初期化処理
//=============================================================================
HRESULT InitSelTitle()
{
	HRESULT hr;

	// テクスチャ読み込み
	hr = CreateTextureFromFile(GetDevice(), SETI_TEXTURENAME, &g_pTexture[0]);
	if (FAILED(hr))return hr;

	// テクスチャ読み込み
	hr = CreateTextureFromFile(GetDevice(), SETI1_TEXTURENAME, &g_pTexture[1]);
	if (FAILED(hr))return hr;
	
	return hr;
}

//=============================================================================
// 背景表示の終了処理
//=============================================================================
void UninitSelTitle()
{
	// テクスチャ開放
	SAFE_RELEASE(g_pTexture[0]);
	SAFE_RELEASE(g_pTexture[1]);
}

//=============================================================================
// 背景表示の更新処理
//=============================================================================
void UpdateSelTitle()
{
	// (何もしない)
}

//=============================================================================
// 背景表示処理
//=============================================================================
void DrawSelTitle()
{
	// 背景描画
	//ポリゴン情報設定
	SetPolygonPos(SETI_POS_X, SETI_POS_Y);			//座標
	SetPolygonSize(SETI_SIZE_X, SETI_SIZE_Y);		//大きさ
	SetPolygonAngle(0.0f);				//角度
	SetPolygonColor(1.0f, 1.0f, 1.0f);	//色(RGB)
	SetPolygonAlpha(1.0f);				//α値(透明度)

	SetPolygonUV(0.0f, 0.0f);			//0番のテクスチャ
	SetPolygonFrameSize(1.0f, 1.0f);	//テクスチャ
	SetPolygonTexture(g_pTexture[0]);		//テクスチャ

	//ポリゴンの描画処理
	DrawPolygon(GetDeviceContext());

}

void DrawSelTitle1()
{
	// 背景描画
	//ポリゴン情報設定
	SetPolygonPos(SETI_POS_X, SETI_POS_Y);			//座標
	SetPolygonSize(SETI_SIZE_X, SETI_SIZE_Y);		//大きさ
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
// ① seltitle.h にseltitle.cppにある関数のプロトタイプ宣言を書く
// ② main.cpp でseltitle.cppの初期化、終了、更新、描画を呼び出す
// ③ seltitle.cpp に座標とサイズとテクスチャーファイル名の定数を書く
// ④ InitSETI()関数を実装する
// ⑤ DrawSETI()関数を実装する
