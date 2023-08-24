//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "AssimpModel.h"
#include "player.h"
#include "main.h"
#include "input.h"
#include "shadow.h"		//�e������ɂ͓����
#include "bullet.h"
#include "gauge.h"
#include "sound.h"
#include "MessageManager.h"
#include"fade.h"
#include"enemy.h"
#include "model.h"
#include "sceneTitle.h"
#include "sceneGame.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_PLAYER		"data/model/kabuBody.obj"
#define MODEL_PLAYER1		"data/model/kabu2.obj"
#define MODEL_PLAYER2		"data/model/kabuAM.obj"
#define MODEL_PLAYER3		"data/model/kabuLG.obj"


#define VALUE_MOVE		(3.0f)		
#define VALUE_ROTATE	(1.0f)


#define PLAYER_GAUGE_POS_X	(-113.0f)
#define PLAYER_GAUGE_POS_Y	(300.0f)


//�\���̒�`
typedef struct _tPlayer
{
	XMFLOAT4X4		mtxWorld;		//���W�ϊ��s��

	XMFLOAT3		pos;			//���W
	XMFLOAT3		rot;			//��]
	XMFLOAT3		scl;			//�g��k��
	XMFLOAT3		vel;			//���x

	int				nPhase;			//�i�K
	int				nState;			//���(0:���g�p�@1:�ʏ�)

	char			nName;			//
	int				nHP;

	XMFLOAT3	basePos;	//��{�ʒu
	XMFLOAT3	targetPos;	//�ڕW�ʒu

	int				bCnt;			//�e�̐�
	int				nGauge;			//�p���[�Q�[�W
	int				nStopTime;		//����s�\����

	int				nShadowIdx;		//�e�̔ԍ�

				
}tPlayer;

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static CAssimpModel		g_model[4];				//���f���f�[�^�i�[�p
			
static tPlayer			g_player[PLAYER_MAX];	//�v���C���[
static tPlayer			g_playerHD[PLAYER_MAX];
static tPlayer			g_playerAM[PLAYER_MAX];
static tPlayer			g_playerLG[PLAYER_MAX];
static int				g_nCameraType = E_CAMERA_VIEW_FIXED;		//�J�����̎��


static int			g_nCnt1;//�s���J�E���g1�R��
static int			g_nCnt2;//�s���J�E���g2�R��
static int			g_nCnt3;//�s���J�E���g3�R��
static int			g_nCnt4;//�s���J�E���g4�R��
static int			g_nCnt5;//�s���J�E���g4�R��

static int			g_nCntF;//


static bool			g_action;
static bool			g_action2;
static bool			g_action3;
static bool			g_action4;

static bool				g_atama = true;//false;


const tMessage testMessage[4] = {
	{L"",MESSAGE_TYPE::E_TYPE_TIMER,60 },
	{L"1����",MESSAGE_TYPE::E_TYPE_ENTER,(long long int)&testMessage[2]},
	{L"2����",MESSAGE_TYPE::E_TYPE_ENTER,NULL},
	{L"2����",MESSAGE_TYPE::E_TYPE_NORMAL,}
};


