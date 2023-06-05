//=============================================================================
//
// �e���� [bullet.cpp]
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
// �}�N����`
//*****************************************************************************
#define TEXTURE_BULLET	L"data/texture/bullet000.png"	// �e�N�X�`��

#define M_DIFFUSE		XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR		XMFLOAT4(0.0f,0.0f,0.0f,0.0f)
#define M_AMBIENT		XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_EMISSIVE		XMFLOAT4(0.0f,0.0f,0.0f,0.0f)

#define BULLET_SPEED (1.0f)		//�e�̑��x
#define BULLET_RADIUS (10.0f)	//�e�̔��a


//*****************************************************************************
// �\���̒�`
//*****************************************************************************

typedef struct _tBullet
{
	XMFLOAT3		pos;		//���W
	XMFLOAT3		vel;		//���x
	int				nState;		//���
	int				nShadowIdx; //�e�ԍ�
}tBullet;

int					bCnt;		//�f�X�g���C�֐��̃J�E���g


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexBullet(ID3D11Device* pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static MESH		g_mesh;		// ���b�V�����
static MATERIAL	g_material;	// �}�e���A�����

static tBullet g_bullet[BULLET_MAX];//�e�X�g�b�N

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBullet(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr;
	HWND hWnd = GetMainWnd();

	bCnt = 0;
	

	// �}�e���A���̏����ݒ�
	g_material.Diffuse = M_DIFFUSE;
	g_material.Ambient = M_AMBIENT;
	g_material.Specular = M_SPECULAR;
	g_material.Power = 50.0f;
	g_material.Emissive = M_EMISSIVE;
	g_mesh.pMaterial = &g_material;

	// �e�N�X�`���̓ǂݍ���
	hr = CreateTextureFromFile(pDevice,
		TEXTURE_BULLET,
		&g_mesh.pTexture);
	if (FAILED(hr))
	{
		MessageBox(hWnd,
			_T("�e�e�N�X�`���ǂݍ��݃G���["),
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
	hr = MakeVertexBullet(pDevice);
	
	for (int i = 0; i < BULLET_MAX; i++)
	{
		g_bullet[i].nState = 0;
		g_bullet[i].nShadowIdx = -1;
	}
	
	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBullet(void)
{
	tBullet* pBullet = g_bullet;
	for (int i = 0; i < BULLET_MAX;
		++i, ++pBullet)
	{
		if (pBullet->nState == 0) continue;
		pBullet->nState = 0;
		// �e�Ƃ̊֘A��؂�
		ReleaseShadow(pBullet->nShadowIdx);
		pBullet->nShadowIdx = -1;
	}

	// ���b�V�����
	ReleaseMesh(&g_mesh);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBullet(void)
{

	// �X�V����
	tBullet* pBullet = g_bullet;
	for (int i = 0; i < BULLET_MAX;++i, ++pBullet)
	{
		if (pBullet->nState == 0) continue;

		//�d��
		//pBullet->vel.y += -0.098f;
		

		// �ʒu�̍X�V
		pBullet->pos.x += pBullet->vel.x;
		pBullet->pos.y += pBullet->vel.y;
		pBullet->pos.z += pBullet->vel.z;


		// ��ʊO����
		if (pBullet->pos.x < -200.0f ||
			pBullet->pos.x > 200.0f ||
			pBullet->pos.z < -200.0f ||
			pBullet->pos.z > 200.0f ||
			pBullet->pos.y < 0.0f ||
			pBullet->pos.y > 1000.0f)
		{
			// �e�Ƃ̊֘A��؂�
			ReleaseShadow(pBullet->nShadowIdx);
			pBullet->nShadowIdx = -1;
			
			DestroyBullet(i);

			continue;
		}
		// �ۉe�ړ�
		MoveShadow(pBullet->nShadowIdx,
			pBullet->pos);
	}

	
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBullet(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	// �����ݒ�
	SetBlendState(BS_ALPHABLEND);
	
	//�r���[�}�g���b�N�X�擾
	XMFLOAT4X4 mView = GetCamera()->GetViewMatrix();
	tBullet* pBullet = g_bullet;
	for (int i = 0; i < BULLET_MAX; ++i, ++pBullet)
	{
		//���o���̒e�͕`�悵�Ȃ�
		if (pBullet->nState == 0) continue;

		//�r���[�s��̉�]�����̓]�u�s���ݒ�
		g_mesh.mtxWorld._11 = mView._11;
		g_mesh.mtxWorld._12 = mView._21;
		g_mesh.mtxWorld._13 = mView._31;
		g_mesh.mtxWorld._21 = mView._12;
		g_mesh.mtxWorld._22 = mView._22;
		g_mesh.mtxWorld._23 = mView._32;
		g_mesh.mtxWorld._31 = mView._13;
		g_mesh.mtxWorld._32 = mView._23;
		g_mesh.mtxWorld._33 = mView._33;

		//�g��k��
		XMMATRIX mtxScale = XMMatrixScaling(BULLET_RADIUS, BULLET_RADIUS, BULLET_RADIUS);
		XMMATRIX mtxRot;
		mtxRot = XMLoadFloat4x4(&g_mesh.mtxWorld);
		mtxRot = XMMatrixMultiply(mtxRot, mtxScale);
		XMStoreFloat4x4(&g_mesh.mtxWorld, mtxRot);

		//�ʒu�𔽉f
		g_mesh.mtxWorld._41 = pBullet->pos.x;
		g_mesh.mtxWorld._42 = pBullet->pos.y;
		g_mesh.mtxWorld._43 = pBullet->pos.z;

		DrawMesh(pDeviceContext, &g_mesh);

	}
	
	
	// ���Ƃɖ߂�
	SetBlendState(BS_NONE);	
}

// �e�̍쐬
int FireBullet(XMFLOAT3 pos, XMFLOAT3 dir,int no)
{
	tBullet* pBullet = g_bullet;
	for (int i = 0; i < BULLET_MAX;	++i, ++pBullet)
	{	// �o�����Ă���e�̓X���[
		if (pBullet->nState > 0) continue;

		pBullet->pos = pos;
		pBullet->vel.x = dir.x * BULLET_SPEED * no;
		pBullet->vel.y = dir.y * BULLET_SPEED * no;
		pBullet->vel.z = dir.z * BULLET_SPEED * no;
		pBullet->nState = 1;	// �o��
		pBullet->nShadowIdx =
			CreateShadow(pBullet->pos, BULLET_RADIUS);
		return i;	// �ԍ���Ԃ�
	}

	return -1;	// ������Ȃ�����
}

HRESULT MakeVertexBullet(ID3D11Device* pDevice)
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

//�����m�F
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
	// �e�Ƃ̊֘A��؂�
	ReleaseShadow(g_bullet[no].nShadowIdx);
	g_bullet[no].nShadowIdx = -1;

	bCnt++;


}

//���W�擾
XMFLOAT3 GetBulletPos(int no)
{
	if (no < 0 || no >= BULLET_MAX)
		return XMFLOAT3(0.0f, 0.0f,0.0f);
	return g_bullet[no].pos;
}

//�T�C�Y�擾
XMFLOAT3 GetBulletSize(int no)
{
	if (no < 0 || no >= BULLET_MAX)
		return XMFLOAT3(0.0f, 0.0f,0.0f);
	return XMFLOAT3(BULLET_RADIUS, BULLET_RADIUS, BULLET_RADIUS);
}
