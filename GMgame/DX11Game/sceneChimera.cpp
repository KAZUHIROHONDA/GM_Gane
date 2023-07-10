//=============================================================================
//
// タイトルシーン[sceneChimera.cpp]
// Author : TAKUYA KATOU
//
//=============================================================================
#include "sceneChimera.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"		
#include "bg.h"
#include "collision.h"
#include "fade.h"
#include "chimeracs.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SAMPLE_TEXTURENAME	_T("data/texture/GO1.jpg")


#define SAMPLE_POS_X	(0)	// 初期位置X
#define SAMPLE_POS_Y	(0)	// 初期位置Y
#define SAMPLE_SIZE_X	(SCREEN_WIDTH)	// 横幅
#define SAMPLE_SIZE_Y	(SCREEN_HEIGHT)	// 縦幅

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTexture;				// テクスチャ
static XMFLOAT3 g_Position;	// 座標

Chimera player;//プレイヤ

//=============================================================================
// タイトル表示の初期化処理
//=============================================================================
HRESULT InitSceneChimera()
{
	// テクスチャ読み込み
	HRESULT hr =
		CreateTextureFromFile(GetDevice(),
			SAMPLE_TEXTURENAME, &g_pTexture);

	//HRESULT hr = S_OK;
	HWND hWnd = GetMainWnd();

	
	player.Init();

	return hr;
}

//=============================================================================
// タイトル表示の終了処理
//=============================================================================
void UninitSceneChimera()
{
	// テクスチャ開放
	SAFE_RELEASE(g_pTexture);
	//背景の終了処理
	UninitBg();
	player.Uninit();
}

//=============================================================================
// タイトル表示の更新処理
//=============================================================================
void UpdateSceneChimera()
{
	
	player.Update();
	
}

//=============================================================================
// タイトル表示処理
//=============================================================================
void DrawSceneChimera()
{

	//サンプル画像描画
		SetPolygonPos(g_Position.x, g_Position.y);	// 座標
	SetPolygonSize(SAMPLE_SIZE_X, SAMPLE_SIZE_Y);	// 大きさ
	SetPolygonAngle(0.0f);				// 角度
	SetPolygonColor(1.0f, 1.0f, 1.0f);	// 色(RGB)
	SetPolygonAlpha(1.0f);				// α値(透明度)

	SetPolygonUV(0.0f, 0.0f);			// 0番のテクスチャ座標
	SetPolygonFrameSize(1.0f, 1.0f);	// テクスチャの横幅,縦幅
	SetPolygonTexture(g_pTexture);		//

	player.Draw();

}
