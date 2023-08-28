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
#include "sceneTitle.h"
#include "sceneGame.h"
#include "fade.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//=======
//<<<<<<< HEAD

//=======
#define MODEL_ENEMY		"data/model/waniBD2.obj"//"data/model/butaBD.obj"
#define MODEL_ENEMY1	 "data/model/wani1.obj" //"data/model/butaHD.obj"
#define MODEL_ENEMY2	"data/model/waniAM.obj" //"data/model/butaAM.obj"
#define MODEL_ENEMY3	"data/model/waniAM.obj" //"data/model/butaLG.obj"
///>>>>>>> feature/matsui2

#define VALUE_MOVE		(0.1f)		
#define VALUE_ROTATE	(2.0f)

#define ENEMY_RADIUS  (10.0f)

#define ENEMY_EGAUGE_POS_X	(110.0f)
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


static int			g_nECnt1;//行動カウント1コメ
static int			g_nECnt2;//行動カウント2コメ
static int			g_nECnt3;//行動カウント3コメ
static int			g_nECnt4;//行動カウント4コメ



static int			g_nECnt5;//行動カウント5コメ
static int			g_nECnt6;//行動カウント6コメ
static int			g_nECnt7;//行動カウント6コメ
static int			g_nECnt8;//行動カウント6コメ
static int			g_nECnt9;//行動カウント6コメ
static int			g_nECnt10;//行動カウント6コメ
static int			g_nECnt11;//行動カウント6コメ
static int			g_nECnt12;//行動カウント6コメ
static bool				g_atama = true;//false;

