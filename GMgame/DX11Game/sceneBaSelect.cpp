//=============================================================================
//
// タイトルシーン[sceneBaSelect.cpp]
// Author : TAKUYA KATOU
//
//=============================================================================
#include "sceneBaSelect.h"
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
#include "enemy.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************



//*****************************************************************************
// グローバル変数
//*****************************************************************************

//=============================================================================
// タイトル表示の初期化処理
//=============================================================================
HRESULT InitSceneBaSelect()
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

	//敵の初期化処理
	hr = InitSelectEnemy();
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("プレイヤー初期化処理エラー"), _T("エラー"), MB_OK | MB_ICONSTOP);
		return hr;
	}
	
	SetEnemy(XMFLOAT3(-50, -5, -150), XMFLOAT3(0, 90, 0));
	SetEnemy(XMFLOAT3(-50, -5, -50), XMFLOAT3(0, 90, 0));
	SetEnemy(XMFLOAT3(-50, -5, 50), XMFLOAT3(0, 90, 0));

	// カメラ更新
	GetCamera()->Init();

	return hr;
}

//=============================================================================
// タイトル表示の終了処理
//=============================================================================
void UninitSceneBaSelect()
{
	//背景の終了処理
	UninitBg();

	//セレクトの終了処理
	UninitSelectStage();

	UninitPChimera();

	UninitEnemy();
}

//=============================================================================
// タイトル表示の更新処理
//=============================================================================
void UpdateSceneBaSelect()
{
	//背景の更新処理
	UpdateBg();

	//終了処理
	UpdateSelectStage();

	// カメラ更新
	GetCamera()->Update();

	UpdatePChimera();

	UpdateSelectEnemy();

	if (GetMouseTrigger(1))
	{
		StartFade(SCENE_SELECT);
	}

}

//=============================================================================
// タイトル表示処理
//=============================================================================
void DrawSceneBaSelect()
{

	// Zバッファ無効
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);
	//半透明オブジェクト(3Dの中でも後ろにかく)
	//SetZWrite(true);

	//背景
	DrawBg1();

	// Zバッファ有効
	SetZBuffer(true);

	//プレイヤー
	DrawPChimera();
	DrawEnemy();


}
