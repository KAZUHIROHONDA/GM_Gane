//=============================================================================
//
// �G���� [enemy.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "AssimpModel.h"
#include "enemy.h"
#include "main.h"
#include "shadow.h"		//�e������ɂ͓����
#include "flame.h"
#include "bullet.h"
#include"input.h"
#include"enemyhp.h"
#include"fade.h"
#include "Egauge.h"
#include "sceneTitle.h"
#include "sceneGame.h"
#include "fade.h"

//*****************************************************************************
// �}�N����`
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


//�\���̒�`
typedef struct _tEnemy
{
	XMFLOAT4X4		mtxWorld;		//���W�ϊ��s��

	XMFLOAT3		pos;			//���W
	XMFLOAT3		rot;			//��]
	XMFLOAT3		scl;			//�g��k��
	XMFLOAT3		vel;			//���x

	int				nPhase;			//�i�K
	int				nState;			//���(0:���g�p�@1:�ʏ�)
	int				Tipe;
	int				nStopTime;		//����s�\����

	int				nHP;
	int				nEGauge;
	int				nShadowIdx;		//�e�̔ԍ�

}tEnemy;

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static CAssimpModel		g_model[4];				//���f���f�[�^�i�[�p
static tEnemy			g_enemy[ENEMY_MAX];	//�v���C���[
static tEnemy			g_enemyHD[ENEMY_MAX];
static tEnemy			g_enemyAM[ENEMY_MAX];
static tEnemy			g_enemyLG[ENEMY_MAX];


static int			g_nECnt1;//�s���J�E���g1�R��
static int			g_nECnt2;//�s���J�E���g2�R��
static int			g_nECnt3;//�s���J�E���g3�R��
static int			g_nECnt4;//�s���J�E���g4�R��



static int			g_nECnt5;//�s���J�E���g5�R��
static int			g_nECnt6;//�s���J�E���g6�R��
static int			g_nECnt7;//�s���J�E���g6�R��
static int			g_nECnt8;//�s���J�E���g6�R��
static int			g_nECnt9;//�s���J�E���g6�R��
static int			g_nECnt10;//�s���J�E���g6�R��
static int			g_nECnt11;//�s���J�E���g6�R��
static int			g_nECnt12;//�s���J�E���g6�R��
static bool				g_atama = true;//false;

static bool			g_Eaction;
static bool			g_Eaction2;
static bool			g_Eaction3;
static bool			g_Eaction4;
static bool			g_Eaction5;
static bool			g_Eaction6;
static bool			g_Eaction7;
static bool			g_Eaction8;
static bool			g_Eaction9;
static bool			g_Eaction10;
static bool			g_Eaction11;


