//=============================================================================
//
// プレイヤー処理 [pchimera.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "AssimpModel.h"
#include "pchimera.h"
#include "main.h"
#include "input.h"
#include "shadow.h"		//影をつけるには入れる
#include "model.h"
#include "sceneGame.h"
#include "sceneTitle.h"
#include "partsmenu.h"
#include "Upartsmenu.h"
#include "Mpartsmenu.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define VALUE_MOVE		(3.0f)		
#define VALUE_ROTATE	(1.0f)

//構造体定義
typedef struct _tPChimera
{
	XMFLOAT4X4		mtxWorld;		//座標変換行列

	XMFLOAT3		pos;			//座標
	XMFLOAT3		rot;			//回転
	XMFLOAT3		scl;			//拡大縮小
	XMFLOAT3		vel;			//速度

	int				nPhase;			//段階
	int				nState;			//状態(0:未使用　1:通常)

	char			nName;			//

	int             nHP;

	int				bCnt;			//弾の数
	int				nGauge;			//パワーゲージ
	int				nStopTime;		//操作不能時間

	int				nShadowIdx;		//影の番号

				
}tPChimera;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static CAssimpModel		g_model[4];				//モデルデータ格納用
			
static tPChimera			playermodel[PCHIMERA_MAX];	//プレイヤー
static tPChimera			playermodelHD[PCHIMERA_MAX];
static tPChimera			playermodelAM[PCHIMERA_MAX];
static tPChimera			playermodelLG[PCHIMERA_MAX];
static int				g_nCameraType = CAMERA_VIEW_FIXED;		//カメラの種類

static bool				g_atama = true;//false;

float CheckCollisionRay(XMVECTOR pos, float fRadius);

