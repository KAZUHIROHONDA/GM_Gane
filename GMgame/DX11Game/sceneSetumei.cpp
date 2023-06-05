//=============================================================================
//
// タイトルシーン[sceneSetumei.cpp]
// Author : TAKUYA KATOU
//
//=============================================================================
#include "sceneSetumei.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"		
#include "fade.h"
#include "bg.h"
#include "sound.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************



//=============================================================================
// タイトル表示の初期化処理
//=============================================================================
HRESULT InitSceneSetumei()
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
	
	//サウンド
	PlaySound(SOUND_LABEL_BGM000);

	return hr;
}

//=============================================================================
// タイトル表示の終了処理
//=============================================================================
void UninitSceneSetumei()
{
	//背景の終了処理
	UninitBg();

}

//=============================================================================
// タイトル表示の更新処理
//=============================================================================
void UpdateSceneSetumei()
{
	//背景の更新処理
	UpdateBg();

	if (GetKeyTrigger(VK_RETURN)||GetJoyTrigger(0, 0))
	{
		//エンターキー押下でゲーム画面へ
		StartFade(SCENE_TITLE, 60);

	}
}

//=============================================================================
// タイトル表示処理
//=============================================================================
void DrawSceneSetumei()
{
	//背景の描画処理
	DrawBg1();

}