int					bfCnt;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		g_enemy[i].pos = XMFLOAT3(0.0f, 20.0f,100.0f);//z100
		g_enemy[i].rot = XMFLOAT3(0.0f, 180.0f, 0.0f);
		g_enemy[i].scl = XMFLOAT3(15.0f, 15.0f, 15.0f);
		g_enemy[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);

		//������
		g_enemy[i].nPhase = 0;
		g_enemy[i].nState = 1;		//�ŏ��͒ʏ�
		g_enemy[i].Tipe = 0;

		g_enemy[i].nHP = GetEnemy()->GetHP();
		g_enemy[i].nEGauge = 100;
		g_enemy[i].nShadowIdx = -1;

		

		g_Eaction = false;
		g_Eaction2 = false;
		g_Eaction3 = false;
		g_Eaction4 = false;
		g_Eaction5 = false;
		g_Eaction6 = false;
		g_Eaction7 = false;
		g_Eaction8 = false;
		g_Eaction9 = false;
		g_Eaction10 = false;
		g_Eaction11 = false;

		g_nECnt1 = 0;					//
		g_nECnt2 = 0;					//
		g_nECnt3 = 0;
		g_nECnt4 = 0;
		g_nECnt5 = 0;
		g_nECnt6 = 0;
		g_nECnt7 = 0;
		g_nECnt8 = 0;
		g_nECnt9 = 0;
		g_nECnt10 = 0;
		g_nECnt11 = 0;
		g_nECnt12 = 0;


		//�� wani
		g_enemyHD[i].pos = XMFLOAT3(0.0f, 0.0f, -1.0f);//z-3
		g_enemyHD[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyHD[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_enemyHD[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyHD[i].nPhase = 0;
		g_enemyHD[i].nState = 1;	// puro�؂�͍ŏ�����o�ꂵ�Ă���
		g_enemyHD[i].nShadowIdx = -1;

		//panda
		//g_enemyHD[i].pos = XMFLOAT3(0.0f, 0.0f, -1.0f);
		//g_enemyHD[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		//g_enemyHD[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		//g_enemyHD[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		//g_enemyHD[i].nPhase = 0;
		//g_enemyHD[i].nState = 1;	// puro�؂�͍ŏ�����o�ꂵ�Ă���
		//g_enemyHD[i].nShadowIdx = -1;

		////�r-0.8f
		g_enemyAM[i].pos = XMFLOAT3(0.0f, -0.5f, 0.0f);//-0.8,-1
		g_enemyAM[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyAM[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_enemyAM[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyAM[i].nPhase = 0;
		g_enemyAM[i].nState = 1;	// puro�؂�͍ŏ�����o�ꂵ�Ă���
		g_enemyAM[i].nShadowIdx = -1;

		//�r
		//g_enemyAM[i].pos = XMFLOAT3(0.0f, -0.3f, -0.7f);
		//g_enemyAM[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		//g_enemyAM[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		//g_enemyAM[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		//g_enemyAM[i].nPhase = 0;
		//g_enemyAM[i].nState = 1;	// puro�؂�͍ŏ�����o�ꂵ�Ă���
		//g_enemyAM[i].nShadowIdx = -1;

		//��
		g_enemyLG[i].pos = XMFLOAT3(0.0f, -0.5f, 1.5f);
		g_enemyLG[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyLG[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_enemyLG[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyLG[i].nPhase = 0;
		g_enemyLG[i].nState = 1;	// puro�؂�͍ŏ�����o�ꂵ�Ă���
		g_enemyLG[i].nShadowIdx = -1;

		////��
		//g_enemyLG[i].pos = XMFLOAT3(0.0f, -0.3f, 0.5f);
		//g_enemyLG[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		//g_enemyLG[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		//g_enemyLG[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		//g_enemyLG[i].nPhase = 0;
		//g_enemyLG[i].nState = 1;	// puro�؂�͍ŏ�����o�ꂵ�Ă���
		//g_enemyLG[i].nShadowIdx = -1;

		

		g_enemy[i].nStopTime = 0; //�ŏ��͓�����

		g_enemy[i].nShadowIdx = CreateShadow(g_enemy[i].pos, 20.0f);
	}
	
	bfCnt = 0;
	g_Eaction = false;
	g_Eaction2 = false;
	g_Eaction3 = false;

	g_nECnt1 = 0;					//
	g_nECnt2 = 0;					//
	g_nECnt3 = 0;
	g_nECnt4 = 0;

	// ���[���h�}�g���b�N�X�̏�����
	XMMATRIX mtxWorld;
	mtxWorld = XMMatrixIdentity();

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		XMStoreFloat4x4(&g_enemy[i].mtxWorld, mtxWorld);
		XMStoreFloat4x4(&g_enemyHD[i].mtxWorld, mtxWorld);
		XMStoreFloat4x4(&g_enemyAM[i].mtxWorld, mtxWorld);
		XMStoreFloat4x4(&g_enemyLG[i].mtxWorld, mtxWorld);

	}

	// ���f���f�[�^�̓ǂݍ���
	hr = g_model[0].Load(pDevice, pDeviceContext,
		MODEL_ENEMY); if (FAILED(hr)) return hr;

	hr = g_model[1].Load(pDevice, pDeviceContext,
		MODEL_ENEMY1); if (FAILED(hr)) return hr;

	hr = g_model[2].Load(pDevice, pDeviceContext,
		MODEL_ENEMY2); if (FAILED(hr)) return hr;

	hr = g_model[3].Load(pDevice, pDeviceContext,
		MODEL_ENEMY3); if (FAILED(hr)) return hr;

	return hr;

}

HRESULT InitSelectEnemy(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		g_enemy[i].pos = XMFLOAT3(0.0f, 20.0f, 100.0f);//z100
		g_enemy[i].rot = XMFLOAT3(0.0f, 180.0f, 0.0f);
		g_enemy[i].scl = XMFLOAT3(15.0f, 15.0f, 15.0f);
		g_enemy[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);

		//������
		g_enemy[i].nPhase = 0;
		g_enemy[i].nState = 0;		//�ŏ��͒ʏ�
		g_enemy[i].Tipe = 0;

		g_enemy[i].nHP = GetEnemy()->GetHP();
		g_enemy[i].nEGauge = 100;
		g_enemy[i].nShadowIdx = -1;

		//�� wani
		g_enemyHD[i].pos = XMFLOAT3(0.0f, 0.0f, -1.0f);//z-3
		g_enemyHD[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyHD[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_enemyHD[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyHD[i].nPhase = 0;
		g_enemyHD[i].nState = 1;	// puro�؂�͍ŏ�����o�ꂵ�Ă���
		g_enemyHD[i].nShadowIdx = -1;

		////�r-0.8f
		g_enemyAM[i].pos = XMFLOAT3(0.0f, -0.5f, 0.0f);//-0.8,-1
		g_enemyAM[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyAM[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_enemyAM[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyAM[i].nPhase = 0;
		g_enemyAM[i].nState = 1;	// puro�؂�͍ŏ�����o�ꂵ�Ă���
		g_enemyAM[i].nShadowIdx = -1;

		//��
		g_enemyLG[i].pos = XMFLOAT3(0.0f, -0.5f, 1.5f);
		g_enemyLG[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyLG[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_enemyLG[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyLG[i].nPhase = 0;
		g_enemyLG[i].nState = 1;	// puro�؂�͍ŏ�����o�ꂵ�Ă���
		g_enemyLG[i].nShadowIdx = -1;


		g_enemy[i].nStopTime = 0; //�ŏ��͓�����

		g_enemy[i].nShadowIdx = CreateShadow(g_enemy[i].pos, 20.0f);
	}

	bfCnt = 0;
	g_Eaction = false;
	g_Eaction2 = false;
	g_Eaction3 = false;

	g_nECnt1 = 0;					//
	g_nECnt2 = 0;					//
	g_nECnt3 = 0;
	g_nECnt4 = 0;

	// ���[���h�}�g���b�N�X�̏�����
	XMMATRIX mtxWorld;
	mtxWorld = XMMatrixIdentity();

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		XMStoreFloat4x4(&g_enemy[i].mtxWorld, mtxWorld);
		XMStoreFloat4x4(&g_enemyHD[i].mtxWorld, mtxWorld);
		XMStoreFloat4x4(&g_enemyAM[i].mtxWorld, mtxWorld);
		XMStoreFloat4x4(&g_enemyLG[i].mtxWorld, mtxWorld);

	}

	// ���f���f�[�^�̓ǂݍ���
	hr = g_model[0].Load(pDevice, pDeviceContext,
		MODEL_ENEMY); if (FAILED(hr)) return hr;

	hr = g_model[1].Load(pDevice, pDeviceContext,
		MODEL_ENEMY1); if (FAILED(hr)) return hr;

	hr = g_model[2].Load(pDevice, pDeviceContext,
		MODEL_ENEMY2); if (FAILED(hr)) return hr;

	hr = g_model[3].Load(pDevice, pDeviceContext,
		MODEL_ENEMY3); if (FAILED(hr)) return hr;

	return hr;

}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy(void)
{
	//�e�̉��
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		ReleaseShadow(g_enemy[i].nShadowIdx);
		ResetEPos(i);
	}



	// ���f���̉��
	g_model[0].Release();
	g_model[1].Release();
	g_model[2].Release();
	g_model[3].Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy(void)
{
	// �v���C���[�̓������L�q����Ȃ炱��
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		//���g�p
		if (g_enemy[i].nState == 0) continue;


		//����s�\����
		if (g_enemy[i].nStopTime > 0)
		{
			g_enemy[i].nStopTime--;
		}
		if (g_enemy[i].nStopTime == 0)
		{
			if (GetKeyTrigger(VK_8))
			{
				g_Eaction = true;
			}
			EAction(g_Eaction);

			if (GetKeyTrigger(VK_6))
			{
				g_Eaction2 = true;
			}
			EAction2(g_Eaction2);

			if (GetKeyTrigger(VK_7))
			{
				g_Eaction3 = true;
			}
			EAction3(g_Eaction3);

			if (GetKeyTrigger(VK_9))
			{
				g_Eaction4 = true;
			}
			EAction4(g_Eaction4);


			if (GetKeyTrigger(VK_0))
			{
				g_Eaction5 = true;
			}
			EAction5(g_Eaction5);

			if (GetKeyTrigger(VK_1))
			{
				g_Eaction6 = true;
			}
			EAction6(g_Eaction6);

			if (GetKeyTrigger(VK_2))
			{
				g_Eaction7 = true;
			}
			EAction7(g_Eaction7);

			if (GetKeyTrigger(VK_3))
			{
				g_Eaction8 = true;
			}
			EAction8(g_Eaction8);

			if (GetKeyTrigger(VK_4))
			{
				g_Eaction9 = true;
			}
			EAction9(g_Eaction9);

			if (GetKeyTrigger(VK_5))
			{
				g_Eaction10 = true;
			}
			EAction10(g_Eaction10);


			if (GetKeyTrigger(VK_M))
			{
				g_Eaction11 = true;
			}
			EAction11(g_Eaction11);

		}
		
		//g_nECnt4++;
		//if (GetKeyPress(VK_0))//�����A�j���[�V����
		//{
		//	g_enemy[i].rot.z = 180;
		//	if (g_nECnt4 >= 20)
		//	{
		//		g_enemyHD[i].pos.z -= 0.05f;
		//		g_enemyHD[i].rot.z -= 1.0f;
		//		g_enemyHD[i].rot.y -= 1.0f;

		//		g_enemyAM[i].pos.x -= 0.05f;
		//		g_enemyAM[i].rot.z -= 1.0f;
		//		g_enemyAM[i].rot.y -= 1.0f;

		//		g_enemyLG[i].pos.z += 0.05f;
		//		g_enemyLG[i].rot.z -= 1.0f;
		//		g_enemyLG[i].rot.y -= 1.0f;
		//	}

		//}




		//�Q�[�W�̓����̏���
		if (g_enemy[i].nEGauge >= (GetEnemy()->GetHP() * 100 / g_enemy[i].nHP))
		{
			g_enemy[i].nEGauge--;
		}

		if (GetKeyTrigger(VK_F))
		{
			DamageEnemy(50);
		}


		XMMATRIX mtxWorld, mtxRot, mtxScl,
			mtxTranslate;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_enemy[i].scl.x, g_enemy[i].scl.y, g_enemy[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);



		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(g_enemy[i].rot.x), XMConvertToRadians(g_enemy[i].rot.y + 180), XMConvertToRadians(g_enemy[i].rot.z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);


		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_enemy[i].pos.x, g_enemy[i].pos.y, g_enemy[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);


		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&g_enemy[i].mtxWorld, mtxWorld);

		//�e�̈ړ�
		MoveShadow(g_enemy[i].nShadowIdx, g_enemy[i].pos);


		//************************************************:
		//��

		XMMATRIX mtxWorldHD;
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorldHD = XMMatrixIdentity();
		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_enemyHD[i].scl.x, g_enemyHD[i].scl.y, g_enemyHD[i].scl.z);
		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxScl);
		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(g_enemyHD[i].rot.x),
			XMConvertToRadians(g_enemyHD[i].rot.y),//+180
			XMConvertToRadians(g_enemyHD[i].rot.z)
		);
		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_enemyHD[i].pos.x, g_enemyHD[i].pos.y, g_enemyHD[i].pos.z);

		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxTranslate);

		//�e�̍s����|����
		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxWorld);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&g_enemyHD[i].mtxWorld, mtxWorldHD);


		//************************************************:
		//�r

		XMMATRIX mtxWorldAM;
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorldAM = XMMatrixIdentity();
		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_enemyAM[i].scl.x, g_enemyAM[i].scl.y, g_enemyAM[i].scl.z);
		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxScl);
		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(g_enemyAM[i].rot.x),
			XMConvertToRadians(g_enemyAM[i].rot.y),//+180
			XMConvertToRadians(g_enemyAM[i].rot.z)
		);
		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_enemyAM[i].pos.x, g_enemyAM[i].pos.y, g_enemyAM[i].pos.z);

		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxTranslate);

		//�e�̍s����|����
		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxWorld);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&g_enemyAM[i].mtxWorld, mtxWorldAM);

		//************************************************:
		//��

		XMMATRIX mtxWorldLG;
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorldLG = XMMatrixIdentity();
		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_enemyLG[i].scl.x, g_enemyLG[i].scl.y, g_enemyLG[i].scl.z);
		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxScl);
		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(g_enemyLG[i].rot.x),
			XMConvertToRadians(g_enemyLG[i].rot.y),//+180
			XMConvertToRadians(g_enemyLG[i].rot.z)
		);
		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_enemyLG[i].pos.x, g_enemyLG[i].pos.y, g_enemyLG[i].pos.z);

		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxTranslate);

		//�e�̍s����|����
		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxWorld);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&g_enemyLG[i].mtxWorld, mtxWorldLG);
	}


}

