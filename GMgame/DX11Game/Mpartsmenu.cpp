//=============================================================================
//
// プレイヤー処理 [Mpartsmenu.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include <stdarg.h>
#include <string.h>
#include "Mpartsmenu.h"
#include "polygon.h"	// ポリゴン使用
#include "Texture.h"// テクスチャ使用
#include "input.h"
#include "sound.h"
#include "collision.h"
#include "model.h"
#include "pchimera.h"
#include "sceneTitle.h"

// マクロ定義
#define	NUM_MPARTS_MENU		(4)			// ポーズメニュー数
#define	MPARTS_MENU_WIDTH	(200.0f)	// ポーズメニュー幅
#define	MPARTS_MENU_HEIGHT	(200.0f)		// ポーズメニュー高さ
#define	MPARTS_MENU_INTERVAL	(300.0f)	// ポーズメニュー間隔
#define	MPARTS_MENU_POS_X	(200.0f)		// ポーズメニュー位置(X座標)
#define	MPARTS_MENU_POS_Y	(200.0f)	// ポーズメニュー位置(Y座標)
#define	PLATE_WIDTH			(360.0f)	// プレートの幅
#define	PLATE_HEIGHT		(340.0f)	// プレートの幅
#define	PLATE_POS_X			(0.0f)		// プレートの位置(X座標)
#define	PLATE_POS_Y			(0.0f)		// プレートの位置(Y座標)

// 構造体定義
// プロトタイプ宣言
// グローバル変数
static ID3D11ShaderResourceView*	g_pTextures[5] = { nullptr };	// テクスチャへのポインタ

static float g_fCurve = 0.0f;
static float g_fCol = 0.0f;
static int	 nStopTime = 0;

static LPCWSTR c_aFileNameMPartsMenu[NUM_MPARTS_MENU] =
{
	L"data/TEXTURE/kabu2.png",	// コンティニュー
	L"data/TEXTURE/wani2.png",	// リトライ
	L"data/TEXTURE/panda2.png",	// リトライ
	L"data/TEXTURE/wani2.png",	// リトライ
};

static MPARTS_MENU g_nMPartsMenu = MPARTS_MENU_KABU;	//	選択中のメニューNo

Player MParts;

// 初期化処理
HRESULT InitMParts(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr;

	for (int nCntMPartsMenu = 0; nCntMPartsMenu < NUM_MPARTS_MENU; ++nCntMPartsMenu) {
		// テクスチャの読み込み
		hr = CreateTextureFromFile(pDevice,									// デバイスへのポインタ
			c_aFileNameMPartsMenu[nCntMPartsMenu],	// ファイルの名前
			&g_pTextures[nCntMPartsMenu]);			// 読み込むメモリー
	}

	MParts.SetHP(10);
	MParts.SetPAat(15);
	MParts.SetGUat(20);
	MParts.SetTYOKIat(25);

	g_nMPartsMenu = MPARTS_MENU_KABU;
	g_fCurve = 0.0f;

	return hr;
}

void UninitMParts(void)
{
	// テクスチャの開放
	for (int nCntMPartsMenu = 0; nCntMPartsMenu < NUM_MPARTS_MENU; ++nCntMPartsMenu) {
		SAFE_RELEASE(g_pTextures[nCntMPartsMenu]);
	}
}

