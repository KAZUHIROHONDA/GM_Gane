//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "AssimpModel.h"
#include "player.h"
#include "main.h"
#include "input.h"
#include "shadow.h"		//影をつけるには入れる
#include "gauge.h"
#include "sound.h"
#include "MessageManager.h"
#include"fade.h"
#include"enemy.h"
#include "model.h"
#include "sceneTitle.h"
#include "sceneGame.h"
#include "partsmenu.h"
#include "Upartsmenu.h"
#include "Mpartsmenu.h"
#include"effect.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_PLAYER		"data/model/kabuBody.obj" 
#define MODEL_PLAYER1		"data/model/kabu2.obj"
#define MODEL_PLAYER2		"data/model/kabuAM.obj"
#define MODEL_PLAYER3		"data/model/kabuLG.obj"


#define VALUE_MOVE		(3.0f)		
#define VALUE_ROTATE	(1.0f)


#define PLAYER_GAUGE_POS_X	(-113.0f)
#define PLAYER_GAUGE_POS_Y	(300.0f)


//構造体定義
typedef struct _tPlayer
{
	XMFLOAT4X4		mtxWorld;		//座標変換行列

	XMFLOAT3		pos;			//座標
	XMFLOAT3		rot;			//回転
	XMFLOAT3		scl;			//拡大縮小
	XMFLOAT3		vel;			//速度

	int				nPhase;			//段階
	int				nState;			//状態(0:未使用　1:通常)

	char			nName;			//
	int				nHP;

	XMFLOAT3	basePos;	//基本位置
	XMFLOAT3	targetPos;	//目標位置

	int				bCnt;			//弾の数
	int				nGauge;			//パワーゲージ
	int				nStopTime;		//操作不能時間

	int				nShadowIdx;		//影の番号

				
}tPlayer;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static CAssimpModel		g_model[4];				//モデルデータ格納用
			
static tPlayer			g_player[PLAYER_MAX];	//プレイヤー
static tPlayer			g_playerHD[PLAYER_MAX];
static tPlayer			g_playerAM[PLAYER_MAX];
static tPlayer			g_playerLG[PLAYER_MAX];
static int				g_nCameraType = E_CAMERA_VIEW_FIXED;		//カメラの種類


static int			g_nCnt1;//行動カウント1コメ
static int			g_nCnt2;//行動カウント2コメ
static int			g_nCnt3;//行動カウント3コメ
static int			g_nCnt4;//行動カウント4コメ
static int			g_nCnt5;//行動カウント4コメ
static int			g_nCnt6;//行動カウント4コメ
static int			g_nCnt7;//行動カウント4コメ
static int			g_nCnt8;//行動カウント4コメ
static int			g_nCnt9;//行動カウント4コメ
static int			g_nCnt10;//行動カウント4コメ
static int			g_nCnt11;//行動カウント4コメ
static int			g_nCnt12;//行動カウント4コメ
static int			g_nCnt13;//行動カウント4コメ
static int			g_nCnt14;//行動カウント4コメ


static int			g_nCntF;//


static bool			g_action;
static bool			g_action2;
static bool			g_action3;
static bool			g_action4;
static bool			g_action5;
static bool			g_action6;
static bool			g_action7;
static bool			g_action8;
static bool			g_action9;
static bool			g_action10;
static bool			g_action11;
static bool			g_action12;
static bool			g_action13;
static bool			g_action14;
static bool			endflag;


static bool				g_atama = true;//false;