void UpdateSelectEnemy(void)
{
	// �v���C���[�̓������L�q����Ȃ炱��
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		//���g�p
		if (g_enemy[i].nState == 0) continue;

		float hitLength;

		//�ʒu�Ɣ��a���w��
		hitLength = ECheckCollisionRay(XMVectorSet(g_enemy[i].pos.x, g_enemy[i].pos.y, g_enemy[i].pos.z, 0.0f), 10.0f);
		if (hitLength < 9999.9f)
		{
			g_enemy[i].rot.y += 1.0f;
			if (GetMouseTrigger(0))
			{
				//�A�N�V�������ꂽ��
				StartFade(SCENE_GAME);

			}
		}
		else
		{
			g_enemy[i].rot = XMFLOAT3(0, 90, 0);
		}

		XMMATRIX mtxWorld, mtxRot, mtxScl,
			mtxTranslate;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_enemy[i].scl.x, g_enemy[i].scl.y, g_enemy[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);



		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(g_enemy[i].rot.x), XMConvertToRadians(g_enemy[i].rot.y + 180), XMConvertToRadians(g_enemy[i].rot.z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);


		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_enemy[i].pos.x, g_enemy[i].pos.y, g_enemy[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);


		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&g_enemy[i].mtxWorld, mtxWorld);

		//�e�̈ړ�
		MoveShadow(g_enemy[i].nShadowIdx, g_enemy[i].pos);


		//************************************************:
		//��

		XMMATRIX mtxWorldHD;
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorldHD = XMMatrixIdentity();
		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_enemyHD[i].scl.x, g_enemyHD[i].scl.y, g_enemyHD[i].scl.z);
		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxScl);
		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(g_enemyHD[i].rot.x),
			XMConvertToRadians(g_enemyHD[i].rot.y),//+180
			XMConvertToRadians(g_enemyHD[i].rot.z)
		);
		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_enemyHD[i].pos.x, g_enemyHD[i].pos.y, g_enemyHD[i].pos.z);

		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxTranslate);

		//�e�̍s����|����
		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxWorld);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&g_enemyHD[i].mtxWorld, mtxWorldHD);


		//************************************************:
		//�r

		XMMATRIX mtxWorldAM;
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorldAM = XMMatrixIdentity();
		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_enemyAM[i].scl.x, g_enemyAM[i].scl.y, g_enemyAM[i].scl.z);
		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxScl);
		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(g_enemyAM[i].rot.x),
			XMConvertToRadians(g_enemyAM[i].rot.y),//+180
			XMConvertToRadians(g_enemyAM[i].rot.z)
		);
		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_enemyAM[i].pos.x, g_enemyAM[i].pos.y, g_enemyAM[i].pos.z);

		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxTranslate);

		//�e�̍s����|����
		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxWorld);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&g_enemyAM[i].mtxWorld, mtxWorldAM);

		//************************************************:
		//��

		XMMATRIX mtxWorldLG;
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorldLG = XMMatrixIdentity();
		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_enemyLG[i].scl.x, g_enemyLG[i].scl.y, g_enemyLG[i].scl.z);
		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxScl);
		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(g_enemyLG[i].rot.x),
			XMConvertToRadians(g_enemyLG[i].rot.y),//+180
			XMConvertToRadians(g_enemyLG[i].rot.z)
		);
		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_enemyLG[i].pos.x, g_enemyLG[i].pos.y, g_enemyLG[i].pos.z);

		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxTranslate);

		//�e�̍s����|����
		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxWorld);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&g_enemyLG[i].mtxWorld, mtxWorldLG);
	}


}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		//���g�p
		if (g_enemy[i].nState == 0) continue;

		// ���f���\��
		g_model[0].Draw(pDeviceContext, g_enemy[i].mtxWorld);
		g_model[1].Draw(pDeviceContext, g_enemyHD[i].mtxWorld);
		g_model[2].Draw(pDeviceContext, g_enemyAM[i].mtxWorld);
		g_model[3].Draw(pDeviceContext, g_enemyLG[i].mtxWorld);

	}
}

