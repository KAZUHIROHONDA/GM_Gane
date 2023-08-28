//=============================================================================
//
// プレイヤー処理 [Bpartsmenu.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include <stdarg.h>
#include <string.h>
#include "Bpartsmenu.h"
#include "polygon.h"	// ポリゴン使用
#include "Texture.h"// テクスチャ使用
#include "input.h"
#include "sound.h"
#include "collision.h"
#include "model.h"
#include "pchimera.h"
#include "sceneTitle.h"

// マクロ定義
#define	NUM_BPARTS_MENU		(6)			// ポーズメニュー数
#define	BPARTS_MENU_WIDTH	(200.0f)	// ポーズメニュー幅
#define	BPARTS_MENU_HEIGHT	(200.0f)		// ポーズメニュー高さ
#define	BPARTS_MENU_INTERVAL	(300.0f)	// ポーズメニュー間隔
#define	BPARTS_MENU_POS_X	(200.0f)		// ポーズメニュー位置(X座標)
#define	BPARTS_MENU_POS_Y	(250.0f)	// ポーズメニュー位置(Y座標)
#define	PLATE_WIDTH			(360.0f)	// プレートの幅
#define	PLATE_HEIGHT		(340.0f)	// プレートの幅
#define	PLATE_POS_X			(0.0f)		// プレートの位置(X座標)
#define	PLATE_POS_Y			(0.0f)		// プレートの位置(Y座標)

// 構造体定義
// プロトタイプ宣言
// グローバル変数
static ID3D11ShaderResourceView*	g_pTextures[7] = { nullptr };	// テクスチャへのポインタ

static float g_fCurve = 0.0f;
static float g_fCol = 0.0f;
static int	 nStopTime = 0;

static LPCWSTR c_aFileNameBPartsMenu[NUM_BPARTS_MENU] =
{
	L"data/TEXTURE/kabu1.png",	//カブトムシ
	L"data/TEXTURE/wani1.png",	//ワニ
	L"data/TEXTURE/panda1.png",	//パンダ
	L"data/TEXTURE/wani1.png",	//とり
	L"data/TEXTURE/buta1.png",	//豚
	L"data/TEXTURE/usi1.png",	//牛
};

static BPARTS_MENU g_nBPartsMenu = BPARTS_MENU_KABU;	//	選択中のメニューNo

Player BParts;

// 初期化処理
HRESULT InitBParts(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr;

	for (int nCntBPartsMenu = 0; nCntBPartsMenu < NUM_BPARTS_MENU; ++nCntBPartsMenu) {
		// テクスチャの読み込み
		hr = CreateTextureFromFile(pDevice,									// デバイスへのポインタ
			c_aFileNameBPartsMenu[nCntBPartsMenu],	// ファイルの名前
			&g_pTextures[nCntBPartsMenu]);			// 読み込むメモリー
	}

	GetPlayer()->SetName("カブトムシ");
	BParts.SetHP(10);
	BParts.SetPAat(10);
	BParts.SetGUat(10);
	BParts.SetTYOKIat(10);

	g_nBPartsMenu = BPARTS_MENU_KABU;
	g_fCurve = 0.0f;

	return hr;
}

void UninitBParts(void)
{
	// テクスチャの開放
	for (int nCntBPartsMenu = 0; nCntBPartsMenu < NUM_BPARTS_MENU; ++nCntBPartsMenu) {
		SAFE_RELEASE(g_pTextures[nCntBPartsMenu]);
	}
}

