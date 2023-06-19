//=============================================================================
//
// ゲージ処理 [gauge.cpp]
//
//=============================================================================
#include "Egauge.h"
#include "polygon.h"	// ポリゴン使用
#include "Texture.h"

// マクロ定義
#define EGAUGE_TEXTURENAME	_T("data/texture/gauge001.png")
#define EGAUGE_SIZE_X	(256)	// サイズX
#define EGAUGE_SIZE_Y	(32)	// サイズY
	    
#define EGAUGE1_TEXTURENAME	_T("data/texture/gaugeframe.png") 
#define EGAUGE1_SIZE_X	(256)	// サイズX
#define EGAUGE1_SIZE_Y	(32)	// サイズY

// 構造体定義
// プロトタイプ宣言
// グローバル変数
static ID3D11ShaderResourceView*	g_pTexture;				// テクスチャ
static ID3D11ShaderResourceView*	g_pTexture1;				// テクスチャ

// 初期化処理
HRESULT InitEGauge( void )
{
	// テクスチャ読み込み
	HRESULT hr = CreateTextureFromFile(GetDevice(),
		EGAUGE_TEXTURENAME, &g_pTexture);
	// テクスチャ読み込み
	      hr = CreateTextureFromFile(GetDevice(),
		EGAUGE1_TEXTURENAME, &g_pTexture1);

	return hr;
}

void UninitEGauge( void )
{
	// テクスチャ開放
	SAFE_RELEASE(g_pTexture);
	// テクスチャ開放
	SAFE_RELEASE(g_pTexture1);
}

void UpdateEGauge( void )
{
	// １フレームごとの更新処理
}

//in:pos:左端座標指定
//　	persent:100%を1.0として受け取る
void DrawEGauge(XMFLOAT2 pos, float percent)
{
	//描画用にデバイスコンテキストを取得
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	SetBlendState(BS_ALPHABLEND);

	//設定
	SetPolygonTexture(g_pTexture);
	SetPolygonSize(EGAUGE_SIZE_X * percent, EGAUGE_SIZE_Y);
	SetPolygonPos(pos.x + EGAUGE_SIZE_X * percent / 2, pos.y);
	SetPolygonUV(0.0f, 0.0f);
	SetPolygonFrameSize(percent, 1.0f);

	//描画
	DrawPolygon(pDeviceContext);
	//元に戻す
	SetPolygonFrameSize(1.0f,1.0f);
	//設定
	SetPolygonTexture(g_pTexture1);
	SetPolygonSize(EGAUGE1_SIZE_X , EGAUGE1_SIZE_Y);
	SetPolygonPos(252.0f, 300.0f);
	SetPolygonUV(0.0f, 0.0f);

	
	//描画
	DrawPolygon(pDeviceContext);
	//元に戻す
	SetPolygonFrameSize(1.0f, 1.0f);

}