//�����m�F
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
	// �e�Ƃ̊֘A��؂�
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

//���W�擾
XMFLOAT3 GetEnemyPos(int no)
{
	if (no < 0 || no >= ENEMY_MAX)
		return XMFLOAT3(0.0f, 0.0f, 0.0f);
	return g_enemy[no].pos;
}

//�T�C�Y�擾
XMFLOAT3 GetEnemySize(int no)
{
	if (no < 0 || no >= ENEMY_MAX)
		return XMFLOAT3(0,0,0);
	//return XMFLOAT3(10.0f,10.0f,10.0f);
	XMFLOAT3 box = g_model[0].GetBBox();
	return XMFLOAT3(box.x / 2 * g_enemy[no].scl.x, box.y / 2 * g_enemy[no].scl.y, box.z / 2 * g_enemy[no].scl.z);
}

// �G�̏o��
int SetEnemy(XMFLOAT3 pos, XMFLOAT3 rot)
{
	tEnemy* pEnemy = g_enemy;
	for (int i = 0; i < ENEMY_MAX; ++i, ++pEnemy)
	{	// �o�����Ă���G�̓X���[
		if (pEnemy->nState > 0) continue;

		pEnemy->pos = pos;

		g_enemy[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemy[i].Tipe = 0;

		pEnemy->nState = 1;	// �o��
		pEnemy->nShadowIdx =
			CreateShadow(pEnemy->pos, ENEMY_RADIUS);
		pEnemy->rot = rot;//���f���̌���
		return i;	// �ԍ���Ԃ�
	}

	return -1;	// ������Ȃ�����
}


int GetEnemyHp(int no)
{
	if (no < 0 || no >= ENEMY_MAX)	return(0);

	return g_enemy[no].nHP;


}

void DamageEnemy(int damage)
{

	int HP = GetEnemy()->GetHP();

	HP -= damage;
	if (HP <= 0)
	{
		HP = 0;

		g_Eaction3 = true;
	}

	GetEnemy()->SetHP(HP);
}

void DestroyEnemy(int no)
{
	if (no < 0 || no >= ENEMY_MAX)	return;

	g_enemy[no].nState = 0;

	//�e�̊J��
	ReleaseShadow(g_enemy[no].nShadowIdx);
	g_enemy[no].nShadowIdx = -1;

	Clearflag();

}
void DrawEnemyEGauge()
{
	const XMFLOAT2 pos = { ENEMY_EGAUGE_POS_X, ENEMY_EGAUGE_POS_Y };

	DrawEGauge(pos, (float)g_enemy[0].nEGauge / EGAUGE_MAX);
}

void ResetEPos(int no)
{
	if (no < 0 || no >= ENEMY_MAX)	return;

	g_enemy[no].pos = XMFLOAT3(0.0f, 20.0f, 100.0f);
	g_enemy[no].rot = XMFLOAT3(0.0f, 180.0f, 0.0f);
	g_enemy[no].scl = XMFLOAT3(15.0f, 15.0f, 15.0f);
	g_enemy[no].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);

	g_enemyHD[no].pos = XMFLOAT3(0.0f, 0.0f, -1.0f);
	g_enemyHD[no].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_enemyHD[no].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
	g_enemyHD[no].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);

	g_enemyAM[no].pos = XMFLOAT3(0.0f, -0.5f, 0.0f);///-0.8,-1
	g_enemyAM[no].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_enemyAM[no].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
	g_enemyAM[no].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);

	g_enemyLG[no].pos = XMFLOAT3(0.0f, -0.5f, 1.5f);
	g_enemyLG[no].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_enemyLG[no].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
	g_enemyLG[no].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);

	g_nECnt1 = 0;
	g_nECnt2 = 0;
	g_nECnt3 = 0;
	g_nECnt4 = 0;
	g_nECnt5 = 0;
	g_nECnt6 = 0;
	g_nECnt7 = 0;
	g_nECnt8 = 0;
	g_nECnt9 = 0;
	g_nECnt10 = 0;
	g_nECnt11 = 0;
	g_nECnt12 = 0;

	g_Eaction = false;
	g_Eaction2 = false;
	g_Eaction3 = false;
	g_Eaction4 = false;
	g_Eaction5 = false;
	g_Eaction6 = false;
	g_Eaction7 = false;
	g_Eaction8 = false;
	g_Eaction9 = false;
	g_Eaction10 = false;
	g_Eaction11 = false;
}