void UpdateBParts(void)
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
					g_nBPartsMenu = (BPARTS_MENU)((g_nBPartsMenu + 1) % NUM_BPARTS_MENU);
					SetBPartsMenu();
					nStopTime = 10;
				}
				if (angle < 135 || angle > 45)
				{
					len *= 0.3f;
					g_nBPartsMenu = (BPARTS_MENU)((g_nBPartsMenu + NUM_BPARTS_MENU - 1) % NUM_BPARTS_MENU);
					SetBPartsMenu();
					nStopTime = 10;
				}


			}
		}
	}

	//マウス
	POINT temp = (*GetMousePosition());
	XMFLOAT2 mousePos = XMFLOAT2(temp.x - SCREEN_CENTER_X, -(temp.y - SCREEN_CENTER_Y));
	XMFLOAT2 pos1 = XMFLOAT2(BPARTS_MENU_POS_X, BPARTS_MENU_POS_Y);
	XMFLOAT2 pos2 = XMFLOAT2(BPARTS_MENU_POS_X + 1 * BPARTS_MENU_INTERVAL, BPARTS_MENU_POS_Y );
	XMFLOAT2 pos3 = XMFLOAT2(BPARTS_MENU_POS_X , BPARTS_MENU_POS_Y - 1 * 250);
	XMFLOAT2 pos4 = XMFLOAT2(BPARTS_MENU_POS_X + 1 * 250, BPARTS_MENU_POS_Y - 1 * 250);
	XMFLOAT2 pos5 = XMFLOAT2(BPARTS_MENU_POS_X, BPARTS_MENU_POS_Y - 2 * 250);
	XMFLOAT2 pos6 = XMFLOAT2(BPARTS_MENU_POS_X + 1 * 250, BPARTS_MENU_POS_Y - 2 * 250);
	XMFLOAT2 radius1 = XMFLOAT2(BPARTS_MENU_WIDTH / 2, BPARTS_MENU_HEIGHT / 2);
	XMFLOAT2 mpos2 = mousePos;
	XMFLOAT2 radius2 = XMFLOAT2(0.1, 0.1);
	if (CollisionBB(&pos1, &radius1, &mpos2, &radius2))
	{
		ResetBPartsMenu();
		if (GetMouseTrigger(0))
		{
			SetBody(MODEL_KABU1);
			GetPlayer()->SetName("カブトムシ");
			BParts.SetHP(10);
			BParts.SetPAat(10);
			BParts.SetGUat(10);
			BParts.SetTYOKIat(10);
			//手札、0グー,1チョキ,2パー
			GetPlayer()->SetHand(0, 1);
			GetPlayer()->SetHand(1, 1);
			GetPlayer()->SetHand(2, 1);
			GetPlayer()->SetHand(3, 1);
			GetPlayer()->SetHand(4, 1);

		    InitPChimera();
		}
	}
	else if (CollisionBB(&pos2, &radius1, &mpos2, &radius2))
	{
		ResetBPartsMenu1();
		if (GetMouseTrigger(0))
		{
			SetBody(MODEL_wani1);
			GetPlayer()->SetName("ワニ");
			BParts.SetHP(20);
			BParts.SetPAat(20);
			BParts.SetGUat(20);
			BParts.SetTYOKIat(20);
			//手札、0グー,1チョキ,2パー
			GetPlayer()->SetHand(0, 0);
			GetPlayer()->SetHand(1, 0);
			GetPlayer()->SetHand(2, 0);
			GetPlayer()->SetHand(3, 0);
			GetPlayer()->SetHand(4, 0);

			InitPChimera();
		}
	}
	else if (CollisionBB(&pos3, &radius1, &mpos2, &radius2))
	{
		ResetBPartsMenu2();
		if (GetMouseTrigger(0))
		{
			SetBody(MODEL_PANDA1);
			GetPlayer()->SetName("パンダ");
			BParts.SetHP(50);
			BParts.SetPAat(50);
			BParts.SetGUat(50);
			BParts.SetTYOKIat(50);
			//手札、0グー,1チョキ,2パー
			GetPlayer()->SetHand(0, 1);
			GetPlayer()->SetHand(1, 1);
			GetPlayer()->SetHand(2, 1);
			GetPlayer()->SetHand(3, 1);
			GetPlayer()->SetHand(4, 1);
	
			InitPChimera();
		}
	}
	else if (CollisionBB(&pos4, &radius1, &mpos2, &radius2))
	{
		ResetBPartsMenu3();
		if (GetMouseTrigger(0))
		{
			SetBody(MODEL_TORI1);
			GetPlayer()->SetName("トリ");
			BParts.SetHP(50);
			BParts.SetPAat(50);
			BParts.SetGUat(50);
			BParts.SetTYOKIat(50);
			//手札、0グー,1チョキ,2パー
			GetPlayer()->SetHand(0, 2);
			GetPlayer()->SetHand(1, 2);
			GetPlayer()->SetHand(2, 2);
			GetPlayer()->SetHand(3, 2);
			GetPlayer()->SetHand(4, 2);
	
			InitPChimera();
		}
	}
	else if (CollisionBB(&pos5, &radius1, &mpos2, &radius2))
	{
		ResetBPartsMenu4();
		if (GetMouseTrigger(0))
		{
			SetBody(MODEL_BUTA1);
			GetPlayer()->SetName("ブタ");
			BParts.SetHP(60);
			BParts.SetPAat(60);
			BParts.SetGUat(60);
			BParts.SetTYOKIat(50);
			//手札、0グー,1チョキ,2パー
			GetPlayer()->SetHand(0, 2);
			GetPlayer()->SetHand(1, 2);
			GetPlayer()->SetHand(2, 2);
			GetPlayer()->SetHand(3, 2);
			GetPlayer()->SetHand(4, 2);

			InitPChimera();
		}
	}
	else if (CollisionBB(&pos6, &radius1, &mpos2, &radius2))
	{
		ResetBPartsMenu5();
		if (GetMouseTrigger(0))
		{
			SetBody(MODEL_USI1);
			GetPlayer()->SetName("ウシ");
			BParts.SetHP(30);
			BParts.SetPAat(30);
			BParts.SetGUat(30);
			BParts.SetTYOKIat(50);
			//手札、0グー,1チョキ,2パー
			GetPlayer()->SetHand(0, 2);
			GetPlayer()->SetHand(1, 2);
			GetPlayer()->SetHand(2, 2);
			GetPlayer()->SetHand(3, 2);
			GetPlayer()->SetHand(4, 2);

			InitPChimera();
		}
	}


	// 上下キーで各項目間の移動
	if (GetKeyRepeat(VK_W) || GetKeyRepeat(VK_UP)) {
		g_nBPartsMenu = (BPARTS_MENU)((g_nBPartsMenu + NUM_BPARTS_MENU - 1) % NUM_BPARTS_MENU);
		SetBPartsMenu();
	}
	else if (GetKeyRepeat(VK_S) || GetKeyRepeat(VK_DOWN)) {
		g_nBPartsMenu = (BPARTS_MENU)((g_nBPartsMenu + 1) % NUM_BPARTS_MENU);
		SetBPartsMenu();
	}

	// 枠の部分の色の変化(グラデーション)
	g_fCurve += XM_PI * 0.01f;
	if (g_fCurve > XM_PI) {
		g_fCurve -= XM_2PI;
	}

	g_fCol = cosf(g_fCurve) * 0.2f + 0.8f;
}

