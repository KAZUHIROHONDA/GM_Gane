#include "Title.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"
#include "MessageManager.h"
#include "jyanken.h"
#include "fade.h"
#include "player.h"
#include "enemy.h"
#include "sceneGame.h"
#include "collision.h"

#define gu 1-1
#define tyoki 2-1
#define pa 3-1

// マクロ定義
#define	NUM_JYANKEN_MENU		(3)			// ポーズメニュー数
#define	JYANKEN_MENU_WIDTH	(100.0f)	// ポーズメニュー幅
#define	JYANKEN_MENU_HEIGHT	(100.0f)		// ポーズメニュー高さ
#define	JYANKEN_MENU_INTERVAL	(100.0f)	// ポーズメニュー間隔
#define	JYANKEN_MENU_POS_X	(100.0f)		// ポーズメニュー位置(X座標)
#define	JYANKEN_MENU_POS_Y	(-250.0f)	// ポーズメニュー位置(Y座標)
#define	PLATE_WIDTH			(360.0f)	// プレートの幅
#define	PLATE_HEIGHT		(340.0f)	// プレートの幅
#define	PLATE_POS_X			(0.0f)		// プレートの位置(X座標)
#define	PLATE_POS_Y			(0.0f)		// プレートの位置(Y座標)

static ID3D11ShaderResourceView*	g_pTextures[3] = { nullptr };

// テクスチャファイル名
#define TITLE_TEXTURENAME	_T("data/texture/button.png")
static float g_fCurve = 0.0f;
static float g_fCol = 0.0f;
static int	 nStopTime = 0;

static LPCWSTR c_aFileNameJyankenMenu[NUM_JYANKEN_MENU] =
{
	L"data/TEXTURE/gu.png",	// コンティニュー
	L"data/TEXTURE/tyoki.png",
	L"data/TEXTURE/pa.png",
};

static JYANKEN_MENU g_nJyankenMenu = JYANKEN_MENU_GU;	//	選択中のメニューNo
static int te[5] = {-1,-1,-1,-1,-1};
static int aite[5] = { -1,-1,-1,-1,-1 };
static int result[4]; //勝ち数, 負け数, あいこ数, 連勝数

bool jadge = false;
int z, i;
int j = 0;
int n = 0;
int f = 0;
int Cnt, Cnt1 = 0;
bool win = false;
bool lose = false;

//OKボタン
#define OK_TEXTURENAME	_T("data/texture/button.png")
#define HATENA_TEXTURENAME	_T("data/texture/1mdpi.png")
		
#define OK_POS_X	(300)					//初期位置X
#define OK_POS_Y	(-250)					//初期位置Y
#define OK_SIZE_X	(100)		//横幅
#define OK_SIZE_Y	(100)		//縦幅

#define HATENA_POS_X	(300)					//初期位置X
#define HATENA_POS_Y	(-250)					//初期位置Y
#define HATENA_SIZE_X	(100)		//横幅
#define HATENA_SIZE_Y	(100)		//縦幅

static ID3D11ShaderResourceView*	g_pTexture;				// テクスチャ
static ID3D11ShaderResourceView*	g_pTexture1;				// テクスチャ
static XMFLOAT3 g_Position;//位置
static XMFLOAT2    g_Angle;   //角度
static XMFLOAT3	g_Rot;
static XMFLOAT2 g_Size;    //サイズ
static XMFLOAT2 g_Scale;   //拡大率 
static float	g_Alpha;   //透過

const tMessage testMessage[9] = {
	{L"じゃんけん５回勝負！！",MESSAGE_TYPE::E_TYPE_ENTER,(long long int)&testMessage[1] },
	{L"かち！",MESSAGE_TYPE::E_TYPE_NORMAL},
	{L"まけ！",MESSAGE_TYPE::E_TYPE_NORMAL},
	{L"あいこ！",MESSAGE_TYPE::E_TYPE_NORMAL},
	{L"１回目",MESSAGE_TYPE::E_TYPE_NORMAL},
	{L"２回目",MESSAGE_TYPE::E_TYPE_NORMAL},
	{L"３回目",MESSAGE_TYPE::E_TYPE_NORMAL},
	{L"４回目",MESSAGE_TYPE::E_TYPE_NORMAL},
	{L"５回目",MESSAGE_TYPE::E_TYPE_NORMAL},
};

