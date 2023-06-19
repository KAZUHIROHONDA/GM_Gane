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
static int te[5];
static int result[4]; //勝ち数, 負け数, あいこ数, 連勝数

int z, i, aite;
int j = 0;
int n = 0;

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


	if (j <= 5)
	{
		SetMessage((tMessage*)&testMessage[j + 4]);
		if (GetKeyTrigger(VK_RETURN) || GetJoyTrigger(0, 0))
		{
			//選択中のものにより分岐
			JYANKEN_MENU menu = GetJyankenMenu();
			switch (menu)
			{
				//グー
			case JYANKEN_MENU_GU:
				te[j] = 0;
				j++;
				//Jyanken(0);
				break;
				//チョキ
			case JYANKEN_MENU_TYOKI:
				te[j] = 1;
				j++;
				//Jyanken(1);
				break;
				//パー
			case JYANKEN_MENU_PA:
				te[j] = 2;
				j++;
				//Jyanken(2);
				break;
			}
		}
	}

	if (j > 5)
	{
		if (n <= 4)
		{
			if (GetKeyTrigger(VK_RETURN) || GetJoyTrigger(0, 0))
			{
				Jyanken(te[n], result);
				n++;
			}
		}
		else if (n > 4)
		{
			j = 0;
			n = 0;
			for (int r = 0; r < 4; r++)
			{
				result[r] = 0;
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
void Jyanken(int no, int *cnt)
{
	i = no;

	aite = rand() % 3;//相手


	if (aite == i)
	{
		SetMessage((tMessage*)&testMessage[3]);
		cnt[2]++;	// あいこ
		//StartFade(SCENE_TITLE,180);
		//z = z - 1;
	}
	//else if (i == 0)i = 3;
	//if (aite < i)
	else if ((i == 0 && aite == 1) || (i == 1 && aite == 2) || (i == 2 && aite == 0))
	{
		SetMessage((tMessage*)&testMessage[1]);
		DamageEnemy(50);
		if (cnt[0] >= 0)
			DamageEnemy(50 * cnt[0]);
		cnt[0]++;	// 勝ち
		cnt[3]++;	// 連勝
		//printf("勝ち\n\n");
	}
	else
	{
		SetMessage((tMessage*)&testMessage[2]);
		DamagePlayer(50);
		if (cnt[1] >= 0)
			DamagePlayer(50 * cnt[1]);
		cnt[1]++;	// まけ
		cnt[3] = 0;	// 連勝リセット
		//printf("負け\n\n");
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






