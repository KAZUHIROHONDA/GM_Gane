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

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_PLAYER		"data/model/kabuBody.obj"
#define MODEL_PLAYER1		"data/model/kabu2.obj"
#define MODEL_PLAYER2		"data/model/kabu3.obj"
#define MODEL_PLAYER3		"data/model/kabu4.obj"


#define VALUE_MOVE		(3.0f)		
#define VALUE_ROTATE	(1.0f)


#define PLAYER_GAUGE_POS_X	(-125.0f)
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

	int             nHP;

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

static float			g_nCnt;//�s���J�E���g

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
		g_player[i].pos = XMFLOAT3(0.0f, 20.0f, -150.0f);
		g_player[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_player[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_player[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		//������
		g_player[i].nPhase = 0;
		g_player[i].nState = 1;		//�ŏ��͒ʏ�

		g_player[i].nHP = 100;
		g_player[i].nName = 0;

		g_player[i].bCnt = 0;	

		g_nCnt = 0.0f;				//

		g_player[i].nGauge = 0;
		g_player[i].nStopTime = 0; //�ŏ��͓�����

		g_player[i].nShadowIdx = CreateShadow(g_player[i].pos, 20.0f);

		//��
		g_playerHD[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
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
		MODEL_PLAYER); if (FAILED(hr)) return hr;

	hr = g_model[1].Load(pDevice, pDeviceContext,
		MODEL_PLAYER1); if (FAILED(hr)) return hr;

	hr = g_model[2].Load(pDevice, pDeviceContext,
		MODEL_PLAYER2); if (FAILED(hr)) return hr;

	hr = g_model[3].Load(pDevice, pDeviceContext,
		MODEL_PLAYER3); if (FAILED(hr)) return hr;
	
	g_nCameraType = E_CAMERA_VIEW_BEHIND;
	
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
			//�Q�[���p�b�g�ňړ�
			if (GetJoyCountSimple() == 1)
			{
				float gx, gy, len2;
				gx = (float)GetJoyX(0) / 0x08000; //-1.0~1.0
				gy = (float)GetJoyY(0) / 0x08000; //�Ɋۂ߂�

				len2 = gx * gx + gy * gy;

				if (len2 > 0.3f*0.3f)
				{
					//�V�т��Ȃ�
					float angle, len;
					angle = atan2f(gy, gx);//�p�x
					angle = XMConvertToDegrees(angle);

					len = sqrtf(len2);

					if (angle < -135 || angle > -45)
						len *= 0.3f;
					g_player[i].vel.x = SinDeg(g_player[i].rot.y + angle + 90)*len*VALUE_MOVE;
					g_player[i].vel.z = CosDeg(g_player[i].rot.y + angle + 90)*len*VALUE_MOVE;

				}
			}

			//��]
			if (GetKeyPress(VK_Q) || GetJoyButton(0, 4))
			{
				g_player[i].rot.y -= VALUE_ROTATE;
			}
			else if (GetKeyPress(VK_E) || GetJoyButton(0, 5))
			{
				g_player[i].rot.y += VALUE_ROTATE;
			}

			//�ړ�
			//�����Ă�������Ɉړ�
			if (GetKeyPress(VK_W))//�O
			{
				g_player[i].vel.x = SinDeg(g_player[i].rot.y)*VALUE_MOVE;
				g_player[i].vel.z = CosDeg(g_player[i].rot.y)*VALUE_MOVE;
			}
			if (GetKeyPress(VK_S))//
			{
				g_player[i].vel.x = -SinDeg(g_player[i].rot.y)*VALUE_MOVE / 2;
				g_player[i].vel.z = -CosDeg(g_player[i].rot.y)*VALUE_MOVE / 2;
			}
			if (GetKeyPress(VK_D))//
			{
				g_player[i].vel.x = SinDeg(g_player[i].rot.y + 90)*VALUE_MOVE / 10;
				g_player[i].vel.z = CosDeg(g_player[i].rot.y + 90)*VALUE_MOVE / 10;
			}
			if (GetKeyPress(VK_A))//
			{
				g_player[i].vel.x = SinDeg(g_player[i].rot.y - 90)*VALUE_MOVE / 2;
				g_player[i].vel.z = CosDeg(g_player[i].rot.y - 90)*VALUE_MOVE / 2;
			}

			////�J�[�\���Ő�ΓI��xz�����Ɉړ�
			//if (GetKeyPress(VK_UP))
			//{
			//	g_player[i].vel.z = VALUE_MOVE;
			//}
			//else if (GetKeyPress(VK_DOWN))
			//{
			//	g_player[i].vel.z = -VALUE_MOVE;
			//}
			//else if (GetKeyPress(VK_RIGHT))
			//{
			//	g_player[i].vel.x = VALUE_MOVE;
			//}
			//else if (GetKeyPress(VK_LEFT))
			//{
			//	g_player[i].vel.x = -VALUE_MOVE;
			//}


			////�W�����v
			if (GetKeyPress(VK_X))
			{
				g_nCnt++;
				if (g_nCnt <= 50)
				{
					g_player[i].pos.z++;

					if (50 < g_nCnt <= 100)
					{
						g_player[i].pos.y++;
					}
				}
				
				//g_player[i].vel.y += 10.0f;

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

		}

		//�d��
		//g_player[i].vel.y += -0.98f;


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

			//������(��~)
			//g_player[i].vel.x = 0.0f;
			//g_player[i].vel.z = 0.0f;
			g_player[i].vel.x *= 0.9f;
			g_player[i].vel.z *= 0.9f;
		}

		if (GetKeyTrigger(VK_O))
		{
			SetMessage((tMessage*)&testMessage[3]);
		}

		////�p���[�Q�[�W
		//if (g_player[i].bCnt < 3)
		//{
		//	if (GetKeyPress(VK_Z) || GetJoyButton(0, 0))
		//	{
		//		//�������ςȂ��ł��߂�
		//		g_player[i].nGauge++;
		//		if (g_player[i].nGauge >= 100)
		//			g_player[i].nGauge = 100;
		//	}
		//	else if (GetKeyRelease(VK_Z) || GetJoyRelease(0, 0))
		//	{
		//		g_player[i].bCnt++;
		//		XMFLOAT3 pos = g_player[i].pos;
		//		pos.y += 15.0f;	//�␳
		//		FireBullet(pos, XMFLOAT3(SinDeg(g_player[i].rot.y), 0.0f, CosDeg(g_player[i].rot.y)), g_player[i].nGauge / 5);
		//		PlaySound(SOUND_LABEL_SE_SELECT);
		//		g_player[i].nGauge = 0;
		//	}
		//}


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
		GetCamera()->SetTarget(1.0f, 1.0f, 0.0f);
		GetCamera()->SetPos(0.0f, 500.0f, 0.0f);
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

//�G�ɂ����������̏���
/*void DamagePlayer(int no, XMFLOAT3 vec)
{
	
	if (no < 0 || no >= PLAYER_MAX) return;
	g_player[no].vel = vec;
	g_player[no].nStopTime = 4;

}*/

//���̃p���[�Q�[�W
void DrawPlayerGauge()
{
	const XMFLOAT2 pos = { PLAYER_GAUGE_POS_X, PLAYER_GAUGE_POS_Y };

	DrawGauge(pos, (float)g_player[0].nGauge / GAUGE_MAX);
}

int GetPlayerHp(int no)
{
	if (no < 0 || no >= PLAYER_MAX)	return(0);

	return g_player[no].nHP;


}
void DamagePlayer(int damage)
{
	
	g_player[0].nHP -= damage;
	if (g_player[0].nHP <= 0)
	{
		g_player[0].nHP = 0;

		DestroyPlayer(0);


		//StartSceneChange(SCENE_GAMECLEAR);
	}
}

void DestroyPlayer(int no)
{
	if (no < 0 || no >= PLAYER_MAX)	return;

	g_player[no].nState = 0;

	//�e�̊J��
	ReleaseShadow(g_player[no].nShadowIdx);
	g_player[no].nShadowIdx = -1;

	StartFade(SCENE_GAMEOVER);

}