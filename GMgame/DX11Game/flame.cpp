//=============================================================================
//
// ������ [flame.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "flame.h"
#include "Shader.h"
#include "Texture.h"
#include "mesh.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_FLAME	L"data/texture/shadow000.jpg"	// �e�N�X�`��

#define M_DIFFUSE		XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR		XMFLOAT4(0.0f,0.0f,0.0f,0.0f)
#define M_AMBIENT		XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_EMISSIVE		XMFLOAT4(0.0f,0.0f,0.0f,0.0f)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct _tFlame
{
	int			nState;	// ���
	XMFLOAT3	pos;	// ���W
	XMFLOAT3	vel;	// ���x
	XMFLOAT4	col;	//�F
	float		fSize;	//�T�C�Y
	int			nLife;	//��������

}tFlame;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexFlame(ID3D11Device* pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static MESH		g_mesh;		// ���b�V�����
static MATERIAL	g_material;	// �}�e���A�����

static tFlame	g_flame[MAX_FLAME];	//���X�g�b�N

//=============================================================================
// ����������
//=============================================================================
HRESULT InitFlame(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr;
	HWND hWnd = GetMainWnd();

	// �}�e���A���̏����ݒ�
	g_material.Diffuse = M_DIFFUSE;
	g_material.Ambient = M_AMBIENT;
	g_material.Specular = M_SPECULAR;
	g_material.Power = 50.0f;
	g_material.Emissive = M_EMISSIVE;
	g_mesh.pMaterial = &g_material;

	// �e�N�X�`���̓ǂݍ���
	hr = CreateTextureFromFile(pDevice,
		TEXTURE_FLAME,
		&g_mesh.pTexture);
	if (FAILED(hr))
	{
		MessageBox(hWnd,
			_T("���e�N�X�`���ǂݍ��݃G���["),
			_T("Error"),
			MB_OK | MB_ICONSTOP);
		return hr;
	}
	XMStoreFloat4x4(&g_mesh.mtxTexture,
		XMMatrixIdentity());

	// ���[���h�}�g���N�X������
	XMStoreFloat4x4(&g_mesh.mtxWorld,
		XMMatrixIdentity());

	// ���_���̍쐬
	hr = MakeVertexFlame(pDevice);

	// �ϐ��̏�����
	for (int i = 0; i < MAX_FLAME; i++)
	{
		g_flame[i].nState = 0;
	}

	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void UninitFlame(void)
{
	// ���b�V�����
	ReleaseMesh(&g_mesh);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateFlame(void)
{

	// �X�V����
	tFlame* pFlame = g_flame;
	for (int i = 0; i < MAX_FLAME;
		++i, ++pFlame)
	{
		if (pFlame->nState == 0) continue;

		// ���x�̍X�V
		pFlame->vel.x *= 0.95f;	//����
		pFlame->vel.y *= 0.98f;
		pFlame->vel.z *= 0.95f;
		
		// �ʒu�̍X�V
		pFlame->pos.x += pFlame->vel.x;
		pFlame->pos.y += pFlame->vel.y;
		pFlame->pos.z += pFlame->vel.z;

		// ���ɍX�V��񂪂���΂����ŏ���
			// �T�C�Y�ɕύX������΂����ŕύX

			// �F�i�A���t�@�l�j��ύX����
		
		if (pFlame->nLife < 20)
		{
			pFlame->col.w -= 1.0f / 20;
		}

		// �������Ԕ���
		if (--pFlame->nLife <= 0)
		{
			pFlame->nState = 0;
			continue;
		}
			
	

		// ���ɍ폜����������΂����ŏ���
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawFlame(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// �����ݒ�
	SetBlendState(BS_ADDITIVE);

	XMFLOAT4X4 mView = GetCamera()->GetViewMatrix();
	tFlame* pFlame = g_flame;
	for (int i = 0; i < MAX_FLAME; ++i, ++pFlame)
	{	// �o�ĂȂ����͕`�悵�Ȃ�
		if (pFlame->nState == 0) continue;
		
		// �r���[�s��̉�]�����̓]�u�s���ݒ�
		g_mesh.mtxWorld._11 = mView._11;
		g_mesh.mtxWorld._12 = mView._21;
		g_mesh.mtxWorld._13 = mView._31;
		g_mesh.mtxWorld._21 = mView._12;
		g_mesh.mtxWorld._22 = mView._22;
		g_mesh.mtxWorld._23 = mView._32;
		g_mesh.mtxWorld._31 = mView._13;
		g_mesh.mtxWorld._32 = mView._23;
		g_mesh.mtxWorld._33 = mView._33;

		// �g��k��
		XMMATRIX mtxScale = XMMatrixScaling(pFlame->fSize,
			pFlame->fSize, pFlame->fSize);
		XMMATRIX mtxRot;
		mtxRot = XMLoadFloat4x4(&g_mesh.mtxWorld);
		mtxRot = XMMatrixMultiply(mtxRot, mtxScale);
		XMStoreFloat4x4(&g_mesh.mtxWorld, mtxRot);

		// �ʒu�𔽉f
		g_mesh.mtxWorld._41 = pFlame->pos.x;
		g_mesh.mtxWorld._42 = pFlame->pos.y;
		g_mesh.mtxWorld._43 = pFlame->pos.z;

		// �J���[�̐ݒ�
		g_material.Diffuse = pFlame->col;

		DrawMesh(pDeviceContext, &g_mesh);
	}
	// ���Ƃɖ߂�
	SetBlendState(BS_NONE);	
}

// ���̍쐬
int SetFlame(XMFLOAT3 pos, float size,int no)
{
	tFlame* pFlame = g_flame;
	for (int i = 0; i < MAX_FLAME;	++i, ++pFlame)
	{	// �o�����Ă��鉊�̓X���[
		if (pFlame->nState > 0) continue;

		pFlame->pos = pos;
		// �����ݒ�
		pFlame->vel.x = (rand() % 101 - 50) / 50.0f;// 0.0f;
		pFlame->vel.y = 1.0f+(rand() % 101 - 50) / 50.0f;//1.0f;
		pFlame->vel.z = (rand() % 101 - 50) / 50.0f;//0.0f;
		pFlame->nLife = 180;
		pFlame->nState = 1;
		pFlame->fSize = size;

		return i;	// �ԍ���Ԃ�
	}

	return -1;	// ������Ȃ�����
}


HRESULT MakeVertexFlame(ID3D11Device* pDevice)
{
	VERTEX_3D vertexWk[NUM_VERTEX]; //��Ɨp���_���
	int		  indexWk[NUM_VERTEX];	//��Ɨp�C���f�b�N�X

	// ���_���W�̐ݒ�
	g_mesh.nNumVertex = NUM_VERTEX;
	vertexWk[0].vtx = XMFLOAT3(-0.5f, 0.5f, 0.0f);
	vertexWk[1].vtx = XMFLOAT3( 0.5f, 0.5f, 0.0f);
	vertexWk[2].vtx = XMFLOAT3(-0.5f,-0.5f, 0.0f);
	vertexWk[3].vtx = XMFLOAT3( 0.5f,-0.5f, 0.0f);

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
	vertexWk[1].tex = XMFLOAT2(1.0f, 0.0f);
	vertexWk[2].tex = XMFLOAT2(0.0f, 1.0f);
	vertexWk[3].tex = XMFLOAT2(1.0f, 1.0f);

	// �C���f�b�N�X�z��̐ݒ�
	g_mesh.nNumIndex = NUM_VERTEX;
	indexWk[0] = 0;
	indexWk[1] = 1;
	indexWk[2] = 2;
	indexWk[3] = 3;

	return MakeMeshVertex(pDevice, &g_mesh, vertexWk, indexWk);
}