void EAction(bool af)//�����h����
{
	if (af)
	{
		for (int i = 0; i < ENEMY_MAX; i++)
		{
			g_nECnt1++;
			g_nECnt2++;
			g_enemyHD[i].rot.z += 50;
			if (g_nECnt1 <= 150)
			{
				g_enemy[i].pos.y++;

			}
			if (g_nECnt1 >= 150 && g_nECnt2 <= 250)
			{
				g_enemy[i].pos.z--;
				g_enemy[i].pos.y--;

				g_enemy[i].rot.x = -45;
			}
			//g_player[i].vel.y += 10.0f;

			if (g_nECnt1 >= 500 && g_nECnt2 >= 500)
			{
				ResetEPos(i);
			}
		}
	}
}
void EAction()
{
	g_Eaction = true;
}

void EAction2(bool af)//�S��
{
	if (af)
	{
		for (int i = 0; i < ENEMY_MAX; i++)
		{
			g_enemy[i].pos.z = +80;
			g_nECnt3++;
			if (g_nECnt3 >= 1 && g_nECnt3 <= 200) // ?�͂ǂ��܂ŌJ��Ԃ����̐��l
			{
				if (g_nECnt3 / 2 % 2 == 0)
				{
					g_enemyHD[i].pos.z -= 3;
					g_enemyHD[i].pos.x = (rand() % 201 - 100) / 30.0f;
					g_enemyHD[i].pos.y = (rand() % 201 - 100) / 30.0f;

				}
				else g_enemyHD[i].pos.z = -1.0f;
			}
			if (g_nECnt3 >= 201)
			{
				ResetEPos(i);
			}
		}
	}
}
void EAction2()
{
	g_Eaction2 = true;
}