static bool			g_Eaction;
static bool			g_Eaction2;
static bool			g_Eaction3;
static bool			g_Eaction4;
static bool			g_Eaction5;
static bool			g_Eaction6;
static bool			g_Eaction7;
static bool			g_Eaction8;
static bool			g_Eaction9;
static bool			g_Eaction10;
static bool			g_Eaction11;


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
		g_enemy[i].pos = XMFLOAT3(0.0f, 20.0f,100.0f);//z100
		g_enemy[i].rot = XMFLOAT3(0.0f, 180.0f, 0.0f);
		g_enemy[i].scl = XMFLOAT3(15.0f, 15.0f, 15.0f);
		g_enemy[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);

		//初期化
		g_enemy[i].nPhase = 0;
		g_enemy[i].nState = 1;		//最初は通常
		g_enemy[i].Tipe = 0;

		g_enemy[i].nHP = GetEnemy()->GetHP();
		g_enemy[i].nEGauge = 100;
		g_enemy[i].nShadowIdx = -1;

		

		g_Eaction = false;
		g_Eaction2 = false;
		g_Eaction3 = false;
		g_Eaction4 = false;
		g_Eaction5 = false;
		g_Eaction6 = false;
		g_Eaction7 = false;
		g_Eaction8 = false;
		g_Eaction9 = false;
		g_Eaction10 = false;
		g_Eaction11 = false;

		g_nECnt1 = 0;					//
		g_nECnt2 = 0;					//
		g_nECnt3 = 0;
		g_nECnt4 = 0;
		g_nECnt5 = 0;
		g_nECnt6 = 0;
		g_nECnt7 = 0;
		g_nECnt8 = 0;
		g_nECnt9 = 0;
		g_nECnt10 = 0;
		g_nECnt11 = 0;
		g_nECnt12 = 0;


		//頭 wani
		g_enemyHD[i].pos = XMFLOAT3(0.0f, 0.0f, -1.0f);//z-3
		g_enemyHD[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyHD[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_enemyHD[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyHD[i].nPhase = 0;
		g_enemyHD[i].nState = 1;	// puroぺらは最初から登場している
		g_enemyHD[i].nShadowIdx = -1;

		//panda
		//g_enemyHD[i].pos = XMFLOAT3(0.0f, 0.0f, -1.0f);
		//g_enemyHD[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		//g_enemyHD[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		//g_enemyHD[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		//g_enemyHD[i].nPhase = 0;
		//g_enemyHD[i].nState = 1;	// puroぺらは最初から登場している
		//g_enemyHD[i].nShadowIdx = -1;

		////腕-0.8f
		g_enemyAM[i].pos = XMFLOAT3(0.0f, -0.5f, 0.0f);//-0.8,-1
		g_enemyAM[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyAM[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_enemyAM[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyAM[i].nPhase = 0;
		g_enemyAM[i].nState = 1;	// puroぺらは最初から登場している
		g_enemyAM[i].nShadowIdx = -1;

		//腕
		//g_enemyAM[i].pos = XMFLOAT3(0.0f, -0.3f, -0.7f);
		//g_enemyAM[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		//g_enemyAM[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		//g_enemyAM[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		//g_enemyAM[i].nPhase = 0;
		//g_enemyAM[i].nState = 1;	// puroぺらは最初から登場している
		//g_enemyAM[i].nShadowIdx = -1;

		//足
		g_enemyLG[i].pos = XMFLOAT3(0.0f, -0.5f, 1.5f);
		g_enemyLG[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyLG[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_enemyLG[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyLG[i].nPhase = 0;
		g_enemyLG[i].nState = 1;	// puroぺらは最初から登場している
		g_enemyLG[i].nShadowIdx = -1;

		////足
		//g_enemyLG[i].pos = XMFLOAT3(0.0f, -0.3f, 0.5f);
		//g_enemyLG[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		//g_enemyLG[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		//g_enemyLG[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		//g_enemyLG[i].nPhase = 0;
		//g_enemyLG[i].nState = 1;	// puroぺらは最初から登場している
		//g_enemyLG[i].nShadowIdx = -1;

		

		g_enemy[i].nStopTime = 0; //最初は動ける

		g_enemy[i].nShadowIdx = CreateShadow(g_enemy[i].pos, 20.0f);
	}
	
	bfCnt = 0;
	g_Eaction = false;
	g_Eaction2 = false;
	g_Eaction3 = false;

	g_nECnt1 = 0;					//
	g_nECnt2 = 0;					//
	g_nECnt3 = 0;
	g_nECnt4 = 0;

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

HRESULT InitSelectEnemy(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// 位置・回転・スケールの初期設定
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		g_enemy[i].pos = XMFLOAT3(0.0f, 20.0f, 100.0f);//z100
		g_enemy[i].rot = XMFLOAT3(0.0f, 180.0f, 0.0f);
		g_enemy[i].scl = XMFLOAT3(15.0f, 15.0f, 15.0f);
		g_enemy[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);

		//初期化
		g_enemy[i].nPhase = 0;
		g_enemy[i].nState = 0;		//最初は通常
		g_enemy[i].Tipe = 0;

		g_enemy[i].nHP = GetEnemy()->GetHP();
		g_enemy[i].nEGauge = 100;
		g_enemy[i].nShadowIdx = -1;

		//頭 wani
		g_enemyHD[i].pos = XMFLOAT3(0.0f, 0.0f, -1.0f);//z-3
		g_enemyHD[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyHD[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_enemyHD[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyHD[i].nPhase = 0;
		g_enemyHD[i].nState = 1;	// puroぺらは最初から登場している
		g_enemyHD[i].nShadowIdx = -1;

		////腕-0.8f
		g_enemyAM[i].pos = XMFLOAT3(0.0f, -0.5f, 0.0f);//-0.8,-1
		g_enemyAM[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyAM[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_enemyAM[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyAM[i].nPhase = 0;
		g_enemyAM[i].nState = 1;	// puroぺらは最初から登場している
		g_enemyAM[i].nShadowIdx = -1;

		//足
		g_enemyLG[i].pos = XMFLOAT3(0.0f, -0.5f, 1.5f);
		g_enemyLG[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyLG[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_enemyLG[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyLG[i].nPhase = 0;
		g_enemyLG[i].nState = 1;	// puroぺらは最初から登場している
		g_enemyLG[i].nShadowIdx = -1;


		g_enemy[i].nStopTime = 0; //最初は動ける

		g_enemy[i].nShadowIdx = CreateShadow(g_enemy[i].pos, 20.0f);
	}

	bfCnt = 0;
	g_Eaction = false;
	g_Eaction2 = false;
	g_Eaction3 = false;

	g_nECnt1 = 0;					//
	g_nECnt2 = 0;					//
	g_nECnt3 = 0;
	g_nECnt4 = 0;

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
		ResetEPos(i);
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


		//操作不能時間
		if (g_enemy[i].nStopTime > 0)
		{
			g_enemy[i].nStopTime--;
		}
		if (g_enemy[i].nStopTime == 0)
		{
			if (GetKeyTrigger(VK_8))
			{
				g_Eaction = true;
			}
			EAction(g_Eaction);

			if (GetKeyTrigger(VK_6))
			{
				g_Eaction2 = true;
			}
			EAction2(g_Eaction2);

			if (GetKeyTrigger(VK_7))
			{
				g_Eaction3 = true;
			}
			EAction3(g_Eaction3);

			if (GetKeyTrigger(VK_9))
			{
				g_Eaction4 = true;
			}
			EAction4(g_Eaction4);


			if (GetKeyTrigger(VK_0))
			{
				g_Eaction5 = true;
			}
			EAction5(g_Eaction5);

			if (GetKeyTrigger(VK_1))
			{
				g_Eaction6 = true;
			}
			EAction6(g_Eaction6);

			if (GetKeyTrigger(VK_2))
			{
				g_Eaction7 = true;
			}
			EAction7(g_Eaction7);

			if (GetKeyTrigger(VK_3))
			{
				g_Eaction8 = true;
			}
			EAction8(g_Eaction8);

			if (GetKeyTrigger(VK_4))
			{
				g_Eaction9 = true;
			}
			EAction9(g_Eaction9);

			if (GetKeyTrigger(VK_5))
			{
				g_Eaction10 = true;
			}
			EAction10(g_Eaction10);


			if (GetKeyTrigger(VK_M))
			{
				g_Eaction11 = true;
			}
			EAction11(g_Eaction11);

		}
		
		//g_nECnt4++;
		//if (GetKeyPress(VK_0))//負けアニメーション
		//{
		//	g_enemy[i].rot.z = 180;
		//	if (g_nECnt4 >= 20)
		//	{
		//		g_enemyHD[i].pos.z -= 0.05f;
		//		g_enemyHD[i].rot.z -= 1.0f;
		//		g_enemyHD[i].rot.y -= 1.0f;

		//		g_enemyAM[i].pos.x -= 0.05f;
		//		g_enemyAM[i].rot.z -= 1.0f;
		//		g_enemyAM[i].rot.y -= 1.0f;

		//		g_enemyLG[i].pos.z += 0.05f;
		//		g_enemyLG[i].rot.z -= 1.0f;
		//		g_enemyLG[i].rot.y -= 1.0f;
		//	}

		//}




		//ゲージの動きの処理
		if (g_enemy[i].nEGauge >= (GetEnemy()->GetHP() * 100 / g_enemy[i].nHP))
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

void UpdateSelectEnemy(void)
{
	// プレイヤーの動きを記述するならここ
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		//未使用
		if (g_enemy[i].nState == 0) continue;

		float hitLength;

		//位置と半径を指定
		hitLength = ECheckCollisionRay(XMVectorSet(g_enemy[i].pos.x, g_enemy[i].pos.y, g_enemy[i].pos.z, 0.0f), 10.0f);
		if (hitLength < 9999.9f)
		{
			g_enemy[i].rot.y += 1.0f;
			if (GetMouseTrigger(0))
			{
				//アクション入れたい
				StartFade(SCENE_GAME);

			}
		}
		else
		{
			g_enemy[i].rot = XMFLOAT3(0, 90, 0);
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
int SetEnemy(XMFLOAT3 pos, XMFLOAT3 rot)
{
	tEnemy* pEnemy = g_enemy;
	for (int i = 0; i < ENEMY_MAX; ++i, ++pEnemy)
	{	// 出現している敵はスルー
		if (pEnemy->nState > 0) continue;

		pEnemy->pos = pos;

		g_enemy[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemy[i].Tipe = 0;

		pEnemy->nState = 1;	// 出現
		pEnemy->nShadowIdx =
			CreateShadow(pEnemy->pos, ENEMY_RADIUS);
		pEnemy->rot = rot;//モデルの向き
		return i;	// 番号を返す
	}

	return -1;	// 見つからなかった
}


int GetEnemyHp(int no)
{
	if (no < 0 || no >= ENEMY_MAX)	return(0);

	return g_enemy[no].nHP;


}

void DamageEnemy(int damage)
{

	int HP = GetEnemy()->GetHP();

	HP -= damage;
	if (HP <= 0)
	{
		HP = 0;

		g_Eaction3 = true;
	}

	GetEnemy()->SetHP(HP);
}

void DestroyEnemy(int no)
{
	if (no < 0 || no >= ENEMY_MAX)	return;

	g_enemy[no].nState = 0;

	//影の開放
	ReleaseShadow(g_enemy[no].nShadowIdx);
	g_enemy[no].nShadowIdx = -1;

	Clearflag();

}
void DrawEnemyEGauge()
{
	const XMFLOAT2 pos = { ENEMY_EGAUGE_POS_X, ENEMY_EGAUGE_POS_Y };

	DrawEGauge(pos, (float)g_enemy[0].nEGauge / EGAUGE_MAX);
}

void ResetEPos(int no)
{
	if (no < 0 || no >= ENEMY_MAX)	return;

	g_enemy[no].pos = XMFLOAT3(0.0f, 20.0f, 100.0f);
	g_enemy[no].rot = XMFLOAT3(0.0f, 180.0f, 0.0f);
	g_enemy[no].scl = XMFLOAT3(15.0f, 15.0f, 15.0f);
	g_enemy[no].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);

	g_enemyHD[no].pos = XMFLOAT3(0.0f, 0.0f, -1.0f);
	g_enemyHD[no].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_enemyHD[no].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
	g_enemyHD[no].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);

	g_enemyAM[no].pos = XMFLOAT3(0.0f, -0.5f, 0.0f);///-0.8,-1
	g_enemyAM[no].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_enemyAM[no].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
	g_enemyAM[no].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);

	g_enemyLG[no].pos = XMFLOAT3(0.0f, -0.5f, 1.5f);
	g_enemyLG[no].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_enemyLG[no].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
	g_enemyLG[no].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);

	g_nECnt1 = 0;
	g_nECnt2 = 0;
	g_nECnt3 = 0;
	g_nECnt4 = 0;
	g_nECnt5 = 0;
	g_nECnt6 = 0;
	g_nECnt7 = 0;
	g_nECnt8 = 0;
	g_nECnt9 = 0;
	g_nECnt10 = 0;
	g_nECnt11 = 0;
	g_nECnt12 = 0;

	g_Eaction = false;
	g_Eaction2 = false;
	g_Eaction3 = false;
	g_Eaction4 = false;
	g_Eaction5 = false;
	g_Eaction6 = false;
	g_Eaction7 = false;
	g_Eaction8 = false;
	g_Eaction9 = false;
	g_Eaction10 = false;
	g_Eaction11 = false;
}

void EAction(bool af)//落下ドリル
{
	if (af)
	{
		for (int i = 0; i < ENEMY_MAX; i++)
		{
			g_nECnt1++;
			g_nECnt2++;
			g_enemyHD[i].rot.z += 50;
			if (g_nECnt1 <= 150)
			{
				g_enemy[i].pos.y++;

			}
			if (g_nECnt1 >= 150 && g_nECnt2 <= 250)
			{
				g_enemy[i].pos.z--;
				g_enemy[i].pos.y--;

				g_enemy[i].rot.x = -45;
			}
			//g_player[i].vel.y += 10.0f;

			if (g_nECnt1 >= 500 && g_nECnt2 >= 500)
			{
				ResetEPos(i);
			}
		}
	}
}
void EAction()
{
	g_Eaction = true;
}

void EAction2(bool af)//百裂拳
{
	if (af)
	{
		for (int i = 0; i < ENEMY_MAX; i++)
		{
			g_enemy[i].pos.z = +80;
			g_nECnt3++;
			if (g_nECnt3 >= 1 && g_nECnt3 <= 200) // ?はどこまで繰り返すかの数値
			{
				if (g_nECnt3 / 2 % 2 == 0)
				{
					g_enemyHD[i].pos.z -= 3;
					g_enemyHD[i].pos.x = (rand() % 201 - 100) / 30.0f;
					g_enemyHD[i].pos.y = (rand() % 201 - 100) / 30.0f;

				}
				else g_enemyHD[i].pos.z = -1.0f;
			}
			if (g_nECnt3 >= 201)
			{
				ResetEPos(i);
			}
		}
	}
}
void EAction2()
{
	g_Eaction2 = true;
}

void EAction3(bool af)//死亡
{
	if (af)
	{
		for (int i = 0; i < ENEMY_MAX; i++)
		{
			g_nECnt4++;
			g_enemy[i].rot.z = 180;
			if (g_nECnt4 >= 20 && g_nECnt4 <= 200)
			{
				g_enemyHD[i].pos.z += 0.05f;
				g_enemyHD[i].rot.z -= 2.0f;
				g_enemyHD[i].rot.y -= 2.0f;

				g_enemyAM[i].pos.x += 0.05f;
				g_enemyAM[i].rot.z -= 2.0f;
				g_enemyAM[i].rot.y -= 2.0f;

				g_enemyLG[i].pos.z -= 0.05f;
				g_enemyLG[i].rot.z -= 2.0f;
				g_enemyLG[i].rot.y -= 2.0f;
			}
		}
		if (g_nECnt4 >= 200)
			DestroyEnemy(0);

	}
}

void EAction4(bool af) //打ち上がるモーション
{
	if (af)
	{
		
		for (int i = 0; i < ENEMY_MAX; i++)
		{
			g_nECnt5++;

			if (g_nECnt5 >= 16 && g_nECnt5 <= 109)
			{
				g_enemy[i].pos.y++;
				g_enemy[i].rot.x++;
			}
			if (g_nECnt5 >= 110 && g_nECnt5 <=200 )
			{
				g_enemy[i].pos.y--;
				g_enemy[i].rot.x++;
			}
			if (g_nECnt5 >= 220 && g_nECnt5 <= 379)
			{
				g_enemy[i].pos.z++;
				g_enemy[i].rot.x++;
				g_enemy[i].rot.y++;
				g_enemy[i].rot.z++;
			}
			
			if (g_nECnt5 >= 380)
			{
				ResetEPos(i);
			}
		}
	}
}

void EAction5(bool af) //分解攻撃　最後BDは大爆発
{
	if (af)
	{

		for (int i = 0; i < ENEMY_MAX; i++)
		{
			g_nECnt6++;

			if (g_nECnt6 >= 0 && g_nECnt6 <= 50)
			{
				g_enemyHD[i].pos.y += 0.1f;
				g_enemyHD[i].rot.x += 20.0f;
				g_enemyHD[i].rot.y -= 20.0f;
				g_enemyHD[i].rot.z += 20.0f;

				g_enemyAM[i].pos.y += 0.1f;
				g_enemyAM[i].rot.x += 20.0f;
				g_enemyAM[i].rot.y += 20.0f;
				g_enemyAM[i].rot.z -= 20.0f;

				g_enemyLG[i].pos.y += 0.1f;
				g_enemyLG[i].rot.x -= 20.0f;
				g_enemyLG[i].rot.y += 20.0f;
				g_enemyLG[i].rot.z += 20.0f;
			}

			if (g_nECnt6 >= 65 && g_nECnt6 <= 75)
			{
				g_enemyHD[i].pos.z -= 7.0f;
				g_enemyHD[i].pos.y -= 2.0f;
			}
			if (g_nECnt6 >= 85 && g_nECnt6 <= 95)
			{
				g_enemyAM[i].pos.z -= 7.0f;
				g_enemyAM[i].pos.y -= 2.0f;
			}
			if (g_nECnt6 >= 105 && g_nECnt6 <= 115)
			{
				g_enemyLG[i].pos.z -= 7.0f;
				g_enemyLG[i].pos.y -= 2.0f;
			}

			if (g_nECnt6 >= 130 && g_nECnt6 <= 165)
			{
				g_enemy[i].pos.z -= 1.5f;
				g_enemy[i].pos.y += 1.5f;
				g_enemy[i].rot.x -= 7.0f;
			}
			if (g_nECnt6 >= 165 && g_nECnt6 <= 170)
			{
				g_enemy[i].pos.z -= 1.5f;
				g_enemy[i].rot.x -= 7.0f;
				
			}
			if (g_nECnt6 >= 170 && g_nECnt6 <= 205)
			{
				g_enemy[i].pos.z -= 1.5f;
				g_enemy[i].pos.y -= 1.5f;
				g_enemy[i].rot.x -= 7.0f;
			}
			if (g_nECnt6 >= 235 && g_nECnt6 <= 400)
			{
				if (g_nECnt6 / 2 % 2 == 0)
				{
					g_enemy[i].pos.x = 1.0f;
				}
				else g_enemy[i].pos.x = -1.0f;
			}
			//爆発


			if (g_nECnt6 >= 500)
			{
				ResetEPos(i);
			}
		}
	}
}


void EAction6(bool af)//二回勝利 転がり攻撃
{
	if (af)
	{

		for (int i = 0; i < ENEMY_MAX; i++)
		{
			g_nECnt7++;
			if (g_nECnt7 >= 0 && g_nECnt7 <= 100)
			{
				g_enemy[i].pos.z -= 3.0f;
				g_enemy[i].rot.x -= 50.0f;
			}

			if (g_nECnt7 >= 200)
			{
				ResetEPos(i);
			}
		}
	}
}

void EAction7(bool af)//二回勝利後ろ足蹴り攻撃
{
	if (af)
	{

		for (int i = 0; i < ENEMY_MAX; i++)
		{
			g_nECnt8++;
			if (g_nECnt8 >= 0 && g_nECnt8 <= 50)
			{
				g_enemy[i].pos.z = -20.0f;
			}

			if(g_nECnt8 >= 50 && g_nECnt8 <= 60)
			{
				g_enemy[i].rot.y = 0.0f;
			}

			if (g_nECnt8 >= 70 && g_nECnt8 <= 80)
			{
				g_enemyLG[i].rot.x = -90.0f;
				g_enemy[i].rot.x = -3.0f;
			}

			if (g_nECnt8 >= 83 && g_nECnt8 <= 90)
			{
				g_enemyLG[i].pos.z++;
			}

			if (g_nECnt8 >= 200)
			{
				ResetEPos(i);
			}
		}
	}
}

void EAction8(bool af)//二回勝利 ボディプレス
{
	if (af)
	{

		for (int i = 0; i < ENEMY_MAX; i++)
		{
			g_nECnt9++;
			if (g_nECnt9 >= 0 && g_nECnt9 <= 50)
			{
				g_enemy[i].pos.y += 3.0f;
				g_enemy[i].pos.z -= 4.0f;
				g_enemy[i].rot.x -= 7.0f;
			}
			if (g_nECnt9 >= 51 && g_nECnt9 <= 60)
			{
				g_enemy[i].rot = XMFLOAT3(0.0f, 180.0f, 0.0f);
			}

			if (g_nECnt9 >= 60 && g_nECnt9 <= 88)
			{
				g_enemy[i].pos.y -= 5.0f;
			}
			
			if (g_nECnt9 >= 200)
			{
				ResetEPos(i);
			}
		}
	}
}


void EAction9(bool af)//　1回勝利体当たり攻撃
{
	if (af)
	{
		for (int i = 0; i < ENEMY_MAX; i++)
		{
			g_nECnt10++;
			if (g_nECnt10 >= 0 && g_nECnt10 <= 50)
			{
				g_enemy[i].pos.z = -40.0f;
			}
			if (g_nECnt10 >= 51 && g_nECnt10 <= 70)
			{
				g_enemy[i].pos.z += 1.0f;
				g_enemy[i].rot.y = 135.0f;
			}
			if (g_nECnt10 >= 71 && g_nECnt10 <= 80)
			{
				g_enemy[i].pos.z = -95.0f;
			}
			if (g_nECnt10 >= 81 && g_nECnt10 <= 82)
			{
				g_enemy[i].pos.z += 25.0f;
			}
			if (g_nECnt10 >= 200)
			{
				ResetEPos(i);
			}
		}
	}
}

void EAction10(bool af)//　竜巻旋風脚　風
{
	if (af)
	{
		for (int i = 0; i < ENEMY_MAX; i++)
		{
			g_nECnt11++;
			if (g_nECnt11 >= 0 && g_nECnt11 <= 50)
			{
				g_enemy[i].pos.y += 5.0f;
				g_enemy[i].rot.x -= 8.0f;
			}

			if (g_nECnt11 >= 51 && g_nECnt11 <= 103)
			{
				g_enemy[i].pos.y -= 5.0f;
				g_enemy[i].rot.x -= 8.0f;
			}
			if (g_nECnt11 >= 110 && g_nECnt11 <= 200)
			{
				g_enemy[i].rot.x = -90.0f;
				g_enemy[i].rot.y += 50;
			}
			if (g_nECnt11 >= 200 && g_nECnt11 <= 240)
			{
				g_enemy[i].pos.z--;
				g_enemy[i].pos.x--;
				g_enemy[i].rot.y += 50;
			}
			if (g_nECnt11 >= 240 && g_nECnt11 <= 330)
			{
				g_enemy[i].pos.z--;
				g_enemy[i].pos.x++;
				g_enemy[i].rot.y += 50;
			}
			if (g_nECnt11 >= 330 && g_nECnt11 <= 380)
			{
				g_enemy[i].pos.z--;
				g_enemy[i].pos.x--;
				g_enemy[i].rot.y += 50;
			}
			if (g_nECnt11 >= 380 && g_nECnt11 <= 600)
			{
				g_enemy[i].pos.y += 0.5f;
				g_enemy[i].rot.y += 50;
			}


			if (g_nECnt11 >= 600)
			{
				ResetEPos(i);
			}

		}
	}
}
void EAction11(bool af)//　気円斬　風
{
	if (af)
	{
		for (int i = 0; i < ENEMY_MAX; i++)
		{
			g_nECnt12++;
			if (g_nECnt12 >= 0 && g_nECnt12 <= 50)
			{
				g_enemy[i].rot.y -= 100;
			}
			if (g_nECnt12 >= 50 && g_nECnt12 <= 70)
			{
				g_enemy[i].rot.y -= 100;
				g_enemy[i].pos.y += 0.4f;
				
			}
			if (g_nECnt12 >= 70 && g_nECnt12 <=200)
			{
				g_enemy[i].rot.y -= 100;
				g_enemy[i].pos.z-=0.8f;
				g_enemy[i].pos.x--;
				g_enemy[i].pos.y += 0.4f;
			}
			if(g_nECnt12 >= 200 && g_nECnt12 <= 280)
			{
				g_enemy[i].rot.y -= 100;
				g_enemy[i].pos.z-=0.8f;
				g_enemy[i].pos.x++;
				g_enemy[i].pos.y -= 0.7f;
			}
			if (g_nECnt12 >= 280 && g_nECnt12 <= 500)
			{
				g_enemy[i].rot.y -= 100;
			}
			

			if (g_nECnt12 >= 600)
			{
				ResetEPos(i);
			}
		}
	}
}

float ECheckCollisionRay(XMVECTOR pos, float fRadius)
{
	//ビューマトリックスとプロジェクションマトリックス
	XMMATRIX mtxView = XMLoadFloat4x4(&GetCamera()->GetViewMatrix());
	XMMATRIX mtxProjection = XMLoadFloat4x4(&GetCamera()->GetProjMatrix());

	//マウス座標
	POINT ptMouse = (*GetMousePosition());
	XMFLOAT3 posM = XMFLOAT3(ptMouse.x, ptMouse.y, 0.0f);

	//からニアクリップ上の座標を計算
	XMVECTOR posNear = XMVectorSet(posM.x, posM.y, 0.0f, 0.0f);
	posNear = XMVector3Unproject(posNear,//マウス座標
		0.0f, 0.0f,//ビューポート用データxy
		SCREEN_WIDTH, SCREEN_HEIGHT, //同幅高
		0.0f, 1.0f,//同minZmaxZ
		mtxProjection,//プロジェクションマトリックス
		mtxView,				//ビューマトリクス
		XMMatrixIdentity()
	);
	//からファークリップ上の座標を計算
	XMVECTOR posFar = XMVectorSet(posM.x, posM.y, 1.0f, 0.0f);
	posFar = XMVector3Unproject(posFar,//マウス座標
		0.0f, 0.0f,//ビューポート用データxy
		SCREEN_WIDTH, SCREEN_HEIGHT, //同幅高
		0.0f, 1.0f,//同minZmaxZ
		mtxProjection,//プロジェクションマトリックス
		mtxView,				//ビューマトリクス
		XMMatrixIdentity()
	);

	//レイを飛ばして当たり判定をとる
	XMVECTOR ray = posFar - posNear;
	XMFLOAT3 eye3 = GetCamera()->GetPos();
	XMVECTOR eye = XMVectorSet(eye3.x, eye3.y, eye3.z, 0.0f);//視点
	XMVECTOR target = pos - eye;
	//判定座標へのベクトルとレイの内積と計算
	XMVECTOR tempVec = XMVector3Dot(ray, target);
	float temp = XMVectorGetX(tempVec);

	//長さの二乗を取得
	tempVec = XMVector3Dot(target, target);//x*x+y*y+z*z
	float tempTarget = XMVectorGetX(tempVec);//値はX,Y,Zどれも同じ
	tempVec = XMVector3Dot(ray, ray);
	float tempRay = XMVectorGetX(tempVec);

	//レイを落とした垂線の交わる点までの距離
	float ret = temp * temp / tempRay;

	//残り一辺の長さを計算
	float length = sqrtf(tempTarget - ret);
	if (length < fRadius)
	{
		//範囲内であたった
		return sqrtf(ret);
	}

	//当たらなかったので
	return 9999.9f; //最大値
}