bool sflag = false;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPChimera(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// 位置・回転・スケールの初期設定
	for (int i = 0; i < PCHIMERA_MAX; i++)
	{
		playermodel[i].pos = XMFLOAT3(0.0f, 0.0f, -60.0f);
		playermodel[i].rot = XMFLOAT3(0.0f, 180.0f, 0.0f);
		playermodel[i].scl = GetPlayer()->Getscl();
		playermodel[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		//初期化
		playermodel[i].nPhase = 0;
		playermodel[i].nState = 1;		//最初は通常

		playermodel[i].nHP = 1000;
		playermodel[i].nName = 0;

		playermodel[i].bCnt = 0;	

		//g_nCnt = 0.0f;				//


		playermodel[i].nGauge = playermodel[i].nHP/10;
		playermodel[i].nStopTime = 0; //最初は動ける

		playermodel[i].nShadowIdx = CreateShadow(playermodel[i].pos, 20.0f);

		//頭
		playermodelHD[i].pos = PartsGet()->Getpos();
		playermodelHD[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		playermodelHD[i].scl = PartsGet()->Getscl();
		playermodelHD[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		playermodelHD[i].nPhase = 0;
		playermodelHD[i].nState = 1;	// puroぺらは最初から登場している
		playermodelHD[i].nShadowIdx = -1;

		//腕
		playermodelAM[i].pos = MPartsGet()->Getpos();
		playermodelAM[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		playermodelAM[i].scl = MPartsGet()->Getscl();
		playermodelAM[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		playermodelAM[i].nPhase = 0;
		playermodelAM[i].nState = 1;	// puroぺらは最初から登場している
		playermodelAM[i].nShadowIdx = -1;

		//足
		playermodelLG[i].pos = UPartsGet()->Getpos();
		playermodelLG[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		playermodelLG[i].scl = UPartsGet()->Getscl();
		playermodelLG[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		playermodelLG[i].nPhase = 0;
		playermodelLG[i].nState = 1;	// puroぺらは最初から登場している
		playermodelLG[i].nShadowIdx = -1;

		if (sflag == true)
		{
			playermodel[i].rot = XMFLOAT3(0.0f, 100.0f, 0.0f);
			g_nCameraType = CAMERA_VIEW_BIRD;
		}
	}

	// ワールドマトリックスの初期化
	XMMATRIX mtxWorld;
	mtxWorld = XMMatrixIdentity();

	for (int i = 0; i < PCHIMERA_MAX; i++)
	{
		XMStoreFloat4x4(&playermodel[i].mtxWorld, mtxWorld);
		XMStoreFloat4x4(&playermodelHD[i].mtxWorld, mtxWorld);
		XMStoreFloat4x4(&playermodelAM[i].mtxWorld, mtxWorld);
		XMStoreFloat4x4(&playermodelLG[i].mtxWorld, mtxWorld);

	}



	// モデルデータの読み込み
	hr = g_model[0].Load(pDevice, pDeviceContext,
		GetBody()); if (FAILED(hr)) return hr;

	hr = g_model[1].Load(pDevice, pDeviceContext,
		GetHead()); if (FAILED(hr)) return hr;

	hr = g_model[2].Load(pDevice, pDeviceContext,
		GetMae()); if (FAILED(hr)) return hr;

	hr = g_model[3].Load(pDevice, pDeviceContext,
		GetBack()); if (FAILED(hr)) return hr;
	
	g_nCameraType = CAMERA_VIEW_FIXED;

	sflag = true;
	
	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPChimera(void)
{
	//影の解放
	for (int i = 0; i < PCHIMERA_MAX; i++)
	{
		ReleaseShadow(playermodel[i].nShadowIdx);
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
void UpdatePChimera(void)
{
	
	// プレイヤーの動きを記述するならここ
	for (int i = 0; i < PCHIMERA_MAX; i++)
	{
		
		//未使用
		if (playermodel[i].nState == 0) continue;


		//操作不能時間
		if (playermodel[i].nStopTime > 0)
		{
			playermodel[i].nStopTime--;
		}


		//速度を座標に加算
		playermodel[i].pos.x += playermodel[i].vel.x;
		playermodel[i].pos.y += playermodel[i].vel.y;
		playermodel[i].pos.z += playermodel[i].vel.z;

		if (sflag == true)
		{
			playermodel[i].pos = XMFLOAT3(50.0f, -50.0f, 0.0f);
			g_nCameraType = CAMERA_VIEW_BIRD;
		}

		playermodelHD[i].scl = PartsGet()->Getscl();
		playermodelAM[i].scl = MPartsGet()->Getscl();
		playermodelLG[i].scl = UPartsGet()->Getscl();

		//着地判定
		//if (playermodel[i].pos.y <= -50.0f)
		//{
		//	playermodel[i].vel.y = 0.0f;
		//	playermodel[i].pos.y = 0.0f;
		//	playermodel[i].vel.x *= 0.9f;
		//	playermodel[i].vel.z *= 0.9f;
		//}

		// 3D座標に変換して当たり判定を取る
		float hitLength;

		//位置と半径を指定
		hitLength = CheckCollisionRay(XMVectorSet(playermodel[i].pos.x, playermodel[i].pos.y, playermodel[i].pos.z, 0.0f), 10.0f);
		if (hitLength < 9999.9f)
		{
			playermodel[i].rot.y += 1.0f;
			if (GetMouseTrigger(0))
			{
				//アクション入れたい

			}
		}
		else
		{
			playermodel[i].rot = XMFLOAT3(0.0f, 100.0f, 0.0f);
		}


		XMMATRIX mtxWorld, mtxRot, mtxScl,
			mtxTranslate;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(playermodel[i].scl.x, playermodel[i].scl.y, playermodel[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);



		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(playermodel[i].rot.x), XMConvertToRadians(playermodel[i].rot.y + 180), XMConvertToRadians(playermodel[i].rot.z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);


		// 移動を反映
		mtxTranslate = XMMatrixTranslation(playermodel[i].pos.x, playermodel[i].pos.y, playermodel[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);


		// ワールドマトリックス設定
		XMStoreFloat4x4(&playermodel[i].mtxWorld, mtxWorld);

		//影の移動
		MoveShadow(playermodel[i].nShadowIdx, playermodel[i].pos);


		//************************************************:
		//頭

		XMMATRIX mtxWorldHD;
		// ワールドマトリックスの初期化
		mtxWorldHD = XMMatrixIdentity();
		// スケールを反映
		mtxScl = XMMatrixScaling(playermodelHD[i].scl.x, playermodelHD[i].scl.y, playermodelHD[i].scl.z);
		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxScl);
		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(playermodelHD[i].rot.x),
			XMConvertToRadians(playermodelHD[i].rot.y),//+180
			XMConvertToRadians(playermodelHD[i].rot.z)
		);
		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(playermodelHD[i].pos.x, playermodelHD[i].pos.y, playermodelHD[i].pos.z);

		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxTranslate);

		//親の行列を掛ける
		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxWorld);

		// ワールドマトリックス設定
		XMStoreFloat4x4(&playermodelHD[i].mtxWorld, mtxWorldHD);


		//************************************************:
		//腕

		XMMATRIX mtxWorldAM;
		// ワールドマトリックスの初期化
		mtxWorldAM = XMMatrixIdentity();
		// スケールを反映
		mtxScl = XMMatrixScaling(playermodelAM[i].scl.x, playermodelAM[i].scl.y, playermodelAM[i].scl.z);
		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxScl);
		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(playermodelAM[i].rot.x),
			XMConvertToRadians(playermodelAM[i].rot.y),//+180
			XMConvertToRadians(playermodelAM[i].rot.z)
		);
		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(playermodelAM[i].pos.x, playermodelAM[i].pos.y, playermodelAM[i].pos.z);

		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxTranslate);

		//親の行列を掛ける
		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxWorld);

		// ワールドマトリックス設定
		XMStoreFloat4x4(&playermodelAM[i].mtxWorld, mtxWorldAM);

		//************************************************:
		//足

		XMMATRIX mtxWorldLG;
		// ワールドマトリックスの初期化
		mtxWorldLG = XMMatrixIdentity();
		// スケールを反映
		mtxScl = XMMatrixScaling(playermodelLG[i].scl.x, playermodelLG[i].scl.y, playermodelLG[i].scl.z);
		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxScl);
		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(playermodelLG[i].rot.x),
			XMConvertToRadians(playermodelLG[i].rot.y),//+180
			XMConvertToRadians(playermodelLG[i].rot.z)
		);
		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(playermodelLG[i].pos.x, playermodelLG[i].pos.y, playermodelLG[i].pos.z);

		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxTranslate);

		//親の行列を掛ける
		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxWorld);

		// ワールドマトリックス設定
		XMStoreFloat4x4(&playermodelLG[i].mtxWorld, mtxWorldLG);

	}
#ifdef CAMERA_DEBUG
	if (GetKeyTrigger(VK_1))
		g_nCameraType = CAMERA_VIEW_FIXED;
	if (GetKeyTrigger(VK_2))
		g_nCameraType = CAMERA_VIEW_BIRD;
	if (GetKeyTrigger(VK_3))
		g_nCameraType = CAMERA_VIEW_BEHIND;
	if (GetKeyTrigger(VK_4))
		g_nCameraType = CAMERA_VIEW_FPS;
	if (GetKeyTrigger(VK_5))
		g_nCameraType = CAMERA_VIEW_CAMERA_DEBUG;
#endif

	switch (g_nCameraType)
	{
	case CAMERA_VIEW_FIXED:
		GetCamera()->SetTarget(1.0f, 1.0f, -1.0f);
		GetCamera()->SetPos(180.0f, 40.0f, -1.0f);
		break;
	case CAMERA_VIEW_BIRD:
		GetCamera()->SetTarget(1.0f, 1.0f, -1.0f);
		GetCamera()->SetPos(250.0f, 100.0f, 0.0f);
		break;
	case CAMERA_VIEW_BEHIND:
		GetCamera()->SetTarget(playermodel[0].pos.x + SinDeg(playermodel[0].rot.y)*100.0f, playermodel[0].pos.y + 15.0f, playermodel[0].pos.z + CosDeg(playermodel[0].rot.y)*100.0f);
		GetCamera()->SetPos(playermodel[0].pos.x - SinDeg(playermodel[0].rot.y)*40.0f, playermodel[0].pos.y + 20.0f, playermodel[0].pos.z - CosDeg(playermodel[0].rot.y)*40.0f);
		break;						
	case CAMERA_VIEW_FPS:
		GetCamera()->SetTarget(playermodel[0].pos.x + SinDeg(playermodel[0].rot.y)*100.0f, playermodel[0].pos.y + 10.0f, playermodel[0].pos.z + CosDeg(playermodel[0].rot.y)*100.0f);
		GetCamera()->SetPos(playermodel[0].pos.x + SinDeg(playermodel[0].rot.y)*10.0f, playermodel[0].pos.y + 20.0f, playermodel[0].pos.z + CosDeg(playermodel[0].rot.y)*10.0f);
		break;
	case CAMERA_VIEW_DIAGONAL:
		GetCamera()->SetTarget(0.0f, 0.0f, 0.0f);
		GetCamera()->SetPos(0.0f, 100.0f, -200.0f);
		break;
	case CAMERA_VIEW_CAMERA_DEBUG:break;
	}
}



