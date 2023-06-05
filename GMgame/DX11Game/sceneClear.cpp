//=============================================================================
//
// クリアシーン[sceneClear.cpp]
// Author : TAKUYA KATOU
//
//=============================================================================
#include "sceneClear.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"		
#include "bg.h"
#include "collision.h"
#include "fade.h"
#include "sceneGameover.h"
#include "sceneSelect.h"
#include"select.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SAMPLE_TEXTURENAME	_T("data/texture/GC.jpg")


#define SAMPLE_POS_X	(0)	// 初期位置X
#define SAMPLE_POS_Y	(0)	// 初期位置Y
#define SAMPLE_SIZE_X	(SCREEN_WIDTH)	// 横幅
#define SAMPLE_SIZE_Y	(SCREEN_HEIGHT)	// 縦幅

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTexture;				// テクスチャ
static XMFLOAT3 g_Position;	// 座標

//=============================================================================
// タイトル表示の初期化処理
//=============================================================================
HRESULT InitSceneClear()
{
	// テクスチャ読み込み
	HRESULT hr =
		CreateTextureFromFile(GetDevice(),
			SAMPLE_TEXTURENAME, &g_pTexture);

	if (FAILED(hr)) return hr;
	//return E_NOTIMPL;
	//HRESULT hr = S_OK;
	HWND hWnd = GetMainWnd();

	//背景の初期化処理
	hr = InitBg();
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("背景初期化処理エラー"), _T("エラー"), MB_OK | MB_ICONSTOP);
		return hr;
	}
	
	//serectの初期化処理
	hr = InitSelect();
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("serect初期化処理エラー"), _T("エラー"), MB_OK | MB_ICONSTOP);
		return hr;
	}


	return hr;
}

//=============================================================================
// タイトル表示の終了処理
//=============================================================================
void UninitSceneClear()
{

	// テクスチャ開放
	SAFE_RELEASE(g_pTexture);
	//背景の終了処理
	UninitBg();
	//背景の終了処理
	UninitSelect();
}

//=============================================================================
// タイトル表示の更新処理
//=============================================================================
void UpdateSceneClear()
{
	//背景の更新処理
	UpdateBg();
	UpdateSelect();
	E_FADE fadeState = GetFade();
	if (fadeState == E_FADE_NONE)
	{
		if (GetKeyTrigger(VK_RETURN) || GetJoyTrigger(0, 0))
		{
			//選択中のものにより分岐
			SELECT_MENU menu = GetSelectMenu();
			switch (menu)
			{
				//ゲームに戻る
			case SELECT_MENU_CONTINUE:
				StartFade(SCENE_GAME);
				//PlaySound(SOUND_LABEL_SE_DECIDE);
				//sCnt++;
				//SetStageNo(sCnt);
				//sNO();
				break;
				//リトライ
			case SELECT_MENU_RETRY:
				StartFade(SCENE_GAME);
				//PlaySound(SOUND_LABEL_SE_DECIDE);
				break;
				//終了(タイトルへ)
			case SELECT_MENU_QUIT:
				StartFade(SCENE_TITLE);
				//PlaySound(SOUND_LABEL_SE_DECIDE);
				//sCnt = 0;
				break;
			}
		}
	}

	/*if (GetKeyTrigger(VK_RETURN))
	{
		//エンターキー押下でゲーム画面へ
		StartFade(SCENE_TITLE, 60);
	}*/
}

//=============================================================================
// タイトル表示処理
//=============================================================================
void DrawSceneClear()
{

	// サンプル画像描画
	SetPolygonPos(g_Position.x, g_Position.y);	// 座標
	SetPolygonSize(SAMPLE_SIZE_X, SAMPLE_SIZE_Y);	// 大きさ
	SetPolygonAngle(0.0f);				// 角度
	SetPolygonColor(1.0f, 1.0f, 1.0f);	// 色(RGB)
	SetPolygonAlpha(1.0f);				// α値(透明度)

	SetPolygonUV(0.0f, 0.0f);			// 0番のテクスチャ座標
	SetPolygonFrameSize(1.0f, 1.0f);	// テクスチャの横幅,縦幅
	SetPolygonTexture(g_pTexture);		// テクスチャ指定


	
	 //ポリゴンの描画処理
	DrawPolygon(GetDeviceContext());
	// Zバッファ有効
	SetZBuffer(true);

	//背景の描画処理
	DrawBg();
	

	// Zバッファ無効
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);
	DrawSelect();


}