//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		g_player[i].pos = XMFLOAT3(0.0f, 20.0f, -100.0f);
		g_player[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_player[i].scl = XMFLOAT3(8.0f, 8.0f, 8.0f);
		g_player[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		//������
		g_player[i].nPhase = 0;
		g_player[i].nState = 1;		//�ŏ��͒ʏ�

	    g_player[i].nName = 0;

		g_player[i].bCnt = 0;	

		//g_nCnt = 0.0f;				//

		g_nCnt1 = 0;					//
		g_nCnt2 = 0;					//
		g_nCnt3 = 0;					//
		g_nCnt4 = 0;
		g_nCnt5 = 0;
		g_nCntF = 0;

		g_action = false;
		g_action2 = false;
		g_action3 = false;
		g_action4 = false;

		g_player[i].nHP = GetPlayer()->GetHP();
		g_player[i].nGauge = 100;
		g_player[i].nStopTime = 0; //�ŏ��͓�����

		g_player[i].nShadowIdx = CreateShadow(g_player[i].pos, 20.0f);

		//��
		g_playerHD[i].pos = XMFLOAT3(0.0f, 0.0f, -2.0f);
		g_playerHD[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_playerHD[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_playerHD[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_playerHD[i].nPhase = 0;
		g_playerHD[i].nState = 1;	// puro�؂�͍ŏ�����o�ꂵ�Ă���
		g_playerHD[i].nShadowIdx = -1;

		//�r
		g_playerAM[i].pos = XMFLOAT3(0.0f, -0.8f, -1.0f);
		g_playerAM[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_playerAM[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_playerAM[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_playerAM[i].nPhase = 0;
		g_playerAM[i].nState = 1;	// puro�؂�͍ŏ�����o�ꂵ�Ă���
		g_playerAM[i].nShadowIdx = -1;

		//��
		g_playerLG[i].pos = XMFLOAT3(0.0f, -0.8f, 0.0f);
		g_playerLG[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_playerLG[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_playerLG[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_playerLG[i].nPhase = 0;
		g_playerLG[i].nState = 1;	// puro�؂�͍ŏ�����o�ꂵ�Ă���
		g_playerLG[i].nShadowIdx = -1;
	}

	// ���[���h�}�g���b�N�X�̏�����
	XMMATRIX mtxWorld;
	mtxWorld = XMMatrixIdentity();

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		XMStoreFloat4x4(&g_player[i].mtxWorld, mtxWorld);
		XMStoreFloat4x4(&g_playerHD[i].mtxWorld, mtxWorld);
		XMStoreFloat4x4(&g_playerAM[i].mtxWorld, mtxWorld);
		XMStoreFloat4x4(&g_playerLG[i].mtxWorld, mtxWorld);

	}

	// ���f���f�[�^�̓ǂݍ���
	hr = g_model[0].Load(pDevice, pDeviceContext,
		GetBody()); if (FAILED(hr)) return hr;

	hr = g_model[1].Load(pDevice, pDeviceContext,
		GetHead()); if (FAILED(hr)) return hr;

	hr = g_model[2].Load(pDevice, pDeviceContext,
		GetMae()); if (FAILED(hr)) return hr;

	hr = g_model[3].Load(pDevice, pDeviceContext,
		GetBack()); if (FAILED(hr)) return hr;
	
	g_nCameraType = E_CAMERA_VIEW_FIXED;
	
	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	//�e�̉��
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		ReleaseShadow(g_player[i].nShadowIdx);
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
void UpdatePlayer(void)
{
	
	// �v���C���[�̓������L�q����Ȃ炱��
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		
		//���g�p
		if (g_player[i].nState == 0) continue;


		//����s�\����
		if (g_player[i].nStopTime > 0)
		{
			g_player[i].nStopTime--;
		}
		if (g_player[i].nStopTime == 0)
		{
			if(GetKeyTrigger(VK_8))
			{
				g_action = true;
			}
			Action(g_action);

			if(GetKeyTrigger(VK_6))
			{
				PlaySound(SOUND_LABEL_SE_PUNCH);
				g_action2 = true;
			}
			Action2(g_action2);

			if(GetKeyTrigger(VK_7))
			{
				g_action3 = true;
			}
			Action3(g_action3);

			if (GetKeyTrigger(VK_9))
			{
				g_action4 = true;
			}
			Action4(g_action4);


		}
			if (GetKeyTrigger(VK_H))
			{
				DamagePlayer(50);
			}
			
			if (GetKeyPress(VK_Z))
			{
				//g_player[i].rot.y+=5;
				g_playerHD[i].rot.z += 5;
				//g_playerAM[i].rot.x += 5;
				g_playerLG[i].rot.x += 5;

			}
			
		//�Q�[�W�̓����̏���
			if (g_player[i].nGauge>= (GetPlayer()->GetHP() * 100 / g_player[i].nHP))
			{
				g_player[i].nGauge--;
			}

		//���x�����W�ɉ��Z
		g_player[i].pos.x += g_player[i].vel.x;
		g_player[i].pos.y += g_player[i].vel.y;
		g_player[i].pos.z += g_player[i].vel.z;

		//��ʊO�ɍs���Ȃ�����
		XMFLOAT3 box = g_model[i].GetBBox();
		//[���[] 
		if (g_player[i].pos.x < -200 + box.x * g_player[i].scl.x)
		{
			g_player[i].pos.x = -200 + box.x * g_player[i].scl.x;
		}
		//[�E�[]
		else if (g_player[i].pos.x > 200 - box.x * g_player[i].scl.x)
		{
			g_player[i].pos.x = 200 - box.x * g_player[i].scl.x;
		}
		//[���[] �v���C���[�̍��[����ʂ̏�[
		if (g_player[i].pos.z > 200 - box.z * g_player[i].scl.z)
		{
			g_player[i].pos.z = 200 - box.z * g_player[i].scl.z;
		}
		//[��[] �v���C���[�̍��[����ʂ̉��[
		else if (g_player[i].pos.z < -200 + box.z * g_player[i].scl.z)
		{
			g_player[i].pos.z = -200 + box.z * g_player[i].scl.z;
		}

		//���n����
		if (g_player[i].pos.y <= 0.0f)
		{
			g_player[i].vel.y = 0.0f;
			g_player[i].pos.y = 0.0f;
			g_player[i].vel.x *= 0.9f;
			g_player[i].vel.z *= 0.9f;
		}

		if (GetKeyTrigger(VK_O))
		{
			SetMessage((tMessage*)&testMessage[3]);
		}

		XMMATRIX mtxWorld, mtxRot, mtxScl,
			mtxTranslate;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_player[i].scl.x, g_player[i].scl.y, g_player[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);



		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(g_player[i].rot.x), XMConvertToRadians(g_player[i].rot.y + 180), XMConvertToRadians(g_player[i].rot.z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);


		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_player[i].pos.x, g_player[i].pos.y, g_player[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);


		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&g_player[i].mtxWorld, mtxWorld);

		//�e�̈ړ�
		MoveShadow(g_player[i].nShadowIdx, g_player[i].pos);


		//************************************************:
		//��

		XMMATRIX mtxWorldHD;
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorldHD = XMMatrixIdentity();
		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_playerHD[i].scl.x, g_playerHD[i].scl.y, g_playerHD[i].scl.z);
		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxScl);
		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(g_playerHD[i].rot.x),
			XMConvertToRadians(g_playerHD[i].rot.y),//+180
			XMConvertToRadians(g_playerHD[i].rot.z)
		);
		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_playerHD[i].pos.x, g_playerHD[i].pos.y, g_playerHD[i].pos.z);

		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxTranslate);

		//�e�̍s����|����
		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxWorld);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&g_playerHD[i].mtxWorld, mtxWorldHD);


		//************************************************:
		//�r

		XMMATRIX mtxWorldAM;
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorldAM = XMMatrixIdentity();
		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_playerAM[i].scl.x, g_playerAM[i].scl.y, g_playerAM[i].scl.z);
		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxScl);
		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(g_playerAM[i].rot.x),
			XMConvertToRadians(g_playerAM[i].rot.y),//+180
			XMConvertToRadians(g_playerAM[i].rot.z)
		);
		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_playerAM[i].pos.x, g_playerAM[i].pos.y, g_playerAM[i].pos.z);

		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxTranslate);

		//�e�̍s����|����
		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxWorld);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&g_playerAM[i].mtxWorld, mtxWorldAM);

		//************************************************:
		//��

		XMMATRIX mtxWorldLG;
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorldLG = XMMatrixIdentity();
		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_playerLG[i].scl.x, g_playerLG[i].scl.y, g_playerLG[i].scl.z);
		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxScl);
		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(g_playerLG[i].rot.x),
			XMConvertToRadians(g_playerLG[i].rot.y),//+180
			XMConvertToRadians(g_playerLG[i].rot.z)
		);
		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_playerLG[i].pos.x, g_playerLG[i].pos.y, g_playerLG[i].pos.z);

		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxTranslate);

		//�e�̍s����|����
		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxWorld);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&g_playerLG[i].mtxWorld, mtxWorldLG);

	}