const tMessage testMessage[4] = {
	{L"",MESSAGE_TYPE::E_TYPE_TIMER,60 },
	{L"1枚目",MESSAGE_TYPE::E_TYPE_ENTER,(long long int)&testMessage[2]},
	{L"2枚目",MESSAGE_TYPE::E_TYPE_ENTER,NULL},
	{L"2枚目",MESSAGE_TYPE::E_TYPE_NORMAL,}
};


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// 位置・回転・スケールの初期設定
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		g_player[i].pos = XMFLOAT3(0.0f, 25.0f, -100.0f);
		g_player[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_player[i].scl = GetPlayer()->Getscl();
		g_player[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		//初期化
		g_player[i].nPhase = 0;
		g_player[i].nState = 1;		//最初は通常

	    g_player[i].nName = 0;

		g_player[i].bCnt = 0;	

		//g_nCnt = 0.0f;				//

		g_nCnt1 = 0;					//
		g_nCnt2 = 0;					//
		g_nCnt3 = 0;					//
		g_nCnt4 = 0;
		g_nCnt5 = 0;
		g_nCnt5 = 0; 
		g_nCnt6 = 0; 
		g_nCnt7 = 0; 
		g_nCnt8 = 0; 
		g_nCnt9 = 0; 
		g_nCnt10 = 0;
		g_nCnt11 = 0;
		g_nCnt12 = 0;
		g_nCnt13 = 0;
		g_nCnt14 = 0;

		endflag = false;

		g_nCntF = 0;
		g_action = false;
		g_action2 = false;
		g_action3 = false;
		g_action4 = false;
		g_action5 = false;
		g_action6 = false;
		g_action7 = false;
		g_action8 = false;
		g_action9 = false;
		g_action10 = false;
		g_action11 = false;
		g_action12 = false;
		g_action13 = false;
		g_action14 = false;

		g_player[i].nHP = GetPlayer()->GetHP();
		g_player[i].nGauge = 100;
		g_player[i].nStopTime = 0; //最初は動ける

		g_player[i].nShadowIdx = CreateShadow(g_player[i].pos, 20.0f);

		//頭
		g_playerHD[i].pos = PartsGet()->Getpos();;//(0.0f, 0.0f, -2.0f);
		g_playerHD[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_playerHD[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_playerHD[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_playerHD[i].nPhase = 0;
		g_playerHD[i].nState = 1;	// puroぺらは最初から登場している
		g_playerHD[i].nShadowIdx = -1;

		//腕
		g_playerAM[i].pos = MPartsGet()->Getpos();//(0.0f, -0.8f, -1.0f);
		g_playerAM[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_playerAM[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_playerAM[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_playerAM[i].nPhase = 0;
		g_playerAM[i].nState = 1;	// puroぺらは最初から登場している
		g_playerAM[i].nShadowIdx = -1;

		//足
		g_playerLG[i].pos = UPartsGet()->Getpos();// (0.0f, -0.8f, 0.0f);
		g_playerLG[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_playerLG[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_playerLG[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_playerLG[i].nPhase = 0;
		g_playerLG[i].nState = 1;	// puroぺらは最初から登場している
		g_playerLG[i].nShadowIdx = -1;
	}

	// ワールドマトリックスの初期化
	XMMATRIX mtxWorld;
	mtxWorld = XMMatrixIdentity();

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		XMStoreFloat4x4(&g_player[i].mtxWorld, mtxWorld);
		XMStoreFloat4x4(&g_playerHD[i].mtxWorld, mtxWorld);
		XMStoreFloat4x4(&g_playerAM[i].mtxWorld, mtxWorld);
		XMStoreFloat4x4(&g_playerLG[i].mtxWorld, mtxWorld);

	}

	// モデルデータの読み込み
	hr = g_model[0].Load(pDevice, pDeviceContext,
		GetPlayerModel()->GetBody()); if (FAILED(hr)) return hr;

	hr = g_model[1].Load(pDevice, pDeviceContext,
		GetPlayerModel()->GetHead()); if (FAILED(hr)) return hr;

	hr = g_model[2].Load(pDevice, pDeviceContext,
		GetPlayerModel()->GetMae()); if (FAILED(hr)) return hr;

	hr = g_model[3].Load(pDevice, pDeviceContext,
		GetPlayerModel()->GetBack()); if (FAILED(hr)) return hr;
	
	g_nCameraType = E_CAMERA_VIEW_FIXED;

	
	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	//影の解放
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		ReleaseShadow(g_player[i].nShadowIdx);
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
void UpdatePlayer(void)
{
	
	// プレイヤーの動きを記述するならここ
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		
		//未使用
		if (g_player[i].nState == 0) continue;


		endflag = false;

		//操作不能時間
		if (g_player[i].nStopTime > 0)
		{
			g_player[i].nStopTime--;
		}
		if (g_player[i].nStopTime == 0)
		{
			
		}
			
		if (Clear() == true||Over() == true)
		{
			g_nCameraType = E_CAMERA_VIEW_FPS;
			if(Clear() == true&& Over() != true)
			Action12(true);
		}



			if (GetKeyPress(VK_Z))
			{
				//g_player[i].rot.y+=5;
				g_playerHD[i].rot.z += 5;
				//g_playerAM[i].rot.x += 5;
				g_playerLG[i].rot.x += 5;

			}
			
		//ゲージの動きの処理
			if (g_player[i].nGauge>= (g_player[i].nHP * 100 / GetPlayer()->GetHP()))
			{
				g_player[i].nGauge--;
			}

			if (GetKeyTrigger(VK_V))
			{
				DamagePlayer(200);
			}

			if (g_action3 == true)
			{
				Action3(g_action3);
			}


		//速度を座標に加算
		g_player[i].pos.x += g_player[i].vel.x;
		g_player[i].pos.y += g_player[i].vel.y;
		g_player[i].pos.z += g_player[i].vel.z;

		//画面外に行かない処理
		XMFLOAT3 box = g_model[i].GetBBox();
		//[左端] 
		if (g_player[i].pos.x < -200 + box.x * g_player[i].scl.x)
		{
			g_player[i].pos.x = -200 + box.x * g_player[i].scl.x;
		}
		//[右端]
		else if (g_player[i].pos.x > 200 - box.x * g_player[i].scl.x)
		{
			g_player[i].pos.x = 200 - box.x * g_player[i].scl.x;
		}
		//[下端] プレイヤーの左端＜画面の上端
		if (g_player[i].pos.z > 200 - box.z * g_player[i].scl.z)
		{
			g_player[i].pos.z = 200 - box.z * g_player[i].scl.z;
		}
		//[上端] プレイヤーの左端＜画面の下端
		else if (g_player[i].pos.z < -200 + box.z * g_player[i].scl.z)
		{
			g_player[i].pos.z = -200 + box.z * g_player[i].scl.z;
		}

		//着地判定
		if (g_player[i].pos.y <= 0.0f)
		{
			g_player[i].vel.y = 0.0f;
			g_player[i].pos.y = 0.0f;
			g_player[i].vel.x *= 0.9f;
			g_player[i].vel.z *= 0.9f;
		}

		if (GetKeyTrigger(VK_O))
		{
			SetMessage((tMessage*)&testMessage[3]);
		}

		XMMATRIX mtxWorld, mtxRot, mtxScl,
			mtxTranslate;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_player[i].scl.x, g_player[i].scl.y, g_player[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);



		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(g_player[i].rot.x), XMConvertToRadians(g_player[i].rot.y + 180), XMConvertToRadians(g_player[i].rot.z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);


		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_player[i].pos.x, g_player[i].pos.y, g_player[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);


		// ワールドマトリックス設定
		XMStoreFloat4x4(&g_player[i].mtxWorld, mtxWorld);

		//影の移動
		MoveShadow(g_player[i].nShadowIdx, g_player[i].pos);


		//************************************************:
		//頭

		XMMATRIX mtxWorldHD;
		// ワールドマトリックスの初期化
		mtxWorldHD = XMMatrixIdentity();
		// スケールを反映
		mtxScl = XMMatrixScaling(g_playerHD[i].scl.x, g_playerHD[i].scl.y, g_playerHD[i].scl.z);
		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxScl);
		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(g_playerHD[i].rot.x),
			XMConvertToRadians(g_playerHD[i].rot.y),//+180
			XMConvertToRadians(g_playerHD[i].rot.z)
		);
		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_playerHD[i].pos.x, g_playerHD[i].pos.y, g_playerHD[i].pos.z);

		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxTranslate);

		//親の行列を掛ける
		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxWorld);

		// ワールドマトリックス設定
		XMStoreFloat4x4(&g_playerHD[i].mtxWorld, mtxWorldHD);


		//************************************************:
		//腕

		XMMATRIX mtxWorldAM;
		// ワールドマトリックスの初期化
		mtxWorldAM = XMMatrixIdentity();
		// スケールを反映
		mtxScl = XMMatrixScaling(g_playerAM[i].scl.x, g_playerAM[i].scl.y, g_playerAM[i].scl.z);
		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxScl);
		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(g_playerAM[i].rot.x),
			XMConvertToRadians(g_playerAM[i].rot.y),//+180
			XMConvertToRadians(g_playerAM[i].rot.z)
		);
		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_playerAM[i].pos.x, g_playerAM[i].pos.y, g_playerAM[i].pos.z);

		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxTranslate);

		//親の行列を掛ける
		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxWorld);

		// ワールドマトリックス設定
		XMStoreFloat4x4(&g_playerAM[i].mtxWorld, mtxWorldAM);

		//************************************************:
		//足

		XMMATRIX mtxWorldLG;
		// ワールドマトリックスの初期化
		mtxWorldLG = XMMatrixIdentity();
		// スケールを反映
		mtxScl = XMMatrixScaling(g_playerLG[i].scl.x, g_playerLG[i].scl.y, g_playerLG[i].scl.z);
		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxScl);
		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(g_playerLG[i].rot.x),
			XMConvertToRadians(g_playerLG[i].rot.y),//+180
			XMConvertToRadians(g_playerLG[i].rot.z)
		);
		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_playerLG[i].pos.x, g_playerLG[i].pos.y, g_playerLG[i].pos.z);

		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxTranslate);

		//親の行列を掛ける
		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxWorld);

		// ワールドマトリックス設定
		XMStoreFloat4x4(&g_playerLG[i].mtxWorld, mtxWorldLG);

	}
