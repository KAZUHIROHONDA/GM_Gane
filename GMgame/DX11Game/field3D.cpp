//=============================================================================
//
// 3D�t�B�[���h���� [field.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "AssimpModel.h"
#include "field3D.h"
#include "main.h"
#include "input.h"
#include "shadow.h"		//�e������ɂ͓����
#include "gauge.h"
#include "sound.h"
#include "MessageManager.h"
#include"fade.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_FIELD		"data/model/field/monu3.obj"
#define MODEL_TREE		"data/model/tree.fbx"

//�\���̒�`
typedef struct _tField3D
{
	XMFLOAT4X4		mtxWorld;		//���W�ϊ��s��

	XMFLOAT3		pos;			//���W
	XMFLOAT3		rot;			//��]
	XMFLOAT3		scl;			//�g��k��
				
}tField3D;

typedef struct _tTree
{
	XMFLOAT4X4		mtxWorld;		//���W�ϊ��s��

	XMFLOAT3		pos;			//���W
	XMFLOAT3		rot;			//��]
	XMFLOAT3		scl;			//�g��k��

}tTree;


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static CAssimpModel		g_model[2];				//���f���f�[�^�i�[�p
			
static tField3D			g_field[FIELD_MAX];	//�t�B�[���h
static tTree			g_tree[TREE_MAX];	//�c���[

//=============================================================================
// ����������
//=============================================================================
HRESULT InitField3D(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
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
		
	// ���[���h�}�g���b�N�X�̏�����
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

	// ���f���f�[�^�̓ǂݍ���
	hr = g_model[0].Load(pDevice, pDeviceContext,
		MODEL_FIELD); if (FAILED(hr)) return hr;
	hr = g_model[1].Load(pDevice, pDeviceContext,
		MODEL_TREE); if (FAILED(hr)) return hr;

	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void UninitField3D(void)
{
	// ���f���̉��
	g_model[0].Release();
	g_model[1].Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateField3D(void)
{

	for (int i = 0; i < FIELD_MAX; i++)
	{
		XMMATRIX mtxWorld, mtxRot, mtxScl,
			mtxTranslate;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_field[i].scl.x, g_field[i].scl.y, g_field[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);



		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(g_field[i].rot.x), XMConvertToRadians(g_field[i].rot.y + 180), XMConvertToRadians(g_field[i].rot.z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);


		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_field[i].pos.x, g_field[i].pos.y, g_field[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);


		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&g_field[i].mtxWorld, mtxWorld);
	}

	for (int i = 0; i < TREE_MAX; i++)
	{
		XMMATRIX mtxWorld, mtxRot, mtxScl,
			mtxTranslate;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_tree[i].scl.x, g_tree[i].scl.y, g_tree[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);



		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(g_tree[i].rot.x), XMConvertToRadians(g_tree[i].rot.y + 180), XMConvertToRadians(g_tree[i].rot.z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);


		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_tree[i].pos.x, g_tree[i].pos.y, g_tree[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);


		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&g_tree[i].mtxWorld, mtxWorld);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawField3D(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	for (int i = 0; i < FIELD_MAX; i++)
	{
		// ���f���\��
		g_model[0].Draw(pDeviceContext, g_field[i].mtxWorld);
	}

	for (int i = 0; i < TREE_MAX; i++)
	{
		// ���f���\��
		g_model[1].Draw(pDeviceContext, g_tree[i].mtxWorld);
	}
}