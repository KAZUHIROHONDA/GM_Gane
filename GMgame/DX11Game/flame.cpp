//=============================================================================
//
// 炎処理 [flame.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "flame.h"
#include "Shader.h"
#include "Texture.h"
#include "mesh.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_FLAME	L"data/texture/shadow000.jpg"	// テクスチャ

#define M_DIFFUSE		XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR		XMFLOAT4(0.0f,0.0f,0.0f,0.0f)
#define M_AMBIENT		XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_EMISSIVE		XMFLOAT4(0.0f,0.0f,0.0f,0.0f)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct _tFlame
{
	int			nState;	// 状態
	XMFLOAT3	pos;	// 座標
	XMFLOAT3	vel;	// 速度
	XMFLOAT4	col;	//色
	float		fSize;	//サイズ
	int			nLife;	//生存時間

}tFlame;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexFlame(ID3D11Device* pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static MESH		g_mesh;		// メッシュ情報
static MATERIAL	g_material;	// マテリアル情報

static tFlame	g_flame[MAX_FLAME];	//炎ストック

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitFlame(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr;
	HWND hWnd = GetMainWnd();

	// マテリアルの初期設定
	g_material.Diffuse = M_DIFFUSE;
	g_material.Ambient = M_AMBIENT;
	g_material.Specular = M_SPECULAR;
	g_material.Power = 50.0f;
	g_material.Emissive = M_EMISSIVE;
	g_mesh.pMaterial = &g_material;

	// テクスチャの読み込み
	hr = CreateTextureFromFile(pDevice,
		TEXTURE_FLAME,
		&g_mesh.pTexture);
	if (FAILED(hr))
	{
		MessageBox(hWnd,
			_T("炎テクスチャ読み込みエラー"),
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
	hr = MakeVertexFlame(pDevice);

	// 変数の初期化
	for (int i = 0; i < MAX_FLAME; i++)
	{
		g_flame[i].nState = 0;
	}

	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitFlame(void)
{
	// メッシュ解放
	ReleaseMesh(&g_mesh);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateFlame(void)
{

	// 更新処理
	tFlame* pFlame = g_flame;
	for (int i = 0; i < MAX_FLAME;
		++i, ++pFlame)
	{
		if (pFlame->nState == 0) continue;

		// 速度の更新
		pFlame->vel.x *= 0.95f;	//減速
		pFlame->vel.y *= 0.98f;
		pFlame->vel.z *= 0.95f;
		
		// 位置の更新
		pFlame->pos.x += pFlame->vel.x;
		pFlame->pos.y += pFlame->vel.y;
		pFlame->pos.z += pFlame->vel.z;

		// 他に更新情報があればここで処理
			// サイズに変更があればここで変更

			// 色（アルファ値）を変更する
		
		if (pFlame->nLife < 20)
		{
			pFlame->col.w -= 1.0f / 20;
		}

		// 生存時間判定
		if (--pFlame->nLife <= 0)
		{
			pFlame->nState = 0;
			continue;
		}
			
	

		// 他に削除条件があればここで処理
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawFlame(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// 合成設定
	SetBlendState(BS_ADDITIVE);

	XMFLOAT4X4 mView = GetCamera()->GetViewMatrix();
	tFlame* pFlame = g_flame;
	for (int i = 0; i < MAX_FLAME; ++i, ++pFlame)
	{	// 出てない炎は描画しない
		if (pFlame->nState == 0) continue;
		
		// ビュー行列の回転成分の転置行列を設定
		g_mesh.mtxWorld._11 = mView._11;
		g_mesh.mtxWorld._12 = mView._21;
		g_mesh.mtxWorld._13 = mView._31;
		g_mesh.mtxWorld._21 = mView._12;
		g_mesh.mtxWorld._22 = mView._22;
		g_mesh.mtxWorld._23 = mView._32;
		g_mesh.mtxWorld._31 = mView._13;
		g_mesh.mtxWorld._32 = mView._23;
		g_mesh.mtxWorld._33 = mView._33;

		// 拡大縮小
		XMMATRIX mtxScale = XMMatrixScaling(pFlame->fSize,
			pFlame->fSize, pFlame->fSize);
		XMMATRIX mtxRot;
		mtxRot = XMLoadFloat4x4(&g_mesh.mtxWorld);
		mtxRot = XMMatrixMultiply(mtxRot, mtxScale);
		XMStoreFloat4x4(&g_mesh.mtxWorld, mtxRot);

		// 位置を反映
		g_mesh.mtxWorld._41 = pFlame->pos.x;
		g_mesh.mtxWorld._42 = pFlame->pos.y;
		g_mesh.mtxWorld._43 = pFlame->pos.z;

		// カラーの設定
		g_material.Diffuse = pFlame->col;

		DrawMesh(pDeviceContext, &g_mesh);
	}
	// もとに戻す
	SetBlendState(BS_NONE);	
}

// 炎の作成
int SetFlame(XMFLOAT3 pos, float size,int no)
{
	tFlame* pFlame = g_flame;
	for (int i = 0; i < MAX_FLAME;	++i, ++pFlame)
	{	// 出現している炎はスルー
		if (pFlame->nState > 0) continue;

		pFlame->pos = pos;
		// 初速設定
		pFlame->vel.x = (rand() % 101 - 50) / 50.0f;// 0.0f;
		pFlame->vel.y = 1.0f+(rand() % 101 - 50) / 50.0f;//1.0f;
		pFlame->vel.z = (rand() % 101 - 50) / 50.0f;//0.0f;
		pFlame->nLife = 180;
		pFlame->nState = 1;
		pFlame->fSize = size;

		return i;	// 番号を返す
	}

	return -1;	// 見つからなかった
}


HRESULT MakeVertexFlame(ID3D11Device* pDevice)
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

