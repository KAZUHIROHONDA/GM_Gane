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
#include "collision.h"
#include "fade.h"

// マクロ定義
#define	NUM_SELECT_STAGE		(2)			// ポーズメニュー数
#define	SELECT_STAGE_WIDTH	(570.0f)	// ポーズメニュー幅
#define	SELECT_STAGE_HEIGHT	(410.0f)		// ポーズメニュー高さ
#define	SELECT_STAGE_INTERVAL	(500.0f)	// ポーズメニュー間隔
#define	SELECT_STAGE_POS_X	(200.0f)		// ポーズメニュー位置(X座標)
#define	SELECT_STAGE_POS_Y	(0.0f)	// ポーズメニュー位置(Y座標)
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
	L"data/TEXTURE/sel1.png",	
	L"data/TEXTURE/sel2.png",	

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
	if (GetKeyRepeat(VK_A) || GetKeyRepeat(VK_LEFT)) {
		g_nSelectMenu = (SELECT_STAGE)((g_nSelectMenu + NUM_SELECT_STAGE - 1)% NUM_SELECT_STAGE);
		
		SetSelectStageMenu();
	}
	else if (GetKeyRepeat(VK_D) || GetKeyRepeat(VK_RIGHT)) {
		g_nSelectMenu = (SELECT_STAGE)((g_nSelectMenu + 1) % NUM_SELECT_STAGE);
		
		SetSelectStageMenu();
	}

	POINT temp = (*GetMousePosition());
	XMFLOAT2 mousePos = XMFLOAT2(temp.x - SCREEN_CENTER_X, -(temp.y - SCREEN_CENTER_Y));
	//マウス
	XMFLOAT2 pos1 = XMFLOAT2(SELECT_STAGE_POS_X, SELECT_STAGE_POS_Y);
	XMFLOAT2 pos2 = XMFLOAT2(SELECT_STAGE_POS_X - 1 * SELECT_STAGE_INTERVAL, SELECT_STAGE_POS_Y);
	XMFLOAT2 radius1 = XMFLOAT2(SELECT_STAGE_WIDTH / 2, SELECT_STAGE_HEIGHT / 2);
	XMFLOAT2 mpos2 = mousePos;
	XMFLOAT2 radius2 = XMFLOAT2(0.1, 0.1);
	if (CollisionBB(&pos1, &radius1, &mpos2, &radius2))
	{
		SetSelectStageMenu1();

	}
	else if (CollisionBB(&pos2, &radius1, &mpos2, &radius2))
	{
		SetSelectStageMenu2();

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
	posx = -250;
	 
	//ポーズメニューの表示
	SetPolygonSize( SELECT_STAGE_WIDTH, SELECT_STAGE_HEIGHT );

		for (int nCntSelectMenu = 0; nCntSelectMenu < 2; ++nCntSelectMenu) 
		{
			SetPolygonPos(posx + nCntSelectMenu * SELECT_STAGE_INTERVAL, SELECT_STAGE_POS_Y);

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
			SetPolygonTexture(g_pTextures[nCntSelectMenu]);
			// ポリゴンの描画
			DrawPolygon(pDeviceContext);
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

void SetSelectStageMenu1(void)
{
	g_nSelectMenu = SELECT_STAGE_2;
	SetSelectStageMenu();
}

void SetSelectStageMenu2(void)
{
	g_nSelectMenu = SELECT_STAGE_1;
	SetSelectStageMenu();
}