#ifdef CAMERA_DEBUG
	if (GetKeyTrigger(VK_1))
		g_nCameraType = E_CAMERA_VIEW_FIXED;
	if (GetKeyTrigger(VK_2))
		g_nCameraType = E_CAMERA_VIEW_BIRD;
	if (GetKeyTrigger(VK_3))
		g_nCameraType = E_CAMERA_VIEW_BEHIND;
	if (GetKeyTrigger(VK_4))
		g_nCameraType = E_CAMERA_VIEW_FPS;
	if (GetKeyTrigger(VK_5))
		g_nCameraType = E_CAMERA_VIEW_CAMERA_DEBUG;
#endif

	switch (g_nCameraType)
	{
	case E_CAMERA_VIEW_FIXED:
		GetCamera()->SetTarget(1.0f, 1.0f, -1.0f);
		GetCamera()->SetPos(150.0f, 150.0f,-150.0f);
		break;
	case E_CAMERA_VIEW_BIRD:
		GetCamera()->SetTarget(g_player[0].pos);
		GetCamera()->SetPos(g_player[0].pos.x + 0.0f, g_player[0].pos.y + 100.0f, g_player[0].pos.z - 200.0f);
		break;
	case E_CAMERA_VIEW_BEHIND:
		GetCamera()->SetTarget(g_player[0].pos.x + SinDeg(g_player[0].rot.y)*100.0f, g_player[0].pos.y + 15.0f, g_player[0].pos.z + CosDeg(g_player[0].rot.y)*100.0f);
		GetCamera()->SetPos(g_player[0].pos.x - SinDeg(g_player[0].rot.y)*40.0f, g_player[0].pos.y + 20.0f, g_player[0].pos.z - CosDeg(g_player[0].rot.y)*40.0f);
		break;						
	case E_CAMERA_VIEW_FPS:
		GetCamera()->SetTarget(g_player[0].pos.x + SinDeg(g_player[0].rot.y)*100.0f, g_player[0].pos.y + 10.0f, g_player[0].pos.z + CosDeg(g_player[0].rot.y)*100.0f);
		GetCamera()->SetPos(g_player[0].pos.x + SinDeg(g_player[0].rot.y)*10.0f, g_player[0].pos.y + 20.0f, g_player[0].pos.z + CosDeg(g_player[0].rot.y)*10.0f);
		break;
	case E_CAMERA_VIEW_DIAGONAL:
		GetCamera()->SetTarget(0.0f, 0.0f, 0.0f);
		GetCamera()->SetPos(0.0f, 100.0f, -200.0f);
		break;
	case E_CAMERA_VIEW_CAMERA_DEBUG:break;
	}
}


