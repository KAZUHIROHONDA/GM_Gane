//=============================================================================
//
// 敵処理 [enemy.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "AssimpModel.h"
#include "enemy.h"
#include "main.h"

#include "shadow.h"		//影をつけるには入れる
#include "flame.h"
#include "bullet.h"
#include"input.h"
#include"enemyhp.h"
#include"fade.h"
#include "Egauge.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_ENEMY		"data/model/wani2.obj"
#define MODEL_ENEMY1	"data/model/wani1.obj"
#define MODEL_ENEMY2	"data/model/wani3.obj"
#define MODEL_ENEMY3	"data/model/wani4.obj"

#define VALUE_MOVE		(0.1f)		
#define VALUE_ROTATE	(2.0f)

#define ENEMY_RADIUS  (10.0f)

#define ENEMY_EGAUGE_POS_X	(125.0f)
#define ENEMY_EGAUGE_POS_Y	(300.0f)


//構造体定義
typedef struct _tEnemy
{
	XMFLOAT4X4		mtxWorld;		//座標変換行列

	XMFLOAT3		pos;			//座標
	XMFLOAT3		rot;			//回転
	XMFLOAT3		scl;			//拡大縮小
	XMFLOAT3		vel;			//速度

	int				nPhase;			//段階
	int				nState;			//状態(0:未使用　1:通常)
	int				Tipe;
	int				nStopTime;		//操作不能時間

	int				nHP;
	int				nEGauge;
	int				nShadowIdx;		//影の番号

}tEnemy;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static CAssimpModel		g_model[4];				//モデルデータ格納用
static tEnemy			g_enemy[ENEMY_MAX];	//プレイヤー
static tEnemy			g_enemyHD[ENEMY_MAX];
static tEnemy			g_enemyAM[ENEMY_MAX];
static tEnemy			g_enemyLG[ENEMY_MAX];


static int			g_nCnt1;//行動カウント1コメ
static int			g_nCnt2;//行動カウント2コメ
static int			g_nCnt3;//行動カウント3コメ
static int			g_nCnt4;//行動カウント4コメ
static bool				g_atama = true;//false;

