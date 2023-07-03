//=============================================================================
//
// タイトルシーン[sceneSelect.cpp]
// Author : TAKUYA KATOU
//
//=============================================================================
#include "sceneSelect.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"		
#include "fade.h"
#include "bg.h"
#include "selectstage.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_PLAYER		"data/model/kabuBody.obj"
#define MODEL_PLAYER1		"data/model/kabu2.obj"
#define MODEL_PLAYER2		"data/model/kabu3.obj"
#define MODEL_PLAYER3		"data/model/kabu4.obj"



//*****************************************************************************
// グローバル変数
//*****************************************************************************

//=============================================================================
// タイトル表示の初期化処理
//=============================================================================
HRESULT InitSceneSelect()
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

	hr = InitSelectStage();
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("セレクト画像初期化処理エラー"), _T("エラー"), MB_OK | MB_ICONSTOP);
		return hr;
	}

	return hr;
}

//=============================================================================
// タイトル表示の終了処理
//=============================================================================
void UninitSceneSelect()
{
	//背景の終了処理
	UninitBg();

	//セレクトの終了処理
	UninitSelectStage();
}

//=============================================================================
// タイトル表示の更新処理
//=============================================================================
void UpdateSceneSelect()
{
	//背景の更新処理
	UpdateBg();

	//終了処理
	UpdateSelectStage();

	//ポーズ中の項目決定

	E_FADE fadeState = GetFade();
	if (fadeState == E_FADE_NONE)
	{
		if (GetKeyTrigger(VK_RETURN) || GetJoyTrigger(0, 0) || GetMouseTrigger(0))
		{
			//選択中のものにより分岐
			SELECT_STAGE menu = GetSelectStageMenu();
			switch (menu)
			{
				//ゲームに戻る
			case SELECT_STAGE_1:
				StartFade(SCENE_GAME);
				break;
				//リトライ
			case SELECT_STAGE_2:
				StartFade(SCENE_TITLE);
				break;
			}
		}
	}



}

//=============================================================================
// タイトル表示処理
//=============================================================================
void DrawSceneSelect()
{
	SetZWrite(false);
	//背景の描画処理
	DrawBg1();

	// Zバッファ有効
	SetZBuffer(true);

	// Zバッファ無効
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);

	DrawSelectStage();

}