void UpdateStart(void)
{
	// �v���C���[�̓������L�q����Ȃ炱��
	for (int i = 0; i < PLAYER_MAX; i++)
	{

		//���g�p
		if (g_player[i].nState == 0) continue;


		//����s�\����
		if (g_player[i].nStopTime > 0)
		{
			g_player[i].nStopTime--;
		}

			
		//�Q�[�W�̓����̏���
		if (g_player[i].nGauge >= (GetPlayer()->GetHP()* 100 /g_player[i].nHP))
		{
			g_player[i].nGauge--;
		}

		//���x�����W�ɉ��Z
		g_player[i].pos.x += g_player[i].vel.x;
		g_player[i].pos.y += g_player[i].vel.y;
		g_player[i].pos.z += g_player[i].vel.z;

		//���n����
		if (g_player[i].pos.y <= 0.0f)
		{
			g_player[i].vel.y = 0.0f;
			g_player[i].pos.y = 0.0f;
			g_player[i].vel.x *= 0.9f;
			g_player[i].vel.z *= 0.9f;
		}

		XMMATRIX mtxWorld, mtxRot, mtxScl,
			mtxTranslate;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_player[i].scl.x, g_player[i].scl.y, g_player[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);



		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(g_player[i].rot.x), XMConvertToRadians(g_player[i].rot.y + 180), XMConvertToRadians(g_player[i].rot.z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);


		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_player[i].pos.x, g_player[i].pos.y, g_player[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);


		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&g_player[i].mtxWorld, mtxWorld);

		//�e�̈ړ�
		MoveShadow(g_player[i].nShadowIdx, g_player[i].pos);


		//************************************************:
		//��

		XMMATRIX mtxWorldHD;
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorldHD = XMMatrixIdentity();
		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_playerHD[i].scl.x, g_playerHD[i].scl.y, g_playerHD[i].scl.z);
		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxScl);
		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(g_playerHD[i].rot.x),
			XMConvertToRadians(g_playerHD[i].rot.y),//+180
			XMConvertToRadians(g_playerHD[i].rot.z)
		);
		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_playerHD[i].pos.x, g_playerHD[i].pos.y, g_playerHD[i].pos.z);

		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxTranslate);

		//�e�̍s����|����
		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxWorld);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&g_playerHD[i].mtxWorld, mtxWorldHD);


		//************************************************:
		//�r

		XMMATRIX mtxWorldAM;
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorldAM = XMMatrixIdentity();
		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_playerAM[i].scl.x, g_playerAM[i].scl.y, g_playerAM[i].scl.z);
		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxScl);
		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(g_playerAM[i].rot.x),
			XMConvertToRadians(g_playerAM[i].rot.y),//+180
			XMConvertToRadians(g_playerAM[i].rot.z)
		);
		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_playerAM[i].pos.x, g_playerAM[i].pos.y, g_playerAM[i].pos.z);

		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxTranslate);

		//�e�̍s����|����
		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxWorld);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&g_playerAM[i].mtxWorld, mtxWorldAM);

		//************************************************:
		//��

		XMMATRIX mtxWorldLG;
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorldLG = XMMatrixIdentity();
		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_playerLG[i].scl.x, g_playerLG[i].scl.y, g_playerLG[i].scl.z);
		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxScl);
		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(g_playerLG[i].rot.x),
			XMConvertToRadians(g_playerLG[i].rot.y),//+180
			XMConvertToRadians(g_playerLG[i].rot.z)
		);
		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_playerLG[i].pos.x, g_playerLG[i].pos.y, g_playerLG[i].pos.z);

		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxTranslate);

		//�e�̍s����|����
		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxWorld);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&g_playerLG[i].mtxWorld, mtxWorldLG);

	}

	GetCamera()->SetTarget(1.0f, 1.0f, -1.0f);
	GetCamera()->SetPos(150.0f, 100.0f, 0.0f);
}

