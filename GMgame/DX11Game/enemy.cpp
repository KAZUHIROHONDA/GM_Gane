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

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_ENEMY		"data/model/kamereon.obj"
//#define MODEL_ENEMY		"data/model/wani.obj"
#define MODEL_ENEMY01	"data/model/enemy01.fbx"
#define MODEL_ENEMY02	"data/model/enemy02.fbx"

#define VALUE_MOVE		(0.1f)		
#define VALUE_ROTATE	(2.0f)

#define ENEMY_RADIUS  (10.0f)

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

	int				nHP;

	int				nShadowIdx;		//影の番号

}tEnemy;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static CAssimpModel		g_model[3];				//モデルデータ格納用
static tEnemy			g_enemy[ENEMY_MAX];	//プレイヤー

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
		g_enemy[i].pos = XMFLOAT3(0.0f, 0.0f, 20.0f);
		g_enemy[i].rot = XMFLOAT3(0.0f, 180.0f, 0.0f);
		g_enemy[i].scl = XMFLOAT3(3.0f,3.0f,3.0f);
		g_enemy[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		//初期化
		g_enemy[i].nPhase = 1;
		g_enemy[i].nState = 0;		//最初は通常
		g_enemy[i].Tipe = 0;

		g_enemy[i].nHP = 1000;

		g_enemy[i].nShadowIdx = -1;
	}
	
	bfCnt = 0;

	// ワールドマトリックスの初期化
	XMMATRIX mtxWorld;
	mtxWorld = XMMatrixIdentity();

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		XMStoreFloat4x4(&g_enemy[i].mtxWorld, mtxWorld);
	}

	// モデルデータの読み込み
	hr = g_model[0].Load(pDevice, pDeviceContext,MODEL_ENEMY);
	// モデルデータの読み込み
	hr = g_model[1].Load(pDevice, pDeviceContext,MODEL_ENEMY01);
	// モデルデータの読み込み
	hr = g_model[2].Load(pDevice, pDeviceContext, MODEL_ENEMY02);
	
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

		//速度
		g_enemy[i].pos.x += g_enemy[i].vel.x;
		g_enemy[i].pos.y += g_enemy[i].vel.y;
		g_enemy[i].pos.z += g_enemy[i].vel.z;

		// モデルの動きを記述するならここ
		switch (g_enemy[i].nPhase)
		{
		case 1:g_enemy[i].pos.y+= VALUE_MOVE;				//右へ
			if (g_enemy[i].pos.y>= 15.0f) g_enemy[i].nPhase = -1;
			break;

		case -1:g_enemy[i].pos.y -= VALUE_MOVE;				//左へ
			if (g_enemy[i].pos.y <= 5.0f) g_enemy[i].nPhase = +1;
			break;
		}

		//画面外に行かない処理
		XMFLOAT3 box = g_model[0].GetBBox();
		//[左端] 
		if (g_enemy[i].pos.x < -200 + box.x * g_enemy[i].scl.x)
		{
			g_enemy[i].pos.x = -200 + box.x * g_enemy[i].scl.x;
			g_enemy[i].vel.x *= -1.0f;
		}
		//[右端]
		else if (g_enemy[i].pos.x > 200 - box.x * g_enemy[i].scl.x)
		{
			g_enemy[i].pos.x = 200 - box.x * g_enemy[i].scl.x;
			g_enemy[i].vel.x *= -1.0f;
		}
		//[下端] プレイヤーの左端＜画面の上端
		if (g_enemy[i].pos.z > 200 - box.z * g_enemy[i].scl.z)
		{
			g_enemy[i].pos.z = 200 - box.z * g_enemy[i].scl.z;
			g_enemy[i].vel.z *= -1.0f;
		}
		//[上端] プレイヤーの左端＜画面の下端
		else if (g_enemy[i].pos.z < -200 + box.z * g_enemy[i].scl.z)
		{
			g_enemy[i].pos.z = -200 + box.z * g_enemy[i].scl.z;
			g_enemy[i].vel.z *= -1.0f;
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
		switch (g_enemy[i].Tipe)
		{  //方向で速度を変える
		case 0:
		{
			g_model[0].Draw(pDeviceContext, g_enemy[i].mtxWorld);

		} break;
		case 1:
		{
			g_model[1].Draw(pDeviceContext, g_enemy[i].mtxWorld);
		} break;
		case 2:
		{
			g_model[2].Draw(pDeviceContext, g_enemy[i].mtxWorld); 
		} break;
		
		}

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
int SetEnemy(XMFLOAT3 pos,int no)
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
}


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