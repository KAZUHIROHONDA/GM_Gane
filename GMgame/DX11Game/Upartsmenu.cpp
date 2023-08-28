//=============================================================================
//
// プレイヤー処理 [Upartsmenu.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include <stdarg.h>
#include <string.h>
#include "Upartsmenu.h"
#include "polygon.h"	// ポリゴン使用
#include "Texture.h"// テクスチャ使用
#include "input.h"
#include "sound.h"
#include "collision.h"
#include "model.h"
#include "pchimera.h"
#include "playercs.h"
#include "sceneTitle.h"

// マクロ定義
#define	NUM_UPARTS_MENU		(6)			// ポーズメニュー数
#define	UPARTS_MENU_WIDTH	(200.0f)	// ポーズメニュー幅
#define	UPARTS_MENU_HEIGHT	(200.0f)		// ポーズメニュー高さ
#define	UPARTS_MENU_INTERVAL	(300.0f)	// ポーズメニュー間隔
#define	UPARTS_MENU_POS_X	(200.0f)		// ポーズメニュー位置(X座標)
#define	UPARTS_MENU_POS_Y	(250.0f)	// ポーズメニュー位置(Y座標)
#define	PLATE_WIDTH			(360.0f)	// プレートの幅
#define	PLATE_HEIGHT		(340.0f)	// プレートの幅
#define	PLATE_POS_X			(0.0f)		// プレートの位置(X座標)
#define	PLATE_POS_Y			(0.0f)		// プレートの位置(Y座標)

// 構造体定義
// プロトタイプ宣言
// グローバル変数
static ID3D11ShaderResourceView*	g_pTextures[7] = { nullptr };	// テクスチャへのポインタ

static float g_fCurve = 0.0f;
static float g_fCol = 0.0f;
static int	 nStopTime = 0;

static LPCWSTR c_aFileNameUPartsMenu[NUM_UPARTS_MENU] =
{
	L"data/TEXTURE/kabu3.png",	
	L"data/TEXTURE/wani3.png",	
	L"data/TEXTURE/panda3.png",	
	L"data/TEXTURE/wani3.png",	
	L"data/TEXTURE/buta3.png",	
	L"data/TEXTURE/usi3.png",	
};

static UPARTS_MENU g_nUPartsMenu = UPARTS_MENU_KABU;	//	選択中のメニューNo

Player UParts;

// 初期化処理
HRESULT InitUParts(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr;

	for (int nCntUPartsMenu = 0; nCntUPartsMenu < NUM_UPARTS_MENU; ++nCntUPartsMenu) {
		// テクスチャの読み込み
		hr = CreateTextureFromFile(pDevice,									// デバイスへのポインタ
			c_aFileNameUPartsMenu[nCntUPartsMenu],	// ファイルの名前
			&g_pTextures[nCntUPartsMenu]);			// 読み込むメモリー
	}

	UParts.SetHP(10);
	UParts.SetPAat(15);
	UParts.SetGUat(20);
	UParts.SetTYOKIat(25);

	g_nUPartsMenu = UPARTS_MENU_KABU;
	g_fCurve = 0.0f;

	return hr;
}

void UninitUParts(void)
{
	// テクスチャの開放
	for (int nCntUPartsMenu = 0; nCntUPartsMenu < NUM_UPARTS_MENU; ++nCntUPartsMenu) {
		SAFE_RELEASE(g_pTextures[nCntUPartsMenu]);
	}
}

