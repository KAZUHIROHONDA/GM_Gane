//=============================================================================
//
// サンプル表示処理 [gameover.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "gameover.h"
#include "polygon.h"
#include "Texture.h"
#include "sceneClear.h"
#include "input.h"		//移動させるところにいれる

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// テクスチャファイル名
#define GAMEOVER_TEXTURENAME	_T("data/texture/Gameover.png")
#define GAMECLEAR_TEXTURENAME	_T("data/texture/Gameclear0.png")


#define GAMEOVER_POS_X	(0)					//初期位置X
#define GAMEOVER_POS_Y	(100)					//初期位置Y
#define GAMEOVER_SIZE_X	(200)		//横幅
#define GAMEOVER_SIZE_Y	(200)		//縦幅

#define GAMECLEAR_POS_X	(0)	
#define GAMECLEAR_POS_Y	(100)
#define GAMECLEAR_SIZE_X	(600)
#define GAMECLEAR_SIZE_Y	(120)


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTexture[2];				// テクスチャ

static XMFLOAT3 g_Position;//位置
static float    g_Angle;   //角度
static XMFLOAT2 g_Size;    //サイズ
static XMFLOAT2 g_Scale;   //拡大率 
static int		g_nCnt;
static float	g_Alpha = 1.0f;   //透過


//=============================================================================
// サンプル表示の初期化処理
//=============================================================================
HRESULT InitGameover()
{
	HRESULT hr;

	// テクスチャ読み込み
	hr = CreateTextureFromFile(GetDevice(), GAMEOVER_TEXTURENAME, &g_pTexture[0]);
	if (FAILED(hr))return hr;
	// テクスチャ読み込み
	hr = CreateTextureFromFile(GetDevice(), GAMECLEAR_TEXTURENAME, &g_pTexture[1]);
	if (FAILED(hr))return hr;

	g_Position = XMFLOAT3(GAMEOVER_POS_X, GAMEOVER_POS_Y, 0.0f);
	

	return hr;
}

//=============================================================================
// サンプル表示の終了処理
//=============================================================================
void UninitGameover()
{
	// テクスチャ開放
	SAFE_RELEASE(g_pTexture[0]);
	SAFE_RELEASE(g_pTexture[1]);
}

//=============================================================================
// サンプル表示の更新処理
//=============================================================================
void UpdateGameover()
{


}

//=============================================================================
// サンプル表示処理
//=============================================================================
void DrawGameover()
{
	// サンプル描画
	//ポリゴン情報設定
	SetPolygonPos(g_Position.x, g_Position.y);			//座標
	SetPolygonSize(GAMEOVER_SIZE_X, GAMEOVER_SIZE_Y);		//大きさ
	SetPolygonAngle(0.0f);				//角度
	SetPolygonColor(1.0f, 1.0f, 1.0f);	//色(RGB)
	SetPolygonAlpha(g_Alpha);				//α値(透明度)

	SetPolygonUV(0.0f, 0.0f);			//0番のテクスチャ
	SetPolygonFrameSize(1.0f, 1.0f);	//テクスチャ
	SetPolygonTexture(g_pTexture[0]);		//テクスチャ


	//ポリゴンの描画処理
	DrawPolygon(GetDeviceContext());

}

void DrawGameclear()
{
	// サンプル描画
	//ポリゴン情報設定
	SetPolygonPos(g_Position.x, g_Position.y);			//座標
	SetPolygonSize(GAMEOVER_SIZE_X, GAMEOVER_SIZE_Y);		//大きさ
	SetPolygonAngle(0.0f);				//角度
	SetPolygonColor(1.0f, 1.0f, 1.0f);	//色(RGB)
	SetPolygonAlpha(g_Alpha);				//α値(透明度)

	SetPolygonUV(0.0f, 0.0f);			//0番のテクスチャ
	SetPolygonFrameSize(1.0f, 1.0f);	//テクスチャ
	SetPolygonTexture(g_pTexture[1]);		//テクスチャ


	//ポリゴンの描画処理
	DrawPolygon(GetDeviceContext());

}

// サンプル描画までの手順
// ① gameover.h にgameover.cppにある関数のプロトタイプ宣言を書く
// ② main.cpp でgameover.cppの初期化、終了、更新、描画を呼び出す
// ③ gameover.cpp に座標とサイズとテクスチャーファイル名の定数を書く
// ④ InitGAMEOVER()関数を実装する
// ⑤ DrawGAMEOVER()関数を実装する
