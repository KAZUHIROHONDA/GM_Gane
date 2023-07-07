//=============================================================================
//
// ポーズ処理 [select2.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include <stdarg.h>
#include <string.h>
#include "select 2.h"
#include "polygon.h"	// ポリゴン使用
#include "Texture.h"// テクスチャ使用
#include "input.h"
#include "sound.h"
#include "collision.h"

// マクロ定義
#define	NUM_SELECT2_MENU		(2)			// ポーズメニュー数
#define	SELECT2_MENU_WIDTH	(200.0f)	// ポーズメニュー幅
#define	SELECT2_MENU_HEIGHT	(40.0f)		// ポーズメニュー高さ
#define	SELECT2_MENU_INTERVAL	(50.0f)	// ポーズメニュー間隔
#define	SELECT2_MENU_POS_X	(0.0f)		// ポーズメニュー位置(X座標)
#define	SELECT2_MENU_POS_Y	(-50.0f)	// ポーズメニュー位置(Y座標)
#define	PLATE_WIDTH			(360.0f)	// プレートの幅
#define	PLATE_HEIGHT		(340.0f)	// プレートの幅
#define	PLATE_POS_X			(0.0f)		// プレートの位置(X座標)
#define	PLATE_POS_Y			(0.0f)		// プレートの位置(Y座標)

// 構造体定義
// プロトタイプ宣言
// グローバル変数
static ID3D11ShaderResourceView*	g_pTextures[2] = { nullptr };	// テクスチャへのポインタ

static float g_fCurve = 0.0f;
static float g_fCol = 0.0f;
static int	 nStopTime = 0;

static LPCWSTR c_aFileNameSelect2Menu[NUM_SELECT2_MENU] =
{
	L"data/TEXTURE/select001.png",	// コンティニュー
	L"data/TEXTURE/select02.png",	// リトライ
};

static SELECT2_MENU g_nSelect2Menu = SELECT2_MENU_CONTINUE;	//	選択中のメニューNo

// 初期化処理
HRESULT InitSelect2( void )
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr;

	for (int nCntSelect2Menu = 0; nCntSelect2Menu < NUM_SELECT2_MENU; ++nCntSelect2Menu) {
		// テクスチャの読み込み
		hr = CreateTextureFromFile( pDevice,									// デバイスへのポインタ
			c_aFileNameSelect2Menu[nCntSelect2Menu],	// ファイルの名前
			&g_pTextures[nCntSelect2Menu] );			// 読み込むメモリー
	}

	g_nSelect2Menu = SELECT2_MENU_CONTINUE;
	g_fCurve = 0.0f;

	return hr;
}

void UninitSelect2( void )
{
	// テクスチャの開放
	for (int nCntSelect2Menu = 0; nCntSelect2Menu < NUM_SELECT2_MENU; ++nCntSelect2Menu) {
		SAFE_RELEASE( g_pTextures[nCntSelect2Menu] );
	}
}

void UpdateSelect2( void )
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
					g_nSelect2Menu = (SELECT2_MENU)((g_nSelect2Menu + 1) % NUM_SELECT2_MENU);
					SetSelect2Menu();
					nStopTime = 10;
				}
				else
				{
					len *= 0.3f;
					g_nSelect2Menu = (SELECT2_MENU)((g_nSelect2Menu + NUM_SELECT2_MENU - 1) % NUM_SELECT2_MENU);
					SetSelect2Menu();
					nStopTime = 10;
				}

			}
		}
	}
	//マウス
	POINT temp = (*GetMousePosition());
	XMFLOAT2 mousePos = XMFLOAT2(temp.x - SCREEN_CENTER_X, -(temp.y - SCREEN_CENTER_Y));
	XMFLOAT2 pos1 = XMFLOAT2(SELECT2_MENU_POS_X, SELECT2_MENU_POS_Y);
	XMFLOAT2 pos2 = XMFLOAT2(SELECT2_MENU_POS_X, SELECT2_MENU_POS_Y - 1 * SELECT2_MENU_INTERVAL);
	XMFLOAT2 radius1 = XMFLOAT2(SELECT2_MENU_WIDTH / 2, SELECT2_MENU_HEIGHT / 2);
	XMFLOAT2 mpos2 = mousePos;
	XMFLOAT2 radius2 = XMFLOAT2(0.1, 0.1);
	if (CollisionBB(&pos1, &radius1, &mpos2, &radius2))
	{
		ResetSelect2Menu();
	}
	else if (CollisionBB(&pos2, &radius1, &mpos2, &radius2))
	{
		ResetSelect2Menu1();
	}


	// 上下キーで各項目間の移動
	if (GetKeyRepeat( VK_W ) || GetKeyRepeat( VK_UP )) {
		g_nSelect2Menu = (SELECT2_MENU)((g_nSelect2Menu + NUM_SELECT2_MENU - 1) % NUM_SELECT2_MENU);
		PlaySound(SOUND_LABEL_SE_SELECT);
		SetSelect2Menu();
	} else if (GetKeyRepeat( VK_S ) || GetKeyRepeat( VK_DOWN )) {
		g_nSelect2Menu = (SELECT2_MENU)((g_nSelect2Menu + 1) % NUM_SELECT2_MENU);
		PlaySound(SOUND_LABEL_SE_SELECT);
		SetSelect2Menu();
	}

	// 枠の部分の色の変化(グラデーション)
	g_fCurve += XM_PI * 0.01f;
	if (g_fCurve > XM_PI) {
		g_fCurve -= XM_2PI;
	}

	g_fCol = cosf( g_fCurve ) * 0.2f + 0.8f;
}

void DrawSelect2( void )
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	
	//ポーズメニューの表示
	SetPolygonSize( SELECT2_MENU_WIDTH, SELECT2_MENU_HEIGHT );
	for (int nCntSelect2Menu = 0; nCntSelect2Menu < NUM_SELECT2_MENU; ++nCntSelect2Menu) {
		SetPolygonPos( SELECT2_MENU_POS_X, SELECT2_MENU_POS_Y - nCntSelect2Menu * SELECT2_MENU_INTERVAL );

		//選択されているメニューを目立たせる
		if (nCntSelect2Menu == g_nSelect2Menu)
		{
			SetPolygonColor(1.0f, 1.0f, 1.0f);
		}
		else
		{
			SetPolygonColor(0.3f, 0.3f, 0.3f);
		}
		
		// テクスチャの設定
		SetPolygonTexture( g_pTextures[nCntSelect2Menu] );
		// ポリゴンの描画
		DrawPolygon( pDeviceContext );
	}

	SetPolygonColor( 1.0f, 1.0f, 1.0f );
}


//=============================================================================
// ポーズメニューの設定
//=============================================================================
void SetSelect2Menu( void )
{
	g_fCurve = 0.0f;
}

//=============================================================================
// ポーズメニューの取得
//=============================================================================
SELECT2_MENU GetSelect2Menu( void )
{
	return g_nSelect2Menu;
}

//=============================================================================
// ポーズメニューのリセット
//=============================================================================
void ResetSelect2Menu( void )
{
	g_nSelect2Menu = SELECT2_MENU_CONTINUE;
	SetSelect2Menu();
}
void ResetSelect2Menu1(void)
{
	g_nSelect2Menu = SELECT2_MENU_RETRY;
	SetSelect2Menu();
}