void UpdateUParts(void)
{

	//操作不能時間
	if (nStopTime > 0)
	{
		nStopTime--;
	}
	if (nStopTime == 0)
	{
		if (GetJoyCountSimple() == 1)
		{
			float gx, gy, len2;
			gx = (float)GetJoyX(0) / 0x08000; //-1.0~1.0
			gy = (float)GetJoyY(0) / 0x08000; //に丸める

			len2 = gx * gx + gy * gy;

			if (len2 > 0.3f*0.3f)
			{
				//遊びを省く
				float angle, len;
				angle = atan2f(gy, gx);//角度
				angle = XMConvertToDegrees(angle);

				len = sqrtf(len2);

				if (angle < -135 || angle > -45)
				{
					len *= 0.3f;
					g_nUPartsMenu = (UPARTS_MENU)((g_nUPartsMenu + 1) % NUM_UPARTS_MENU);
					SetUPartsMenu();
					nStopTime = 10;
				}
				if (angle < 135 || angle > 45)
				{
					len *= 0.3f;
					g_nUPartsMenu = (UPARTS_MENU)((g_nUPartsMenu + NUM_UPARTS_MENU - 1) % NUM_UPARTS_MENU);
					SetUPartsMenu();
					nStopTime = 10;
				}


			}
		}
	}

	//マウス
	POINT temp = (*GetMousePosition());
	XMFLOAT2 mousePos = XMFLOAT2(temp.x - SCREEN_CENTER_X, -(temp.y - SCREEN_CENTER_Y));
	XMFLOAT2 pos1 = XMFLOAT2(UPARTS_MENU_POS_X, UPARTS_MENU_POS_Y);
	XMFLOAT2 pos2 = XMFLOAT2(UPARTS_MENU_POS_X + 1 * UPARTS_MENU_INTERVAL, UPARTS_MENU_POS_Y );
	XMFLOAT2 pos3 = XMFLOAT2(UPARTS_MENU_POS_X, UPARTS_MENU_POS_Y - 1 * 250);
	XMFLOAT2 pos4 = XMFLOAT2(UPARTS_MENU_POS_X + 1 * 250, UPARTS_MENU_POS_Y - 1 * 250);
	XMFLOAT2 pos5 = XMFLOAT2(UPARTS_MENU_POS_X, UPARTS_MENU_POS_Y - 2 * 250);
	XMFLOAT2 pos6 = XMFLOAT2(UPARTS_MENU_POS_X + 1 * 250, UPARTS_MENU_POS_Y - 2 * 250);
	XMFLOAT2 radius1 = XMFLOAT2(UPARTS_MENU_WIDTH / 2, UPARTS_MENU_HEIGHT / 2);
	XMFLOAT2 mpos2 = mousePos;
	XMFLOAT2 radius2 = XMFLOAT2(0.1, 0.1);
	if (CollisionBB(&pos1, &radius1, &mpos2, &radius2))
	{
		if (GetMouseTrigger(0))
		{
			SetBack(MODEL_KABU3);
			UParts.SetHP(10);
			UParts.SetPAat(15);
			UParts.SetGUat(20);
			UParts.SetTYOKIat(25);
			//手札、0グー,1チョキ,2パー
			GetPlayer()->SetHand(10, 1);
			GetPlayer()->SetHand(11, 1);
			GetPlayer()->SetHand(12, 1);
			GetPlayer()->SetHand(13, 1);
			GetPlayer()->SetHand(14, 1);

			InitPChimera();
		}
	}
	else if (CollisionBB(&pos2, &radius1, &mpos2, &radius2))
	{
		if (GetMouseTrigger(0))
		{
			SetBack(MODEL_wani3);
			UParts.SetHP(10);
			UParts.SetPAat(15);
			UParts.SetGUat(20);
			UParts.SetTYOKIat(25);
			//手札、0グー,1チョキ,2パー
			GetPlayer()->SetHand(10, 0);
			GetPlayer()->SetHand(11, 0);
			GetPlayer()->SetHand(12, 0);
			GetPlayer()->SetHand(13, 0);
			GetPlayer()->SetHand(14, 0);

			InitPChimera();
		}
	}
	else if (CollisionBB(&pos3, &radius1, &mpos2, &radius2))
	{

		if (GetMouseTrigger(0))
		{
			SetBack(MODEL_PANDA3);
			UParts.SetHP(50);
			UParts.SetPAat(50);
			UParts.SetGUat(50);
			UParts.SetTYOKIat(50);
			//手札、0グー,1チョキ,2パー
			GetPlayer()->SetHand(10, 1);
			GetPlayer()->SetHand(11, 1);
			GetPlayer()->SetHand(12, 1);
			GetPlayer()->SetHand(13, 1);
			GetPlayer()->SetHand(14, 1);

			InitPChimera();
		}
	}
	else if (CollisionBB(&pos4, &radius1, &mpos2, &radius2))
	{

		if (GetMouseTrigger(0))
		{
			SetBack(MODEL_TORI3);
			UParts.SetHP(50);
			UParts.SetPAat(50);
			UParts.SetGUat(50);
			UParts.SetTYOKIat(50);
			//手札、0グー,1チョキ,2パー
			GetPlayer()->SetHand(10, 2);
			GetPlayer()->SetHand(11, 2);
			GetPlayer()->SetHand(12, 2);
			GetPlayer()->SetHand(13, 2);
			GetPlayer()->SetHand(14, 2);

			InitPChimera();
		}
	}
	else if (CollisionBB(&pos5, &radius1, &mpos2, &radius2))
	{

		if (GetMouseTrigger(0))
		{
			SetBack(MODEL_BUTA3);
			UParts.SetHP(50);
			UParts.SetPAat(50);
			UParts.SetGUat(50);
			UParts.SetTYOKIat(50);
			//手札、0グー,1チョキ,2パー
			GetPlayer()->SetHand(10, 2);
			GetPlayer()->SetHand(11, 2);
			GetPlayer()->SetHand(12, 2);
			GetPlayer()->SetHand(13, 2);
			GetPlayer()->SetHand(14, 2);

			InitPChimera();
		}
	}
	else if (CollisionBB(&pos6, &radius1, &mpos2, &radius2))
	{

		if (GetMouseTrigger(0))
		{
			SetBack(MODEL_USI3);
			UParts.SetHP(50);
			UParts.SetPAat(50);
			UParts.SetGUat(50);
			UParts.SetTYOKIat(50);
			//手札、0グー,1チョキ,2パー
			GetPlayer()->SetHand(10, 2);
			GetPlayer()->SetHand(11, 2);
			GetPlayer()->SetHand(12, 2);
			GetPlayer()->SetHand(13, 2);
			GetPlayer()->SetHand(14, 2);

			InitPChimera();
		}
	}


	// 上下キーで各項目間の移動
	if (GetKeyRepeat(VK_W) || GetKeyRepeat(VK_UP)) {
		g_nUPartsMenu = (UPARTS_MENU)((g_nUPartsMenu + NUM_UPARTS_MENU - 1) % NUM_UPARTS_MENU);
		SetUPartsMenu();
	}
	else if (GetKeyRepeat(VK_S) || GetKeyRepeat(VK_DOWN)) {
		g_nUPartsMenu = (UPARTS_MENU)((g_nUPartsMenu + 1) % NUM_UPARTS_MENU);
		SetUPartsMenu();
	}

	// 枠の部分の色の変化(グラデーション)
	g_fCurve += XM_PI * 0.01f;
	if (g_fCurve > XM_PI) {
		g_fCurve -= XM_2PI;
	}

	g_fCol = cosf(g_fCurve) * 0.2f + 0.8f;
}

