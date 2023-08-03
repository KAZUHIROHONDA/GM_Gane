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

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************

Player g_player;
Player g_enemy;

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
	g_player.SetHP(100);
	g_player.SetPAat(60);
	g_player.SetGUat(70);
	g_player.SetTYOKIat(50);

	//デバック
	g_enemy.Init();
	g_enemy.SetName("わに");
	g_enemy.SetHP(10);
	g_enemy.SetPAat(15);
	g_enemy.SetGUat(20);
	g_enemy.SetTYOKIat(25);

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