//#ifdef CAMERA_DEBUG
//	if (GetKeyTrigger(VK_1))
//		g_nCameraType = E_CAMERA_VIEW_FIXED;
//	if (GetKeyTrigger(VK_2))
//		g_nCameraType = E_CAMERA_VIEW_BIRD;
//	if (GetKeyTrigger(VK_3))
//		g_nCameraType = E_CAMERA_VIEW_BEHIND;
//	if (GetKeyTrigger(VK_4))
//		g_nCameraType = E_CAMERA_VIEW_FPS;
//	if (GetKeyTrigger(VK_5))
//		g_nCameraType = E_CAMERA_VIEW_CAMERA_DEBUG;
//#endif

	switch (g_nCameraType)
	{
	case E_CAMERA_VIEW_FIXED:
		GetCamera()->SetTarget(1.0f, 1.0f, -1.0f);
		GetCamera()->SetPos(150.0f, 150.0f,-150.0f);
		break;
	case E_CAMERA_VIEW_BIRD:
		GetCamera()->SetTarget(g_player[0].pos);
		GetCamera()->SetPos(g_player[0].pos.x + 0.0f, g_player[0].pos.y + 100.0f, g_player[0].pos.z - 200.0f);
		break;
	case E_CAMERA_VIEW_BEHIND:
		GetCamera()->SetTarget(g_player[0].pos.x + SinDeg(g_player[0].rot.y)*100.0f, g_player[0].pos.y + 15.0f, g_player[0].pos.z + CosDeg(g_player[0].rot.y)*100.0f);
		GetCamera()->SetPos(g_player[0].pos.x - SinDeg(g_player[0].rot.y)*40.0f, g_player[0].pos.y + 20.0f, g_player[0].pos.z - CosDeg(g_player[0].rot.y)*40.0f);
		break;						
	case E_CAMERA_VIEW_FPS:
		GetCamera()->SetTarget(0.0f, 70.0f, 0.0f);
		GetCamera()->SetPos(0.0f, 100.0f, 100.0f);
		break;
	case E_CAMERA_VIEW_DIAGONAL:
		GetCamera()->SetTarget(0.0f, 0.0f, 0.0f);
		GetCamera()->SetPos(0.0f, 100.0f, -200.0f);
		break;
	case E_CAMERA_VIEW_CAMERA_DEBUG:break;
	}
}