void UpdateClear(void)
{
	// �v���C���[�̓������L�q����Ȃ炱��
	for (int i = 0; i < PLAYER_MAX; i++)
	{

		//���g�p
		if (g_player[i].nState == 0) continue;


		//���x�����W�ɉ��Z
		g_player[i].pos.x += g_player[i].vel.x;
		g_player[i].pos.y += g_player[i].vel.y;
		g_player[i].pos.z += g_player[i].vel.z;

		//���n����
		if (g_player[i].pos.y <= 0.0f)
		{
			g_player[i].vel.y = 0.0f;
			g_player[i].pos.y = 0.0f;
			g_player[i].vel.x *= 0.9f;
			g_player[i].vel.z *= 0.9f;
		}

		XMMATRIX mtxWorld, mtxRot, mtxScl,
			mtxTranslate;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_player[i].scl.x, g_player[i].scl.y, g_player[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);



		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(g_player[i].rot.x), XMConvertToRadians(g_player[i].rot.y + 180), XMConvertToRadians(g_player[i].rot.z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);


		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_player[i].pos.x, g_player[i].pos.y, g_player[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);


		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&g_player[i].mtxWorld, mtxWorld);

		//�e�̈ړ�
		MoveShadow(g_player[i].nShadowIdx, g_player[i].pos);


		//************************************************:
		//��

		XMMATRIX mtxWorldHD;
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorldHD = XMMatrixIdentity();
		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_playerHD[i].scl.x, g_playerHD[i].scl.y, g_playerHD[i].scl.z);
		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxScl);
		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(g_playerHD[i].rot.x),
			XMConvertToRadians(g_playerHD[i].rot.y),//+180
			XMConvertToRadians(g_playerHD[i].rot.z)
		);
		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_playerHD[i].pos.x, g_playerHD[i].pos.y, g_playerHD[i].pos.z);

		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxTranslate);

		//�e�̍s����|����
		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxWorld);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&g_playerHD[i].mtxWorld, mtxWorldHD);


		//************************************************:
		//�r

		XMMATRIX mtxWorldAM;
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorldAM = XMMatrixIdentity();
		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_playerAM[i].scl.x, g_playerAM[i].scl.y, g_playerAM[i].scl.z);
		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxScl);
		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(g_playerAM[i].rot.x),
			XMConvertToRadians(g_playerAM[i].rot.y),//+180
			XMConvertToRadians(g_playerAM[i].rot.z)
		);
		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_playerAM[i].pos.x, g_playerAM[i].pos.y, g_playerAM[i].pos.z);

		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxTranslate);

		//�e�̍s����|����
		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxWorld);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&g_playerAM[i].mtxWorld, mtxWorldAM);

		//************************************************:
		//��

		XMMATRIX mtxWorldLG;
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorldLG = XMMatrixIdentity();
		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_playerLG[i].scl.x, g_playerLG[i].scl.y, g_playerLG[i].scl.z);
		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxScl);
		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(g_playerLG[i].rot.x),
			XMConvertToRadians(g_playerLG[i].rot.y),//+180
			XMConvertToRadians(g_playerLG[i].rot.z)
		);
		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_playerLG[i].pos.x, g_playerLG[i].pos.y, g_playerLG[i].pos.z);

		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxTranslate);

		//�e�̍s����|����
		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxWorld);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&g_playerLG[i].mtxWorld, mtxWorldLG);

	}

	GetCamera()->SetTarget(g_player[0].pos.x + SinDeg(g_player[0].rot.y)*100.0f, g_player[0].pos.y + 15.0f, g_player[0].pos.z + CosDeg(g_player[0].rot.y)*100.0f);
	GetCamera()->SetPos(0.0f, 70.0f, 100.0f);
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		//���g�p
		if (g_player[i].nState == 0) continue;

		// ���f���\��
		g_model[0].Draw(pDeviceContext, g_player[i].mtxWorld);
		g_model[1].Draw(pDeviceContext, g_playerHD[i].mtxWorld);
		g_model[2].Draw(pDeviceContext, g_playerAM[i].mtxWorld);
		g_model[3].Draw(pDeviceContext, g_playerLG[i].mtxWorld);
	}
}