void EAction3(bool af)//���S
{
	if (af)
	{
		for (int i = 0; i < ENEMY_MAX; i++)
		{
			g_nECnt4++;
			g_enemy[i].rot.z = 180;
			if (g_nECnt4 >= 20 && g_nECnt4 <= 200)
			{
				g_enemyHD[i].pos.z += 0.05f;
				g_enemyHD[i].rot.z -= 2.0f;
				g_enemyHD[i].rot.y -= 2.0f;

				g_enemyAM[i].pos.x += 0.05f;
				g_enemyAM[i].rot.z -= 2.0f;
				g_enemyAM[i].rot.y -= 2.0f;

				g_enemyLG[i].pos.z -= 0.05f;
				g_enemyLG[i].rot.z -= 2.0f;
				g_enemyLG[i].rot.y -= 2.0f;
			}
		}
		if (g_nECnt4 >= 200)
			DestroyEnemy(0);

	}
}

void EAction4(bool af) //�ł��オ�郂�[�V����
{
	if (af)
	{
		
		for (int i = 0; i < ENEMY_MAX; i++)
		{
			g_nECnt5++;

			if (g_nECnt5 >= 16 && g_nECnt5 <= 109)
			{
				g_enemy[i].pos.y++;
				g_enemy[i].rot.x++;
			}
			if (g_nECnt5 >= 110 && g_nECnt5 <=200 )
			{
				g_enemy[i].pos.y--;
				g_enemy[i].rot.x++;
			}
			if (g_nECnt5 >= 220 && g_nECnt5 <= 379)
			{
				g_enemy[i].pos.z++;
				g_enemy[i].rot.x++;
				g_enemy[i].rot.y++;
				g_enemy[i].rot.z++;
			}
			
			if (g_nECnt5 >= 380)
			{
				ResetEPos(i);
			}
		}
	}
}

void EAction5(bool af) //�����U���@�Ō�BD�͑唚��
{
	if (af)
	{

		for (int i = 0; i < ENEMY_MAX; i++)
		{
			g_nECnt6++;

			if (g_nECnt6 >= 0 && g_nECnt6 <= 50)
			{
				g_enemyHD[i].pos.y += 0.1f;
				g_enemyHD[i].rot.x += 20.0f;
				g_enemyHD[i].rot.y -= 20.0f;
				g_enemyHD[i].rot.z += 20.0f;

				g_enemyAM[i].pos.y += 0.1f;
				g_enemyAM[i].rot.x += 20.0f;
				g_enemyAM[i].rot.y += 20.0f;
				g_enemyAM[i].rot.z -= 20.0f;

				g_enemyLG[i].pos.y += 0.1f;
				g_enemyLG[i].rot.x -= 20.0f;
				g_enemyLG[i].rot.y += 20.0f;
				g_enemyLG[i].rot.z += 20.0f;
			}

			if (g_nECnt6 >= 65 && g_nECnt6 <= 75)
			{
				g_enemyHD[i].pos.z -= 7.0f;
				g_enemyHD[i].pos.y -= 2.0f;
			}
			if (g_nECnt6 >= 85 && g_nECnt6 <= 95)
			{
				g_enemyAM[i].pos.z -= 7.0f;
				g_enemyAM[i].pos.y -= 2.0f;
			}
			if (g_nECnt6 >= 105 && g_nECnt6 <= 115)
			{
				g_enemyLG[i].pos.z -= 7.0f;
				g_enemyLG[i].pos.y -= 2.0f;
			}

			if (g_nECnt6 >= 130 && g_nECnt6 <= 165)
			{
				g_enemy[i].pos.z -= 1.5f;
				g_enemy[i].pos.y += 1.5f;
				g_enemy[i].rot.x -= 7.0f;
			}
			if (g_nECnt6 >= 165 && g_nECnt6 <= 170)
			{
				g_enemy[i].pos.z -= 1.5f;
				g_enemy[i].rot.x -= 7.0f;
				
			}
			if (g_nECnt6 >= 170 && g_nECnt6 <= 205)
			{
				g_enemy[i].pos.z -= 1.5f;
				g_enemy[i].pos.y -= 1.5f;
				g_enemy[i].rot.x -= 7.0f;
			}
			if (g_nECnt6 >= 235 && g_nECnt6 <= 400)
			{
				if (g_nECnt6 / 2 % 2 == 0)
				{
					g_enemy[i].pos.x = 1.0f;
				}
				else g_enemy[i].pos.x = -1.0f;
			}
			//����


			if (g_nECnt6 >= 500)
			{
				ResetEPos(i);
			}
		}
	}
}


