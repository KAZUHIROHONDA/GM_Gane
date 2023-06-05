//=============================================================================
//
// ゲージ処理 [gauge.cpp]
//
//=============================================================================
#include "gauge.h"
#include "polygon.h"	// ポリゴン使用
#include "Texture.h"

// マクロ定義
#define GAUGE_TEXTURENAME	_T("data/texture/gauge000.png")
#define GAUGE_SIZE_X	(256)	// サイズX
#define GAUGE_SIZE_Y	(32)	// サイズY

#define GAUGE1_TEXTURENAME	_T("data/texture/gaugeframe.png")
#define GAUGE1_SIZE_X	(256)	// サイズX
#define GAUGE1_SIZE_Y	(32)	// サイズY

// 構造体定義
// プロトタイプ宣言
// グローバル変数
static ID3D11ShaderResourceView*	g_pTexture;				// テクスチャ
static ID3D11ShaderResourceView*	g_pTexture1;				// テクスチャ

// 初期化処理
HRESULT InitGauge( void )
{
	// テクスチャ読み込み
	HRESULT hr = CreateTextureFromFile(GetDevice(),
		GAUGE_TEXTURENAME, &g_pTexture);
	// テクスチャ読み込み
	      hr = CreateTextureFromFile(GetDevice(),
		GAUGE1_TEXTURENAME, &g_pTexture1);

	return hr;
}

void UninitGauge( void )
{
	// テクスチャ開放
	SAFE_RELEASE(g_pTexture);
	// テクスチャ開放
	SAFE_RELEASE(g_pTexture1);
}

void UpdateGauge( void )
{
	// １フレームごとの更新処理
}

//in:pos:左端座標指定
//　	persent:100%を1.0として受け取る
void DrawGauge(XMFLOAT2 pos, float percent)
{
	//描画用にデバイスコンテキストを取得
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	SetBlendState(BS_ALPHABLEND);

	//設定
	SetPolygonTexture(g_pTexture);
	SetPolygonSize(GAUGE_SIZE_X * percent, GAUGE_SIZE_Y);
	SetPolygonPos(pos.x + GAUGE_SIZE_X * percent / 2, pos.y);
	SetPolygonUV(0.0f, 0.0f);
	SetPolygonFrameSize(percent, 1.0f);

	//描画
	DrawPolygon(pDeviceContext);
	//元に戻す
	SetPolygonFrameSize(1.0f,1.0f);
	//設定
	SetPolygonTexture(g_pTexture1);
	SetPolygonSize(GAUGE1_SIZE_X , GAUGE1_SIZE_Y);
	SetPolygonPos(3.0f, 300.0f);
	SetPolygonUV(0.0f, 0.0f);

	
	//描画
	DrawPolygon(pDeviceContext);
	//元に戻す
	SetPolygonFrameSize(1.0f, 1.0f);

}


