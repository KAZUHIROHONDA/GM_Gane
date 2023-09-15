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
#include "pchimera.h"
#include "Light.h"
#include "Camera.h"
#include "tab.h"
#include "tex.h"
#include "playercs.h"
#include "sceneTitle.h"
#include "Bpartsmenu.h"
#include "Mpartsmenu.h"
#include "partsmenu.h"
#include "Upartsmenu.h"
#include "backtex.h"
#include"sound.h"




//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SAMPLE_TEXTURENAME	_T("data/texture/tex.png")


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


	//プレイヤーの初期化処理
	hr = InitPChimera();
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("プレイヤー初期化処理エラー"), _T("エラー"), MB_OK | MB_ICONSTOP);
		return hr;
	}

	InitBg();

	InitTab();

	InitTex();

	InitBackTex();

	Sflag();

	PlaySound(SOUND_LABEL_BGM_3);

	// カメラ更新
	GetCamera()->Init();

	player.Init();

	GetPlayer()->Percent();

	return hr;
}

//=============================================================================
// タイトル表示の終了処理
//=============================================================================
void UninitSceneChimera()
{

	UninitPChimera();

	UninitBg();

	UninitTab();

	UninitTex();

	UninitBackTex();
	StopSound();

	GetPlayer()->Uninit();

	player.Uninit();
}

//=============================================================================
// タイトル表示の更新処理
//=============================================================================
void UpdateSceneChimera()
{
	// カメラ更新
	GetCamera()->Update();

	UpdatePChimera();

	UpdateBg();

	player.Update();

	UpdateTab();

	UpdateTex();

	UpdateBackTex();
	
	GetPlayer()->Update();

	Sflag();

	if (GetMouseTrigger(1))
	{
		StartFade(SCENE_SELECT);
	}

}

//=============================================================================
// タイトル表示処理
//=============================================================================
void DrawSceneChimera()
{

	// Zバッファ無効
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);
	//半透明オブジェクト(3Dの中でも後ろにかく)
	//SetZWrite(true);

	//背景
	DrawBg1();

	DrawTex();

	// Zバッファ有効
	SetZBuffer(true);

	//プレイヤー
	DrawPChimera();

	//光源処理無効
	GetLight()->SetDisable();
	
	//光源処理有効
	GetLight()->SetEnable();

	// Zバッファ無効
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);


	//光源処理無効
	GetLight()->SetDisable();
	//半透明オブジェクト(3Dの中でも後ろにかく)
	//ビルボード弾びょうが
	SetZWrite(false);

	//光源処理有効
	GetLight()->SetEnable();

	DrawBackTex();

	DrawTab();

	player.Draw();

	GetPlayer()->DrawChimera(50,50);

}

Chimera* Get()
{
	return &player;
}