void EAction6(bool af)//��񏟗� �]����U��
{
	if (af)
	{

		for (int i = 0; i < ENEMY_MAX; i++)
		{
			g_nECnt7++;
			if (g_nECnt7 >= 0 && g_nECnt7 <= 100)
			{
				g_enemy[i].pos.z -= 3.0f;
				g_enemy[i].rot.x -= 50.0f;
			}

			if (g_nECnt7 >= 200)
			{
				ResetEPos(i);
			}
		}
	}
}

void EAction7(bool af)//��񏟗���둫�R��U��
{
	if (af)
	{

		for (int i = 0; i < ENEMY_MAX; i++)
		{
			g_nECnt8++;
			if (g_nECnt8 >= 0 && g_nECnt8 <= 50)
			{
				g_enemy[i].pos.z = -20.0f;
			}

			if(g_nECnt8 >= 50 && g_nECnt8 <= 60)
			{
				g_enemy[i].rot.y = 0.0f;
			}

			if (g_nECnt8 >= 70 && g_nECnt8 <= 80)
			{
				g_enemyLG[i].rot.x = -90.0f;
				g_enemy[i].rot.x = -3.0f;
			}

			if (g_nECnt8 >= 83 && g_nECnt8 <= 90)
			{
				g_enemyLG[i].pos.z++;
			}

			if (g_nECnt8 >= 200)
			{
				ResetEPos(i);
			}
		}
	}
}

void EAction8(bool af)//��񏟗� �{�f�B�v���X
{
	if (af)
	{

		for (int i = 0; i < ENEMY_MAX; i++)
		{
			g_nECnt9++;
			if (g_nECnt9 >= 0 && g_nECnt9 <= 50)
			{
				g_enemy[i].pos.y += 3.0f;
				g_enemy[i].pos.z -= 4.0f;
				g_enemy[i].rot.x -= 7.0f;
			}
			if (g_nECnt9 >= 51 && g_nECnt9 <= 60)
			{
				g_enemy[i].rot = XMFLOAT3(0.0f, 180.0f, 0.0f);
			}

			if (g_nECnt9 >= 60 && g_nECnt9 <= 88)
			{
				g_enemy[i].pos.y -= 5.0f;
			}
			
			if (g_nECnt9 >= 200)
			{
				ResetEPos(i);
			}
		}
	}
}


void EAction9(bool af)//�@1�񏟗��̓�����U��
{
	if (af)
	{
		for (int i = 0; i < ENEMY_MAX; i++)
		{
			g_nECnt10++;
			if (g_nECnt10 >= 0 && g_nECnt10 <= 50)
			{
				g_enemy[i].pos.z = -40.0f;
			}
			if (g_nECnt10 >= 51 && g_nECnt10 <= 70)
			{
				g_enemy[i].pos.z += 1.0f;
				g_enemy[i].rot.y = 135.0f;
			}
			if (g_nECnt10 >= 71 && g_nECnt10 <= 80)
			{
				g_enemy[i].pos.z = -95.0f;
			}
			if (g_nECnt10 >= 81 && g_nECnt10 <= 82)
			{
				g_enemy[i].pos.z += 25.0f;
			}
			if (g_nECnt10 >= 200)
			{
				ResetEPos(i);
			}
		}
	}
}