void UpdateStart(void)
{
	// プレイヤーの動きを記述するならここ
	for (int i = 0; i < PLAYER_MAX; i++)
	{

		//未使用
		if (g_player[i].nState == 0) continue;


		//操作不能時間
		if (g_player[i].nStopTime > 0)
		{
			g_player[i].nStopTime--;
		}

			
		//ゲージの動きの処理
		if (g_player[i].nGauge >= (g_player[i].nHP * 100 / GetPlayer()->GetHP() ))
		{
			g_player[i].nGauge--;
		}

		//速度を座標に加算
		g_player[i].pos.x += g_player[i].vel.x;
		g_player[i].pos.y += g_player[i].vel.y;
		g_player[i].pos.z += g_player[i].vel.z;

		//着地判定
		if (g_player[i].pos.y <= 0.0f)
		{
			g_player[i].vel.y = 0.0f;
			g_player[i].pos.y = 0.0f;
			g_player[i].vel.x *= 0.9f;
			g_player[i].vel.z *= 0.9f;
		}



		XMMATRIX mtxWorld, mtxRot, mtxScl,
			mtxTranslate;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_player[i].scl.x, g_player[i].scl.y, g_player[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);



		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(g_player[i].rot.x), XMConvertToRadians(g_player[i].rot.y + 180), XMConvertToRadians(g_player[i].rot.z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);


		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_player[i].pos.x, g_player[i].pos.y, g_player[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);


		// ワールドマトリックス設定
		XMStoreFloat4x4(&g_player[i].mtxWorld, mtxWorld);

		//影の移動
		MoveShadow(g_player[i].nShadowIdx, g_player[i].pos);


		//************************************************:
		//頭

		XMMATRIX mtxWorldHD;
		// ワールドマトリックスの初期化
		mtxWorldHD = XMMatrixIdentity();
		// スケールを反映
		mtxScl = XMMatrixScaling(g_playerHD[i].scl.x, g_playerHD[i].scl.y, g_playerHD[i].scl.z);
		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxScl);
		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(g_playerHD[i].rot.x),
			XMConvertToRadians(g_playerHD[i].rot.y),//+180
			XMConvertToRadians(g_playerHD[i].rot.z)
		);
		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_playerHD[i].pos.x, g_playerHD[i].pos.y, g_playerHD[i].pos.z);

		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxTranslate);

		//親の行列を掛ける
		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxWorld);

		// ワールドマトリックス設定
		XMStoreFloat4x4(&g_playerHD[i].mtxWorld, mtxWorldHD);


		//************************************************:
		//腕

		XMMATRIX mtxWorldAM;
		// ワールドマトリックスの初期化
		mtxWorldAM = XMMatrixIdentity();
		// スケールを反映
		mtxScl = XMMatrixScaling(g_playerAM[i].scl.x, g_playerAM[i].scl.y, g_playerAM[i].scl.z);
		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxScl);
		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(g_playerAM[i].rot.x),
			XMConvertToRadians(g_playerAM[i].rot.y),//+180
			XMConvertToRadians(g_playerAM[i].rot.z)
		);
		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_playerAM[i].pos.x, g_playerAM[i].pos.y, g_playerAM[i].pos.z);

		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxTranslate);

		//親の行列を掛ける
		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxWorld);

		// ワールドマトリックス設定
		XMStoreFloat4x4(&g_playerAM[i].mtxWorld, mtxWorldAM);

		//************************************************:
		//足

		XMMATRIX mtxWorldLG;
		// ワールドマトリックスの初期化
		mtxWorldLG = XMMatrixIdentity();
		// スケールを反映
		mtxScl = XMMatrixScaling(g_playerLG[i].scl.x, g_playerLG[i].scl.y, g_playerLG[i].scl.z);
		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxScl);
		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(g_playerLG[i].rot.x),
			XMConvertToRadians(g_playerLG[i].rot.y),//+180
			XMConvertToRadians(g_playerLG[i].rot.z)
		);
		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_playerLG[i].pos.x, g_playerLG[i].pos.y, g_playerLG[i].pos.z);

		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxTranslate);

		//親の行列を掛ける
		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxWorld);

		// ワールドマトリックス設定
		XMStoreFloat4x4(&g_playerLG[i].mtxWorld, mtxWorldLG);

	}

	GetCamera()->SetTarget(1.0f, 1.0f, -1.0f);
	GetCamera()->SetPos(150.0f, 100.0f, 0.0f);
}

