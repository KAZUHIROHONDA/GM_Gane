//=============================================================================
//
// タイトルシーン[sceneGameover.cpp]
// Author : TAKUYA KATOU
//
//=============================================================================
#include "sceneGameover.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"		
#include "bg.h"
#include "collision.h"
#include "stage.h"
#include "fade.h"
#include "gameover.h"
#include "select 2.h"
#include "player.h"
#include "sound.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SAMPLE_TEXTURENAME	_T("data/texture/GO1.jpg")


#define SAMPLE_POS_X	(0)	// 初期位置X
#define SAMPLE_POS_Y	(0)	// 初期位置Y
#define SAMPLE_SIZE_X	(SCREEN_WIDTH)	// 横幅
#define SAMPLE_SIZE_Y	(SCREEN_HEIGHT)	// 縦幅

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTexture;				// テクスチャ
static XMFLOAT3 g_Position;	// 座標

int sNo = 0;


//=============================================================================
// タイトル表示の初期化処理
//=============================================================================
HRESULT InitSceneGameover()
{
	// テクスチャ読み込み
	HRESULT hr =
		CreateTextureFromFile(GetDevice(),
			SAMPLE_TEXTURENAME, &g_pTexture);

	//HRESULT hr = S_OK;
	HWND hWnd = GetMainWnd();

	//背景の初期化処理
	hr = InitBg();
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("背景初期化処理エラー"), _T("エラー"), MB_OK | MB_ICONSTOP);
		return hr;
	}

	//プレイヤーの初期化処理
	hr = InitPlayer();
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("プレイヤー初期化処理エラー"), _T("エラー"), MB_OK | MB_ICONSTOP);
		return hr;
	}

	//serectの初期化処理
	hr = InitSelect2();
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("serect初期化処理エラー"), _T("エラー"), MB_OK | MB_ICONSTOP);
		return hr;
	}
	PlaySound(SOUND_LABEL_BGM_5);

	return hr;
}

//=============================================================================
// タイトル表示の終了処理
//=============================================================================
void UninitSceneGameover()
{
	// テクスチャ開放
	SAFE_RELEASE(g_pTexture);
	//背景の終了処理
	UninitBg();
	//プレイヤーの終了処理
	UninitPlayer();
	//背景の終了処理
	UninitSelect2();

	StopSound();
}

//=============================================================================
// タイトル表示の更新処理
//=============================================================================
void UpdateSceneGameover()
{
	//ゲームオーバーテクスチャ
	UpdateGameover();
	UpdateSelect2();
	//プレイヤー
	UpdatePlayer();

	E_FADE fadeState = GetFade();
	if (fadeState == E_FADE_NONE)
	{
		if (GetKeyTrigger(VK_RETURN)|| GetJoyTrigger(0, 0) || GetMouseTrigger(0))
		{
			//選択中のものにより分岐
			SELECT2_MENU menu = GetSelect2Menu();
			switch (menu)
			{
				//ゲームに戻る
			case SELECT2_MENU_CONTINUE:
				StartFade(SCENE_GAME);
				SetStageNo(sNo);
				break;
				//リトライ
			case SELECT2_MENU_RETRY:
				StartFade(SCENE_TITLE);
				break;
			}
		}
	}
}

//=============================================================================
// タイトル表示処理
//=============================================================================
void DrawSceneGameover()
{

	//サンプル画像描画
	SetPolygonPos(g_Position.x, g_Position.y);	// 座標
	SetPolygonSize(SAMPLE_SIZE_X, SAMPLE_SIZE_Y);	// 大きさ
	SetPolygonAngle(0.0f);				// 角度
	SetPolygonColor(1.0f, 1.0f, 1.0f);	// 色(RGB)
	SetPolygonAlpha(1.0f);				// α値(透明度)

	SetPolygonUV(0.0f, 0.0f);			// 0番のテクスチャ座標
	SetPolygonFrameSize(1.0f, 1.0f);	// テクスチャの横幅,縦幅
	SetPolygonTexture(g_pTexture);		//

	
	 //ポリゴンの描画処理
	DrawPolygon(GetDeviceContext());
	//背景の描画処理
	DrawBg();
	// Zバッファ有効
	SetZBuffer(true);
	
	//プレイヤー
	DrawPlayer();


	// Zバッファ無効
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);
	DrawSelect2();

}