void DrawBParts(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	int nCnt = 0;

	//ポーズメニューの表示
	SetPolygonSize(BPARTS_MENU_WIDTH, BPARTS_MENU_HEIGHT);
	for (int nCntBPartsMenu = 0; nCntBPartsMenu < 3; ++nCntBPartsMenu) {
		
		for (int nCntBPartsMenu1 = 0; nCntBPartsMenu1 < 2; ++nCntBPartsMenu1) {
			SetPolygonPos(BPARTS_MENU_POS_X + nCntBPartsMenu1 * BPARTS_MENU_INTERVAL, BPARTS_MENU_POS_Y - nCntBPartsMenu * 250);

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
void SetBPartsMenu(void)
{
	g_fCurve = 0.0f;
}

//=============================================================================
// ポーズメニューの取得
//=============================================================================
BPARTS_MENU GetBPartsMenu(void)
{
	return g_nBPartsMenu;
}

//=============================================================================
// ポーズメニューのリセット
//=============================================================================
void ResetBPartsMenu(void)
{
	g_nBPartsMenu = BPARTS_MENU_KABU;
	SetBPartsMenu();
}
void ResetBPartsMenu1(void)
{
	g_nBPartsMenu = BPARTS_MENU_WANI;
	SetBPartsMenu();
}
void ResetBPartsMenu2(void)
{
	g_nBPartsMenu = BPARTS_MENU_PANDA;
	SetBPartsMenu();
}
void ResetBPartsMenu3(void)
{
	g_nBPartsMenu = BPARTS_MENU_TORI;
	SetBPartsMenu();
}
void ResetBPartsMenu4(void)
{
	g_nBPartsMenu = BPARTS_MENU_BUTA;
	SetBPartsMenu();
}
void ResetBPartsMenu5(void)
{
	g_nBPartsMenu = BPARTS_MENU_USI;
	SetBPartsMenu();
}

Player* BPartsGet()
{
	return &BParts;
}

