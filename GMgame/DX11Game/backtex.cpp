//=============================================================================
//
// 表示処理 [backtex.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "backtex.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"		//移動させるところにいれる

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// テクスチャファイル名
#define BT_TEXTURENAME	_T("data/texture/backtex.png")

#define BT_POS_X	(-380)					//初期位置X
#define BT_POS_Y	(200)					//初期位置Y
#define BT_SIZE_X	(450)		//横幅
#define BT_SIZE_Y	(250)		//縦幅


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTexture;				// テクスチャ

static XMFLOAT3 g_Position;//位置
static XMFLOAT2    g_Angle;   //角度
static XMFLOAT3	g_Rot;
static XMFLOAT2 g_Size;    //サイズ
static XMFLOAT2 g_Scale;   //拡大率 
static float	g_Alpha;   //透過


//=============================================================================
// タイトル表示の初期化処理
//=============================================================================
HRESULT InitBackTex()
{
	HRESULT hr;

	// テクスチャ読み込み
	hr = CreateTextureFromFile(GetDevice(), BT_TEXTURENAME, &g_pTexture);
	if (FAILED(hr))return hr;


	g_Position = XMFLOAT3(BT_POS_X, BT_POS_Y, 0.0f);
	g_Alpha = 1.0f;
	g_Scale = XMFLOAT2(1.0f, 1.0f);

	return hr;
}

//=============================================================================
// タイトル表示の終了処理
//=============================================================================
void UninitBackTex()
{
	// テクスチャ開放
	SAFE_RELEASE(g_pTexture);
}

//=============================================================================
// タイトル表示の更新処理
//=============================================================================
void UpdateBackTex()
{

}

//=============================================================================
// タイトル表示処理
//=============================================================================
void DrawBackTex()
{
	// タイトル描画
	//ポリゴン情報設定
	SetPolygonPos(g_Position.x, g_Position.y);			//座標
	SetPolygonSize(BT_SIZE_X, BT_SIZE_Y);		//大きさ
	SetPolygonAngle(g_Angle.y);				//角度
	SetPolygonColor(1.0f, 1.0f, 1.0f);	//色(RGB)
	SetPolygonAlpha(g_Alpha);				//α値(透明度)
	
	SetPolygonUV(0.0f, 0.0f);			//0番のテクスチャ
	SetPolygonFrameSize(1.0f, 1.0f);	//テクスチャ
	SetPolygonTexture(g_pTexture);		//テクスチャ

	//ポリゴンの描画処理
	DrawPolygon(GetDeviceContext());

}

void DrawBackTex1()
{
	// タイトル描画
	//ポリゴン情報設定
	SetPolygonPos(50, 250);			//座標
	SetPolygonSize(BT_SIZE_X, BT_SIZE_Y);		//大きさ
	SetPolygonAngle(g_Angle.y);				//角度
	SetPolygonColor(1.0f, 1.0f, 1.0f);	//色(RGB)
	SetPolygonAlpha(g_Alpha);				//α値(透明度)

	SetPolygonUV(0.0f, 0.0f);			//0番のテクスチャ
	SetPolygonFrameSize(1.0f, 1.0f);	//テクスチャ
	SetPolygonTexture(g_pTexture);		//テクスチャ

	//ポリゴンの描画処理
	DrawPolygon(GetDeviceContext());

}

// タイトル描画までの手順
// ① BT.h にBT.cppにある関数のプロトタイプ宣言を書く
// ② main.cpp でBT.cppの初期化、終了、更新、描画を呼び出す
// ③ BT.cpp に座標とサイズとテクスチャーファイル名の定数を書く
// ④ InitBT()関数を実装する
// ⑤ DrawBT()関数を実装する
