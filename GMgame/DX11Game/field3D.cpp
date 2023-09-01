//=============================================================================
//
// 3Dフィールド処理 [field.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "AssimpModel.h"
#include "field3D.h"
#include "main.h"
#include "input.h"
#include "shadow.h"		//影をつけるには入れる
#include "gauge.h"
#include "sound.h"
#include "MessageManager.h"
#include"fade.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_FIELD		"data/model/field/monu3.obj"
#define MODEL_TREE		"data/model/tree.fbx"

//構造体定義
typedef struct _tField3D
{
	XMFLOAT4X4		mtxWorld;		//座標変換行列

	XMFLOAT3		pos;			//座標
	XMFLOAT3		rot;			//回転
	XMFLOAT3		scl;			//拡大縮小
				
}tField3D;

typedef struct _tTree
{
	XMFLOAT4X4		mtxWorld;		//座標変換行列

	XMFLOAT3		pos;			//座標
	XMFLOAT3		rot;			//回転
	XMFLOAT3		scl;			//拡大縮小

}tTree;


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static CAssimpModel		g_model[2];				//モデルデータ格納用
			
static tField3D			g_field[FIELD_MAX];	//フィールド
static tTree			g_tree[TREE_MAX];	//ツリー

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitField3D(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// 位置・回転・スケールの初期設定
	for (int i = 0; i < FIELD_MAX; i++)
	{
		g_field[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_field[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_field[i].scl = XMFLOAT3(100.0f, 100.0f, 100.0f);
	}

	for (int i = 0; i < TREE_MAX; i++)
	{
		g_tree[i].pos = XMFLOAT3(30.0f, 60.0f, 350.0f);
		g_tree[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_tree[i].scl = XMFLOAT3(30.0f, 30.0f, 30.0f);
	}
		
	// ワールドマトリックスの初期化
	XMMATRIX mtxWorld;
	mtxWorld = XMMatrixIdentity();

	for (int i = 0; i < FIELD_MAX; i++)
	{
		XMStoreFloat4x4(&g_field[i].mtxWorld, mtxWorld);
	}

	for (int i = 0; i < TREE_MAX; i++)
	{
		XMStoreFloat4x4(&g_tree[i].mtxWorld, mtxWorld);
	}

	// モデルデータの読み込み
	hr = g_model[0].Load(pDevice, pDeviceContext,
		MODEL_FIELD); if (FAILED(hr)) return hr;
	hr = g_model[1].Load(pDevice, pDeviceContext,
		MODEL_TREE); if (FAILED(hr)) return hr;

	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitField3D(void)
{
	// モデルの解放
	g_model[0].Release();
	g_model[1].Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateField3D(void)
{

	for (int i = 0; i < FIELD_MAX; i++)
	{
		XMMATRIX mtxWorld, mtxRot, mtxScl,
			mtxTranslate;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_field[i].scl.x, g_field[i].scl.y, g_field[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);



		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(g_field[i].rot.x), XMConvertToRadians(g_field[i].rot.y + 180), XMConvertToRadians(g_field[i].rot.z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);


		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_field[i].pos.x, g_field[i].pos.y, g_field[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);


		// ワールドマトリックス設定
		XMStoreFloat4x4(&g_field[i].mtxWorld, mtxWorld);
	}

	for (int i = 0; i < TREE_MAX; i++)
	{
		XMMATRIX mtxWorld, mtxRot, mtxScl,
			mtxTranslate;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_tree[i].scl.x, g_tree[i].scl.y, g_tree[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);



		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(g_tree[i].rot.x), XMConvertToRadians(g_tree[i].rot.y + 180), XMConvertToRadians(g_tree[i].rot.z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);


		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_tree[i].pos.x, g_tree[i].pos.y, g_tree[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);


		// ワールドマトリックス設定
		XMStoreFloat4x4(&g_tree[i].mtxWorld, mtxWorld);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawField3D(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	for (int i = 0; i < FIELD_MAX; i++)
	{
		// モデル表示
		g_model[0].Draw(pDeviceContext, g_field[i].mtxWorld);
	}

	for (int i = 0; i < TREE_MAX; i++)
	{
		// モデル表示
		g_model[1].Draw(pDeviceContext, g_tree[i].mtxWorld);
	}
}