HRESULT InitJyanken()
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr;

	// テクスチャ読み込み
	hr = CreateTextureFromFile(GetDevice(), TITLE_TEXTURENAME, &g_pTexture);
	if (FAILED(hr))return hr;
	hr = CreateTextureFromFile(GetDevice(), HATENA_TEXTURENAME, &g_pTexture1);
	if (FAILED(hr))return hr;


	for (int nCntJyankenMenu = 0; nCntJyankenMenu < NUM_JYANKEN_MENU; ++nCntJyankenMenu) {
		// テクスチャの読み込み
		hr = CreateTextureFromFile(pDevice,									// デバイスへのポインタ
			c_aFileNameJyankenMenu[nCntJyankenMenu],	// ファイルの名前
			&g_pTextures[nCntJyankenMenu]);			// 読み込むメモリー
	}
	for (int f = 0; f < 5; f++)
	{
		aite[f] = rand() % 3;//相手
	}

	g_Position = XMFLOAT3(OK_POS_X, OK_POS_Y, 0.0f);
	g_Alpha = 1.0f;
	g_Scale = XMFLOAT2(1.0f, 1.0f);


	g_nJyankenMenu = JYANKEN_MENU_GU;
	g_fCurve = 0.0f;

	return hr;
}

void UninitJyanken()
{
	// テクスチャの開放
	for (int nCntJyankenMenu = 0; nCntJyankenMenu < NUM_JYANKEN_MENU; ++nCntJyankenMenu) {
		SAFE_RELEASE(g_pTextures[nCntJyankenMenu]);
	}
	// テクスチャ開放
	SAFE_RELEASE(g_pTexture);
	SAFE_RELEASE(g_pTexture1);
}

void UpdateJyankenStart()
{

}

void UpdateJyankenSet()
{

	// 上下キーで各項目間の移動
	if (GetKeyRepeat(VK_D) || GetKeyRepeat(VK_RIGHT)) {
		g_nJyankenMenu = (JYANKEN_MENU)((g_nJyankenMenu + NUM_JYANKEN_MENU - 1) % NUM_JYANKEN_MENU);
		SetJyankenMenu();
	}
	else if (GetKeyRepeat(VK_A) || GetKeyRepeat(VK_LEFT)) {
		g_nJyankenMenu = (JYANKEN_MENU)((g_nJyankenMenu + 1) % NUM_JYANKEN_MENU);
		SetJyankenMenu();
	}

	POINT temp = (*GetMousePosition());
	XMFLOAT2 mousePos = XMFLOAT2(temp.x - SCREEN_CENTER_X, -(temp.y - SCREEN_CENTER_Y));


	//マウス
	XMFLOAT2 pos1 = XMFLOAT2(JYANKEN_MENU_POS_X, JYANKEN_MENU_POS_Y);
	XMFLOAT2 pos2 = XMFLOAT2(JYANKEN_MENU_POS_X - 1 * JYANKEN_MENU_INTERVAL, JYANKEN_MENU_POS_Y);
	XMFLOAT2 pos3 = XMFLOAT2(JYANKEN_MENU_POS_X - 2 * JYANKEN_MENU_INTERVAL, JYANKEN_MENU_POS_Y);
	XMFLOAT2 radius1 = XMFLOAT2(JYANKEN_MENU_WIDTH / 2, JYANKEN_MENU_HEIGHT / 2);
	XMFLOAT2 mpos2 = mousePos;
	XMFLOAT2 radius2 = XMFLOAT2(0.1, 0.1);
	if (CollisionBB(&pos1, &radius1, &mpos2, &radius2))
	{
		SetJyankenMenuGU();
	}
	else if (CollisionBB(&pos2, &radius1, &mpos2, &radius2))
	{
		SetJyankenMenuTYOKI();
	}
	else if (CollisionBB(&pos3, &radius1, &mpos2, &radius2))
	{
		SetJyankenMenuPA();
	}

	if (j >= 5)
	{
		XMFLOAT2 pos4 = XMFLOAT2(OK_POS_X, OK_POS_Y);
		XMFLOAT2 radius3 = XMFLOAT2(OK_SIZE_X / 2, OK_SIZE_X / 2);
		if (CollisionBB(&pos4, &radius3, &mpos2, &radius2))
		{
			if (GetMouseTrigger(0))
			{
				GetPhase()->ChangePhase(JUDGEPHASE);
				Cntadd();
			}
		}
	}

	// 枠の部分の色の変化(グラデーション)
	g_fCurve += XM_PI * 0.01f;
	if (g_fCurve > XM_PI) {
		g_fCurve -= XM_2PI;
	}

	g_fCol = cosf(g_fCurve) * 0.2f + 0.8f;

	if (j < 5)
	{
		if (GetKeyTrigger(VK_RETURN) || GetJoyTrigger(0, 0)|| GetMouseTrigger(0))
		{
			//選択中のものにより分岐
			JYANKEN_MENU menu = GetJyankenMenu();
			switch (menu)
			{
				//グー
			case JYANKEN_MENU_GU:
				te[j] = 0;
				j++;
				break;
				//チョキ
			case JYANKEN_MENU_TYOKI:
				te[j] = 1;
				j++;
				break;
				//パー
			case JYANKEN_MENU_PA:
				te[j] = 2;
				j++;
				break;
			}
		}
	}



}

