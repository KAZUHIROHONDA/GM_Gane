//=============================================================================
//
// ポーズ処理 [tab.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include <stdarg.h>
#include <string.h>
#include "tab.h"
#include "polygon.h"	// ポリゴン使用
#include "Texture.h"// テクスチャ使用
#include "input.h"
#include "sound.h"
#include "collision.h"
#include "sceneChimera.h"
#include "chimeracs.h"

// マクロ定義
#define	NUM_TAB_MENU		(4)			// ポーズメニュー数
#define	TAB_MENU_WIDTH	(28.0f)	// ポーズメニュー幅
#define	TAB_MENU_HEIGHT	(128.0f)		// ポーズメニュー高さ
#define	TAB_MENU_INTERVAL	(130.0f)	// ポーズメニュー間隔
#define	TAB_MENU_POS_X	(50.0f)		// ポーズメニュー位置(X座標)
#define	TAB_MENU_POS_Y	(300.0f)	// ポーズメニュー位置(Y座標)
#define	PLATE_WIDTH			(360.0f)	// プレートの幅
#define	PLATE_HEIGHT		(340.0f)	// プレートの幅
#define	PLATE_POS_X			(0.0f)		// プレートの位置(X座標)
#define	PLATE_POS_Y			(0.0f)		// プレートの位置(Y座標)

// 構造体定義
// プロトタイプ宣言
// グローバル変数
static ID3D11ShaderResourceView*	g_pTextures[4] = { nullptr };	// テクスチャへのポインタ

static float g_fCurve = 0.0f;
static float g_fCol = 0.0f;
static int	 nStopTime = 0;

static LPCWSTR c_aFileNameTabMenu[NUM_TAB_MENU] =
{
	L"data/TEXTURE/Htab.png",	
	L"data/TEXTURE/Btab.png",	
	L"data/TEXTURE/Mtab.png",
	L"data/TEXTURE/Utab.png",	
};

static TAB_MENU g_nTabMenu = TAB_MENU_ATAMA;	//	選択中のメニューNo

// 初期化処理
HRESULT InitTab( void )
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr;

	for (int nCntTabMenu = 0; nCntTabMenu < NUM_TAB_MENU; ++nCntTabMenu) {
		// テクスチャの読み込み
		hr = CreateTextureFromFile( pDevice,									// デバイスへのポインタ
			c_aFileNameTabMenu[nCntTabMenu],	// ファイルの名前
			&g_pTextures[nCntTabMenu] );			// 読み込むメモリー
	}

	g_nTabMenu = TAB_MENU_ATAMA;
	g_fCurve = 0.0f;

	return hr;
}

void UninitTab( void )
{
	// テクスチャの開放
	for (int nCntTabMenu = 0; nCntTabMenu < NUM_TAB_MENU; ++nCntTabMenu) {
		SAFE_RELEASE( g_pTextures[nCntTabMenu] );
	}
}

