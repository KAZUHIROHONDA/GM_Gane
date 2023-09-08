//=============================================================================
//
// 敵処理 [enemyselect.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "AssimpModel.h"
#include "enemyselect.h"
#include "main.h"
#include "shadow.h"		//影をつけるには入れる
#include "flame.h"
#include"input.h"
#include"fade.h"
#include "Egauge.h"
#include "sceneTitle.h"
#include "sceneGame.h"
#include "enemymodel.h"
#include "sound.h"

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


#define MODELMAX	(40)

//構造体定義
typedef struct _tEnemyselect
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

}tEnemyselect;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static CAssimpModel			g_model[MODELMAX];				//モデルデータ格納用
static tEnemyselect			g_enemyselect[ENEMYSEL_MAX];	//プレイヤー
static tEnemyselect			g_enemyselectHD[ENEMYSEL_MAX];
static tEnemyselect			g_enemyselectAM[ENEMYSEL_MAX];
static tEnemyselect			g_enemyselectLG[ENEMYSEL_MAX];

static int enemyno;//敵の番号

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitSelectEnemyselect(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// 位置・回転・スケールの初期設定
	for (int i = 0; i < ENEMYSEL_MAX; i++)
	{
		g_enemyselect[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);//z100
		g_enemyselect[i].rot = XMFLOAT3(0.0f, 180.0f, 0.0f);
		g_enemyselect[i].scl = EnemyScal(i * 4);
		g_enemyselect[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);

		//初期化
		g_enemyselect[i].nPhase = 0;
		g_enemyselect[i].nState = 0;		//最初は通常
		g_enemyselect[i].Tipe = 0;

		g_enemyselect[i].nShadowIdx = -1;

		//頭 wani
		g_enemyselectHD[i].pos = EnemyScal(1 + i * 4);//z-3
		g_enemyselectHD[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyselectHD[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_enemyselectHD[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyselectHD[i].nPhase = 0;
		g_enemyselectHD[i].nState = 1;	// puroぺらは最初から登場している
		g_enemyselectHD[i].nShadowIdx = -1;

		////腕-0.8f
		g_enemyselectAM[i].pos = EnemyScal(2 + i * 4);//-0.8,-1
		g_enemyselectAM[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyselectAM[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_enemyselectAM[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyselectAM[i].nPhase = 0;
		g_enemyselectAM[i].nState = 1;	// puroぺらは最初から登場している
		g_enemyselectAM[i].nShadowIdx = -1;

		//足
		g_enemyselectLG[i].pos = EnemyScal(3 + i * 4);
		g_enemyselectLG[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyselectLG[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_enemyselectLG[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyselectLG[i].nPhase = 0;
		g_enemyselectLG[i].nState = 1;	// puroぺらは最初から登場している
		g_enemyselectLG[i].nShadowIdx = -1;


		g_enemyselect[i].nStopTime = 0; //最初は動ける

		g_enemyselect[i].nShadowIdx = CreateShadow(g_enemyselect[i].pos, 20.0f);
	}

	// ワールドマトリックスの初期化
	XMMATRIX mtxWorld;
	mtxWorld = XMMatrixIdentity();

	for (int i = 0; i < ENEMYSEL_MAX; i++)
	{
		XMStoreFloat4x4(&g_enemyselect[i].mtxWorld, mtxWorld);
		XMStoreFloat4x4(&g_enemyselectHD[i].mtxWorld, mtxWorld);
		XMStoreFloat4x4(&g_enemyselectAM[i].mtxWorld, mtxWorld);
		XMStoreFloat4x4(&g_enemyselectLG[i].mtxWorld, mtxWorld);

	}

	// モデルデータの読み込み0
	hr = g_model[0].Load(pDevice, pDeviceContext,
		GetEnemychimera(0)->GetBody()); if (FAILED(hr)) return hr;

	hr = g_model[1].Load(pDevice, pDeviceContext,
		GetEnemychimera(0)->GetHead()); if (FAILED(hr)) return hr;

	hr = g_model[2].Load(pDevice, pDeviceContext,
		GetEnemychimera(0)->GetMae()); if (FAILED(hr)) return hr;

	hr = g_model[3].Load(pDevice, pDeviceContext,
		GetEnemychimera(0)->GetBack()); if (FAILED(hr)) return hr;

	// モデルデータの読み込み1
	hr = g_model[4].Load(pDevice, pDeviceContext,
		GetEnemychimera(1)->GetBody()); if (FAILED(hr)) return hr;

	hr = g_model[5].Load(pDevice, pDeviceContext,
		GetEnemychimera(1)->GetHead()); if (FAILED(hr)) return hr;

	hr = g_model[6].Load(pDevice, pDeviceContext,
		GetEnemychimera(1)->GetMae()); if (FAILED(hr)) return hr;

	hr = g_model[7].Load(pDevice, pDeviceContext,
		GetEnemychimera(1)->GetBack()); if (FAILED(hr)) return hr;

	// モデルデータの読み込み2
	hr = g_model[8].Load(pDevice, pDeviceContext,
		GetEnemychimera(2)->GetBody()); if (FAILED(hr)) return hr;

	hr = g_model[9].Load(pDevice, pDeviceContext,
		GetEnemychimera(2)->GetHead()); if (FAILED(hr)) return hr;

	hr = g_model[10].Load(pDevice, pDeviceContext,
		GetEnemychimera(2)->GetMae()); if (FAILED(hr)) return hr;

	hr = g_model[11].Load(pDevice, pDeviceContext,
		GetEnemychimera(2)->GetBack()); if (FAILED(hr)) return hr;

	// モデルデータの読み込み3
	hr = g_model[12].Load(pDevice, pDeviceContext,
		GetEnemychimera(3)->GetBody()); if (FAILED(hr)) return hr;

	hr = g_model[13].Load(pDevice, pDeviceContext,
		GetEnemychimera(3)->GetHead()); if (FAILED(hr)) return hr;

	hr = g_model[14].Load(pDevice, pDeviceContext,
		GetEnemychimera(3)->GetMae()); if (FAILED(hr)) return hr;

	hr = g_model[15].Load(pDevice, pDeviceContext,
		GetEnemychimera(3)->GetBack()); if (FAILED(hr)) return hr;

	// モデルデータの読み込み3
	hr = g_model[16].Load(pDevice, pDeviceContext,
		GetEnemychimera(4)->GetBody()); if (FAILED(hr)) return hr;

	hr = g_model[17].Load(pDevice, pDeviceContext,
		GetEnemychimera(4)->GetHead()); if (FAILED(hr)) return hr;

	hr = g_model[18].Load(pDevice, pDeviceContext,
		GetEnemychimera(4)->GetMae()); if (FAILED(hr)) return hr;

	hr = g_model[19].Load(pDevice, pDeviceContext,
		GetEnemychimera(4)->GetBack()); if (FAILED(hr)) return hr;

	// モデルデータの読み込み4
	hr = g_model[20].Load(pDevice, pDeviceContext,
		GetEnemychimera(5)->GetBody()); if (FAILED(hr)) return hr;

	hr = g_model[21].Load(pDevice, pDeviceContext,
		GetEnemychimera(5)->GetHead()); if (FAILED(hr)) return hr;

	hr = g_model[22].Load(pDevice, pDeviceContext,
		GetEnemychimera(5)->GetMae()); if (FAILED(hr)) return hr;

	hr = g_model[23].Load(pDevice, pDeviceContext,
		GetEnemychimera(5)->GetBack()); if (FAILED(hr)) return hr;

	// モデルデータの読み込み5
	hr = g_model[24].Load(pDevice, pDeviceContext,
		GetEnemychimera(6)->GetBody()); if (FAILED(hr)) return hr;

	hr = g_model[25].Load(pDevice, pDeviceContext,
		GetEnemychimera(6)->GetHead()); if (FAILED(hr)) return hr;

	hr = g_model[26].Load(pDevice, pDeviceContext,
		GetEnemychimera(6)->GetMae()); if (FAILED(hr)) return hr;

	hr = g_model[27].Load(pDevice, pDeviceContext,
		GetEnemychimera(6)->GetBack()); if (FAILED(hr)) return hr;

	// モデルデータの読み込み6
	hr = g_model[28].Load(pDevice, pDeviceContext,
		GetEnemychimera(7)->GetBody()); if (FAILED(hr)) return hr;

	hr = g_model[29].Load(pDevice, pDeviceContext,
		GetEnemychimera(7)->GetHead()); if (FAILED(hr)) return hr;

	hr = g_model[30].Load(pDevice, pDeviceContext,
		GetEnemychimera(7)->GetMae()); if (FAILED(hr)) return hr;

	hr = g_model[31].Load(pDevice, pDeviceContext,
		GetEnemychimera(7)->GetBack()); if (FAILED(hr)) return hr;

	// モデルデータの読み込み7
	hr = g_model[32].Load(pDevice, pDeviceContext,
		GetEnemychimera(8)->GetBody()); if (FAILED(hr)) return hr;

	hr = g_model[33].Load(pDevice, pDeviceContext,
		GetEnemychimera(8)->GetHead()); if (FAILED(hr)) return hr;

	hr = g_model[34].Load(pDevice, pDeviceContext,
		GetEnemychimera(8)->GetMae()); if (FAILED(hr)) return hr;

	hr = g_model[35].Load(pDevice, pDeviceContext,
		GetEnemychimera(8)->GetBack()); if (FAILED(hr)) return hr;

	// モデルデータの読み込み8
	hr = g_model[36].Load(pDevice, pDeviceContext,
		GetEnemychimera(9)->GetBody()); if (FAILED(hr)) return hr;

	hr = g_model[37].Load(pDevice, pDeviceContext,
		GetEnemychimera(9)->GetHead()); if (FAILED(hr)) return hr;

	hr = g_model[38].Load(pDevice, pDeviceContext,
		GetEnemychimera(9)->GetMae()); if (FAILED(hr)) return hr;

	hr = g_model[39].Load(pDevice, pDeviceContext,
		GetEnemychimera(9)->GetBack()); if (FAILED(hr)) return hr;


	return hr;

}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemyselect(void)
{
	//影の解放
	for (int i = 0; i < ENEMYSEL_MAX; i++)
	{
		ReleaseShadow(g_enemyselect[i].nShadowIdx);
	}


	// モデルの解放
	for (int i = 0; i < MODELMAX; i++)
	{
		g_model[i].Release();
	}
}

//=============================================================================
// 更新処理
//=============================================================================
//enemyselectのセレクトアップデート
void UpdateSelectEnemyselect(void)
{
	// プレイヤーの動きを記述するならここ
	for (int i = 0; i < ENEMYSEL_MAX; i++)
	{
		//未使用
		if (g_enemyselect[i].nState == 0) continue;


		//ホイール回転
		int temp2 = GetMouseWheelDelta();
		wchar_t str[256];
		wsprintf(str,L"%d", temp2);
		OutputDebugString(str);
        g_enemyselect[i].pos.z += static_cast<float>(temp2) / 5;
		


		float hitLength[10];

		for (int i = 0; i < 10; i++)
		{//位置と半径を指定
			hitLength[i] = ECheckCollisionRay1(XMVectorSet(g_enemyselect[i].pos.x, g_enemyselect[i].pos.y, g_enemyselect[i].pos.z, 0.0f), 20.0f);
			if (hitLength[i] < 9999.9f)
			{
				g_enemyselect[i].rot.y += 0.1f;
				SelectEnemy(i);
				GetEnemy()->Percent();
				if (GetMouseTrigger(0))
				{
					PlaySound(SOUND_LABEL_SE_SELECTS);
					//敵の選択
					enemyno = i;
					StartFade(SCENE_GAME);
				}
			}
			else
			{
				g_enemyselect[i].rot = XMFLOAT3(0.0f, 90.0f, 0.0f);
			}
		}



		XMMATRIX mtxWorld, mtxRot, mtxScl,
			mtxTranslate;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_enemyselect[i].scl.x, g_enemyselect[i].scl.y, g_enemyselect[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);



		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(g_enemyselect[i].rot.x), XMConvertToRadians(g_enemyselect[i].rot.y + 180), XMConvertToRadians(g_enemyselect[i].rot.z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);


		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_enemyselect[i].pos.x, g_enemyselect[i].pos.y, g_enemyselect[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);


		// ワールドマトリックス設定
		XMStoreFloat4x4(&g_enemyselect[i].mtxWorld, mtxWorld);

		//影の移動
		MoveShadow(g_enemyselect[i].nShadowIdx, g_enemyselect[i].pos);


		//************************************************:
		//頭

		XMMATRIX mtxWorldHD;
		// ワールドマトリックスの初期化
		mtxWorldHD = XMMatrixIdentity();
		// スケールを反映
		mtxScl = XMMatrixScaling(g_enemyselectHD[i].scl.x, g_enemyselectHD[i].scl.y, g_enemyselectHD[i].scl.z);
		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxScl);
		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(g_enemyselectHD[i].rot.x),
			XMConvertToRadians(g_enemyselectHD[i].rot.y),//+180
			XMConvertToRadians(g_enemyselectHD[i].rot.z)
		);
		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_enemyselectHD[i].pos.x, g_enemyselectHD[i].pos.y, g_enemyselectHD[i].pos.z);

		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxTranslate);

		//親の行列を掛ける
		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxWorld);

		// ワールドマトリックス設定
		XMStoreFloat4x4(&g_enemyselectHD[i].mtxWorld, mtxWorldHD);


		//************************************************:
		//腕

		XMMATRIX mtxWorldAM;
		// ワールドマトリックスの初期化
		mtxWorldAM = XMMatrixIdentity();
		// スケールを反映
		mtxScl = XMMatrixScaling(g_enemyselectAM[i].scl.x, g_enemyselectAM[i].scl.y, g_enemyselectAM[i].scl.z);
		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxScl);
		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(g_enemyselectAM[i].rot.x),
			XMConvertToRadians(g_enemyselectAM[i].rot.y),//+180
			XMConvertToRadians(g_enemyselectAM[i].rot.z)
		);
		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_enemyselectAM[i].pos.x, g_enemyselectAM[i].pos.y, g_enemyselectAM[i].pos.z);

		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxTranslate);

		//親の行列を掛ける
		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxWorld);

		// ワールドマトリックス設定
		XMStoreFloat4x4(&g_enemyselectAM[i].mtxWorld, mtxWorldAM);

		//************************************************:
		//足

		XMMATRIX mtxWorldLG;
		// ワールドマトリックスの初期化
		mtxWorldLG = XMMatrixIdentity();
		// スケールを反映
		mtxScl = XMMatrixScaling(g_enemyselectLG[i].scl.x, g_enemyselectLG[i].scl.y, g_enemyselectLG[i].scl.z);
		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxScl);
		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(g_enemyselectLG[i].rot.x),
			XMConvertToRadians(g_enemyselectLG[i].rot.y),//+180
			XMConvertToRadians(g_enemyselectLG[i].rot.z)
		);
		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_enemyselectLG[i].pos.x, g_enemyselectLG[i].pos.y, g_enemyselectLG[i].pos.z);

		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxTranslate);

		//親の行列を掛ける
		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxWorld);

		// ワールドマトリックス設定
		XMStoreFloat4x4(&g_enemyselectLG[i].mtxWorld, mtxWorldLG);
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemyselect(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	for (int i = 0; i < ENEMYSEL_MAX; i++)
	{
		//未使用
		if (g_enemyselect[i].nState == 0) continue;

		// モデル表示
		g_model[0 + i * 4].Draw(pDeviceContext, g_enemyselect[i].mtxWorld);
		g_model[1 + i * 4].Draw(pDeviceContext, g_enemyselectHD[i].mtxWorld);
		g_model[2 + i * 4].Draw(pDeviceContext, g_enemyselectAM[i].mtxWorld);
		g_model[3 + i * 4].Draw(pDeviceContext, g_enemyselectLG[i].mtxWorld);

	}

	GetEnemy()->DrawChimera(500, 0);
}

