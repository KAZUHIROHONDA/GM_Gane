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
#include "enemyselect.h"
#include "enemymodel.h"
#include "sceneTitle.h"
#include "selecttitle.h"
#include "backtex.h"

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

	InitSelTitle();

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

	Enemychimera();

	InitBackTex();

	//敵の初期化処理
	hr = InitSelectEnemyselect();
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("プレイヤー初期化処理エラー"), _T("エラー"), MB_OK | MB_ICONSTOP);
		return hr;
	}
	
	SetEnemyselect(XMFLOAT3(0, -15, -150), XMFLOAT3(0, 90, 0));
	SetEnemyselect(XMFLOAT3(0, -15, -50), XMFLOAT3(0, 90, 0));
	SetEnemyselect(XMFLOAT3(0, -15, 50), XMFLOAT3(0, 90, 0));
	SetEnemyselect(XMFLOAT3(0, -15, 150), XMFLOAT3(0, 90, 0));
	SetEnemyselect(XMFLOAT3(0, -15, 250), XMFLOAT3(0, 90, 0));
	SetEnemyselect(XMFLOAT3(0, -15, 350), XMFLOAT3(0, 90, 0));
	SetEnemyselect(XMFLOAT3(0, -15, 450), XMFLOAT3(0, 90, 0));
	SetEnemyselect(XMFLOAT3(0, -15, 550), XMFLOAT3(0, 90, 0));
	SetEnemyselect(XMFLOAT3(0, -15, 650), XMFLOAT3(0, 90, 0));
	SetEnemyselect(XMFLOAT3(0, -15, 750), XMFLOAT3(0, 90, 0));

	// カメラ更新
	GetCamera()->Init();

	GetEnemy()->Init();

	return hr;
}

//=============================================================================
// タイトル表示の終了処理
//=============================================================================
void UninitSceneBaSelect()
{
	//背景の終了処理
	UninitBg();

	UninitSelTitle();

	UninitBackTex();

	//セレクトの終了処理
	UninitSelectStage();

	UninitPChimera();

	UninitEnemyselect();

	GetEnemy()->Uninit();
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

	UpdateSelTitle();

	UpdateBackTex();

	UpdateSelectEnemyselect();

	GetEnemy()->Update();

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
	DrawSelTitle1();
	DrawBackTex1();
	// Zバッファ有効
	SetZBuffer(true);

	//プレイヤー
	DrawPChimera();
	DrawEnemyselect();

}