void UpdateClear(void)
{
	// プレイヤーの動きを記述するならここ
	for (int i = 0; i < PLAYER_MAX; i++)
	{

		//未使用
		if (g_player[i].nState == 0) continue;


		//速度を座標に加算
		g_player[i].pos.x += g_player[i].vel.x;
		g_player[i].pos.y += g_player[i].vel.y;
		g_player[i].pos.z += g_player[i].vel.z;

		//着地判定
		if (g_player[i].pos.y <= 0.0f)
		{
			g_player[i].vel.y = 0.0f;
			g_player[i].pos.y = 0.0f;
			g_player[i].vel.x *= 0.9f;
			g_player[i].vel.z *= 0.9f;
		}

		XMMATRIX mtxWorld, mtxRot, mtxScl,
			mtxTranslate;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_player[i].scl.x, g_player[i].scl.y, g_player[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);



		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(g_player[i].rot.x), XMConvertToRadians(g_player[i].rot.y + 180), XMConvertToRadians(g_player[i].rot.z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);


		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_player[i].pos.x, g_player[i].pos.y, g_player[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);


		// ワールドマトリックス設定
		XMStoreFloat4x4(&g_player[i].mtxWorld, mtxWorld);

		//影の移動
		MoveShadow(g_player[i].nShadowIdx, g_player[i].pos);


		//************************************************:
		//頭

		XMMATRIX mtxWorldHD;
		// ワールドマトリックスの初期化
		mtxWorldHD = XMMatrixIdentity();
		// スケールを反映
		mtxScl = XMMatrixScaling(g_playerHD[i].scl.x, g_playerHD[i].scl.y, g_playerHD[i].scl.z);
		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxScl);
		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(g_playerHD[i].rot.x),
			XMConvertToRadians(g_playerHD[i].rot.y),//+180
			XMConvertToRadians(g_playerHD[i].rot.z)
		);
		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_playerHD[i].pos.x, g_playerHD[i].pos.y, g_playerHD[i].pos.z);

		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxTranslate);

		//親の行列を掛ける
		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxWorld);

		// ワールドマトリックス設定
		XMStoreFloat4x4(&g_playerHD[i].mtxWorld, mtxWorldHD);


		//************************************************:
		//腕

		XMMATRIX mtxWorldAM;
		// ワールドマトリックスの初期化
		mtxWorldAM = XMMatrixIdentity();
		// スケールを反映
		mtxScl = XMMatrixScaling(g_playerAM[i].scl.x, g_playerAM[i].scl.y, g_playerAM[i].scl.z);
		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxScl);
		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(g_playerAM[i].rot.x),
			XMConvertToRadians(g_playerAM[i].rot.y),//+180
			XMConvertToRadians(g_playerAM[i].rot.z)
		);
		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_playerAM[i].pos.x, g_playerAM[i].pos.y, g_playerAM[i].pos.z);

		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxTranslate);

		//親の行列を掛ける
		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxWorld);

		// ワールドマトリックス設定
		XMStoreFloat4x4(&g_playerAM[i].mtxWorld, mtxWorldAM);

		//************************************************:
		//足

		XMMATRIX mtxWorldLG;
		// ワールドマトリックスの初期化
		mtxWorldLG = XMMatrixIdentity();
		// スケールを反映
		mtxScl = XMMatrixScaling(g_playerLG[i].scl.x, g_playerLG[i].scl.y, g_playerLG[i].scl.z);
		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxScl);
		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(g_playerLG[i].rot.x),
			XMConvertToRadians(g_playerLG[i].rot.y),//+180
			XMConvertToRadians(g_playerLG[i].rot.z)
		);
		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_playerLG[i].pos.x, g_playerLG[i].pos.y, g_playerLG[i].pos.z);

		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxTranslate);

		//親の行列を掛ける
		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxWorld);

		// ワールドマトリックス設定
		XMStoreFloat4x4(&g_playerLG[i].mtxWorld, mtxWorldLG);

	}

	GetCamera()->SetTarget(g_player[0].pos.x + SinDeg(g_player[0].rot.y)*100.0f, g_player[0].pos.y + 15.0f, g_player[0].pos.z + CosDeg(g_player[0].rot.y)*100.0f);
	GetCamera()->SetPos(0.0f, 70.0f, 100.0f);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		//未使用
		if (g_player[i].nState == 0) continue;

		// モデル表示
		g_model[0].Draw(pDeviceContext, g_player[i].mtxWorld);
		g_model[1].Draw(pDeviceContext, g_playerHD[i].mtxWorld);
		g_model[2].Draw(pDeviceContext, g_playerAM[i].mtxWorld);
		g_model[3].Draw(pDeviceContext, g_playerLG[i].mtxWorld);
	}
}

//生存確認
bool IsPlayer(int no)
{
	if (no < 0 || no >= PLAYER_MAX) return false;
	return g_player[no].nState > 0 ?
		true : false;
}

/*void DestroyPlayer(int no)
{
	if (no < 0 || no >= PLAYER_MAX) return;
	g_player[no].nState = 0;
	// 影との関連を切る
	ReleaseShadow(g_player[no].nShadowIdx);
	g_player[no].nShadowIdx = -1;
}*/

//座標取得
XMFLOAT3 GetPlayerPos(int no)
{
	if (no < 0 || no >= PLAYER_MAX)
		return XMFLOAT3(0.0f, 0.0f, 0.0f);
	return g_player[no].pos;
}

//サイズ取得
XMFLOAT3 GetPlayerSize(int no)
{
	if (no < 0 || no >= PLAYER_MAX)
		return XMFLOAT3(0, 0, 0);
	//return XMFLOAT3(10.0f,10.0f,10.0f);
	XMFLOAT3 box = g_model[0].GetBBox();
	return XMFLOAT3(box.x / 2 * g_player[no].scl.x, box.y / 2 * g_player[no].scl.y, box.z / 2 * g_player[no].scl.z);
}


//球のパワーゲージ
void DrawPlayerGauge()
{
	const XMFLOAT2 pos = { PLAYER_GAUGE_POS_X, PLAYER_GAUGE_POS_Y };

	DrawGauge(pos, (float)g_player[0].nGauge / GAUGE_MAX);
}

int GetPlayerHp(int no)
{
	if (no < 0 || no >= PLAYER_MAX)	return(0);

	return g_player[no].nHP;


}

void DamagePlayer(int damage)
{	

	g_player[0].nHP -= damage;
	if (g_player[0].nHP <= 0)
	{
		g_player[0].nHP = 0;

		g_action3 = true;
	}

}