void UpdateTab( void )
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
					g_nTabMenu = (TAB_MENU)((g_nTabMenu + 1) % NUM_TAB_MENU);
					SetTabMenu();
					nStopTime = 10;
				}
				if (angle < 135 || angle > 45)
				{
					len *= 0.3f;
					g_nTabMenu = (TAB_MENU)((g_nTabMenu + NUM_TAB_MENU - 1) % NUM_TAB_MENU);
					SetTabMenu();
					nStopTime = 10;
				}


			}
		}
	}

	//マウス
	POINT temp = (*GetMousePosition());
	XMFLOAT2 mousePos = XMFLOAT2(temp.x - SCREEN_CENTER_X, -(temp.y - SCREEN_CENTER_Y));
	XMFLOAT2 pos1 = XMFLOAT2(TAB_MENU_POS_X, TAB_MENU_POS_Y);
	XMFLOAT2 pos2 = XMFLOAT2(TAB_MENU_POS_X, TAB_MENU_POS_Y - 1 * TAB_MENU_INTERVAL);
	XMFLOAT2 pos3 = XMFLOAT2(TAB_MENU_POS_X, TAB_MENU_POS_Y - 2 * TAB_MENU_INTERVAL);
	XMFLOAT2 pos4 = XMFLOAT2(TAB_MENU_POS_X, TAB_MENU_POS_Y - 3 * TAB_MENU_INTERVAL);
	XMFLOAT2 radius1 = XMFLOAT2(TAB_MENU_WIDTH / 2, TAB_MENU_HEIGHT / 2);
	XMFLOAT2 mpos2 = mousePos;
	XMFLOAT2 radius2 = XMFLOAT2(0.1, 0.1);
	if (CollisionBB(&pos1, &radius1, &mpos2, &radius2))
	{
		ResetTabMenu();
		if (GetMouseTrigger(0))
		{
			Get()->ChangeChimera(HEAD);
		}
	}
	else if (CollisionBB(&pos2, &radius1, &mpos2, &radius2))
	{
		ResetTabMenu1();
		if (GetMouseTrigger(0))
		{
			Get()->ChangeChimera(BODY);
		}
	}
	else if (CollisionBB(&pos3, &radius1, &mpos2, &radius2))
	{
		ResetTabMenu2();
		if (GetMouseTrigger(0))
		{
			Get()->ChangeChimera(FOREFOOT);
		}
	}
	else if (CollisionBB(&pos4, &radius1, &mpos2, &radius2))
	{
		ResetTabMenu3();
		if (GetMouseTrigger(0))
		{
			Get()->ChangeChimera(HINDFOOT);
		}
	}

	// 上下キーで各項目間の移動
	if (GetKeyRepeat( VK_W ) || GetKeyRepeat( VK_UP )) {
		g_nTabMenu = (TAB_MENU)((g_nTabMenu + NUM_TAB_MENU - 1) % NUM_TAB_MENU);
		SetTabMenu();
	} else if (GetKeyRepeat( VK_S ) || GetKeyRepeat( VK_DOWN )) {
		g_nTabMenu = (TAB_MENU)((g_nTabMenu + 1) % NUM_TAB_MENU);
		SetTabMenu();
	}

	// 枠の部分の色の変化(グラデーション)
	g_fCurve += XM_PI * 0.01f;
	if (g_fCurve > XM_PI) {
		g_fCurve -= XM_2PI;
	}

	g_fCol = cosf( g_fCurve ) * 0.2f + 0.8f;
}

void DrawTab( void )
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	
	//ポーズメニューの表示
	SetPolygonSize( TAB_MENU_WIDTH, TAB_MENU_HEIGHT );
	for (int nCntTabMenu = 0; nCntTabMenu < NUM_TAB_MENU; ++nCntTabMenu) {
		SetPolygonPos( TAB_MENU_POS_X, TAB_MENU_POS_Y - nCntTabMenu * TAB_MENU_INTERVAL );



		//選択されているメニューを目立たせる
		if (nCntTabMenu == g_nTabMenu)
		{
			SetPolygonColor(1.0f, 1.0f, 1.0f);
		}
		else
		{
			SetPolygonColor(0.3f, 0.3f, 0.3f);
		}
		
		// テクスチャの設定
		SetPolygonTexture( g_pTextures[nCntTabMenu] );
		// ポリゴンの描画
		DrawPolygon( pDeviceContext );
	}

	SetPolygonColor( 1.0f, 1.0f, 1.0f );
}


//=============================================================================
// ポーズメニューの設定
//=============================================================================
void SetTabMenu( void )
{
	g_fCurve = 0.0f;
}

//=============================================================================
// ポーズメニューの取得
//=============================================================================
TAB_MENU GetTabMenu( void )
{
	return g_nTabMenu;
}

//=============================================================================
// ポーズメニューのリセット
//=============================================================================
void ResetTabMenu( void )
{
	g_nTabMenu = TAB_MENU_ATAMA;
	SetTabMenu();
}
void ResetTabMenu1(void)
{
	g_nTabMenu = TAB_MENU_KARADA;
	SetTabMenu();
}
void ResetTabMenu2(void)
{
	g_nTabMenu = TAB_MENU_MAE;
	SetTabMenu();
}
void ResetTabMenu3(void)
{
	g_nTabMenu = TAB_MENU_USIRO;
	SetTabMenu();
}