int					bfCnt;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// 位置・回転・スケールの初期設定
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		g_enemy[i].pos = XMFLOAT3(0.0f, 20.0f, 20.0f);
		g_enemy[i].rot = XMFLOAT3(0.0f, 180.0f, 0.0f);
		g_enemy[i].scl = XMFLOAT3(15.0f, 15.0f, 15.0f);
		g_enemy[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		//初期化
		g_enemy[i].nPhase = 0;
		g_enemy[i].nState = 1;		//最初は通常
		g_enemy[i].Tipe = 0;

		g_enemy[i].nHP = 1000;
		g_enemy[i].nEGauge = g_enemy[i].nHP / 10;
		g_enemy[i].nShadowIdx = -1;


		//頭
		g_enemyHD[i].pos = XMFLOAT3(0.0f, 0.0f, -3.0f);
		g_enemyHD[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyHD[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_enemyHD[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyHD[i].nPhase = 0;
		g_enemyHD[i].nState = 1;	// puroぺらは最初から登場している
		g_enemyHD[i].nShadowIdx = -1;

		//腕
		g_enemyAM[i].pos = XMFLOAT3(0.0f, -0.8f, -1.0f);
		g_enemyAM[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyAM[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_enemyAM[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyAM[i].nPhase = 0;
		g_enemyAM[i].nState = 1;	// puroぺらは最初から登場している
		g_enemyAM[i].nShadowIdx = -1;

		//足
		g_enemyLG[i].pos = XMFLOAT3(0.0f, -0.8f, 0.0f);
		g_enemyLG[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyLG[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_enemyLG[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyLG[i].nPhase = 0;
		g_enemyLG[i].nState = 1;	// puroぺらは最初から登場している
		g_enemyLG[i].nShadowIdx = -1;

		g_nCnt1 = 0;					//
		g_nCnt2 = 0;					//
		g_nCnt3 = 0;
		g_nCnt4 = 0;

		g_enemy[i].nStopTime = 0; //最初は動ける

		g_enemy[i].nShadowIdx = CreateShadow(g_enemy[i].pos, 20.0f);
	}
	
	bfCnt = 0;

	// ワールドマトリックスの初期化
	XMMATRIX mtxWorld;
	mtxWorld = XMMatrixIdentity();

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		XMStoreFloat4x4(&g_enemy[i].mtxWorld, mtxWorld);
		XMStoreFloat4x4(&g_enemyHD[i].mtxWorld, mtxWorld);
		XMStoreFloat4x4(&g_enemyAM[i].mtxWorld, mtxWorld);
		XMStoreFloat4x4(&g_enemyLG[i].mtxWorld, mtxWorld);

	}

	// モデルデータの読み込み
	hr = g_model[0].Load(pDevice, pDeviceContext,
		MODEL_ENEMY); if (FAILED(hr)) return hr;

	hr = g_model[1].Load(pDevice, pDeviceContext,
		MODEL_ENEMY1); if (FAILED(hr)) return hr;

	hr = g_model[2].Load(pDevice, pDeviceContext,
		MODEL_ENEMY2); if (FAILED(hr)) return hr;

	hr = g_model[3].Load(pDevice, pDeviceContext,
		MODEL_ENEMY3); if (FAILED(hr)) return hr;

	return hr;

}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	//影の解放
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		ReleaseShadow(g_enemy[i].nShadowIdx);
	}

	// モデルの解放
	g_model[0].Release();
	g_model[1].Release();
	g_model[2].Release();
	g_model[3].Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	// プレイヤーの動きを記述するならここ
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		//未使用
		if (g_enemy[i].nState == 0) continue;

		if (GetKeyPress(VK_Z))
		{
			//g_enemy[i].rot.y+=5;
			g_enemyHD[i].rot.z += 5;
			//g_enemyAM[i].rot.x += 5;
			g_enemyLG[i].rot.x += 5;

		}


		if (GetKeyPress(VK_7))
		{
			g_nCnt1++;
			g_enemy[i].pos.z = 30;
			g_enemy[i].rot.x = 30;
		}




		g_nCnt4++;
		if (GetKeyPress(VK_0))//負けアニメーション
		{
			g_enemy[i].rot.z = 180;
			if (g_nCnt4 >= 20)
			{
				g_enemyHD[i].pos.z -= 0.05f;
				g_enemyHD[i].rot.z -= 1.0f;
				g_enemyHD[i].rot.y -= 1.0f;

				g_enemyAM[i].pos.x -= 0.05f;
				g_enemyAM[i].rot.z -= 1.0f;
				g_enemyAM[i].rot.y -= 1.0f;

				g_enemyLG[i].pos.z += 0.05f;
				g_enemyLG[i].rot.z -= 1.0f;
				g_enemyLG[i].rot.y -= 1.0f;
			}

		}




		//ゲージの動きの処理
		if (g_enemy[i].nEGauge >= g_enemy[i].nHP / 10)
		{
			g_enemy[i].nEGauge--;
		}

		if (GetKeyTrigger(VK_F))
		{
			DamageEnemy(50);
		}


		XMMATRIX mtxWorld, mtxRot, mtxScl,
			mtxTranslate;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_enemy[i].scl.x, g_enemy[i].scl.y, g_enemy[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);



		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(g_enemy[i].rot.x), XMConvertToRadians(g_enemy[i].rot.y + 180), XMConvertToRadians(g_enemy[i].rot.z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);


		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_enemy[i].pos.x, g_enemy[i].pos.y, g_enemy[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);


		// ワールドマトリックス設定
		XMStoreFloat4x4(&g_enemy[i].mtxWorld, mtxWorld);

		//影の移動
		MoveShadow(g_enemy[i].nShadowIdx, g_enemy[i].pos);


		//************************************************:
		//頭

		XMMATRIX mtxWorldHD;
		// ワールドマトリックスの初期化
		mtxWorldHD = XMMatrixIdentity();
		// スケールを反映
		mtxScl = XMMatrixScaling(g_enemyHD[i].scl.x, g_enemyHD[i].scl.y, g_enemyHD[i].scl.z);
		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxScl);
		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(g_enemyHD[i].rot.x),
			XMConvertToRadians(g_enemyHD[i].rot.y),//+180
			XMConvertToRadians(g_enemyHD[i].rot.z)
		);
		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_enemyHD[i].pos.x, g_enemyHD[i].pos.y, g_enemyHD[i].pos.z);

		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxTranslate);

		//親の行列を掛ける
		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxWorld);

		// ワールドマトリックス設定
		XMStoreFloat4x4(&g_enemyHD[i].mtxWorld, mtxWorldHD);


		//************************************************:
		//腕

		XMMATRIX mtxWorldAM;
		// ワールドマトリックスの初期化
		mtxWorldAM = XMMatrixIdentity();
		// スケールを反映
		mtxScl = XMMatrixScaling(g_enemyAM[i].scl.x, g_enemyAM[i].scl.y, g_enemyAM[i].scl.z);
		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxScl);
		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(g_enemyAM[i].rot.x),
			XMConvertToRadians(g_enemyAM[i].rot.y),//+180
			XMConvertToRadians(g_enemyAM[i].rot.z)
		);
		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_enemyAM[i].pos.x, g_enemyAM[i].pos.y, g_enemyAM[i].pos.z);

		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxTranslate);

		//親の行列を掛ける
		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxWorld);

		// ワールドマトリックス設定
		XMStoreFloat4x4(&g_enemyAM[i].mtxWorld, mtxWorldAM);

		//************************************************:
		//足

		XMMATRIX mtxWorldLG;
		// ワールドマトリックスの初期化
		mtxWorldLG = XMMatrixIdentity();
		// スケールを反映
		mtxScl = XMMatrixScaling(g_enemyLG[i].scl.x, g_enemyLG[i].scl.y, g_enemyLG[i].scl.z);
		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxScl);
		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(g_enemyLG[i].rot.x),
			XMConvertToRadians(g_enemyLG[i].rot.y),//+180
			XMConvertToRadians(g_enemyLG[i].rot.z)
		);
		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_enemyLG[i].pos.x, g_enemyLG[i].pos.y, g_enemyLG[i].pos.z);

		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxTranslate);

		//親の行列を掛ける
		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxWorld);

		// ワールドマトリックス設定
		XMStoreFloat4x4(&g_enemyLG[i].mtxWorld, mtxWorldLG);
	}


}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		//未使用
		if (g_enemy[i].nState == 0) continue;

		// モデル表示
		g_model[0].Draw(pDeviceContext, g_enemy[i].mtxWorld);
		g_model[1].Draw(pDeviceContext, g_enemyHD[i].mtxWorld);
		g_model[2].Draw(pDeviceContext, g_enemyAM[i].mtxWorld);
		g_model[3].Draw(pDeviceContext, g_enemyLG[i].mtxWorld);

	}
}

