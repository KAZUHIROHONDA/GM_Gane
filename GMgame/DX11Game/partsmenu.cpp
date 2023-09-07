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
#include "sceneTitle.h"

// マクロ定義
#define	NUM_PARTS_MENU		(8)			// ポーズメニュー数
#define	PARTS_MENU_WIDTH	(200.0f)	// ポーズメニュー幅
#define	PARTS_MENU_HEIGHT	(200.0f)		// ポーズメニュー高さ
#define	PARTS_MENU_INTERVAL	(300.0f)	// ポーズメニュー間隔
#define	PARTS_MENU_POS_X	(200.0f)		// ポーズメニュー位置(X座標)
#define	PARTS_MENU_POS_Y	(250.0f)	// ポーズメニュー位置(Y座標)
#define	PLATE_WIDTH			(360.0f)	// プレートの幅
#define	PLATE_HEIGHT		(340.0f)	// プレートの幅
#define	PLATE_POS_X			(0.0f)		// プレートの位置(X座標)
#define	PLATE_POS_Y			(0.0f)		// プレートの位置(Y座標)


// 構造体定義
// プロトタイプ宣言
// グローバル変数
static ID3D11ShaderResourceView*	g_pTextures[9] = { nullptr };	// テクスチャへのポインタ

static float g_fCurve = 0.0f;
static float g_fCol = 0.0f;
static int	 nStopTime = 0;
static int	 charano = 0;
static int	 mousescroll = 0;

