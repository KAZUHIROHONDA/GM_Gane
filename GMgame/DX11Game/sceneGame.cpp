//=============================================================================
//
// ゲームシーン [sceneGame.cpp]
// Author : TAKUYA KATOU
//
//=============================================================================
#include "sceneGame.h"
#include "stage.h"
#include "AssimpModel.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"	
#include "player.h"
#include "field3D.h"
#include "player.h"
#include "sceneGame.h"
#include "shadow.h"
#include "bullet.h"
#include "gauge.h"
#include "Egauge.h"
#include "wall.h"
#include "enemy.h"
#include "fade.h"
#include "bg.h"
#include "pause.h"
#include "MessageManager.h"
#include "playercs.h"
#include "playerhp.h"
#include "number.h"
#include"enemyhp.h"
#include"jyanken.h"
#include "phasecs.h"
#include "sceneTitle.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static bool g_bPause = false;	//tureでポーズ中
static bool g_bClear = false;

int			eCnt;			//敵を倒した
int			sTime;			//行動不能時間カウント


Phase  phase;


//=============================================================================
//　初期化処理
//=============================================================================
HRESULT InitSceneGame()
{
	HRESULT hr = S_OK;
	HWND hWnd = GetMainWnd();
	

	phase.Init();

	eCnt = 0;
	sTime = 0;

	//背景の初期化処理
	InitBg();
	//丸影初期化処理(ほかのモデルより前)
	hr = InitShadow();
	if (FAILED(hr))
		return hr;

	//プレイヤーの初期化処理
	hr = InitPlayer();
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("プレイヤー初期化処理エラー"), _T("エラー"), MB_OK | MB_ICONSTOP);
		return hr;
	}

	//プレイヤーの初期化処理
	hr = InitJyanken();
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("じゃんけん初期化処理エラー"), _T("エラー"), MB_OK | MB_ICONSTOP);
		return hr;
	}


	// 弾処理初期化
	hr = InitBullet();
	if (FAILED(hr))
		return hr;

	// 敵処理初期化
	hr = InitEnemy();

	// tairyokuの初期化処理
	hr = InitPlayerhp();
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("プレイヤーHP初期化処理エラー"),
			_T("エラー"), MB_OK | MB_ICONSTOP);
		return hr;
	}

	// tairyokuの初期化処理
	hr = InitEnemyhp();
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("プレイヤーHP初期化処理エラー"),
			_T("エラー"), MB_OK | MB_ICONSTOP);
		return hr;
	}


	// 数字の初期化処理
	hr = InitNumber();
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("数字初期化処理エラー"),
			_T("エラー"), MB_OK | MB_ICONSTOP);
		return hr;
	}


	hr = InitGauge();
	if (FAILED(hr))
		return hr;
	
	hr = InitEGauge();
	if (FAILED(hr))
		return hr;

	//地面初期化処理
	hr = InitField3D();
	if (FAILED(hr))
		return hr;

	//壁
	hr = InitWall();
	if (FAILED(hr))
		return hr;

	//ポーズ初期化処理
	hr = InitPause();
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("ポーズ初期化処理エラー"), _T("エラー"), MB_OK | MB_ICONSTOP);
		return hr;

	}
	g_bPause = false;
	g_bClear = false;

	//SetEnemy(XMFLOAT3(0.0f, 70.0f, 200.0f),0);

	//ステージの初期化
	InitStage();

	//文字初期化処理
	InitMessage();


	return hr;
}

//=============================================================================
//　終了処理
//=============================================================================
void UninitSceneGame()
{
	//文字終了処理
	UninitMessage();

	//背景の終了処理
	UninitBg();
	//敵
	UninitEnemy();

	UninitGauge();

	UninitEGauge();

	// tairyokuの終了処理
	UninitPlayerhp();

	// tairyokuの終了処理
	UninitEnemyhp();

	// 数字の終了処理
	UninitNumber();

	//地面終了処理
	UninitField3D();
	//地面終了処理
	UninitWall();
	//プレイヤーの終了処理
	UninitPlayer();

	//プレイヤーの終了処理
	UninitJyanken();

	//弾
	UninitBullet();
	
	//影
	UninitShadow();
	//ポーズ
	UninitPause();

	//デバック用

	phase.Uninit();
}

//=============================================================================
//　更新処理
//=============================================================================
void UpdateSceneGame()
{
	if (g_bPause == true)
	{	//ポーズ更新
		UpdatePause();
	}
	if (g_bClear == true)
	{
		UpdateBullet();
	}
	else if(g_bPause == false && g_bClear == false)
	{
		//デバック
		phase.Update();


		//背景
		UpdateBg();
		//敵
		UpdateEnemy();
		//たま
		UpdateBullet();
		
		//地面更新処理
		UpdateField3D();


		UpdatePlayerhp();
		UpdateEnemyhp();
		
		//地面更新処理
		UpdateWall();
		//当たり判定
		
		//影
		UpdateShadow();
		//ステージ
		UpdateStage();

	}

	//ポーズのオンオフ
	if (GetKeyTrigger(VK_P)|| GetJoyTrigger(0,1)|| GetMouseTrigger(2))
	{
		//フェード中は反応しない用にする
		E_FADE fadeState = GetFade();
		if (fadeState == E_FADE_NONE)
		{
			//[フラグの切り替え]
			//g_bPauseを"true"なら"false"に"false"なら"true"にする
			g_bPause = !g_bPause;

			//ポーズ開始時にやること
			if (g_bPause == true)
			{
				ResetPauseMenu();
			}
		}
	}

	//ポーズ中の項目決定
	if (g_bPause == true)
	{
		E_FADE fadeState = GetFade();
		if (fadeState == E_FADE_NONE)
		{
			if (GetKeyTrigger(VK_RETURN)|| GetJoyTrigger(0, 0)|| GetMouseTrigger(0))
			{
				//選択中のものにより分岐
				PAUSE_MENU menu = GetPauseMenu();
				switch (menu)
				{
					//ゲームに戻る
				case PAUSE_MENU_CONTINUE:
					g_bPause = !g_bPause;
					break;
					//リトライ
				case PAUSE_MENU_RETRY:
					StartFade(SCENE_GAME);
					break;
					//終了(タイトルへ)
				case PAUSE_MENU_QUIT:
					StartFade(SCENE_TITLE);
					break;
				}
			}
		}

	}
		
	//文字更新処理
	UpdateMessage();
}

//=============================================================================
//　表示処理
//=============================================================================
void DrawSceneGame()
{	

	//半透明オブジェクト(3Dの中でも後ろにかく)
	SetZWrite(false);
	//背景
	DrawBg();

	// Zバッファ有効
	SetZBuffer(true);
	//プレイヤー
	DrawPlayer();
	//敵
	DrawEnemy();
	//地面
	DrawField3D();

	//光源処理無効
	GetLight()->SetDisable();
	//地面
	DrawWall();
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
	//弾
	DrawBullet();
	//爆発
	//DrawExplosion();
	DrawShadow();
	//ゲージ
	DrawPlayerGauge();

	DrawEnemyEGauge();
	//光源処理有効
	GetLight()->SetEnable();

	//文字描画
	DrawMessage();

	GetPlayer()->Draw(50,100);
	GetPlayer()->Draw(1100, 100);
	phase.Draw();

	DrawPlayerhp();
	DrawEnemyhp();
	
	//ポーズ画面
	if (true == g_bPause)
	{
		DrawPause();
	}
}

Phase* GetPhase()
{
	return &phase;
}