void UpdateJyankenJadge()
{

    Cnt--;
	
    if(Cnt < 0)
	{
		jadge = false;
		Jyanken(n, result);
		n++;
		Cnt = 35;
	}
	if (Cnt == 30)
	{
		jadge = true;
	}
	
	if (n > 4)
	{
		Cnt1 = 180;
		GetPhase()->ChangePhase(BATTLEPHASE);
	}

}

void UpdateJyankenBattle()
{

	Cnt1--;

	//勝ったとき
	if (result[0] > result[1])
	{
		Action2();
	}
	//負けた時
	if (result[0] < result[1])
	{
		EAction();
	}

	if (Cnt1 < 0)
	{
		if (result[0] > result[1])
		{
			SetMessage((tMessage*)&testMessage[1]);
			DamageEnemy(50);
			if (result[0] >= 0)
				DamageEnemy(50 * result[0]);
		}
		if (result[0] < result[1])
		{
			SetMessage((tMessage*)&testMessage[2]);
			DamagePlayer(50);
			if (result[1] >= 0)
				DamagePlayer(50 * result[1]);
		}
		j = 0;
		n = 0;
		f = 0;
		for (int r = 0; r < 4; r++)
		{
			result[r] = 0;
		}
		for (int p = 0; p < 5; p++)
		{
			te[p] = -1;
			aite[p] = -1;
		}
		for (int f = 0; f < 5; f++)
		{
			aite[f] = rand() % 3;//相手
		}
		win = false;
		lose = false;
		GetPhase()->ChangePhase(SETPHASE);
	}
	
}

void DrawJyankenStart()
{

}


void DrawJyankenSet()
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();


	//ポーズメニューの表示
	SetPolygonSize(JYANKEN_MENU_WIDTH, JYANKEN_MENU_HEIGHT);
	for (int nCntJyankenMenu = 0; nCntJyankenMenu < NUM_JYANKEN_MENU; ++nCntJyankenMenu) {
		SetPolygonPos(JYANKEN_MENU_POS_X - nCntJyankenMenu * JYANKEN_MENU_INTERVAL, JYANKEN_MENU_POS_Y);


		//選択されているメニューを目立たせる
		if (nCntJyankenMenu == g_nJyankenMenu)
		{
			SetPolygonColor(1.0f, 1.0f, 1.0f);
		}
		else
		{
			SetPolygonColor(0.3f, 0.3f, 0.3f);
		}

		// テクスチャの設定
		SetPolygonTexture(g_pTextures[nCntJyankenMenu]);
		// ポリゴンの描画
		DrawPolygon(pDeviceContext);
	}

	SetPolygonColor(1.0f, 1.0f, 1.0f);

	for (int k = 0; k < 5;k++)
	{
		if (te[k] != -1)
		{
			//ポリゴン情報設定
			SetPolygonPos(-200 + 100 * k, -150);			//座標
			SetPolygonSize(JYANKEN_MENU_WIDTH, JYANKEN_MENU_HEIGHT);		//大きさ
			SetPolygonTexture(g_pTextures[te[k]]);		//テクスチャ

			//ポリゴンの描画処理
			DrawPolygon(GetDeviceContext());
		}
		if (aite[k] != -1)
		{
			//ポリゴン情報設定
			SetPolygonPos(300 + 50 * k, -300);			//座標
			SetPolygonSize(JYANKEN_MENU_WIDTH/2, JYANKEN_MENU_HEIGHT/2);		//大きさ
			SetPolygonTexture(g_pTexture1);		//テクスチャ

			//ポリゴンの描画処理
			DrawPolygon(GetDeviceContext());
		}
	}

	if (j >= 5)
	{
		// タイトル描画
	//ポリゴン情報設定
		SetPolygonPos(g_Position.x, g_Position.y);			//座標
		SetPolygonSize(OK_SIZE_X, OK_SIZE_Y);		//大きさ
		SetPolygonAngle(g_Angle.y);				//角度
		SetPolygonAlpha(g_Alpha);				//α値(透明度)

		SetPolygonUV(0.0f, 0.0f);			//0番のテクスチャ
		SetPolygonFrameSize(1.0f, 1.0f);	//テクスチャ
		SetPolygonTexture(g_pTexture);		//テクスチャ
		SetPolygonColor(1.0f, 1.0f, 1.0f);	//色(RGB)

		//ポリゴンの描画処理
		DrawPolygon(GetDeviceContext());
	}

}