//生存確認
bool IsEnemyselect(int no)
{
	if (no < 0 || no >= ENEMYSEL_MAX) return false;
	return g_enemyselect[no].nState > 0 ?
		true : false;
}


//座標取得
XMFLOAT3 GetEnemyselectPos(int no)
{
	if (no < 0 || no >= ENEMYSEL_MAX)
		return XMFLOAT3(0.0f, 0.0f, 0.0f);
	return g_enemyselect[no].pos;
}

//サイズ取得
XMFLOAT3 GetEnemyselectSize(int no)
{
	if (no < 0 || no >= ENEMYSEL_MAX)
		return XMFLOAT3(0,0,0);
	//return XMFLOAT3(10.0f,10.0f,10.0f);
	XMFLOAT3 box = g_model[0].GetBBox();
	return XMFLOAT3(box.x / 2 * g_enemyselect[no].scl.x, box.y / 2 * g_enemyselect[no].scl.y, box.z / 2 * g_enemyselect[no].scl.z);
}

int Enemyno()
{
	return enemyno;
}

//敵の値セット
void SelectEnemy(int val)
{
	switch (val)
	{
	case 0: 
	{
		GetEnemy()->Setmodel(0);
		GetEnemy()->Setmodel(10);
		GetEnemy()->Setmodel(20);
		GetEnemy()->Setmodel(30);
		GetEnemy()->SetName("カブトムシ");
		break;
	}
	case 1:
	{
		GetEnemy()->Setmodel(1);
		GetEnemy()->Setmodel(11);
		GetEnemy()->Setmodel(21);
		GetEnemy()->Setmodel(31);
		GetEnemy()->SetName("ワニ");
		break;
	}
	case 2:
	{
		GetEnemy()->Setmodel(2);
		GetEnemy()->Setmodel(12);
		GetEnemy()->Setmodel(22);
		GetEnemy()->Setmodel(32);
		GetEnemy()->SetName("パンダ");
		break;
	}
	case 3:
	{
		GetEnemy()->Setmodel(3);
		GetEnemy()->Setmodel(13);
		GetEnemy()->Setmodel(23);
		GetEnemy()->Setmodel(33);
		GetEnemy()->SetName("トリ");
		break;
	}
	case 4:
	{
		GetEnemy()->Setmodel(4);
		GetEnemy()->Setmodel(14);
		GetEnemy()->Setmodel(24);
		GetEnemy()->Setmodel(34);
		GetEnemy()->SetName("ぶた");
		break;
	}
	case 5:
	{
		GetEnemy()->Setmodel(2);
		GetEnemy()->Setmodel(12);
		GetEnemy()->Setmodel(22);
		GetEnemy()->Setmodel(32);
		GetEnemy()->SetName("ウシ");
		break;
	}
	case 6:
	{
		GetEnemy()->Setmodel(2);
		GetEnemy()->Setmodel(12);
		GetEnemy()->Setmodel(22);
		GetEnemy()->Setmodel(32);
		GetEnemy()->SetName("キメラ１");
		break;
	}
	case 7:
	{
		GetEnemy()->Setmodel(2);
		GetEnemy()->Setmodel(12);
		GetEnemy()->Setmodel(22);
		GetEnemy()->Setmodel(32);
		GetEnemy()->SetName("キメラ２");
		break;
	}
	case 8:
	{
		GetEnemy()->Setmodel(2);
		GetEnemy()->Setmodel(12);
		GetEnemy()->Setmodel(22);
		GetEnemy()->Setmodel(32);
		GetEnemy()->SetName("キメラ３");
		break;
	}
	case 9:
	{
		GetEnemy()->Setmodel(2);
		GetEnemy()->Setmodel(12);
		GetEnemy()->Setmodel(22);
		GetEnemy()->Setmodel(32);
		GetEnemy()->SetName("キメラ４");
		break;
	}

	default:
		break;
	}
}


float ECheckCollisionRay1(XMVECTOR pos, float fRadius)
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

int SetEnemyselect(XMFLOAT3 pos, XMFLOAT3 rot)
{
	tEnemyselect* pEnemy = g_enemyselect;
	for (int i = 0; i < ENEMYSEL_MAX; ++i, ++pEnemy)
	{	// 出現している敵はスルー
		if (pEnemy->nState > 0) continue;

		pEnemy->pos = pos;

		g_enemyselect[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyselect[i].Tipe = 0;

		pEnemy->nState = 1;	// 出現
		pEnemy->nShadowIdx =
			CreateShadow(pEnemy->pos, ENEMY_RADIUS);
		pEnemy->rot = rot;//モデルの向き
		return i;	// 番号を返す
	}

	return -1;	// 見つからなかった
}
