//=============================================================================
//
// �Ǐ��� [wall.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "wall.h"
#include "Shader.h"
#include "Texture.h"
#include "mesh.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WALL	L"data/texture/wall.png"

#define M_DIFFUSE		XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR		XMFLOAT4(0.0f,0.0f,0.0f,0.0f)
#define M_AMBIENT		XMFLOAT4(0.3f,0.3f,0.3f,1.0f)
#define M_EMISSIVE		XMFLOAT4(0.0f,0.0f,0.0f,0.0f)

#define SIZE_X			(200.0f)	// �ǂ̃T�C�Y��
#define SIZE_Y			(20.0f)		// �ǂ̃T�C�Y���s��

#define WALL_MAX		(10)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct _tWall
{
	int					nState = 0;
	MESH				mesh;		// ���b�V�����
	MATERIAL			material;	// �}�e���A�����
}tWall;
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexWall(ID3D11Device* pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
tWall g_wall[WALL_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitWall(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr;
	HWND hWnd = GetMainWnd();

	

	for (int i = 0; i < WALL_MAX; i++)
	{
		g_wall[i].nState = 0;

		// �ʒu��]�g�嗦�̏�����
		g_wall[i].mesh.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_wall[i].mesh.rot = XMFLOAT3(0.0f, 0.0f, 0.0f);

		// �}�e���A���̏����ݒ�
		g_wall[i].material.Diffuse = M_DIFFUSE;
		g_wall[i].material.Ambient = M_AMBIENT;
		g_wall[i].material.Specular = M_SPECULAR;
		g_wall[i].material.Power = 1.0f;
		g_wall[i].material.Emissive = M_EMISSIVE;
		g_wall[i].mesh.pMaterial = &g_wall[i].material;

		XMStoreFloat4x4(&g_wall[i].mesh.mtxTexture,
			XMMatrixIdentity());

	//	g_wall[i].mesh.pTexture = g_wall[0].mesh.pTexture;

		// �e�N�X�`���̓ǂݍ���
		hr = CreateTextureFromFile(pDevice,
			TEXTURE_WALL,
			&g_wall[i].mesh.pTexture);
		if (FAILED(hr))
		{
			MessageBox(hWnd,
				_T("�ǃe�N�X�`���ǂݍ��݃G���["),
				_T("Error"),
				MB_OK | MB_ICONSTOP);
			return hr;
		}
	}

	// ���_���̍쐬
	hr = MakeVertexWall(pDevice);

	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void UninitWall(void)
{
	// ���b�V�����
	for (int i = 0; i < WALL_MAX; i++)
		ReleaseMesh(&g_wall[i].mesh);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateWall(void)
{
	// �X�V����
	
	

	// ���b�V���X�V
	for (int i = 0; i < WALL_MAX; i++)
		UpdateMesh(&g_wall[i].mesh);
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawWall(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	SetBlendState(BS_ALPHABLEND);
	for (int i = 0; i < WALL_MAX; i++)
	{
		if (g_wall[i].nState == 0) continue;
		DrawMesh(pDeviceContext, &g_wall[i].mesh);	// ���b�V���̕`��
	}
	SetBlendState(BS_NONE);
}

HRESULT MakeVertexWall(ID3D11Device* pDevice)
{
	HRESULT hr;

	for (int i = 0; i < WALL_MAX; i++)
	{

		VERTEX_3D vertexWk[NUM_VERTEX]; //��Ɨp���_���
		int		  indexWk[NUM_VERTEX];	//��Ɨp�C���f�b�N�X

		// ���_���W�̐ݒ�
		g_wall[i].mesh.nNumVertex = NUM_VERTEX;
		vertexWk[0].vtx = XMFLOAT3(-SIZE_X, +SIZE_Y, 0.0f);
		vertexWk[1].vtx = XMFLOAT3(+SIZE_X, +SIZE_Y, 0.0f);
		vertexWk[2].vtx = XMFLOAT3(-SIZE_X, -SIZE_Y, 0.0f);
		vertexWk[3].vtx = XMFLOAT3(+SIZE_X, -SIZE_Y, 0.0f);

		// �f�B�t���[�Y�̐ݒ�
		vertexWk[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertexWk[1].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertexWk[2].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertexWk[3].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		// �@���x�N�g���̐ݒ�
		vertexWk[0].nor = XMFLOAT3(0.0f, 0.0f,-1.0f);
		vertexWk[1].nor = XMFLOAT3(0.0f, 0.0f,-1.0f);
		vertexWk[2].nor = XMFLOAT3(0.0f, 0.0f,-1.0f);
		vertexWk[3].nor = XMFLOAT3(0.0f, 0.0f,-1.0f);

		// �e�N�X�`�����W�̐ݒ�
		vertexWk[0].tex = XMFLOAT2(0.0f, 0.0f);
		vertexWk[1].tex = XMFLOAT2(3.0f, 0.0f);
		vertexWk[2].tex = XMFLOAT2(0.0f, 1.0f);
		vertexWk[3].tex = XMFLOAT2(3.0f, 1.0f);

		// �C���f�b�N�X�z��̐ݒ�
		g_wall[i].mesh.nNumIndex = NUM_VERTEX;
		indexWk[0] = 0;
		indexWk[1] = 1;
		indexWk[2] = 2;
		indexWk[3] = 3;

		hr = MakeMeshVertex(pDevice, &g_wall[i].mesh, vertexWk, indexWk);
		if (FAILED(hr)) return hr;
	}
	return hr;
}

void SetWall(XMFLOAT3 pos, float rotY)
{
	for (int i = 0; i < WALL_MAX; i++)
	{
		if (g_wall[i].nState != 0) continue;
		g_wall[i].mesh.pos = pos;
		g_wall[i].mesh.rot.y = rotY;
		g_wall[i].nState = 1;
		break;
	}
}

// ���
void ClearWall()
{
	for (int i = 0; i < WALL_MAX; i++)
		g_wall[i].nState = 0;
}


