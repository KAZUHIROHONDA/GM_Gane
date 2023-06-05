//=============================================================================
//
// ポーズ処理 [selectstage.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include <stdarg.h>
#include <string.h>
#include "selectstage.h"
#include "polygon.h"	// ポリゴン使用
#include "Texture.h"// テクスチャ使用
#include "input.h"
#include "sound.h"

// マクロ定義
#define	NUM_SELECT_STAGE		(9)			// ポーズメニュー数
#define	SELECT_STAGE_WIDTH	(200.0f)	// ポーズメニュー幅
#define	SELECT_STAGE_HEIGHT	(40.0f)		// ポーズメニュー高さ
#define	SELECT_STAGE_INTERVAL	(200.0f)	// ポーズメニュー間隔
#define	SELECT_STAGE_POS_X	(0.0f)		// ポーズメニュー位置(X座標)
#define	SELECT_STAGE_POS_Y	(200.0f)	// ポーズメニュー位置(Y座標)
#define	PLATE_WIDTH			(360.0f)	// プレートの幅
#define	PLATE_HEIGHT		(340.0f)	// プレートの幅
#define	PLATE_POS_X			(0.0f)		// プレートの位置(X座標)
#define	PLATE_POS_Y			(0.0f)		// プレートの位置(Y座標)

// 構造体定義
// プロトタイプ宣言
// グローバル変数
static ID3D11ShaderResourceView*	g_pTextures[NUM_SELECT_STAGE] = { nullptr };	// テクスチャへのポインタ

static float g_fCurve = 0.0f;
static float g_fCol = 0.0f;
static int	 nStopTime = 0;
static int   posx = -300;

static LPCWSTR c_aFileNameSelectMenu[NUM_SELECT_STAGE] =
{
	L"data/TEXTURE/stage1.png",	// コンティニュー
	L"data/TEXTURE/stage2.png",	// リトライ
	L"data/TEXTURE/stage3.png",	// クイット
	L"data/TEXTURE/stage4.png",
	L"data/TEXTURE/stage5.png",
	L"data/TEXTURE/stage6.png",
	L"data/TEXTURE/stage7.png",
	L"data/TEXTURE/stage8.png",
	L"data/TEXTURE/stage9.png",
};

static SELECT_STAGE g_nSelectMenu = SELECT_STAGE_1;	//	選択中のメニューNo

// 初期化処理
HRESULT InitSelectStage( void )
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr;

	for (int nCntSelectMenu = 0; nCntSelectMenu < NUM_SELECT_STAGE; ++nCntSelectMenu) {
		// テクスチャの読み込み
		hr = CreateTextureFromFile( pDevice,									// デバイスへのポインタ
			c_aFileNameSelectMenu[nCntSelectMenu],	// ファイルの名前
			&g_pTextures[nCntSelectMenu] );			// 読み込むメモリー
	}

	g_nSelectMenu = SELECT_STAGE_1;
	g_fCurve = 0.0f;

	return hr;
}

void UninitSelectStage( void )
{
	// テクスチャの開放
	for (int nCntSelectMenu = 0; nCntSelectMenu < NUM_SELECT_STAGE; ++nCntSelectMenu) {
		SAFE_RELEASE( g_pTextures[nCntSelectMenu] );
	}
}

void UpdateSelectStage( void )
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
					g_nSelectMenu = (SELECT_STAGE)
						 ((g_nSelectMenu + 1) % NUM_SELECT_STAGE);
					SetSelectStageMenu();
					nStopTime = 10;
				}
				else 
				{
					len *= 0.3f;
					g_nSelectMenu = (SELECT_STAGE)
						 ((g_nSelectMenu + NUM_SELECT_STAGE - 1) % NUM_SELECT_STAGE);
					SetSelectStageMenu();
					nStopTime = 10;
				}

			}
		}
	}
	
	// 上下キーで各項目間の移動
	if (GetKeyRepeat( VK_W ) || GetKeyRepeat( VK_UP )) {
		g_nSelectMenu = (SELECT_STAGE)((g_nSelectMenu + NUM_SELECT_STAGE - 1) % NUM_SELECT_STAGE);
		PlaySound(SOUND_LABEL_SE_SELECT);
		SetSelectStageMenu();
	} else if (GetKeyRepeat( VK_S ) || GetKeyRepeat( VK_DOWN )) {
		g_nSelectMenu = (SELECT_STAGE)((g_nSelectMenu + 1) % NUM_SELECT_STAGE);
		PlaySound(SOUND_LABEL_SE_SELECT);
		SetSelectStageMenu();
	}
	else if (GetKeyRepeat(VK_A) || GetKeyRepeat(VK_LEFT)) {
		g_nSelectMenu = (SELECT_STAGE)((g_nSelectMenu + NUM_SELECT_STAGE - 3)% NUM_SELECT_STAGE);
		PlaySound(SOUND_LABEL_SE_SELECT);
		SetSelectStageMenu();
	}
	else if (GetKeyRepeat(VK_D) || GetKeyRepeat(VK_RIGHT)) {
		g_nSelectMenu = (SELECT_STAGE)((g_nSelectMenu + 3) % NUM_SELECT_STAGE);
		PlaySound(SOUND_LABEL_SE_SELECT);
		SetSelectStageMenu();
	}

	// 枠の部分の色の変化(グラデーション)
	g_fCurve += XM_PI * 0.01f;
	if (g_fCurve > XM_PI) {
		g_fCurve -= XM_2PI;
	}

	g_fCol = cosf( g_fCurve ) * 0.2f + 0.8f;
}

void DrawSelectStage( void )
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	int nCnt = 0;
	posx = -300;
	 
	//ポーズメニューの表示
	SetPolygonSize( SELECT_STAGE_WIDTH, SELECT_STAGE_HEIGHT );
	for (int CntSelectMenu = 0; CntSelectMenu < 3; ++CntSelectMenu)
	{

		for (int nCntSelectMenu = 0; nCntSelectMenu < 3; ++nCntSelectMenu) 
		{
			SetPolygonPos(posx, SELECT_STAGE_POS_Y - nCntSelectMenu * SELECT_STAGE_INTERVAL);

			//選択されているメニューを目立たせる
			if (nCntSelectMenu + CntSelectMenu * 3 == g_nSelectMenu)
			{
				SetPolygonColor(1.0f, 1.0f, 1.0f);
			}
			else
			{
				SetPolygonColor(0.3f, 0.3f, 0.3f);
			}

			// テクスチャの設定
			SetPolygonTexture(g_pTextures[nCnt]);
			// ポリゴンの描画
			DrawPolygon(pDeviceContext);

			nCnt++;
		}

		posx += 300;
	}

	SetPolygonColor( 1.0f, 1.0f, 1.0f );
}


//=============================================================================
// ポーズメニューの設定
//=============================================================================
void SetSelectStageMenu( void )
{
	g_fCurve = 0.0f;
}

//=============================================================================
// ポーズメニューの取得
//=============================================================================
SELECT_STAGE GetSelectStageMenu( void )
{
	return g_nSelectMenu;
}

//=============================================================================
// ポーズメニューのリセット
//=============================================================================
void ResetSelectStageMenu( void )
{
	g_nSelectMenu = SELECT_STAGE_1;
	SetSelectStageMenu();
}