void ResetPos(int no)
{
	if (no < 0 || no >= PLAYER_MAX)	return;

	g_player[no].pos = XMFLOAT3(0.0f, 20.0f, -100.0f);
	g_player[no].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_player[no].scl = XMFLOAT3(8.0f, 8.0f, 8.0f);
	g_player[no].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);

	g_playerHD[no].pos = XMFLOAT3(0.0f, 0.0f, -2.0f);
	g_playerHD[no].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_playerHD[no].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
	g_playerHD[no].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);

	g_playerAM[no].pos = MPartsGet()->Getpos();
	g_playerAM[no].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_playerAM[no].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
	g_playerAM[no].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);

	g_playerLG[no].pos = UPartsGet()->Getpos();
	g_playerLG[no].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_playerLG[no].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
	g_playerLG[no].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);

	endflag = true;

	g_nCnt1=0;
	g_nCnt2=0;
	g_nCnt3 = 0;
	g_nCnt4 = 0;
	g_nCnt5 = 0;
	g_nCnt6 = 0;
	g_nCnt7 = 0;
	g_nCnt8 = 0;
	g_nCnt9 = 0;
	g_nCnt10 = 0;
	g_nCnt11 = 0;
	g_nCnt12 = 0;
	g_nCnt13 = 0;
	g_nCnt14 = 0;
	g_action = false;
	g_action2 = false;
	g_action3 = false;
	g_action4 = false;
	g_action5 = false;
	g_action6 = false;
	g_action7 = false;
	g_action8 = false;
	g_action9 = false;
	g_action10 = false;
	g_action11 = false;
	g_action12 = false;
	g_action13 = false;
	g_action14 = false;
}

void Action(bool af)
{
	if (af)
	{
		for (int i = 0; i < PLAYER_MAX; i++)
		{
			g_nCnt1++;
			g_nCnt2++;
			g_playerHD[i].rot.z += 50;
			if (g_nCnt1 <= 150)
			{
				g_player[i].pos.y++;

			}
			if (g_nCnt1 >= 150 && g_nCnt2 <= 250)
			{
				g_player[i].pos.z++;
				g_player[i].pos.y--;

				g_player[i].rot.x = -45;

				
			}
			//g_player[i].vel.y += 10.0f;

			if (g_nCnt1 >= 500 && g_nCnt2 >= 500)
			{
				ResetPos(i);
			}
		}
	}
}
void Action()
{
	g_action = true;
}

void Action2(bool af)
{
	if (af)
	{

		for (int i = 0; i < PLAYER_MAX; i++)
		{
			g_player[i].pos.z = -80;
			g_nCnt3++;
			
			if (g_nCnt3 >= 1 && g_nCnt3 <= 200 ) // ?はどこまで繰り返すかの数値
			{
				if (g_nCnt3 / 2 % 2 == 0)
				{
					g_playerHD[i].pos.z -= 3;
					g_playerHD[i].pos.x = (rand() % 201 - 100) / 30.0f;
					g_playerHD[i].pos.y = (rand() % 201 - 100) / 30.0f;

				}
				else g_playerHD[i].pos.z = -1.0f;
			}
			if (g_nCnt3 >= 201)
			{
				ResetPos(i);
			}
		}
	}
}
void Action2()
{
	g_action2 = true;
}

void Action3(bool af)
{
	if (af)
	{
		for (int i = 0; i < PLAYER_MAX; i++)
		{
			g_nCnt4++;
			g_player[i].rot.z = 180;
			if (g_nCnt4 >= 20 && g_nCnt4 <= 200)
			{
				g_playerHD[i].pos.z -= 0.05f;
				g_playerHD[i].rot.z -= 2.0f;
				g_playerHD[i].rot.y -= 2.0f;

				g_playerAM[i].pos.x -= 0.05f;
				g_playerAM[i].rot.z -= 2.0f;
				g_playerAM[i].rot.y -= 2.0f;

				g_playerLG[i].pos.z += 0.05f;
				g_playerLG[i].rot.z -= 2.0f;
				g_playerLG[i].rot.y -= 2.0f;
			}
		}
		if(g_nCnt4 >= 200)
		DestroyPlayer(0);

	}
}

void Action4(bool af) //打ち上がるモーション
{
	if (af)
	{

		for (int i = 0; i < PLAYER_MAX; i++)
		{
			g_nCnt5++;

			if (g_nCnt5 >= 16 && g_nCnt5 <= 109)
			{
				g_player[i].pos.y++;
				g_player[i].rot.x++;
			}
			if (g_nCnt5 >= 110 && g_nCnt5 <= 200)
			{
				g_player[i].pos.y--;
				g_player[i].rot.x++;
			}
			if (g_nCnt5 >= 220 && g_nCnt5 <= 379)
			{
				g_player[i].pos.z--;
				g_player[i].rot.x++;
				g_player[i].rot.y++;
				g_player[i].rot.z++;
			}

			if (g_nCnt5 >= 380)
			{
				ResetPos(i);
			}
		}
	}
}

