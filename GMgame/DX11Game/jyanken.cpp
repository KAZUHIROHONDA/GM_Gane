#include "Title.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"
#include "MessageManager.h"
#include "jyanken.h"
#include"fade.h"
#include"player.h"
#include"enemy.h"

#define gu 1-1
#define tyoki 2-1
#define pa 3-1

// マクロ定義
#define	NUM_JYANKEN_MENU		(3)			// ポーズメニュー数
#define	JYANKEN_MENU_WIDTH	(200.0f)	// ポーズメニュー幅
#define	JYANKEN_MENU_HEIGHT	(200.0f)		// ポーズメニュー高さ
#define	JYANKEN_MENU_INTERVAL	(200.0f)	// ポーズメニュー間隔
#define	JYANKEN_MENU_POS_X	(0.0f)		// ポーズメニュー位置(X座標)
#define	JYANKEN_MENU_POS_Y	(-50.0f)	// ポーズメニュー位置(Y座標)
#define	PLATE_WIDTH			(360.0f)	// プレートの幅
#define	PLATE_HEIGHT		(340.0f)	// プレートの幅
#define	PLATE_POS_X			(0.0f)		// プレートの位置(X座標)
#define	PLATE_POS_Y			(0.0f)		// プレートの位置(Y座標)

static ID3D11ShaderResourceView*	g_pTextures[3] = { nullptr };

// テクスチャファイル名
#define TITLE_TEXTURENAME	_T("data/texture/Title.png")
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


int z, i, aite;
int bef;
int win;

const tMessage testMessage[7] = {
	{L"じゃんけん５回勝負！！",MESSAGE_TYPE::E_TYPE_ENTER,(long long int)&testMessage[1] },
	{L"グーはV、チョキはB、パーはNを入力してね！",MESSAGE_TYPE::E_TYPE_ENTER},
	{L"かち！",MESSAGE_TYPE::E_TYPE_ENTER},
	{L"まけ！",MESSAGE_TYPE::E_TYPE_ENTER},
	{L"あいこ！",MESSAGE_TYPE::E_TYPE_ENTER},
	{L"2枚目",MESSAGE_TYPE::E_TYPE_ENTER,NULL},
	{L"2枚目",MESSAGE_TYPE::E_TYPE_NORMAL,}
};

HRESULT InitJyanken()
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr;
	bef = 0;
	win = 0;

	SetMessage((tMessage*)&testMessage[0]);
	for (int nCntJyankenMenu = 0; nCntJyankenMenu < NUM_JYANKEN_MENU; ++nCntJyankenMenu) {
		// テクスチャの読み込み
		hr = CreateTextureFromFile(pDevice,									// デバイスへのポインタ
			c_aFileNameJyankenMenu[nCntJyankenMenu],	// ファイルの名前
			&g_pTextures[nCntJyankenMenu]);			// 読み込むメモリー
	}

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
}

void UpdateJyanken()
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
					g_nJyankenMenu = (JYANKEN_MENU)((g_nJyankenMenu + 1) % NUM_JYANKEN_MENU);
					SetJyankenMenu();
					nStopTime = 10;
				}
				if (angle < 135 || angle > 45)
				{
					len *= 0.3f;
					g_nJyankenMenu = (JYANKEN_MENU)((g_nJyankenMenu + NUM_JYANKEN_MENU - 1) % NUM_JYANKEN_MENU);
					SetJyankenMenu();
					nStopTime = 10;
				}


			}
		}
	}

	// 上下キーで各項目間の移動
	if (GetKeyRepeat(VK_A) || GetKeyRepeat(VK_RIGHT)) {
		g_nJyankenMenu = (JYANKEN_MENU)((g_nJyankenMenu + NUM_JYANKEN_MENU - 1) % NUM_JYANKEN_MENU);
		SetJyankenMenu();
	}
	else if (GetKeyRepeat(VK_D) || GetKeyRepeat(VK_LEFT)) {
		g_nJyankenMenu = (JYANKEN_MENU)((g_nJyankenMenu + 1) % NUM_JYANKEN_MENU);
		SetJyankenMenu();
	}

	// 枠の部分の色の変化(グラデーション)
	g_fCurve += XM_PI * 0.01f;
	if (g_fCurve > XM_PI) {
		g_fCurve -= XM_2PI;
	}

	g_fCol = cosf(g_fCurve) * 0.2f + 0.8f;

	//ポーズ中の項目決定

	E_FADE fadeState = GetFade();
	if (fadeState == E_FADE_NONE)
	{
		if (GetKeyTrigger(VK_RETURN) || GetJoyTrigger(0, 0))
		{
			//選択中のものにより分岐
			JYANKEN_MENU menu = GetJyankenMenu();
			switch (menu)
			{
				//グー
			case JYANKEN_MENU_GU:
				Jyanken(0);
				break;
				//チョキ
			case JYANKEN_MENU_TYOKI:
				Jyanken(1);
				break;
				//パー
			case JYANKEN_MENU_PA:
				Jyanken(2);
				break;
			}
		}
	}



}


void DrawJyanken()
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

}

//じゃんけん
void Jyanken(int no)
{
	i = no;

	aite = rand() % 3;//相手


	if (aite == i)
	{
		SetMessage((tMessage*)&testMessage[4]);
		//StartFade(SCENE_TITLE,180);
		//printf("あいこ\nもう一度！\n\n>");
		//z = z - 1;
	}
	//else if (i == 0)i = 3;
	//if (aite < i)
	else if ((i == 0 && aite == 1) || (i == 1 && aite == 2) || (i == 2 && aite == 0))
	{
		SetMessage((tMessage*)&testMessage[2]);
		DamageEnemy(50);
		//printf("勝ち\n\n");
		//win++;
		//StartSceneChange(SCENE_CLEAR);
	}
	else
	{
		SetMessage((tMessage*)&testMessage[3]);
		DamagePlayer(50);
		//printf("負け\n\n");
		//StartSceneChange(SCENE_GAMEOVER);
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