//生存確認
bool IsEnemy(int no)
{
	if (no < 0 || no >= ENEMY_MAX) return false;
	return g_enemy[no].nState > 0 ?
		true : false;
}

/*void DestroyEnemy(int no)
{
	if (no < 0 || no >= ENEMY_MAX) return;
	g_enemy[no].nState = 0;
	// 影との関連を切る
	ReleaseShadow(g_enemy[no].nShadowIdx);
	g_enemy[no].nShadowIdx = -1;

	for (int i = 0; i <= 10; i++)
	{
		SetFlame(XMFLOAT3(g_enemy[no].pos.x, g_enemy[no].pos.y, g_enemy[no].pos.z), 5, bfCnt);
	}
	bfCnt++;
}*/

int FlameCnt()
{
	return bfCnt = 0;
}

//座標取得
XMFLOAT3 GetEnemyPos(int no)
{
	if (no < 0 || no >= ENEMY_MAX)
		return XMFLOAT3(0.0f, 0.0f, 0.0f);
	return g_enemy[no].pos;
}

//サイズ取得
XMFLOAT3 GetEnemySize(int no)
{
	if (no < 0 || no >= ENEMY_MAX)
		return XMFLOAT3(0,0,0);
	//return XMFLOAT3(10.0f,10.0f,10.0f);
	XMFLOAT3 box = g_model[0].GetBBox();
	return XMFLOAT3(box.x / 2 * g_enemy[no].scl.x, box.y / 2 * g_enemy[no].scl.y, box.z / 2 * g_enemy[no].scl.z);
}