void EAction10(bool af)//�@���������r�@��
{
	if (af)
	{
		for (int i = 0; i < ENEMY_MAX; i++)
		{
			g_nECnt11++;
			if (g_nECnt11 >= 0 && g_nECnt11 <= 50)
			{
				g_enemy[i].pos.y += 5.0f;
				g_enemy[i].rot.x -= 8.0f;
			}

			if (g_nECnt11 >= 51 && g_nECnt11 <= 103)
			{
				g_enemy[i].pos.y -= 5.0f;
				g_enemy[i].rot.x -= 8.0f;
			}
			if (g_nECnt11 >= 110 && g_nECnt11 <= 200)
			{
				g_enemy[i].rot.x = -90.0f;
				g_enemy[i].rot.y += 50;
			}
			if (g_nECnt11 >= 200 && g_nECnt11 <= 240)
			{
				g_enemy[i].pos.z--;
				g_enemy[i].pos.x--;
				g_enemy[i].rot.y += 50;
			}
			if (g_nECnt11 >= 240 && g_nECnt11 <= 330)
			{
				g_enemy[i].pos.z--;
				g_enemy[i].pos.x++;
				g_enemy[i].rot.y += 50;
			}
			if (g_nECnt11 >= 330 && g_nECnt11 <= 380)
			{
				g_enemy[i].pos.z--;
				g_enemy[i].pos.x--;
				g_enemy[i].rot.y += 50;
			}
			if (g_nECnt11 >= 380 && g_nECnt11 <= 600)
			{
				g_enemy[i].pos.y += 0.5f;
				g_enemy[i].rot.y += 50;
			}


			if (g_nECnt11 >= 600)
			{
				ResetEPos(i);
			}

		}
	}
}
void EAction11(bool af)//�@�C�~�a�@��
{
	if (af)
	{
		for (int i = 0; i < ENEMY_MAX; i++)
		{
			g_nECnt12++;
			if (g_nECnt12 >= 0 && g_nECnt12 <= 50)
			{
				g_enemy[i].rot.y -= 100;
			}
			if (g_nECnt12 >= 50 && g_nECnt12 <= 70)
			{
				g_enemy[i].rot.y -= 100;
				g_enemy[i].pos.y += 0.4f;
				
			}
			if (g_nECnt12 >= 70 && g_nECnt12 <=200)
			{
				g_enemy[i].rot.y -= 100;
				g_enemy[i].pos.z-=0.8f;
				g_enemy[i].pos.x--;
				g_enemy[i].pos.y += 0.4f;
			}
			if(g_nECnt12 >= 200 && g_nECnt12 <= 280)
			{
				g_enemy[i].rot.y -= 100;
				g_enemy[i].pos.z-=0.8f;
				g_enemy[i].pos.x++;
				g_enemy[i].pos.y -= 0.7f;
			}
			if (g_nECnt12 >= 280 && g_nECnt12 <= 500)
			{
				g_enemy[i].rot.y -= 100;
			}
			

			if (g_nECnt12 >= 600)
			{
				ResetEPos(i);
			}
		}
	}
}

float ECheckCollisionRay(XMVECTOR pos, float fRadius)
{
	//�r���[�}�g���b�N�X�ƃv���W�F�N�V�����}�g���b�N�X
	XMMATRIX mtxView = XMLoadFloat4x4(&GetCamera()->GetViewMatrix());
	XMMATRIX mtxProjection = XMLoadFloat4x4(&GetCamera()->GetProjMatrix());

	//�}�E�X���W
	POINT ptMouse = (*GetMousePosition());
	XMFLOAT3 posM = XMFLOAT3(ptMouse.x, ptMouse.y, 0.0f);

	//����j�A�N���b�v��̍��W���v�Z
	XMVECTOR posNear = XMVectorSet(posM.x, posM.y, 0.0f, 0.0f);
	posNear = XMVector3Unproject(posNear,//�}�E�X���W
		0.0f, 0.0f,//�r���[�|�[�g�p�f�[�^xy
		SCREEN_WIDTH, SCREEN_HEIGHT, //������
		0.0f, 1.0f,//��minZmaxZ
		mtxProjection,//�v���W�F�N�V�����}�g���b�N�X
		mtxView,				//�r���[�}�g���N�X
		XMMatrixIdentity()
	);
	//����t�@�[�N���b�v��̍��W���v�Z
	XMVECTOR posFar = XMVectorSet(posM.x, posM.y, 1.0f, 0.0f);
	posFar = XMVector3Unproject(posFar,//�}�E�X���W
		0.0f, 0.0f,//�r���[�|�[�g�p�f�[�^xy
		SCREEN_WIDTH, SCREEN_HEIGHT, //������
		0.0f, 1.0f,//��minZmaxZ
		mtxProjection,//�v���W�F�N�V�����}�g���b�N�X
		mtxView,				//�r���[�}�g���N�X
		XMMatrixIdentity()
	);

	//���C���΂��ē����蔻����Ƃ�
	XMVECTOR ray = posFar - posNear;
	XMFLOAT3 eye3 = GetCamera()->GetPos();
	XMVECTOR eye = XMVectorSet(eye3.x, eye3.y, eye3.z, 0.0f);//���_
	XMVECTOR target = pos - eye;
	//������W�ւ̃x�N�g���ƃ��C�̓��ςƌv�Z
	XMVECTOR tempVec = XMVector3Dot(ray, target);
	float temp = XMVectorGetX(tempVec);

	//�����̓����擾
	tempVec = XMVector3Dot(target, target);//x*x+y*y+z*z
	float tempTarget = XMVectorGetX(tempVec);//�l��X,Y,Z�ǂ������
	tempVec = XMVector3Dot(ray, ray);
	float tempRay = XMVectorGetX(tempVec);

	//���C�𗎂Ƃ��������̌����_�܂ł̋���
	float ret = temp * temp / tempRay;

	//�c���ӂ̒������v�Z
	float length = sqrtf(tempTarget - ret);
	if (length < fRadius)
	{
		//�͈͓��ł�������
		return sqrtf(ret);
	}

	//������Ȃ������̂�
	return 9999.9f; //�ő�l
}