void DrawJyankenJadge()
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	if (jadge == true)
	{
		if (te[n] != -1)
		{
			//ポリゴン情報設定
			SetPolygonPos(-100, 0);			//座標
			SetPolygonSize(JYANKEN_MENU_WIDTH, JYANKEN_MENU_HEIGHT);		//大きさ
			SetPolygonTexture(g_pTextures[te[n]]);		//テクスチャ

			//ポリゴンの描画処理
			DrawPolygon(GetDeviceContext());
		}
		if (aite[n] != -1)
		{
			//ポリゴン情報設定
			SetPolygonPos(100, 0);			//座標
			SetPolygonSize(JYANKEN_MENU_WIDTH, JYANKEN_MENU_HEIGHT);		//大きさ
			SetPolygonTexture(g_pTextures[aite[n]]);		//テクスチャ

			//ポリゴンの描画処理
			DrawPolygon(GetDeviceContext());
		}
	}

	for (int k = 0; k < 5; k++)
	{
		if (te[k] != -1)
		{
			//ポリゴン情報設定
			SetPolygonPos(-400 + 50 * k, -300);			//座標
			SetPolygonSize(JYANKEN_MENU_WIDTH/2, JYANKEN_MENU_HEIGHT/2);		//大きさ
			SetPolygonTexture(g_pTextures[te[k]]);		//テクスチャ

			//ポリゴンの描画処理
			DrawPolygon(GetDeviceContext());
		}
		if (aite[k] != -1)
		{
			//ポリゴン情報設定
			SetPolygonPos(200 + 50 * k, -300);			//座標
			SetPolygonSize(JYANKEN_MENU_WIDTH / 2, JYANKEN_MENU_HEIGHT / 2);		//大きさ
			SetPolygonTexture(g_pTextures[aite[k]]);		//テクスチャ

			//ポリゴンの描画処理
			DrawPolygon(GetDeviceContext());
		}
	}

}


//じゃんけん
void Jyanken(int no, int *cnt)
{

	if (aite[no] == te[no])
	{
		cnt[2]++;	// あいこ
		//z = z - 1;
	}
	else if ((te[no] == 0 && aite[no] == 1) || (te[no] == 1 && aite[no] == 2) || (te[no] == 2 && aite[no] == 0))
	{
		win = true;
		cnt[0]++;	// 勝ち
		cnt[3]++;	// 連勝
	}
	else
	{
		lose = true;
		cnt[1]++;	// まけ
		cnt[3] = 0;	// 連勝リセット
	}


}

//=============================================================================
// ポーズメニューの設定
//=============================================================================
void SetJyankenMenu(void)
{
	g_fCurve = 0.0f;
}

//=============================================================================
// ポーズメニューの取得
//=============================================================================
JYANKEN_MENU GetJyankenMenu(void)
{
	return g_nJyankenMenu;
}

//=============================================================================
// ポーズメニューのリセット
//=============================================================================
void ResetJyankenMenu(void)
{
	g_nJyankenMenu = JYANKEN_MENU_GU;
	SetJyankenMenu();
}

void SetJyankenMenuGU(void)
{
	g_nJyankenMenu = JYANKEN_MENU_GU;
	SetJyankenMenu();
}
void SetJyankenMenuTYOKI(void)
{
	g_nJyankenMenu = JYANKEN_MENU_TYOKI;
	SetJyankenMenu();
}
void SetJyankenMenuPA(void)
{
	g_nJyankenMenu = JYANKEN_MENU_PA;
	SetJyankenMenu();
}


void Cntadd()
{
	Cnt = 60;
}

