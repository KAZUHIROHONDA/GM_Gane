//=============================================================================
//
// プレイヤー処理 [partsmenu.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include <stdarg.h>
#include <string.h>
#include "partsmenu.h"
#include "polygon.h"	// ポリゴン使用
#include "Texture.h"// テクスチャ使用
#include "input.h"
#include "sound.h"
#include "collision.h"
#include "model.h"
#include "pchimera.h"
#include "playercs.h"

// マクロ定義
#define	NUM_PARTS_MENU		(2)			// ポーズメニュー数
#define	PARTS_MENU_WIDTH	(200.0f)	// ポーズメニュー幅
#define	PARTS_MENU_HEIGHT	(200.0f)		// ポーズメニュー高さ
#define	PARTS_MENU_INTERVAL	(300.0f)	// ポーズメニュー間隔
#define	PARTS_MENU_POS_X	(200.0f)		// ポーズメニュー位置(X座標)
#define	PARTS_MENU_POS_Y	(200.0f)	// ポーズメニュー位置(Y座標)
#define	PLATE_WIDTH			(360.0f)	// プレートの幅
#define	PLATE_HEIGHT		(340.0f)	// プレートの幅
#define	PLATE_POS_X			(0.0f)		// プレートの位置(X座標)
#define	PLATE_POS_Y			(0.0f)		// プレートの位置(Y座標)

// 構造体定義
// プロトタイプ宣言
// グローバル変数
static ID3D11ShaderResourceView*	g_pTextures[3] = { nullptr };	// テクスチャへのポインタ

static float g_fCurve = 0.0f;
static float g_fCol = 0.0f;
static int	 nStopTime = 0;

static LPCWSTR c_aFileNamePartsMenu[NUM_PARTS_MENU] =
{
	L"data/TEXTURE/kabu.png",	// コンティニュー
	L"data/TEXTURE/wani.png",	// リトライ
};

static PARTS_MENU g_nPartsMenu = PARTS_MENU_KABU;	//	選択中のメニューNo

Player Parts;

// 初期化処理
HRESULT InitParts(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr;

	for (int nCntPartsMenu = 0; nCntPartsMenu < NUM_PARTS_MENU; ++nCntPartsMenu) {
		// テクスチャの読み込み
		hr = CreateTextureFromFile(pDevice,									// デバイスへのポインタ
			c_aFileNamePartsMenu[nCntPartsMenu],	// ファイルの名前
			&g_pTextures[nCntPartsMenu]);			// 読み込むメモリー
	}

	Parts.SetHP(10);
	Parts.SetPAat(10);
	Parts.SetGUat(10);
	Parts.SetTYOKIat(10);

	g_nPartsMenu = PARTS_MENU_KABU;
	g_fCurve = 0.0f;

	return hr;
}

void UninitParts(void)
{
	// テクスチャの開放
	for (int nCntPartsMenu = 0; nCntPartsMenu < NUM_PARTS_MENU; ++nCntPartsMenu) {
		SAFE_RELEASE(g_pTextures[nCntPartsMenu]);
	}
}

void UpdateParts(void)
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
					g_nPartsMenu = (PARTS_MENU)((g_nPartsMenu + 1) % NUM_PARTS_MENU);
					SetPartsMenu();
					nStopTime = 10;
				}
				if (angle < 135 || angle > 45)
				{
					len *= 0.3f;
					g_nPartsMenu = (PARTS_MENU)((g_nPartsMenu + NUM_PARTS_MENU - 1) % NUM_PARTS_MENU);
					SetPartsMenu();
					nStopTime = 10;
				}


			}
		}
	}

	//マウス
	POINT temp = (*GetMousePosition());
	XMFLOAT2 mousePos = XMFLOAT2(temp.x - SCREEN_CENTER_X, -(temp.y - SCREEN_CENTER_Y));
	XMFLOAT2 pos1 = XMFLOAT2(PARTS_MENU_POS_X, PARTS_MENU_POS_Y);
	XMFLOAT2 pos2 = XMFLOAT2(PARTS_MENU_POS_X + 1 * PARTS_MENU_INTERVAL, PARTS_MENU_POS_Y );
	XMFLOAT2 radius1 = XMFLOAT2(PARTS_MENU_WIDTH / 2, PARTS_MENU_HEIGHT / 2);
	XMFLOAT2 mpos2 = mousePos;
	XMFLOAT2 radius2 = XMFLOAT2(0.1, 0.1);
	if (CollisionBB(&pos1, &radius1, &mpos2, &radius2))
	{
		ResetPartsMenu();
		if (GetMouseTrigger(0))
		{
			SetHead(MODEL_KABU);
			Parts.SetHP(10);
			Parts.SetPAat(10);
			Parts.SetGUat(10);
			Parts.SetTYOKIat(10);
			InitPChimera();
		}
	}
	else if (CollisionBB(&pos2, &radius1, &mpos2, &radius2))
	{
		ResetPartsMenu1();
		if (GetMouseTrigger(0))
		{
			SetHead(MODEL_wani);
			Parts.SetHP(15);
			Parts.SetPAat(15);
			Parts.SetGUat(15);
			Parts.SetTYOKIat(15);
			InitPChimera();
		}
	}


	// 上下キーで各項目間の移動
	if (GetKeyRepeat(VK_W) || GetKeyRepeat(VK_UP)) {
		g_nPartsMenu = (PARTS_MENU)((g_nPartsMenu + NUM_PARTS_MENU - 1) % NUM_PARTS_MENU);
		SetPartsMenu();
	}
	else if (GetKeyRepeat(VK_S) || GetKeyRepeat(VK_DOWN)) {
		g_nPartsMenu = (PARTS_MENU)((g_nPartsMenu + 1) % NUM_PARTS_MENU);
		SetPartsMenu();
	}

	// 枠の部分の色の変化(グラデーション)
	g_fCurve += XM_PI * 0.01f;
	if (g_fCurve > XM_PI) {
		g_fCurve -= XM_2PI;
	}

	g_fCol = cosf(g_fCurve) * 0.2f + 0.8f;
}

void DrawParts(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();


	//ポーズメニューの表示
	SetPolygonSize(PARTS_MENU_WIDTH, PARTS_MENU_HEIGHT);
	for (int nCntPartsMenu = 0; nCntPartsMenu < NUM_PARTS_MENU; ++nCntPartsMenu) {
		SetPolygonPos(PARTS_MENU_POS_X + nCntPartsMenu * PARTS_MENU_INTERVAL, PARTS_MENU_POS_Y );


		// テクスチャの設定
		SetPolygonTexture(g_pTextures[nCntPartsMenu]);
		// ポリゴンの描画
		DrawPolygon(pDeviceContext);
	}

	SetPolygonColor(1.0f, 1.0f, 1.0f);
}


//=============================================================================
// ポーズメニューの設定
//=============================================================================
void SetPartsMenu(void)
{
	g_fCurve = 0.0f;
}

//=============================================================================
// ポーズメニューの取得
//=============================================================================
PARTS_MENU GetPartsMenu(void)
{
	return g_nPartsMenu;
}

//=============================================================================
// ポーズメニューのリセット
//=============================================================================
void ResetPartsMenu(void)
{
	g_nPartsMenu = PARTS_MENU_KABU;
	SetPartsMenu();
}
void ResetPartsMenu1(void)
{
	g_nPartsMenu = PARTS_MENU_WANI;
	SetPartsMenu();
}
void ResetPartsMenu2(void)
{
	//g_nPartsMenu = PARTS_MENU_QUIT;
	//SetPartsMenu();
}

Player* PartsGet()
{
	return &Parts;
}
