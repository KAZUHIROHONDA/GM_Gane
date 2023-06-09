//= ============================================================================
//
// 地面処理 [field.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "field.h"
#include "Shader.h"
#include "Texture.h"
#include "mesh.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_FIELD	L"data/texture/field.jpg"

#define M_DIFFUSE		XMFLOAT4(1.0f,1.0f,1.0f,1.0f)	//拡散反射光
#define M_SPECULAR		XMFLOAT4(0.0f,0.0f,0.0f,0.0f)	//鏡面反射光
#define M_AMBIENT		XMFLOAT4(0.0f,0.0f,0.0f,1.0f)	//環境光
#define M_EMISSIVE		XMFLOAT4(0.0f,0.0f,0.0f,0.0f)	//自己発光率

#define VALUE_MOVE		(5.0f)	// 移動量
#define VALUE_ROTATE	(XM_PI*0.02f)	// 回転量

#define SIZE_X			(500.0f)	//地面のサイズX
#define SIZE_Z			(500.0f)	//地面のサイズY

//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexField(ID3D11Device* pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static MESH		g_mesh;		 //メッシュ情報
static MATERIAL g_material;  //マテリアル情報

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitField(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr;
	HWND hWnd = GetMainWnd();

	// 位置回転拡大率の初期化
	g_mesh.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_mesh.rot = XMFLOAT3(0.0f, 0.0f, 0.0f);


	// マテリアルの初期設定
	g_material.Diffuse = M_DIFFUSE;
	g_material.Ambient = M_AMBIENT;
	g_material.Specular = M_SPECULAR;
	g_material.Power = 1.0f;
	g_material.Emissive = M_EMISSIVE;
	g_mesh.pMaterial = &g_material;

	// テクスチャの読み込み
	hr = CreateTextureFromFile(pDevice,
		TEXTURE_FIELD,
		&g_mesh.pTexture);
	if (FAILED(hr))
	{
		MessageBox(hWnd,
			_T("地面テクスチャ読み込みエラー"),
			_T("Error"),
			MB_OK | MB_ICONSTOP);
		return hr;
	}
	XMStoreFloat4x4(&g_mesh.mtxTexture,
		XMMatrixIdentity());

	// 頂点情報の作成
	hr = MakeVertexField(pDevice);

	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitField(void)
{
	// メッシュ解放
	ReleaseMesh(&g_mesh);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateField(void)
{
	// 更新処理

	// メッシュ更新
	UpdateMesh(&g_mesh);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawField(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// メッシュの描画
	DrawMesh(pDeviceContext, &g_mesh);
}

HRESULT MakeVertexField(ID3D11Device* pDevice)
{
	VERTEX_3D vertexWk[NUM_VERTEX]; //作業用頂点情報
	int		  indexWk[NUM_VERTEX];	//作業用インデックス

	// 頂点座標の設定
	g_mesh.nNumVertex = NUM_VERTEX;
	vertexWk[0].vtx = XMFLOAT3(-SIZE_X, 0.0f, SIZE_Z);
	vertexWk[1].vtx = XMFLOAT3(SIZE_X, 0.0f, SIZE_Z);
	vertexWk[2].vtx = XMFLOAT3(-SIZE_X, 0.0f, -SIZE_Z);
	vertexWk[3].vtx = XMFLOAT3(SIZE_X, 0.0f, -SIZE_Z);


	// ディフューズの設定
	vertexWk[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertexWk[1].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertexWk[2].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertexWk[3].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// 法線ベクトルの設定
	vertexWk[0].nor = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertexWk[1].nor = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertexWk[2].nor = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertexWk[3].nor = XMFLOAT3(0.0f, 1.0f, 0.0f);

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