//=============================================================================
//
// ポーズ処理 [select1.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include <stdarg.h>
#include <string.h>
#include "select1.h"
#include "polygon.h"	// ポリゴン使用
#include "Texture.h"// テクスチャ使用
#include "input.h"
#include "sound.h"

// マクロ定義
#define	NUM_SELECT1_MENU		(3)			// ポーズメニュー数
#define	SELECT1_MENU_WIDTH	(200.0f)	// ポーズメニュー幅
#define	SELECT1_MENU_HEIGHT	(40.0f)		// ポーズメニュー高さ
#define	SELECT1_MENU_INTERVAL	(80.0f)	// ポーズメニュー間隔
#define	SELECT1_MENU_POS_X	(0.0f)		// ポーズメニュー位置(X座標)
#define	SELECT1_MENU_POS_Y	(-50.0f)	// ポーズメニュー位置(Y座標)
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
static float g_fAlpha = 1.0f;
static int   g_fCnt = 0;

static LPCWSTR c_aFileNameSelect1Menu[NUM_SELECT1_MENU] =
{
	L"data/TEXTURE/select0.png",	// コンティニュー
	L"data/TEXTURE/select1.png",	// リトライ
	L"data/TEXTURE/select2.png",	// クイット
};

static SELECT1_MENU g_nSelect1Menu = SELECT1_MENU_CONTINUE;	//	選択中のメニューNo

// 初期化処理
HRESULT InitSelect1( void )
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr;

	for (int nCntSelect1Menu = 0; nCntSelect1Menu < NUM_SELECT1_MENU; ++nCntSelect1Menu) {
		// テクスチャの読み込み
		hr = CreateTextureFromFile( pDevice,									// デバイスへのポインタ
			c_aFileNameSelect1Menu[nCntSelect1Menu],	// ファイルの名前
			&g_pTextures[nCntSelect1Menu] );			// 読み込むメモリー
	}

	g_nSelect1Menu = SELECT1_MENU_CONTINUE;
	g_fCurve = 0.0f;

	return hr;
}

void UninitSelect1( void )
{
	// テクスチャの開放
	for (int nCntSelect1Menu = 0; nCntSelect1Menu < NUM_SELECT1_MENU; ++nCntSelect1Menu) {
		SAFE_RELEASE( g_pTextures[nCntSelect1Menu] );
	}
}

void UpdateSelect1( void )
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
					g_nSelect1Menu = (SELECT1_MENU)((g_nSelect1Menu + 1) % NUM_SELECT1_MENU);
					SetSelect1Menu();
					nStopTime = 10;
				}
				else
				{
					len *= 0.3f;
					g_nSelect1Menu = (SELECT1_MENU)((g_nSelect1Menu + NUM_SELECT1_MENU - 1) % NUM_SELECT1_MENU);
					SetSelect1Menu();
					nStopTime = 10;
				}

			}
		}
	}
	// 上下キーで各項目間の移動
	if (GetKeyRepeat( VK_W ) || GetKeyRepeat( VK_UP )) {
		g_nSelect1Menu = (SELECT1_MENU)((g_nSelect1Menu + NUM_SELECT1_MENU - 1) % NUM_SELECT1_MENU);
		PlaySound(SOUND_LABEL_SE_SELECT);
		SetSelect1Menu();
	} else if (GetKeyRepeat( VK_S ) || GetKeyRepeat( VK_DOWN )) {
		g_nSelect1Menu = (SELECT1_MENU)((g_nSelect1Menu + 1) % NUM_SELECT1_MENU);
		PlaySound(SOUND_LABEL_SE_SELECT);
		SetSelect1Menu();
	}

	// 枠の部分の色の変化(グラデーション)
	g_fCurve += XM_PI * 0.01f;
	if (g_fCurve > XM_PI) {
		g_fCurve -= XM_2PI;
	}

	g_fCnt++;
	if (g_fCnt <= 120)
	{
		g_fAlpha -= 0.01f;
	}
	if (g_fAlpha <= 0.02f)
	{
		g_fAlpha = 1.0f;
		g_fCnt = 0;
	}

	g_fCol = cosf( g_fCurve ) * 0.2f + 0.8f;
}

void DrawSelect1( void )
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	
	//ポーズメニューの表示
	SetPolygonSize( SELECT1_MENU_WIDTH, SELECT1_MENU_HEIGHT );
	for (int nCntSelect1Menu = 0; nCntSelect1Menu < NUM_SELECT1_MENU; ++nCntSelect1Menu) {
		SetPolygonPos( SELECT1_MENU_POS_X, SELECT1_MENU_POS_Y - nCntSelect1Menu * SELECT1_MENU_INTERVAL );

		//選択されているメニューを目立たせる
		if (nCntSelect1Menu == g_nSelect1Menu)
		{
			SetPolygonColor(1.0f, 1.0f, 1.0f);
			SetPolygonAlpha(g_fAlpha);
		}
		else
		{
			SetPolygonColor(0.3f, 0.3f, 0.3f);
			SetPolygonAlpha(1.0f);
		}
		
		// テクスチャの設定
		SetPolygonTexture( g_pTextures[nCntSelect1Menu] );
		// ポリゴンの描画
		DrawPolygon( pDeviceContext );
	}

	SetPolygonColor( 1.0f, 1.0f, 1.0f );
}

void DrawSelect11(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();


	//ポーズメニューの表示
	SetPolygonSize(SELECT1_MENU_WIDTH, SELECT1_MENU_HEIGHT);
	for (int nCntSelect1Menu = 1; nCntSelect1Menu < NUM_SELECT1_MENU; ++nCntSelect1Menu) {
		SetPolygonPos(SELECT1_MENU_POS_X, SELECT1_MENU_POS_Y - nCntSelect1Menu * SELECT1_MENU_INTERVAL);

		//選択されているメニューを目立たせる
		if (nCntSelect1Menu == g_nSelect1Menu)
		{
			SetPolygonColor(1.0f, 1.0f, 1.0f);
			SetPolygonAlpha(g_fAlpha);
		}
		else
		{
			SetPolygonColor(0.3f, 0.3f, 0.3f);
			SetPolygonAlpha(1.0f);
		}

		// テクスチャの設定
		SetPolygonTexture(g_pTextures[nCntSelect1Menu]);
		// ポリゴンの描画
		DrawPolygon(pDeviceContext);
	}

	SetPolygonColor(1.0f, 1.0f, 1.0f);
}

//=============================================================================
// ポーズメニューの設定
//=============================================================================
void SetSelect1Menu( void )
{
	g_fCurve = 0.0f;
}

//=============================================================================
// ポーズメニューの取得
//=============================================================================
SELECT1_MENU GetSelect1Menu( void )
{
	return g_nSelect1Menu;
}

//=============================================================================
// ポーズメニューのリセット
//=============================================================================
void ResetSelect1Menu( void )
{
	g_nSelect1Menu = SELECT1_MENU_CONTINUE;
	SetSelect1Menu();
}