void Action5(bool af) //分解攻撃　最後BDは大爆発
{
	if (af)
	{

		for (int i = 0; i < PLAYER_MAX; i++)
		{
			g_nCnt6++;

			if (g_nCnt6 >= 0 && g_nCnt6 <= 50)
			{
				g_playerHD[i].pos.y += 0.1f;
				g_playerHD[i].rot.x += 20.0f;
				g_playerHD[i].rot.y -= 20.0f;
				g_playerHD[i].rot.z += 20.0f;

				g_playerAM[i].pos.y += 0.1f;
				g_playerAM[i].rot.x += 20.0f;
				g_playerAM[i].rot.y += 20.0f;
				g_playerAM[i].rot.z -= 20.0f;

				g_playerLG[i].pos.y += 0.1f;
				g_playerLG[i].rot.x -= 20.0f;
				g_playerLG[i].rot.y += 20.0f;
				g_playerLG[i].rot.z += 20.0f;
			}

			if (g_nCnt6 >= 65 && g_nCnt6 <= 75)
			{
				g_playerHD[i].pos.z += 7.0f;
				g_playerHD[i].pos.y -= 2.0f;
			}
			if (g_nCnt6 >= 85 && g_nCnt6 <= 95)
			{
				g_playerAM[i].pos.z += 7.0f;
				g_playerAM[i].pos.y -= 2.0f;
			}
			if (g_nCnt6 >= 105 && g_nCnt6 <= 115)
			{
				g_playerLG[i].pos.z += 7.0f;
				g_playerLG[i].pos.y -= 2.0f;
			}

			if (g_nCnt6 >= 130 && g_nCnt6 <= 165)
			{
				g_player[i].pos.z += 1.5f;
				g_player[i].pos.y += 1.5f;
				g_player[i].rot.x -= 7.0f;
			}
			if (g_nCnt6 >= 165 && g_nCnt6 <= 170)
			{
				g_player[i].pos.z += 1.5f;
				g_player[i].rot.x -= 7.0f;

			}
			if (g_nCnt6 >= 170 && g_nCnt6 <= 205)
			{
				g_player[i].pos.z += 1.5f;
				g_player[i].pos.y -= 1.5f;
				g_player[i].rot.x -= 7.0f;
			}
			if (g_nCnt6 >= 235 && g_nCnt6 <= 400)
			{
				if (g_nCnt6 / 2 % 2 == 0)
				{
					g_player[i].pos.x = 1.0f;
				}
				else g_player[i].pos.x = -1.0f;
			}
			//爆発


			if (g_nCnt6 >= 500)
			{
				ResetPos(i);
			}
		}
	}
}


void Action6(bool af)//二回勝利 転がり攻撃
{
	if (af)
	{

		for (int i = 0; i < PLAYER_MAX; i++)
		{
			g_nCnt7++;
			if (g_nCnt7 >= 0 && g_nCnt7 <= 100)
			{
				g_player[i].pos.z += 3.0f;
				g_player[i].rot.x += 50.0f;
			}

			if (g_nCnt7 >= 200)
			{
				ResetPos(i);
			}
		}
	}
}

void Action7(bool af)//二回勝利後ろ足蹴り攻撃
{
	if (af)
	{

		for (int i = 0; i < PLAYER_MAX; i++)
		{
			g_nCnt8++;
			if (g_nCnt8 >= 0 && g_nCnt8 <= 50)
			{
				g_player[i].pos.z = 20.0f;
			}

			if (g_nCnt8 >= 50 && g_nCnt8 <= 60)
			{
				g_player[i].rot.y = 0.0f;
			}

			if (g_nCnt8 >= 70 && g_nCnt8 <= 80)
			{
				g_playerLG[i].rot.x = -90.0f;
				g_player[i].rot.x = -3.0f;
			}

			if (g_nCnt8 >= 83 && g_nCnt8 <= 90)
			{
				g_playerLG[i].pos.z--;
			}

			if (g_nCnt8 >= 200)
			{
				ResetPos(i);
			}
		}
	}
}