void UpdateMParts(void)
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
					g_nMPartsMenu = (MPARTS_MENU)((g_nMPartsMenu + 1) % NUM_MPARTS_MENU);
					SetMPartsMenu();
					nStopTime = 10;
				}
				if (angle < 135 || angle > 45)
				{
					len *= 0.3f;
					g_nMPartsMenu = (MPARTS_MENU)((g_nMPartsMenu + NUM_MPARTS_MENU - 1) % NUM_MPARTS_MENU);
					SetMPartsMenu();
					nStopTime = 10;
				}


			}
		}
	}

	//マウス
	POINT temp = (*GetMousePosition());
	XMFLOAT2 mousePos = XMFLOAT2(temp.x - SCREEN_CENTER_X, -(temp.y - SCREEN_CENTER_Y));
	XMFLOAT2 pos1 = XMFLOAT2(MPARTS_MENU_POS_X, MPARTS_MENU_POS_Y);
	XMFLOAT2 pos2 = XMFLOAT2(MPARTS_MENU_POS_X + 1 * MPARTS_MENU_INTERVAL, MPARTS_MENU_POS_Y );
	XMFLOAT2 pos3 = XMFLOAT2(MPARTS_MENU_POS_X, MPARTS_MENU_POS_Y - 1 * 250);
	XMFLOAT2 pos4 = XMFLOAT2(MPARTS_MENU_POS_X + 1 * 250, MPARTS_MENU_POS_Y - 1 * 250);
	XMFLOAT2 radius1 = XMFLOAT2(MPARTS_MENU_WIDTH / 2, MPARTS_MENU_HEIGHT / 2);
	XMFLOAT2 mpos2 = mousePos;
	XMFLOAT2 radius2 = XMFLOAT2(0.1, 0.1);
	if (CollisionBB(&pos1, &radius1, &mpos2, &radius2))
	{
		ResetMPartsMenu();
		if (GetMouseTrigger(0))
		{
			SetMae(MODEL_KABU2);
			MParts.SetHP(10);
			MParts.SetPAat(15);
			MParts.SetGUat(20);
			MParts.SetTYOKIat(25);
			InitPChimera();
		}
	}
	else if (CollisionBB(&pos2, &radius1, &mpos2, &radius2))
	{
		ResetMPartsMenu1();
		if (GetMouseTrigger(0))
		{
			SetMae(MODEL_wani2);
			MParts.SetHP(20);
			MParts.SetPAat(35);
			MParts.SetGUat(40);
			MParts.SetTYOKIat(55);
			InitPChimera();
		}
	}
	else if (CollisionBB(&pos3, &radius1, &mpos2, &radius2))
	{

		if (GetMouseTrigger(0))
		{
			SetMae(MODEL_PANDA3);
			MParts.SetHP(50);
			MParts.SetPAat(50);
			MParts.SetGUat(50);
			MParts.SetTYOKIat(50);
			InitPChimera();
		}
	}
	else if (CollisionBB(&pos4, &radius1, &mpos2, &radius2))
	{

		if (GetMouseTrigger(0))
		{
			SetMae(MODEL_TORI3);
			MParts.SetHP(50);
			MParts.SetPAat(50);
			MParts.SetGUat(50);
			MParts.SetTYOKIat(50);
			InitPChimera();
		}
	}


	// 上下キーで各項目間の移動
	if (GetKeyRepeat(VK_W) || GetKeyRepeat(VK_UP)) {
		g_nMPartsMenu = (MPARTS_MENU)((g_nMPartsMenu + NUM_MPARTS_MENU - 1) % NUM_MPARTS_MENU);
		SetMPartsMenu();
	}
	else if (GetKeyRepeat(VK_S) || GetKeyRepeat(VK_DOWN)) {
		g_nMPartsMenu = (MPARTS_MENU)((g_nMPartsMenu + 1) % NUM_MPARTS_MENU);
		SetMPartsMenu();
	}

	// 枠の部分の色の変化(グラデーション)
	g_fCurve += XM_PI * 0.01f;
	if (g_fCurve > XM_PI) {
		g_fCurve -= XM_2PI;
	}

	g_fCol = cosf(g_fCurve) * 0.2f + 0.8f;
}

void DrawMParts(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	int nCnt = 0;

	//ポーズメニューの表示
	SetPolygonSize(MPARTS_MENU_WIDTH, MPARTS_MENU_HEIGHT);
	for (int nCntBPartsMenu = 0; nCntBPartsMenu < 2; ++nCntBPartsMenu) {

		for (int nCntBPartsMenu1 = 0; nCntBPartsMenu1 < 2; ++nCntBPartsMenu1) {
			SetPolygonPos(MPARTS_MENU_POS_X + nCntBPartsMenu1 * MPARTS_MENU_INTERVAL,MPARTS_MENU_POS_Y - nCntBPartsMenu * 250);

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
void SetMPartsMenu(void)
{
	g_fCurve = 0.0f;
}

//=============================================================================
// ポーズメニューの取得
//=============================================================================
MPARTS_MENU GetMPartsMenu(void)
{
	return g_nMPartsMenu;
}

//=============================================================================
// ポーズメニューのリセット
//=============================================================================
void ResetMPartsMenu(void)
{
	g_nMPartsMenu = MPARTS_MENU_KABU;
	SetMPartsMenu();
}
void ResetMPartsMenu1(void)
{
	g_nMPartsMenu = MPARTS_MENU_WANI;
	SetMPartsMenu();
}
void ResetMPartsMenu2(void)
{
	//g_nMPartsMenu = MPARTS_MENU_QUIT;
	//SetMPartsMenu();
}

Player* MPartsGet()
{
	return &MParts;
}
