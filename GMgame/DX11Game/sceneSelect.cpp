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
#include "stage.h"
#include "select1.h"
#include "selectstage.h"


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
	
	return hr;
}

//=============================================================================
// タイトル表示の終了処理
//=============================================================================
void UninitSceneSelect()
{
	//背景の終了処理
	UninitBg();

}

//=============================================================================
// タイトル表示の更新処理
//=============================================================================
void UpdateSceneSelect()
{
	//背景の更新処理
	UpdateBg();

	

}

//=============================================================================
// タイトル表示処理
//=============================================================================
void DrawSceneSelect()
{
	SetZWrite(false);
	//背景の描画処理
	DrawBg();

	// Zバッファ有効
	SetZBuffer(true);

	// Zバッファ無効
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);
	

}