void Action8(bool af)//二回勝利 ボディプレス
{
	if (af)
	{

		for (int i = 0; i < PLAYER_MAX; i++)
		{
			g_nCnt9++;
			if (g_nCnt9 >= 0 && g_nCnt9 <= 50)
			{
				g_player[i].pos.y += 3.0f;
				g_player[i].pos.z += 4.0f;
				g_player[i].rot.x -= 7.0f;
			}
			if (g_nCnt9 >= 51 && g_nCnt9 <= 60)
			{
				g_player[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
			}

			if (g_nCnt9 >= 60 && g_nCnt9 <= 88)
			{
				g_player[i].pos.y -= 5.0f;
			}

			if (g_nCnt9 >= 200)
			{
				ResetPos(i);
			}
		}
	}
}


void Action9(bool af)//　1回勝利体当たり攻撃
{
	if (af)
	{
		for (int i = 0; i < PLAYER_MAX; i++)
		{
			g_nCnt10++;
			if (g_nCnt10 >= 0 && g_nCnt10 <= 50)
			{
				g_player[i].pos.z = 40.0f;
			}
			if (g_nCnt10 >= 51 && g_nCnt10 <= 70)
			{
				g_player[i].pos.z -= 1.0f;
				g_player[i].rot.y = 135.0f;
			}
			if (g_nCnt10 >= 71 && g_nCnt10 <= 80)
			{
				g_player[i].pos.z = 95.0f;
			}
			if (g_nCnt10 >= 81 && g_nCnt10 <= 82)
			{
				g_player[i].pos.z -= 25.0f;
			}
			if (g_nCnt10 >= 200)
			{
				ResetPos(i);
			}
		}
	}
}

void Action10(bool af)//　竜巻旋風脚　風
{
	if (af)
	{
		for (int i = 0; i < PLAYER_MAX; i++)
		{
			g_nCnt11++;
			if (g_nCnt11 >= 0 && g_nCnt11 <= 50)
			{
				g_player[i].pos.y += 5.0f;
				g_player[i].rot.x -= 8.0f;
			}

			if (g_nCnt11 >= 51 && g_nCnt11 <= 103)
			{
				g_player[i].pos.y -= 5.0f;
				g_player[i].rot.x -= 8.0f;
			}
			if (g_nCnt11 >= 110 && g_nCnt11 <= 200)
			{
				g_player[i].rot.x = -90.0f;
				g_player[i].rot.y += 50;
			}
			if (g_nCnt11 >= 200 && g_nCnt11 <= 240)
			{
				g_player[i].pos.z++;
				g_player[i].pos.x--;
				g_player[i].rot.y += 50;
			}
			if (g_nCnt11 >= 240 && g_nCnt11 <= 330)
			{
				g_player[i].pos.z++;
				g_player[i].pos.x++;
				g_player[i].rot.y += 50;
			}
			if (g_nCnt11 >= 330 && g_nCnt11 <= 380)
			{
				g_player[i].pos.z++;
				g_player[i].pos.x--;
				g_player[i].rot.y += 50;
			}
			if (g_nCnt11 >= 380 && g_nCnt11 <= 600)
			{
				g_player[i].pos.y += 0.5f;
				g_player[i].rot.y += 50;
			}


			if (g_nCnt11 >= 600)
			{
				ResetPos(i);
			}

		}
	}
}
void Action11(bool af)//　気円斬　風
{
	if (af)
	{
		for (int i = 0; i < PLAYER_MAX; i++)
		{
			g_nCnt12++;
			if (g_nCnt12 >= 0 && g_nCnt12 <= 50)
			{
				g_player[i].rot.y -= 100;
			}
			if (g_nCnt12 >= 50 && g_nCnt12 <= 70)
			{
				g_player[i].rot.y -= 100;
				g_player[i].pos.y += 0.4f;

			}
			if (g_nCnt12 >= 70 && g_nCnt12 <= 200)
			{
				g_player[i].rot.y -= 100;
				g_player[i].pos.z += 0.8f;
				g_player[i].pos.x--;
				g_player[i].pos.y += 0.4f;
			}
			if (g_nCnt12 >= 200 && g_nCnt12 <= 280)
			{
				g_player[i].rot.y -= 100;
				g_player[i].pos.z += 0.8f;
				g_player[i].pos.x--;
				g_player[i].pos.y -= 0.7f;
			}
			if (g_nCnt12 >= 280 && g_nCnt12 <= 500)
			{
				g_player[i].rot.y -= 100;
			}


			if (g_nCnt12 >= 600)
			{
				ResetPos(i);
			}
		}
	}
}






void Action12(bool af)//勝利アニメーション
{
	if (af)
	{
		for (int i = 0; i < PLAYER_MAX; i++)
		{
			g_player[i].pos.y += 10.0f;
			g_player[i].vel.y += -0.98f;
			if (g_player[i].pos.y <= 15.0f)
			{
				g_player[i].pos.y = 15.0f;
			}
		}
	}
}

void Action13(bool af)//竜巻　ダメージ
{
	if (af)
	{
		for (int i = 0; i < PLAYER_MAX; i++)
		{
			g_nCnt13++;
			if (g_nCnt13 >= 390 && g_nCnt13 <= 600)
			{
				g_player[i].pos.y += 0.7f;
				g_player[i].rot.y--;
				g_player[i].rot.x--;
				g_player[i].rot.z--;
			}
			if (g_nCnt13 >= 601 && g_nCnt13 <= 800)
			{
				g_player[i].pos.y -= 0.73f;
			}
			if (g_nCnt13 >= 801)
			{
				ResetPos(i);
			}
		}

	}
}

void Action14(bool af) //打ち上げドリル攻撃
{
	if (af)
	{
		for (int i = 0; i < PLAYER_MAX; i++)
		{

			g_nCnt14++;

			if (g_nCnt14 >= 1 && g_nCnt14 <= 349)
			{

				if (g_nCnt14 <= 12 && g_nCnt14 >= 10)
				{
					g_player[i].pos.z -= 50;
				}
				if (g_nCnt14 <= 24 && g_nCnt14 >= 16)
				{
					g_playerHD[i].rot.x = 35.0f;
				}
				if (g_nCnt14 <= 54 && g_nCnt14 >= 25)//上り
				{
					g_playerHD[i].rot.x = 0.0f;
					g_player[i].rot.y = 0.0f;
					g_player[i].rot.x -= 18.0f;
					g_player[i].pos.z += 4;
					g_player[i].pos.y += 1;
				}
				if (g_nCnt14 <= 84 && g_nCnt14 >= 55)//下り
				{
					//g_player[i].rot.y = 0.0f;
					g_player[i].rot.x -= 18.0f;
					g_player[i].pos.z += 4;
					g_player[i].pos.y -= 1;
				}
				if (g_nCnt14 <= 100 && g_nCnt14 >= 90)
				{
					g_playerHD[i].rot.z += 100.0f;
				}
				if (g_nCnt14 <= 199 && g_nCnt14 >= 101)
				{
					g_playerHD[i].rot.z += 200.0f;
					g_player[i].rot.z -= 100.0f;
				}
				if (g_nCnt14 <= 340 && g_nCnt14 >= 200)
				{
					g_playerHD[i].rot.z += 200.0f;
					g_player[i].rot.z -= 100.0f;
					g_player[i].pos.z -= 20;

				}

			}
			if (g_nCnt14 >= 380)
			{
				ResetPos(i);
			}

		}

	}


}
void DestroyPlayer(int no)
{
	if (no < 0 || no >= PLAYER_MAX)	return;

	//g_player[no].nState = 0;

	//影の開放
	ReleaseShadow(g_player[no].nShadowIdx);
	g_player[no].nShadowIdx = -1;

	g_nCntF++;
	if (g_nCntF>=250)
	{
		Overflag();
	}
	
}

void CameraDEBUG()
{
	g_nCameraType = E_CAMERA_VIEW_CAMERA_DEBUG;
}
void CameraFIXED()
{
	g_nCameraType = E_CAMERA_VIEW_FIXED;
}

bool Getendflag()
{
	return endflag;
}
void Setendflag()
{
	endflag = true;
}