void DrawUParts(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	int nCnt = 0;

	//ポーズメニューの表示
	SetPolygonSize(UPARTS_MENU_WIDTH, UPARTS_MENU_HEIGHT);
	for (int nCntBPartsMenu = 0; nCntBPartsMenu < 3; ++nCntBPartsMenu) {

		for (int nCntBPartsMenu1 = 0; nCntBPartsMenu1 < 2; ++nCntBPartsMenu1) {
			SetPolygonPos(UPARTS_MENU_POS_X + nCntBPartsMenu1 * UPARTS_MENU_INTERVAL, UPARTS_MENU_POS_Y - nCntBPartsMenu * 250);

			// テクスチャの設定
			SetPolygonTexture(g_pTextures[nCnt]);
			// ポリゴンの描画
			DrawPolygon(pDeviceContext);
			nCnt++;
		}
	}

	SetPolygonColor(1.0f, 1.0f, 1.0f);
}


//=============================================================================
// ポーズメニューの設定
//=============================================================================
void SetUPartsMenu(void)
{
	g_fCurve = 0.0f;
}

//=============================================================================
// ポーズメニューの取得
//=============================================================================
UPARTS_MENU GetUPartsMenu(void)
{
	return g_nUPartsMenu;
}

//=============================================================================
// ポーズメニューのリセット
//=============================================================================
void ResetUPartsMenu(void)
{
	g_nUPartsMenu = UPARTS_MENU_KABU;
	SetUPartsMenu();
}
void ResetUPartsMenu1(void)
{
	g_nUPartsMenu = UPARTS_MENU_WANI;
	SetUPartsMenu();
}
void ResetUPartsMenu2(void)
{
	//g_nUPartsMenu = UPARTS_MENU_QUIT;
	//SetUPartsMenu();
}

Player* UPartsGet()
{
	return &UParts;
}