//�����m�F
bool IsPlayer(int no)
{
	if (no < 0 || no >= PLAYER_MAX) return false;
	return g_player[no].nState > 0 ?
		true : false;
}

/*void DestroyPlayer(int no)
{
	if (no < 0 || no >= PLAYER_MAX) return;
	g_player[no].nState = 0;
	// �e�Ƃ̊֘A��؂�
	ReleaseShadow(g_player[no].nShadowIdx);
	g_player[no].nShadowIdx = -1;
}*/

//���W�擾
XMFLOAT3 GetPlayerPos(int no)
{
	if (no < 0 || no >= PLAYER_MAX)
		return XMFLOAT3(0.0f, 0.0f, 0.0f);
	return g_player[no].pos;
}

//�T�C�Y�擾
XMFLOAT3 GetPlayerSize(int no)
{
	if (no < 0 || no >= PLAYER_MAX)
		return XMFLOAT3(0, 0, 0);
	//return XMFLOAT3(10.0f,10.0f,10.0f);
	XMFLOAT3 box = g_model[0].GetBBox();
	return XMFLOAT3(box.x / 2 * g_player[no].scl.x, box.y / 2 * g_player[no].scl.y, box.z / 2 * g_player[no].scl.z);
}


//���̃p���[�Q�[�W
void DrawPlayerGauge()
{
	const XMFLOAT2 pos = { PLAYER_GAUGE_POS_X, PLAYER_GAUGE_POS_Y };

	DrawGauge(pos, (float)g_player[0].nGauge / GAUGE_MAX);
}

