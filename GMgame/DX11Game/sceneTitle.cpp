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
#include "select1.h"
#include "sceneClear.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************

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

	//タイトル
	InitTitle();

	//選択
	InitSelect1();

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

	//選択
	UninitSelect1();
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
	//選択
	UpdateSelect1();

	E_FADE fadeState = GetFade();
	if (fadeState == E_FADE_NONE)
	{
		if (GetKeyTrigger(VK_RETURN)|| GetJoyTrigger(0, 0))
		{
			//選択中のものにより分岐
			SELECT1_MENU menu = GetSelect1Menu();
			switch (menu)
			{
				//ゲームスタート
			case SELECT1_MENU_CONTINUE:
				StartFade(SCENE_GAME);
				SetStageNo(0);
				break;
				//説明画面
			case SELECT1_MENU_RETRY:
				StartFade(SCENE_SETUMEI);
				break;
				//ステージセレクト
			case SELECT1_MENU_QUIT:
				StartFade(SCENE_SELECT);
				break;
			}
		}
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
	//選択
	DrawSelect1();

	
}
