//=============================================================================
//
// ポーズ処理 [pause.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include <stdarg.h>
#include <string.h>
#include "pause.h"
#include "polygon.h"	// ポリゴン使用
#include "Texture.h"// テクスチャ使用
#include "input.h"

// マクロ定義
#define	NUM_PAUSE_MENU		(3)			// ポーズメニュー数
#define	PAUSE_MENU_WIDTH	(320.0f)	// ポーズメニュー幅
#define	PAUSE_MENU_HEIGHT	(60.0f)		// ポーズメニュー高さ
#define	PAUSE_MENU_INTERVAL	(100.0f)	// ポーズメニュー間隔
#define	PAUSE_MENU_POS_X	(0.0f)		// ポーズメニュー位置(X座標)
#define	PAUSE_MENU_POS_Y	(PAUSE_MENU_INTERVAL)	// ポーズメニュー位置(Y座標)
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

static LPCWSTR c_aFileNamePauseMenu[NUM_PAUSE_MENU] =
{
	L"data/TEXTURE/pause000.png",	// コンティニュー
	L"data/TEXTURE/select001.png",	// リトライ
	L"data/TEXTURE/select002.png",	// クイット
};

static PAUSE_MENU g_nSelectMenu = PAUSE_MENU_CONTINUE;	//	選択中のメニューNo

// 初期化処理
HRESULT InitPause( void )
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr;

	for (int nCntPauseMenu = 0; nCntPauseMenu < NUM_PAUSE_MENU; ++nCntPauseMenu) {
		// テクスチャの読み込み
		hr = CreateTextureFromFile( pDevice,									// デバイスへのポインタ
			c_aFileNamePauseMenu[nCntPauseMenu],	// ファイルの名前
			&g_pTextures[nCntPauseMenu] );			// 読み込むメモリー
	}

	g_nSelectMenu = PAUSE_MENU_CONTINUE;
	g_fCurve = 0.0f;

	return hr;
}

void UninitPause( void )
{
	// テクスチャの開放
	for (int nCntPauseMenu = 0; nCntPauseMenu < NUM_PAUSE_MENU; ++nCntPauseMenu) {
		SAFE_RELEASE( g_pTextures[nCntPauseMenu] );
	}
}

void UpdatePause( void )
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
					g_nSelectMenu = (PAUSE_MENU)((g_nSelectMenu + 1) % NUM_PAUSE_MENU);
					SetPauseMenu();
					nStopTime = 10;
				}
				else
				{
					len *= 0.3f;
					g_nSelectMenu = (PAUSE_MENU)((g_nSelectMenu + NUM_PAUSE_MENU - 1) % NUM_PAUSE_MENU);
					SetPauseMenu();
					nStopTime = 10;
				}

			}
		}
	}

	// 上下キーで各項目間の移動
	if (GetKeyRepeat( VK_W ) || GetKeyRepeat( VK_UP )) {
		g_nSelectMenu = (PAUSE_MENU)((g_nSelectMenu + NUM_PAUSE_MENU - 1) % NUM_PAUSE_MENU);
		SetPauseMenu();
	} else if (GetKeyRepeat( VK_S ) || GetKeyRepeat( VK_DOWN )) {
		g_nSelectMenu = (PAUSE_MENU)((g_nSelectMenu + 1) % NUM_PAUSE_MENU);
		SetPauseMenu();
	}

	// 枠の部分の色の変化(グラデーション)
	g_fCurve += XM_PI * 0.01f;
	if (g_fCurve > XM_PI) {
		g_fCurve -= XM_2PI;
	}

	g_fCol = cosf( g_fCurve ) * 0.2f + 0.8f;
}

void DrawPause( void )
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	
	//ポーズメニューの表示
	SetPolygonSize( PAUSE_MENU_WIDTH, PAUSE_MENU_HEIGHT );
	for (int nCntPauseMenu = 0; nCntPauseMenu < NUM_PAUSE_MENU; ++nCntPauseMenu) {
		SetPolygonPos( PAUSE_MENU_POS_X, PAUSE_MENU_POS_Y - nCntPauseMenu * PAUSE_MENU_INTERVAL );

		//選択されているメニューを目立たせる
		if (nCntPauseMenu == g_nSelectMenu)
		{
			SetPolygonColor(1.0f, 1.0f, 1.0f);
		}
		else
		{
			SetPolygonColor(0.3f, 0.3f, 0.3f);
		}
		
		// テクスチャの設定
		SetPolygonTexture( g_pTextures[nCntPauseMenu] );
		// ポリゴンの描画
		DrawPolygon( pDeviceContext );
	}

	SetPolygonColor( 1.0f, 1.0f, 1.0f );
}

//=============================================================================
// ポーズメニューの設定
//=============================================================================
void SetPauseMenu( void )
{
	g_fCurve = 0.0f;
}

//=============================================================================
// ポーズメニューの取得
//=============================================================================
PAUSE_MENU GetPauseMenu( void )
{
	return g_nSelectMenu;
}

//=============================================================================
// ポーズメニューのリセット
//=============================================================================
void ResetPauseMenu( void )
{
	g_nSelectMenu = PAUSE_MENU_CONTINUE;
	SetPauseMenu();
}
