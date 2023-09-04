//=============================================================================
//
// タイトルシーン[sceneChimera.cpp]
// Author : TAKUYA KATOU
//
//=============================================================================
#include "sceneChimera.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"		
#include "bg.h"
#include "collision.h"
#include "fade.h"
#include "chimeracs.h"
#include "pchimera.h"
#include "Light.h"
#include "Camera.h"
#include "tab.h"
#include "tex.h"
#include "playercs.h"
#include "sceneTitle.h"
#include "Bpartsmenu.h"
#include "Mpartsmenu.h"
#include "partsmenu.h"
#include "Upartsmenu.h"



//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SAMPLE_TEXTURENAME	_T("data/texture/tex.png")


#define SAMPLE_POS_X	(0)	// 初期位置X
#define SAMPLE_POS_Y	(0)	// 初期位置Y
#define SAMPLE_SIZE_X	(SCREEN_WIDTH)	// 横幅
#define SAMPLE_SIZE_Y	(SCREEN_HEIGHT)	// 縦幅

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTexture;				// テクスチャ
static XMFLOAT3 g_Position;	// 座標

Chimera player;//プレイヤ

//=============================================================================
// タイトル表示の初期化処理
//=============================================================================
HRESULT InitSceneChimera()
{
	// テクスチャ読み込み
	HRESULT hr =
		CreateTextureFromFile(GetDevice(),
			SAMPLE_TEXTURENAME, &g_pTexture);

	//HRESULT hr = S_OK;
	HWND hWnd = GetMainWnd();


	//プレイヤーの初期化処理
	hr = InitPChimera();
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("プレイヤー初期化処理エラー"), _T("エラー"), MB_OK | MB_ICONSTOP);
		return hr;
	}

	InitBg();

	InitTab();

	InitTex();

	Sflag();

	// カメラ更新
	GetCamera()->Init();

	player.Init();

	GetPlayer()->Percent();

	return hr;
}

//=============================================================================
// タイトル表示の終了処理
//=============================================================================
void UninitSceneChimera()
{

	UninitPChimera();

	UninitBg();

	UninitTab();

	UninitTex();

	player.Uninit();
}