int GetPlayerHp(int no)
{
	if (no < 0 || no >= PLAYER_MAX)	return(0);

	return GetPlayer()->GetHP();


}

void DamagePlayer(int damage)
{
	
	int HP = GetPlayer()->GetHP();

	HP -= damage;
	if (HP <= 0)
	{
		HP = 0;

		g_action3 = true;
	}

	GetPlayer()->SetHP(HP);
}


void ResetPos(int no)
{
	if (no < 0 || no >= PLAYER_MAX)	return;

	g_player[no].pos = XMFLOAT3(0.0f, 20.0f, -100.0f);
	g_player[no].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_player[no].scl = XMFLOAT3(8.0f, 8.0f, 8.0f);
	g_player[no].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_playerHD[no].pos = XMFLOAT3(0.0f, 0.0f, -2.0f);
	g_playerHD[no].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_playerHD[no].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
	g_playerHD[no].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);

	g_nCnt1=0;
	g_nCnt2=0;
	g_nCnt3 = 0;
	g_nCnt4 = 0;
	g_nCnt5 = 0;
	g_action = false;
	g_action2 = false;
	g_action3 = false;
	g_action4 = false;
}

void Action(bool af)
{
	if (af)
	{
		for (int i = 0; i < PLAYER_MAX; i++)
		{
			g_nCnt1++;
			g_nCnt2++;
			g_playerHD[i].rot.z += 50;
			if (g_nCnt1 <= 150)
			{
				g_player[i].pos.y++;

			}
			if (g_nCnt1 >= 150 && g_nCnt2 <= 250)
			{
				g_player[i].pos.z++;
				g_player[i].pos.y--;

				g_player[i].rot.x = -45;
			}
			//g_player[i].vel.y += 10.0f;

			if (g_nCnt1 >= 500 && g_nCnt2 >= 500)
			{
				ResetPos(i);
			}
		}
	}
}
void Action()
{
	g_action = true;
}

void Action2(bool af)
{
	if (af)
	{

		for (int i = 0; i < PLAYER_MAX; i++)
		{
			g_player[i].pos.z = -80;
			g_nCnt3++;
			
			if (g_nCnt3 >= 1 && g_nCnt3 <= 200 ) // ?�͂ǂ��܂ŌJ��Ԃ����̐��l
			{
				if (g_nCnt3 / 2 % 2 == 0)
				{
					g_playerHD[i].pos.z -= 3;
					g_playerHD[i].pos.x = (rand() % 201 - 100) / 30.0f;
					g_playerHD[i].pos.y = (rand() % 201 - 100) / 30.0f;

				}
				else g_playerHD[i].pos.z = -1.0f;
			}
			if (g_nCnt3 >= 201)
			{
				ResetPos(i);
			}
		}
	}
}
void Action2()
{
	g_action2 = true;
}

