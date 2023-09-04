//=============================================================================
//
// タイトルシーン[sceneTitle.cpp]
// Author : TAKUYA KATOU
//
//=============================================================================
#include "sceneTitle.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"		
#include "fade.h"
#include "bg.h"
#include "Title.h"
#include "enter.h"
#include "stage.h"
#include "sceneClear.h"
#include "Bpartsmenu.h"
#include "partsmenu.h"
#include "Upartsmenu.h"
#include "Mpartsmenu.h"
#include "model.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************

Player g_player;
Model  player;
Player g_enemy;
Model  enemy;

//=============================================================================
// タイトル表示の初期化処理
//=============================================================================
HRESULT InitSceneTitle()
{
	HRESULT hr = S_OK;
	HWND hWnd = GetMainWnd();

	//背景の初期化処理
	hr = InitBg();
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("背景初期化処理エラー"), _T("エラー"), MB_OK | MB_ICONSTOP);
		return hr;
	}

	g_player.Init();
	g_player.SetName("カブトムシ");
	g_player.Setmodel(0);
	g_player.Setmodel(10);
	g_player.Setmodel(20);
	g_player.Setmodel(30);
	g_player.Setscl(XMFLOAT3(8.0f, 8.0f, 8.0f));
	PartsGet()->Setpos(XMFLOAT3(0.0f, 0.0f, -2.0f));
	MPartsGet()->Setpos(XMFLOAT3(0.0f, -0.8f, -1.0f));
	UPartsGet()->Setpos(XMFLOAT3(0.0f, -0.8f, 0.0f));
	PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
	MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
	UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
	player.SetHead(MODEL_KABU);
	player.SetBody(MODEL_KABU1);
	player.SetMae(MODEL_KABU2);
	player.SetBack(MODEL_KABU3);

	//デバック
	g_enemy.Init();
	g_enemy.SetName("わに");
	g_enemy.Setmodel(1);
	g_enemy.Setmodel(11);
	g_enemy.Setmodel(21);
	g_enemy.Setmodel(31);
	enemy.SetHead(MODEL_wani);
	enemy.SetBody(MODEL_wani1);
	enemy.SetMae(MODEL_wani2);
	enemy.SetBack(MODEL_wani3);
	
	//タイトル
	InitTitle();

	InitEnter();

	//ステージの初期化
	SetStageNo(0);

	return hr;
}

//=============================================================================
// タイトル表示の終了処理
//=============================================================================
void UninitSceneTitle()
{

	//背景の終了処理
	UninitBg();

	//タイトル
	UninitTitle();

	UninitEnter();

	g_player.Uninit();
	g_enemy.Uninit();

}

//=============================================================================
// タイトル表示の更新処理
//=============================================================================
void UpdateSceneTitle()
{

	//背景の更新処理
	UpdateBg();

	//タイトル
	UpdateTitle();

	g_player.Update();
	g_enemy.Update();

	UpdateEnter();

	if (GetKeyTrigger(VK_RETURN) || GetJoyTrigger(0, 0)|| GetMouseTrigger(0))
	{

		StartFade(SCENE_SELECT);
	}


}

//=============================================================================
// タイトル表示処理
//=============================================================================
void DrawSceneTitle()
{
	SetZWrite(false);
	//背景の描画処理
	DrawBg();

	// Zバッファ有効
	SetZBuffer(true);

	// Zバッファ無効
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);
	

	//タイトル
	DrawTitle();
	DrawEnter();
	
}

Player* GetPlayer()
{
	return &g_player;
}

Player* GetEnemy()
{
	return &g_enemy;
}

Model* GetPlayerModel()
{
	return &player;
}

