//=============================================================================
//
// タイトル表示処理 [VS.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "VS.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"		//移動させるところにいれる

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// テクスチャファイル名
#define VS_TEXTURENAME	_T("data/texture/VS.png")

#define VS_POS_X	(0)					//初期位置X
#define VS_POS_Y	(0)					//初期位置Y
#define VS_SIZE_X	(300)		//横幅
#define VS_SIZE_Y	(300)		//縦幅


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
HRESULT InitVS()
{
	HRESULT hr;

	// テクスチャ読み込み
	hr = CreateTextureFromFile(GetDevice(), VS_TEXTURENAME, &g_pTexture);
	if (FAILED(hr))return hr;


	g_Position = XMFLOAT3(VS_POS_X, VS_POS_Y, 0.0f);
	g_Alpha = 1.0f;
	g_Scale = XMFLOAT2(1.0f, 1.0f);

	return hr;
}

//=============================================================================
// タイトル表示の終了処理
//=============================================================================
void UninitVS()
{
	// テクスチャ開放
	SAFE_RELEASE(g_pTexture);
}

//=============================================================================
// タイトル表示の更新処理
//=============================================================================
void UpdateVS()
{

}

//=============================================================================
// タイトル表示処理
//=============================================================================
void DrawVS()
{
	// タイトル描画
	//ポリゴン情報設定
	SetPolygonPos(g_Position.x, g_Position.y);			//座標
	SetPolygonSize(VS_SIZE_X, VS_SIZE_Y);		//大きさ
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
// ① VS.h にVS.cppにある関数のプロトタイプ宣言を書く
// ② main.cpp でVS.cppの初期化、終了、更新、描画を呼び出す
// ③ VS.cpp に座標とサイズとテクスチャーファイル名の定数を書く
// ④ InitVS()関数を実装する
// ⑤ DrawVS()関数を実装する
