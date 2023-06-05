//=============================================================================
//
// 弾処理 [bullet.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "bullet.h"
#include "Shader.h"
#include "Texture.h"
#include "mesh.h"
#include "shadow.h"
#include "fade.h"
#include "enemy.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_BULLET	L"data/texture/bullet000.png"	// テクスチャ

#define M_DIFFUSE		XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR		XMFLOAT4(0.0f,0.0f,0.0f,0.0f)
#define M_AMBIENT		XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_EMISSIVE		XMFLOAT4(0.0f,0.0f,0.0f,0.0f)

#define BULLET_SPEED (1.0f)		//弾の速度
#define BULLET_RADIUS (10.0f)	//弾の半径


//*****************************************************************************
// 構造体定義
//*****************************************************************************

typedef struct _tBullet
{
	XMFLOAT3		pos;		//座標
	XMFLOAT3		vel;		//速度
	int				nState;		//状態
	int				nShadowIdx; //影番号
}tBullet;

int					bCnt;		//デストロイ関数のカウント


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBullet(ID3D11Device* pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static MESH		g_mesh;		// メッシュ情報
static MATERIAL	g_material;	// マテリアル情報

static tBullet g_bullet[BULLET_MAX];//弾ストック

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBullet(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr;
	HWND hWnd = GetMainWnd();

	bCnt = 0;
	

	// マテリアルの初期設定
	g_material.Diffuse = M_DIFFUSE;
	g_material.Ambient = M_AMBIENT;
	g_material.Specular = M_SPECULAR;
	g_material.Power = 50.0f;
	g_material.Emissive = M_EMISSIVE;
	g_mesh.pMaterial = &g_material;

	// テクスチャの読み込み
	hr = CreateTextureFromFile(pDevice,
		TEXTURE_BULLET,
		&g_mesh.pTexture);
	if (FAILED(hr))
	{
		MessageBox(hWnd,
			_T("弾テクスチャ読み込みエラー"),
			_T("Error"),
			MB_OK | MB_ICONSTOP);
		return hr;
	}
	XMStoreFloat4x4(&g_mesh.mtxTexture,
		XMMatrixIdentity());

	// ワールドマトリクス初期化
	XMStoreFloat4x4(&g_mesh.mtxWorld,
		XMMatrixIdentity());

	// 頂点情報の作成
	hr = MakeVertexBullet(pDevice);
	
	for (int i = 0; i < BULLET_MAX; i++)
	{
		g_bullet[i].nState = 0;
		g_bullet[i].nShadowIdx = -1;
	}
	
	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBullet(void)
{
	tBullet* pBullet = g_bullet;
	for (int i = 0; i < BULLET_MAX;
		++i, ++pBullet)
	{
		if (pBullet->nState == 0) continue;
		pBullet->nState = 0;
		// 影との関連を切る
		ReleaseShadow(pBullet->nShadowIdx);
		pBullet->nShadowIdx = -1;
	}

	// メッシュ解放
	ReleaseMesh(&g_mesh);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBullet(void)
{

	// 更新処理
	tBullet* pBullet = g_bullet;
	for (int i = 0; i < BULLET_MAX;++i, ++pBullet)
	{
		if (pBullet->nState == 0) continue;

		//重力
		//pBullet->vel.y += -0.098f;
		

		// 位置の更新
		pBullet->pos.x += pBullet->vel.x;
		pBullet->pos.y += pBullet->vel.y;
		pBullet->pos.z += pBullet->vel.z;


		// 画面外判定
		if (pBullet->pos.x < -200.0f ||
			pBullet->pos.x > 200.0f ||
			pBullet->pos.z < -200.0f ||
			pBullet->pos.z > 200.0f ||
			pBullet->pos.y < 0.0f ||
			pBullet->pos.y > 1000.0f)
		{
			// 影との関連を切る
			ReleaseShadow(pBullet->nShadowIdx);
			pBullet->nShadowIdx = -1;
			
			DestroyBullet(i);

			continue;
		}
		// 丸影移動
		MoveShadow(pBullet->nShadowIdx,
			pBullet->pos);
	}

	
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBullet(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	// 合成設定
	SetBlendState(BS_ALPHABLEND);
	
	//ビューマトリックス取得
	XMFLOAT4X4 mView = GetCamera()->GetViewMatrix();
	tBullet* pBullet = g_bullet;
	for (int i = 0; i < BULLET_MAX; ++i, ++pBullet)
	{
		//未出現の弾は描画しない
		if (pBullet->nState == 0) continue;

		//ビュー行列の回転成分の転置行列を設定
		g_mesh.mtxWorld._11 = mView._11;
		g_mesh.mtxWorld._12 = mView._21;
		g_mesh.mtxWorld._13 = mView._31;
		g_mesh.mtxWorld._21 = mView._12;
		g_mesh.mtxWorld._22 = mView._22;
		g_mesh.mtxWorld._23 = mView._32;
		g_mesh.mtxWorld._31 = mView._13;
		g_mesh.mtxWorld._32 = mView._23;
		g_mesh.mtxWorld._33 = mView._33;

		//拡大縮小
		XMMATRIX mtxScale = XMMatrixScaling(BULLET_RADIUS, BULLET_RADIUS, BULLET_RADIUS);
		XMMATRIX mtxRot;
		mtxRot = XMLoadFloat4x4(&g_mesh.mtxWorld);
		mtxRot = XMMatrixMultiply(mtxRot, mtxScale);
		XMStoreFloat4x4(&g_mesh.mtxWorld, mtxRot);

		//位置を反映
		g_mesh.mtxWorld._41 = pBullet->pos.x;
		g_mesh.mtxWorld._42 = pBullet->pos.y;
		g_mesh.mtxWorld._43 = pBullet->pos.z;

		DrawMesh(pDeviceContext, &g_mesh);

	}
	
	
	// もとに戻す
	SetBlendState(BS_NONE);	
}

// 弾の作成
int FireBullet(XMFLOAT3 pos, XMFLOAT3 dir,int no)
{
	tBullet* pBullet = g_bullet;
	for (int i = 0; i < BULLET_MAX;	++i, ++pBullet)
	{	// 出現している弾はスルー
		if (pBullet->nState > 0) continue;

		pBullet->pos = pos;
		pBullet->vel.x = dir.x * BULLET_SPEED * no;
		pBullet->vel.y = dir.y * BULLET_SPEED * no;
		pBullet->vel.z = dir.z * BULLET_SPEED * no;
		pBullet->nState = 1;	// 出現
		pBullet->nShadowIdx =
			CreateShadow(pBullet->pos, BULLET_RADIUS);
		return i;	// 番号を返す
	}

	return -1;	// 見つからなかった
}

HRESULT MakeVertexBullet(ID3D11Device* pDevice)
{
	VERTEX_3D vertexWk[NUM_VERTEX]; //作業用頂点情報
	int		  indexWk[NUM_VERTEX];	//作業用インデックス

	// 頂点座標の設定
	g_mesh.nNumVertex = NUM_VERTEX;
	vertexWk[0].vtx = XMFLOAT3(-0.5f, 0.5f, 0.0f);
	vertexWk[1].vtx = XMFLOAT3( 0.5f, 0.5f, 0.0f);
	vertexWk[2].vtx = XMFLOAT3(-0.5f,-0.5f, 0.0f);
	vertexWk[3].vtx = XMFLOAT3( 0.5f,-0.5f, 0.0f);

	// ディフューズの設定
	vertexWk[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertexWk[1].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertexWk[2].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertexWk[3].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// 法線ベクトルの設定
	vertexWk[0].nor = XMFLOAT3(0.0f, 0.0f,-1.0f);
	vertexWk[1].nor = XMFLOAT3(0.0f, 0.0f,-1.0f);
	vertexWk[2].nor = XMFLOAT3(0.0f, 0.0f,-1.0f);
	vertexWk[3].nor = XMFLOAT3(0.0f, 0.0f,-1.0f);

	// テクスチャ座標の設定
	vertexWk[0].tex = XMFLOAT2(0.0f, 0.0f);
	vertexWk[1].tex = XMFLOAT2(1.0f, 0.0f);
	vertexWk[2].tex = XMFLOAT2(0.0f, 1.0f);
	vertexWk[3].tex = XMFLOAT2(1.0f, 1.0f);

	// インデックス配列の設定
	g_mesh.nNumIndex = NUM_VERTEX;
	indexWk[0] = 0;
	indexWk[1] = 1;
	indexWk[2] = 2;
	indexWk[3] = 3;

	return MakeMeshVertex(pDevice, &g_mesh, vertexWk, indexWk);
}

//生存確認
bool IsBullet(int no)
{
	if (no < 0 || no >= BULLET_MAX) return false;
	return g_bullet[no].nState > 0 ?
		true : false;
}

void DestroyBullet(int no)
{
	if (no < 0 || no >= BULLET_MAX) return;
	g_bullet[no].nState = 0;
	// 影との関連を切る
	ReleaseShadow(g_bullet[no].nShadowIdx);
	g_bullet[no].nShadowIdx = -1;

	bCnt++;


}

//座標取得
XMFLOAT3 GetBulletPos(int no)
{
	if (no < 0 || no >= BULLET_MAX)
		return XMFLOAT3(0.0f, 0.0f,0.0f);
	return g_bullet[no].pos;
}

//サイズ取得
XMFLOAT3 GetBulletSize(int no)
{
	if (no < 0 || no >= BULLET_MAX)
		return XMFLOAT3(0.0f, 0.0f,0.0f);
	return XMFLOAT3(BULLET_RADIUS, BULLET_RADIUS, BULLET_RADIUS);
}