static LPCWSTR c_aFileNamePartsMenu[NUM_PARTS_MENU] =
{
	L"data/TEXTURE/kabu.png",	
	L"data/TEXTURE/wani.png",	
	L"data/TEXTURE/panda.png",	
	L"data/TEXTURE/tori.png",	
	L"data/TEXTURE/buta.png",	
	L"data/TEXTURE/usi.png",	
	L"data/TEXTURE/kuwa.png",
	L"data/TEXTURE/goki.png",
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

	GetPlayer()->Setmodel(0);

	g_nPartsMenu = PARTS_MENU_KABU;
	g_fCurve = 0.0f;
	charano = 0;
	mousescroll = 0;

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

	//ホイール回転
	int temp2 = GetMouseWheelDelta();
	wchar_t str[256];
	wsprintf(str, L"%d", temp2);
	OutputDebugString(str);
	if (temp2 > 0 && mousescroll <= 250)
	{
		mousescroll += 250;
	}
	else if (temp2 < 0 && mousescroll >= -250)
	{
		mousescroll -= 250;
	}


	//マウス
	POINT temp = (*GetMousePosition());
	XMFLOAT2 mousePos = XMFLOAT2(temp.x - SCREEN_CENTER_X, -(temp.y - SCREEN_CENTER_Y));
	XMFLOAT2 pos1 = XMFLOAT2(PARTS_MENU_POS_X, PARTS_MENU_POS_Y + mousescroll);
	XMFLOAT2 pos2 = XMFLOAT2(PARTS_MENU_POS_X + 1 * PARTS_MENU_INTERVAL, PARTS_MENU_POS_Y + mousescroll);
	XMFLOAT2 pos3 = XMFLOAT2(PARTS_MENU_POS_X, PARTS_MENU_POS_Y - 1 * 250 + mousescroll);
	XMFLOAT2 pos4 = XMFLOAT2(PARTS_MENU_POS_X + 1 * PARTS_MENU_INTERVAL, PARTS_MENU_POS_Y - 1 * 250 + mousescroll);
	XMFLOAT2 pos5 = XMFLOAT2(PARTS_MENU_POS_X, PARTS_MENU_POS_Y - 2 * 250 + mousescroll);
	XMFLOAT2 pos6 = XMFLOAT2(PARTS_MENU_POS_X + 1 * PARTS_MENU_INTERVAL, PARTS_MENU_POS_Y - 2 * 250 + mousescroll);
	XMFLOAT2 pos7 = XMFLOAT2(PARTS_MENU_POS_X, PARTS_MENU_POS_Y - 3 * 250 + mousescroll);
	XMFLOAT2 pos8 = XMFLOAT2(PARTS_MENU_POS_X + 1 * PARTS_MENU_INTERVAL, PARTS_MENU_POS_Y - 3 * 250 + mousescroll);
	XMFLOAT2 radius1 = XMFLOAT2(PARTS_MENU_WIDTH / 2, PARTS_MENU_HEIGHT / 2);
	XMFLOAT2 mpos2 = mousePos;
	XMFLOAT2 radius2 = XMFLOAT2(0.1, 0.1);
	if (CollisionBB(&pos1, &radius1, &mpos2, &radius2))
	{
		ResetPartsMenu();
		if (GetMouseTrigger(0))
		{
			GetPlayerModel()->SetHead(MODEL_KABU);
			GetPlayer()->Setmodel(0);
			charano = 0;

			InitPChimera();
		}
	}
	else if (CollisionBB(&pos2, &radius1, &mpos2, &radius2))
	{
		if (GetMouseTrigger(0))
		{
			GetPlayerModel()->SetHead(MODEL_wani);
			GetPlayer()->Setmodel(1);
			charano = 1;

			InitPChimera();
		}
	}
	else if (CollisionBB(&pos3, &radius1, &mpos2, &radius2))
	{

		if (GetMouseTrigger(0))
		{
			GetPlayerModel()->SetHead(MODEL_PANDA);
			GetPlayer()->Setmodel(2);
			charano = 2;

			InitPChimera();
		}
	}
	else if (CollisionBB(&pos4, &radius1, &mpos2, &radius2))
	{

		if (GetMouseTrigger(0))
		{
			GetPlayerModel()->SetHead(MODEL_TORI);
			GetPlayer()->Setmodel(3);
			charano = 3;

			InitPChimera();
		}
	}
	else if (CollisionBB(&pos5, &radius1, &mpos2, &radius2))
	{

		if (GetMouseTrigger(0))
		{
			GetPlayerModel()->SetHead(MODEL_BUTA);
			GetPlayer()->Setmodel(4);
			charano = 4;

			InitPChimera();
		}
	}
	else if (CollisionBB(&pos6, &radius1, &mpos2, &radius2))
	{

		if (GetMouseTrigger(0))
		{
			GetPlayerModel()->SetHead(MODEL_USI);
			GetPlayer()->Setmodel(5);
			charano = 5;

			InitPChimera();
		}
	}
	else if (CollisionBB(&pos7, &radius1, &mpos2, &radius2))
	{

		if (GetMouseTrigger(0))
		{
			GetPlayerModel()->SetHead(MODEL_KUWA);
			GetPlayer()->Setmodel(6);
			charano = 5;//6

			InitPChimera();
		}
	}
	else if (CollisionBB(&pos8, &radius1, &mpos2, &radius2))
	{

		if (GetMouseTrigger(0))
		{
			GetPlayerModel()->SetHead(MODEL_GOKI);
			GetPlayer()->Setmodel(7);
			charano = 5;//7

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

	int nCnt = 0;

	//表示
	SetPolygonSize(PARTS_MENU_WIDTH, PARTS_MENU_HEIGHT);
	for (int nCntBPartsMenu = 0; nCntBPartsMenu < 4; ++nCntBPartsMenu) {
		
		for (int nCntBPartsMenu1 = 0; nCntBPartsMenu1 < 2; ++nCntBPartsMenu1) {
			SetPolygonPos(PARTS_MENU_POS_X + nCntBPartsMenu1 * PARTS_MENU_INTERVAL, PARTS_MENU_POS_Y - nCntBPartsMenu * 250 + mousescroll);

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

Player* PartsGet()
{
	return &Parts;
}

int CharaNo()
{
	return charano;
}