//=============================================================================
// タイトル表示の更新処理
//=============================================================================
void UpdateSceneChimera()
{
	// カメラ更新
	GetCamera()->Update();

	UpdatePChimera();

	UpdateBg();

	player.Update();

	UpdateTab();

	UpdateTex();

	
	switch (BCharaNo())
	{
	case 0://かぶ
	{
		switch (CharaNo())//あたま
		{
		case 0: {	//かぶ
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 1: {//わに
			PartsGet()->Setscl(XMFLOAT3(1.8f, 1.8f, 1.8f));
			break; }
		case 2: {//ぱんだ
			PartsGet()->Setscl(XMFLOAT3(2.5f, 2.5f, 2.5f));
			break; }
		case 3: {//とり
			PartsGet()->Setscl(XMFLOAT3(1.2f, 1.2f, 1.2f));
			break; }
		case 4: {//ぶた
			PartsGet()->Setscl(XMFLOAT3(2.5f, 2.5f, 2.5f));
			break; }
		case 5: {//うし
			PartsGet()->Setscl(XMFLOAT3(0.8f, 0.8f, 0.8f));
			break; }
		default:
			break;
		}
		switch (UCharaNo())//うしろあし
		{
		case 0: {	//かぶ
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 1: {//わに
			UPartsGet()->Setscl(XMFLOAT3(1.1f, 1.1f, 1.1f));
			break; }
		case 2: {//ぱんだ
			UPartsGet()->Setscl(XMFLOAT3(2.2f, 2.2f, 2.2f));
			break; }
		case 3: {//とり
			UPartsGet()->Setscl(XMFLOAT3(3.0f, 3.0f, 3.0f));
			break; }
		case 4: {//ぶた
			UPartsGet()->Setscl(XMFLOAT3(2.5f, 2.5f, 2.5f));
			break; }
		case 5: {//うし
			UPartsGet()->Setscl(XMFLOAT3(0.6f, 0.6f, 0.6f));
			break; }
		default:
			break;
		}
		switch (MCharaNo())//まえあし
		{
		case 0: {	//かぶ
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 1: {//わに
			MPartsGet()->Setscl(XMFLOAT3(1.1f, 1.1f, 1.1f));
			break; }
		case 2: {//ぱんだ
			MPartsGet()->Setscl(XMFLOAT3(2.2f, 2.2f, 2.2f));
			break; }
		case 3: {//とり
			MPartsGet()->Setscl(XMFLOAT3(3.0f, 3.0f, 3.0f));
			break; }
		case 4: {//ぶた
			MPartsGet()->Setscl(XMFLOAT3(2.5f, 2.5f, 2.5f));
			break; }
		case 5: {//うし
			MPartsGet()->Setscl(XMFLOAT3(0.6f, 0.6f, 0.6f));
			break; }
		default:
			break;
		}
		break;
	}
	case 1://わに
	{
		switch (CharaNo())//あたま
		{
		case 0: {	//かぶ
			PartsGet()->Setscl(XMFLOAT3(0.8f, 0.8f, 0.8f));
			break; }
		case 1: {//わに
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 2: {//ぱんだ
			PartsGet()->Setscl(XMFLOAT3(1.6f, 1.6f, 1.6f));
			break; }
		case 3: {//とり
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 4: {//ぶた
			PartsGet()->Setscl(XMFLOAT3(1.6f, 1.6f, 1.6f));
			break; }
		case 5: {//うし
			PartsGet()->Setscl(XMFLOAT3(0.5f, 0.5f, 0.5f));
			break; }
		default:
			break;
		}
		switch (UCharaNo())//うしろあし
		{
		case 0: {	//かぶ
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 1: {//わに
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 2: {//ぱんだ
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 3: {//とり
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 4: {//ぶた
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 5: {//うし
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		default:
			break;
		}
		switch (MCharaNo())//まえあし
		{
		case 0: {	//かぶ
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 1: {//わに
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 2: {//ぱんだ
			MPartsGet()->Setscl(XMFLOAT3(1.6f, 1.6f, 1.6f));
			break; }
		case 3: {//とり
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 4: {//ぶた
			MPartsGet()->Setscl(XMFLOAT3(1.6f, 1.6f, 1.6f));
			break; }
		case 5: {//うし
			MPartsGet()->Setscl(XMFLOAT3(0.4f, 0.4f, 0.4f));
			break; }
		default:
			break;
		}
		break;
	}
	case 2://ぱんだ
	{
		switch (CharaNo())
		{
		case 0: {	//かぶ
			PartsGet()->Setscl(XMFLOAT3(0.5f, 0.5f, 0.5f));
			break; }
		case 1: {//わに
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 2: {//ぱんだ
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 3: {//とり
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 4: {//ぶた
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 5: {//うし
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		default:
			break;
		}
		switch (UCharaNo())
		{
		case 0: {	//かぶ
			UPartsGet()->Setscl(XMFLOAT3(0.5f, 0.5f, 0.5f));
			break; }
		case 1: {//わに
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 2: {//ぱんだ
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 3: {//とり
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 4: {//ぶた
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 5: {//うし
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		default:
			break;
		}
		switch (MCharaNo())
		{
		case 0: {	//かぶ
			MPartsGet()->Setscl(XMFLOAT3(0.5f, 0.5f, 0.5f));
			break; }
		case 1: {//わに
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 2: {//ぱんだ
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 3: {//とり
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 4: {//ぶた
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 5: {//うし
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		default:
			break;
		}
		break;
	}
	case 3://とり
	{
		switch (CharaNo())
		{
		case 0: {	//かぶ
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 1: {//わに
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 2: {//ぱんだ
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 3: {//とり
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 4: {//ぶた
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 5: {//うし
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		default:
			break;
		}
		switch (UCharaNo())
		{
		case 0: {	//かぶ
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 1: {//わに
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 2: {//ぱんだ
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 3: {//とり
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 4: {//ぶた
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 5: {//うし
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		default:
			break;
		}
		switch (MCharaNo())
		{
		case 0: {	//かぶ
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 1: {//わに
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 2: {//ぱんだ
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 3: {//とり
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 4: {//ぶた
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 5: {//うし
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		default:
			break;
		}
		break;
	}
	case 4://ぶた
	{
		switch (CharaNo())
		{
		case 0: {	//かぶ
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 1: {//わに
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 2: {//ぱんだ
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 3: {//とり
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 4: {//ぶた
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 5: {//うし
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		default:
			break;
		}
		switch (UCharaNo())
		{
		case 0: {	//かぶ
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 1: {//わに
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 2: {//ぱんだ
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 3: {//とり
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 4: {//ぶた
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 5: {//うし
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		default:
			break;
		}
		switch (MCharaNo())
		{
		case 0: {	//かぶ
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 1: {//わに
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 2: {//ぱんだ
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 3: {//とり
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 4: {//ぶた
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 5: {//うし
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		default:
			break;
		}
		break;
	}
	case 5://うし
	{
		switch (CharaNo())
		{
		case 0: {	//かぶ
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 1: {//わに
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 2: {//ぱんだ
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 3: {//とり
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 4: {//ぶた
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 5: {//うし
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		default:
			break;
		}
		switch (UCharaNo())
		{
		case 0: {	//かぶ
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 1: {//わに
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 2: {//ぱんだ
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 3: {//とり
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 4: {//ぶた
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 5: {//うし
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		default:
			break;
		}
		switch (MCharaNo())
		{
		case 0: {	//かぶ
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 1: {//わに
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 2: {//ぱんだ
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 3: {//とり
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 4: {//ぶた
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 5: {//うし
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		default:
			break;
		}
		break;
	}
	default:
		break;
	}

	Sflag();

	if (GetMouseTrigger(1))
	{
		StartFade(SCENE_SELECT);
	}

}

//=============================================================================
// タイトル表示処理
//=============================================================================
void DrawSceneChimera()
{

	// Zバッファ無効
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);
	//半透明オブジェクト(3Dの中でも後ろにかく)
	//SetZWrite(true);

	//背景
	DrawBg1();

	DrawTex();

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

	DrawTab();

	player.Draw();

	GetPlayer()->DrawChimera(50,50);

}

Chimera* Get()
{
	return &player;
}
