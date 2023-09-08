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
#include "Camera.h"
#include "pchimera.h"
#include "Light.h"
#include "selecttitle.h"
#include"sound.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************



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

	//プレイヤーの初期化処理
	hr = InitPChimera();
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("プレイヤー初期化処理エラー"), _T("エラー"), MB_OK | MB_ICONSTOP);
		return hr;
	}

	InitSelTitle();

	PlaySound(SOUND_LABEL_BGM_4);

	// カメラ更新
	GetCamera()->Init();

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
	
	UninitSelTitle();
	
	UninitPChimera();
<<<<<<< HEAD

=======
	StopSound();
>>>>>>> feature/matsui3
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

	// カメラ更新
	GetCamera()->Update();

	UpdatePChimera();

	UpdateSelTitle();

	//ポーズ中の項目決定

	E_FADE fadeState = GetFade();
	if (fadeState == E_FADE_NONE)
	{
		if (GetKeyTrigger(VK_RETURN) || GetJoyTrigger(0, 0)||GetMouseTrigger(0))
		{
			PlaySound(SOUND_LABEL_SE_SELECTS);
			//選択中のものにより分岐
			SELECT_STAGE menu = GetSelectStageMenu();
			switch (menu)
			{
				//ゲームに戻る
			case SELECT_STAGE_1:
				StartFade(SCENE_BASELECT);
				break;
				//リトライ
			case SELECT_STAGE_2:
				StartFade(SCENE_CHIMERA);
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

	// Zバッファ無効
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);
	//半透明オブジェクト(3Dの中でも後ろにかく)
	//SetZWrite(true);

	//背景
	DrawBg1();

	DrawSelectStage();

	DrawSelTitle();

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




}
