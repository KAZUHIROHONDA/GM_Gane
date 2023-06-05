//=============================================================================
//
// ポーズ処理 [select.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include <stdarg.h>
#include <string.h>
#include "select.h"
#include "polygon.h"	// ポリゴン使用
#include "Texture.h"// テクスチャ使用
#include "input.h"
#include "sound.h"

// マクロ定義
#define	NUM_SELECT_MENU		(3)			// ポーズメニュー数
#define	SELECT_MENU_WIDTH	(200.0f)	// ポーズメニュー幅
#define	SELECT_MENU_HEIGHT	(40.0f)		// ポーズメニュー高さ
#define	SELECT_MENU_INTERVAL	(50.0f)	// ポーズメニュー間隔
#define	SELECT_MENU_POS_X	(0.0f)		// ポーズメニュー位置(X座標)
#define	SELECT_MENU_POS_Y	(-50.0f)	// ポーズメニュー位置(Y座標)
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

static LPCWSTR c_aFileNameSelectMenu[NUM_SELECT_MENU] =
{
	L"data/TEXTURE/select000.png",	// コンティニュー
	L"data/TEXTURE/select001.png",	// リトライ
	L"data/TEXTURE/select002.png",	// クイット
};

static SELECT_MENU g_nSelectMenu = SELECT_MENU_CONTINUE;	//	選択中のメニューNo

// 初期化処理
HRESULT InitSelect( void )
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr;

	for (int nCntSelectMenu = 0; nCntSelectMenu < NUM_SELECT_MENU; ++nCntSelectMenu) {
		// テクスチャの読み込み
		hr = CreateTextureFromFile( pDevice,									// デバイスへのポインタ
			c_aFileNameSelectMenu[nCntSelectMenu],	// ファイルの名前
			&g_pTextures[nCntSelectMenu] );			// 読み込むメモリー
	}

	g_nSelectMenu = SELECT_MENU_CONTINUE;
	g_fCurve = 0.0f;

	return hr;
}

void UninitSelect( void )
{
	// テクスチャの開放
	for (int nCntSelectMenu = 0; nCntSelectMenu < NUM_SELECT_MENU; ++nCntSelectMenu) {
		SAFE_RELEASE( g_pTextures[nCntSelectMenu] );
	}
}

void UpdateSelect( void )
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
					g_nSelectMenu = (SELECT_MENU)((g_nSelectMenu + 1) % NUM_SELECT_MENU);
					SetSelectMenu();
					nStopTime = 10;
				}
				if (angle < 135 || angle > 45)
				{
					len *= 0.3f;
					g_nSelectMenu = (SELECT_MENU)((g_nSelectMenu + NUM_SELECT_MENU - 1) % NUM_SELECT_MENU);
					SetSelectMenu();
					nStopTime = 10;
				}


			}
		}
	}
	
	// 上下キーで各項目間の移動
	if (GetKeyRepeat( VK_W ) || GetKeyRepeat( VK_UP )) {
		g_nSelectMenu = (SELECT_MENU)((g_nSelectMenu + NUM_SELECT_MENU - 1) % NUM_SELECT_MENU);
		PlaySound(SOUND_LABEL_SE_SELECT);
		SetSelectMenu();
	} else if (GetKeyRepeat( VK_S ) || GetKeyRepeat( VK_DOWN )) {
		g_nSelectMenu = (SELECT_MENU)((g_nSelectMenu + 1) % NUM_SELECT_MENU);
		PlaySound(SOUND_LABEL_SE_SELECT);
		SetSelectMenu();
	}

	// 枠の部分の色の変化(グラデーション)
	g_fCurve += XM_PI * 0.01f;
	if (g_fCurve > XM_PI) {
		g_fCurve -= XM_2PI;
	}

	g_fCol = cosf( g_fCurve ) * 0.2f + 0.8f;
}

void DrawSelect( void )
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	
	//ポーズメニューの表示
	SetPolygonSize( SELECT_MENU_WIDTH, SELECT_MENU_HEIGHT );
	for (int nCntSelectMenu = 0; nCntSelectMenu < NUM_SELECT_MENU; ++nCntSelectMenu) {
		SetPolygonPos( SELECT_MENU_POS_X, SELECT_MENU_POS_Y - nCntSelectMenu * SELECT_MENU_INTERVAL );



		//選択されているメニューを目立たせる
		if (nCntSelectMenu == g_nSelectMenu)
		{
			SetPolygonColor(1.0f, 1.0f, 1.0f);
		}
		else
		{
			SetPolygonColor(0.3f, 0.3f, 0.3f);
		}
		
		// テクスチャの設定
		SetPolygonTexture( g_pTextures[nCntSelectMenu] );
		// ポリゴンの描画
		DrawPolygon( pDeviceContext );
	}

	SetPolygonColor( 1.0f, 1.0f, 1.0f );
}


//=============================================================================
// ポーズメニューの設定
//=============================================================================
void SetSelectMenu( void )
{
	g_fCurve = 0.0f;
}

//=============================================================================
// ポーズメニューの取得
//=============================================================================
SELECT_MENU GetSelectMenu( void )
{
	return g_nSelectMenu;
}

//=============================================================================
// ポーズメニューのリセット
//=============================================================================
void ResetSelectMenu( void )
{
	g_nSelectMenu = SELECT_MENU_CONTINUE;
	SetSelectMenu();
}
