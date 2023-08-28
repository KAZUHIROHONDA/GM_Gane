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
	g_player.SetHP(300);
	g_player.SetPAat(60);
	g_player.SetGUat(70);
	g_player.SetTYOKIat(50);
	g_player.Setscl(XMFLOAT3(8.0f, 8.0f, 8.0f));
	PartsGet()->Setpos(XMFLOAT3(0.0f, 0.0f, -2.0f));
	MPartsGet()->Setpos(XMFLOAT3(0.0f, -0.8f, -1.0f));
	UPartsGet()->Setpos(XMFLOAT3(0.0f, -0.8f, 0.0f));
	PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
	MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
	UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
	g_player.SetHand(0, 2);g_player.SetHand(1, 1);
	g_player.SetHand(2, 2);g_player.SetHand(3, 1);
	g_player.SetHand(4, 2);g_player.SetHand(5, 1);
	g_player.SetHand(6, 2);g_player.SetHand(7, 1);
	g_player.SetHand(8, 2);g_player.SetHand(9, 1); 
	g_player.SetHand(10, 2);g_player.SetHand(11, 1);
	g_player.SetHand(12, 2); g_player.SetHand(13, 1);
	g_player.SetHand(14, 2); g_player.SetHand(15, 1);
	g_player.SetHand(16, 2); g_player.SetHand(17, 1);
	g_player.SetHand(18, 2); g_player.SetHand(19, 1);

	//デバック
	g_enemy.Init();
	g_enemy.SetName("わに");
	g_enemy.SetHP(50);
	g_enemy.SetPAat(15);
	g_enemy.SetGUat(20);
	g_enemy.SetTYOKIat(25);
	g_enemy.SetHand(0, 1); g_enemy.SetHand(1, 1);
	g_enemy.SetHand(2, 1); g_enemy.SetHand(3, 1);
	g_enemy.SetHand(4, 1); g_enemy.SetHand(5, 1);
	g_enemy.SetHand(6, 1); g_enemy.SetHand(7, 1);
	g_enemy.SetHand(8, 1); g_enemy.SetHand(9, 1);
	g_enemy.SetHand(10, 1);g_enemy.SetHand(11, 1);
	g_enemy.SetHand(12, 1);g_enemy.SetHand(13, 1);
	g_enemy.SetHand(14, 1);g_enemy.SetHand(15, 1);
	g_enemy.SetHand(16, 1);g_enemy.SetHand(17, 1);
	g_enemy.SetHand(18, 1);g_enemy.SetHand(19, 1);


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