// 敵の出現
/*int SetEnemy(XMFLOAT3 pos,int no)
{
	tEnemy* pEnemy = g_enemy;
	for (int i = 0; i < ENEMY_MAX; ++i, ++pEnemy)
	{	// 出現している敵はスルー
		if (pEnemy->nState > 0) continue;

		pEnemy->pos = pos;
		switch (no)
		{  
		case 0:
		{
			g_enemy[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
			g_enemy[i].Tipe = 0;
		} break;
		case 1:
		{
			g_enemy[i].vel = XMFLOAT3(-VALUE_MOVE, 0.0f, 0.0f);
			g_enemy[i].Tipe = 1;
		} break;
		case 2: 
		{
			g_enemy[i].vel = XMFLOAT3(VALUE_MOVE,  0.0f, 0.0f);
			g_enemy[i].Tipe = 1; 
		} break;
		case 3: 
		{
			g_enemy[i].vel = XMFLOAT3(VALUE_MOVE*5, 0.0f, 0.0f);
			g_enemy[i].Tipe = 2; 
		} break;
		case 4:
		{
			g_enemy[i].vel = XMFLOAT3(-VALUE_MOVE*5, 0.0f, 0.0f);
			g_enemy[i].Tipe = 2;
		} break;
		case 5:
		{
			g_enemy[i].vel = XMFLOAT3( 0.0f, 0.0f, -VALUE_MOVE*5);
			g_enemy[i].Tipe = 2;
		} break;
		case 6:
		{
			g_enemy[i].vel = XMFLOAT3( 0.0f, 0.0f, VALUE_MOVE*5);
			g_enemy[i].Tipe = 2;
		} break;
		case 7:
		{
			g_enemy[i].vel = XMFLOAT3(-VALUE_MOVE * 5, 0.0f, -VALUE_MOVE * 5);
			g_enemy[i].Tipe = 2;
		} break;
		case 8:
		{
			g_enemy[i].vel = XMFLOAT3(VALUE_MOVE * 5, 0.0f, VALUE_MOVE * 5);
			g_enemy[i].Tipe = 2;
		} break;
		}
		pEnemy->nState = 1;	// 出現
		pEnemy->nShadowIdx =
			CreateShadow(pEnemy->pos, ENEMY_RADIUS);
		//pEnemy->rot.y = XMConvertToDegrees(atan2f(dir.x, dir.z));//モデルの向き
		return i;	// 番号を返す
	}

	return -1;	// 見つからなかった
}*/


int GetEnemyHp(int no)
{
	if (no < 0 || no >= ENEMY_MAX)	return(0);

	return g_enemy[no].nHP;


}

void DamageEnemy(int damage)
{

	g_enemy[0].nHP -= damage;
	if (g_enemy[0].nHP <= 0)
	{
		g_enemy[0].nHP = 0;

		DestroyEnemy(0);


		//StartSceneChange(SCENE_GAMECLEAR);
	}
}

void DestroyEnemy(int no)
{
	if (no < 0 || no >= ENEMY_MAX)	return;

	g_enemy[no].nState = 0;

	//影の開放
	ReleaseShadow(g_enemy[no].nShadowIdx);
	g_enemy[no].nShadowIdx = -1;

	StartFade(SCENE_CLEAR);

}
void DrawEnemyEGauge()
{
	const XMFLOAT2 pos = { ENEMY_EGAUGE_POS_X, ENEMY_EGAUGE_POS_Y };

	DrawEGauge(pos, (float)g_enemy[0].nEGauge / EGAUGE_MAX);
}

void ResetEPos(int no)
{
	if (no < 0 || no >= ENEMY_MAX)	return;

	g_enemy[no].pos = XMFLOAT3(0.0f, 20.0f, 20.0f);
	g_enemy[no].rot = XMFLOAT3(0.0f, 180.0f, 0.0f);
	g_enemy[no].scl = XMFLOAT3(8.0f, 8.0f, 8.0f);
	g_enemy[no].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_enemyHD[no].pos = XMFLOAT3(0.0f, 0.0f, -3.0f);
	g_enemyHD[no].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_enemyHD[no].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
	g_enemyHD[no].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);

	g_nCnt1 = 0;
	g_nCnt2 = 0;
}