void Action3(bool af)
{
	if (af)
	{
		for (int i = 0; i < PLAYER_MAX; i++)
		{
			g_nCnt4++;
			g_player[i].rot.z = 180;
			if (g_nCnt4 >= 20 && g_nCnt4 <= 200)
			{
				g_playerHD[i].pos.z -= 0.05f;
				g_playerHD[i].rot.z -= 2.0f;
				g_playerHD[i].rot.y -= 2.0f;

				g_playerAM[i].pos.x -= 0.05f;
				g_playerAM[i].rot.z -= 2.0f;
				g_playerAM[i].rot.y -= 2.0f;

				g_playerLG[i].pos.z += 0.05f;
				g_playerLG[i].rot.z -= 2.0f;
				g_playerLG[i].rot.y -= 2.0f;
			}
		}
		if(g_nCnt4 >= 200)
		DestroyPlayer(0);

	}
}
void Action4(bool af) 
{
	if (af)
	{
		for (int i = 0; i < PLAYER_MAX; i++)
		{
			g_nCnt5++;
			if (g_nCnt5 >= 1 && g_nCnt5 <= 2)
			{
				g_player[i].pos.z += 70;
			}
			if (g_nCnt5 >= 6 && g_nCnt5 <= 10)
			{
				g_player[i].rot.x = 70;
			}
			if (g_nCnt5 >= 15 && g_nCnt5 <= 20)
			{
				g_player[i].pos.y = 50;
				g_player[i].rot.x = 0;
				g_player[i].pos.z = 20;
				g_player[i].pos.x = 80;
								

			}
			if (g_nCnt5 >= 21 && g_nCnt5 <= 90)
			{
				XMFLOAT3 pPos = GetEnemyPos(0);
				// ���t���[���v���C���[��F�����Ēǂ�������
				//g_player[i].targetPos = pPos;
				// �ړ��ʂ̌v�Z
				XMFLOAT3 temp;	// ����
				temp.x = pPos.x - g_player[i].pos.x;
				temp.y = pPos.y - g_player[i].pos.y;
				temp.z = pPos.z - g_player[i].pos.z;
				float len = sqrtf(temp.x*temp.x + temp.z*temp.z);
				if ( g_nCnt5 >= 21 && g_nCnt5 <= 90  )
				{	
					temp.x /= len;
					temp.z /= len;	// �������P�̌��������̏��ɂȂ�

					temp.x *= VALUE_MOVE*4;
					temp.z *= VALUE_MOVE*4;
					g_player[i].vel = XMFLOAT3(temp.x, 0.0f, temp.z);
					g_player[i].rot.y = XMConvertToDegrees(
						atan2f(temp.x, temp.z));// ���f���̌���

					/*g_player[i].pos.z = rand();
					g_player[i].pos.x = rand();*/
					
				}
				
			}
			

			else {	// ��苗���ȓ��͒�~
				g_player[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
			}

			if (g_nCnt5 >= 200)
			{
				ResetPos(i);
			}
		}

	}
		
}

//void Action4(bool af) ��]�ۑ�
//{
//	if (af)
//	{
//		for (int i = 0; i < PLAYER_MAX; i++)
//		{
//			
//			g_nCnt5++;
//			
//			if (g_nCnt5 >= 1 && g_nCnt5 <= 100)
//			{
//				
//				if (g_nCnt5 <= 19 && g_nCnt5 >= 10)
//				{
//					g_player[i].pos.z += 100;
//				}
//				if (g_nCnt5 <= 24 && g_nCnt5 >= 15)
//				{
//					
//				}
//				if (g_nCnt5 <= 54 && g_nCnt5 >= 25)
//				{
//					g_player[i].rot.y = 0.0f;
//					g_player[i].rot.x -= 18.0f;
//					g_player[i].pos.z -= 4;
//					g_player[i].pos.y += 1;
//				}
//				if (g_nCnt5 <= 84 && g_nCnt5 >= 55)
//				{
//					//g_player[i].rot.y = 0.0f;
//					g_player[i].rot.x -= 18.0f;
//					g_player[i].pos.z -= 4;
//					g_player[i].pos.y -= 1;
//				}
//				if (g_nCnt5 <= 110 && g_nCnt5 >= 90)
//				{
//
//				}
//
//				
//			}
//			if (g_nCnt5 >= 200)
//			{
//				ResetPos(i);
//			}
//			
//		}
//		
//	}
//	
//	
//}

void DestroyPlayer(int no)
{
	if (no < 0 || no >= PLAYER_MAX)	return;

	//g_player[no].nState = 0;

	//�e�̊J��
	ReleaseShadow(g_player[no].nShadowIdx);
	g_player[no].nShadowIdx = -1;

	g_nCntF++;
	if (g_nCntF>=250)
	{
		Clearflag();
	}
	
}

void CameraDEBUG()
{
	g_nCameraType = E_CAMERA_VIEW_CAMERA_DEBUG;
}
void CameraFIXED()
{
	g_nCameraType = E_CAMERA_VIEW_FIXED;
}