//=============================================================================
// 描画処理
//=============================================================================
void DrawPChimera(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	for (int i = 0; i < PCHIMERA_MAX; i++)
	{
		//未使用
		if (playermodel[i].nState == 0) continue;

		// モデル表示
		g_model[0].Draw(pDeviceContext, playermodel[i].mtxWorld);
		g_model[1].Draw(pDeviceContext, playermodelHD[i].mtxWorld);
		g_model[2].Draw(pDeviceContext, playermodelAM[i].mtxWorld);
		g_model[3].Draw(pDeviceContext, playermodelLG[i].mtxWorld);
	}
}

//座標取得
XMFLOAT3 GetPChimeraPos(int no)
{
	if (no < 0 || no >= PCHIMERA_MAX)
		return XMFLOAT3(0.0f, 0.0f, 0.0f);
	return playermodel[no].pos;
}

//サイズ取得
XMFLOAT3 GetPChimeraSize(int no)
{
	if (no < 0 || no >= PCHIMERA_MAX)
		return XMFLOAT3(0, 0, 0);
	//return XMFLOAT3(10.0f,10.0f,10.0f);
	XMFLOAT3 box = g_model[0].GetBBox();
	return XMFLOAT3(box.x / 2 * playermodel[no].scl.x, box.y / 2 * playermodel[no].scl.y, box.z / 2 * playermodel[no].scl.z);
}

float CheckCollisionRay(XMVECTOR pos, float fRadius)
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

void Sflag()
{
	sflag = false;
}