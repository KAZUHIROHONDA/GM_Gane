//=============================================================================
//
// �v���C���[���� [pchimera.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "AssimpModel.h"
#include "pchimera.h"
#include "main.h"
#include "input.h"
#include "shadow.h"		//�e������ɂ͓����
#include "model.h"
#include "sceneGame.h"
#include "sceneTitle.h"
#include "partsmenu.h"
#include "Upartsmenu.h"
#include "Mpartsmenu.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define VALUE_MOVE		(3.0f)		
#define VALUE_ROTATE	(1.0f)

//�\���̒�`
typedef struct _tPChimera
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

				
}tPChimera;

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static CAssimpModel		g_model[4];				//���f���f�[�^�i�[�p
			
static tPChimera			playermodel[PCHIMERA_MAX];	//�v���C���[
static tPChimera			playermodelHD[PCHIMERA_MAX];
static tPChimera			playermodelAM[PCHIMERA_MAX];
static tPChimera			playermodelLG[PCHIMERA_MAX];
static int				g_nCameraType = CAMERA_VIEW_FIXED;		//�J�����̎��

static bool				g_atama = true;//false;

float CheckCollisionRay(XMVECTOR pos, float fRadius);

bool sflag = false;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPChimera(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	for (int i = 0; i < PCHIMERA_MAX; i++)
	{
		playermodel[i].pos = XMFLOAT3(0.0f, 0.0f, -60.0f);
		playermodel[i].rot = XMFLOAT3(0.0f, 180.0f, 0.0f);
		playermodel[i].scl = GetPlayer()->Getscl();
		playermodel[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		//������
		playermodel[i].nPhase = 0;
		playermodel[i].nState = 1;		//�ŏ��͒ʏ�

		playermodel[i].nHP = 1000;
		playermodel[i].nName = 0;

		playermodel[i].bCnt = 0;	

		//g_nCnt = 0.0f;				//


		playermodel[i].nGauge = playermodel[i].nHP/10;
		playermodel[i].nStopTime = 0; //�ŏ��͓�����

		playermodel[i].nShadowIdx = CreateShadow(playermodel[i].pos, 20.0f);

		//��
		playermodelHD[i].pos = PartsGet()->Getpos();
		playermodelHD[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		playermodelHD[i].scl = PartsGet()->Getscl();
		playermodelHD[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		playermodelHD[i].nPhase = 0;
		playermodelHD[i].nState = 1;	// puro�؂�͍ŏ�����o�ꂵ�Ă���
		playermodelHD[i].nShadowIdx = -1;

		//�r
		playermodelAM[i].pos = MPartsGet()->Getpos();
		playermodelAM[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		playermodelAM[i].scl = MPartsGet()->Getscl();
		playermodelAM[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		playermodelAM[i].nPhase = 0;
		playermodelAM[i].nState = 1;	// puro�؂�͍ŏ�����o�ꂵ�Ă���
		playermodelAM[i].nShadowIdx = -1;

		//��
		playermodelLG[i].pos = UPartsGet()->Getpos();
		playermodelLG[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		playermodelLG[i].scl = UPartsGet()->Getscl();
		playermodelLG[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		playermodelLG[i].nPhase = 0;
		playermodelLG[i].nState = 1;	// puro�؂�͍ŏ�����o�ꂵ�Ă���
		playermodelLG[i].nShadowIdx = -1;

		if (sflag == true)
		{
			playermodel[i].rot = XMFLOAT3(0.0f, 100.0f, 0.0f);
			g_nCameraType = CAMERA_VIEW_BIRD;
		}
	}

	// ���[���h�}�g���b�N�X�̏�����
	XMMATRIX mtxWorld;
	mtxWorld = XMMatrixIdentity();

	for (int i = 0; i < PCHIMERA_MAX; i++)
	{
		XMStoreFloat4x4(&playermodel[i].mtxWorld, mtxWorld);
		XMStoreFloat4x4(&playermodelHD[i].mtxWorld, mtxWorld);
		XMStoreFloat4x4(&playermodelAM[i].mtxWorld, mtxWorld);
		XMStoreFloat4x4(&playermodelLG[i].mtxWorld, mtxWorld);

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
	
	g_nCameraType = CAMERA_VIEW_FIXED;

	sflag = true;
	
	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPChimera(void)
{
	//�e�̉��
	for (int i = 0; i < PCHIMERA_MAX; i++)
	{
		ReleaseShadow(playermodel[i].nShadowIdx);
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
void UpdatePChimera(void)
{
	
	// �v���C���[�̓������L�q����Ȃ炱��
	for (int i = 0; i < PCHIMERA_MAX; i++)
	{
		
		//���g�p
		if (playermodel[i].nState == 0) continue;


		//����s�\����
		if (playermodel[i].nStopTime > 0)
		{
			playermodel[i].nStopTime--;
		}


		//���x�����W�ɉ��Z
		playermodel[i].pos.x += playermodel[i].vel.x;
		playermodel[i].pos.y += playermodel[i].vel.y;
		playermodel[i].pos.z += playermodel[i].vel.z;

		if (sflag == true)
		{
			playermodel[i].pos = XMFLOAT3(50.0f, -50.0f, 0.0f);
			g_nCameraType = CAMERA_VIEW_BIRD;
		}

		playermodelHD[i].scl = PartsGet()->Getscl();
		playermodelAM[i].scl = MPartsGet()->Getscl();
		playermodelLG[i].scl = UPartsGet()->Getscl();

		//���n����
		//if (playermodel[i].pos.y <= -50.0f)
		//{
		//	playermodel[i].vel.y = 0.0f;
		//	playermodel[i].pos.y = 0.0f;
		//	playermodel[i].vel.x *= 0.9f;
		//	playermodel[i].vel.z *= 0.9f;
		//}

		// 3D���W�ɕϊ����ē����蔻������
		float hitLength;

		//�ʒu�Ɣ��a���w��
		hitLength = CheckCollisionRay(XMVectorSet(playermodel[i].pos.x, playermodel[i].pos.y, playermodel[i].pos.z, 0.0f), 10.0f);
		if (hitLength < 9999.9f)
		{
			playermodel[i].rot.y += 1.0f;
			if (GetMouseTrigger(0))
			{
				//�A�N�V�������ꂽ��

			}
		}
		else
		{
			playermodel[i].rot = XMFLOAT3(0.0f, 100.0f, 0.0f);
		}


		XMMATRIX mtxWorld, mtxRot, mtxScl,
			mtxTranslate;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(playermodel[i].scl.x, playermodel[i].scl.y, playermodel[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);



		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(playermodel[i].rot.x), XMConvertToRadians(playermodel[i].rot.y + 180), XMConvertToRadians(playermodel[i].rot.z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);


		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(playermodel[i].pos.x, playermodel[i].pos.y, playermodel[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);


		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&playermodel[i].mtxWorld, mtxWorld);

		//�e�̈ړ�
		MoveShadow(playermodel[i].nShadowIdx, playermodel[i].pos);


		//************************************************:
		//��

		XMMATRIX mtxWorldHD;
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorldHD = XMMatrixIdentity();
		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(playermodelHD[i].scl.x, playermodelHD[i].scl.y, playermodelHD[i].scl.z);
		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxScl);
		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(playermodelHD[i].rot.x),
			XMConvertToRadians(playermodelHD[i].rot.y),//+180
			XMConvertToRadians(playermodelHD[i].rot.z)
		);
		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(playermodelHD[i].pos.x, playermodelHD[i].pos.y, playermodelHD[i].pos.z);

		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxTranslate);

		//�e�̍s����|����
		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxWorld);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&playermodelHD[i].mtxWorld, mtxWorldHD);


		//************************************************:
		//�r

		XMMATRIX mtxWorldAM;
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorldAM = XMMatrixIdentity();
		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(playermodelAM[i].scl.x, playermodelAM[i].scl.y, playermodelAM[i].scl.z);
		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxScl);
		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(playermodelAM[i].rot.x),
			XMConvertToRadians(playermodelAM[i].rot.y),//+180
			XMConvertToRadians(playermodelAM[i].rot.z)
		);
		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(playermodelAM[i].pos.x, playermodelAM[i].pos.y, playermodelAM[i].pos.z);

		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxTranslate);

		//�e�̍s����|����
		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxWorld);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&playermodelAM[i].mtxWorld, mtxWorldAM);

		//************************************************:
		//��

		XMMATRIX mtxWorldLG;
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorldLG = XMMatrixIdentity();
		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(playermodelLG[i].scl.x, playermodelLG[i].scl.y, playermodelLG[i].scl.z);
		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxScl);
		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(playermodelLG[i].rot.x),
			XMConvertToRadians(playermodelLG[i].rot.y),//+180
			XMConvertToRadians(playermodelLG[i].rot.z)
		);
		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(playermodelLG[i].pos.x, playermodelLG[i].pos.y, playermodelLG[i].pos.z);

		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxTranslate);

		//�e�̍s����|����
		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxWorld);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&playermodelLG[i].mtxWorld, mtxWorldLG);

	}
#ifdef CAMERA_DEBUG
	if (GetKeyTrigger(VK_1))
		g_nCameraType = CAMERA_VIEW_FIXED;
	if (GetKeyTrigger(VK_2))
		g_nCameraType = CAMERA_VIEW_BIRD;
	if (GetKeyTrigger(VK_3))
		g_nCameraType = CAMERA_VIEW_BEHIND;
	if (GetKeyTrigger(VK_4))
		g_nCameraType = CAMERA_VIEW_FPS;
	if (GetKeyTrigger(VK_5))
		g_nCameraType = CAMERA_VIEW_CAMERA_DEBUG;
#endif

	switch (g_nCameraType)
	{
	case CAMERA_VIEW_FIXED:
		GetCamera()->SetTarget(1.0f, 1.0f, -1.0f);
		GetCamera()->SetPos(180.0f, 40.0f, -1.0f);
		break;
	case CAMERA_VIEW_BIRD:
		GetCamera()->SetTarget(1.0f, 1.0f, -1.0f);
		GetCamera()->SetPos(250.0f, 100.0f, 0.0f);
		break;
	case CAMERA_VIEW_BEHIND:
		GetCamera()->SetTarget(playermodel[0].pos.x + SinDeg(playermodel[0].rot.y)*100.0f, playermodel[0].pos.y + 15.0f, playermodel[0].pos.z + CosDeg(playermodel[0].rot.y)*100.0f);
		GetCamera()->SetPos(playermodel[0].pos.x - SinDeg(playermodel[0].rot.y)*40.0f, playermodel[0].pos.y + 20.0f, playermodel[0].pos.z - CosDeg(playermodel[0].rot.y)*40.0f);
		break;						
	case CAMERA_VIEW_FPS:
		GetCamera()->SetTarget(playermodel[0].pos.x + SinDeg(playermodel[0].rot.y)*100.0f, playermodel[0].pos.y + 10.0f, playermodel[0].pos.z + CosDeg(playermodel[0].rot.y)*100.0f);
		GetCamera()->SetPos(playermodel[0].pos.x + SinDeg(playermodel[0].rot.y)*10.0f, playermodel[0].pos.y + 20.0f, playermodel[0].pos.z + CosDeg(playermodel[0].rot.y)*10.0f);
		break;
	case CAMERA_VIEW_DIAGONAL:
		GetCamera()->SetTarget(0.0f, 0.0f, 0.0f);
		GetCamera()->SetPos(0.0f, 100.0f, -200.0f);
		break;
	case CAMERA_VIEW_CAMERA_DEBUG:break;
	}
}



//=============================================================================
// �`�揈��
//=============================================================================
void DrawPChimera(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	for (int i = 0; i < PCHIMERA_MAX; i++)
	{
		//���g�p
		if (playermodel[i].nState == 0) continue;

		// ���f���\��
		g_model[0].Draw(pDeviceContext, playermodel[i].mtxWorld);
		g_model[1].Draw(pDeviceContext, playermodelHD[i].mtxWorld);
		g_model[2].Draw(pDeviceContext, playermodelAM[i].mtxWorld);
		g_model[3].Draw(pDeviceContext, playermodelLG[i].mtxWorld);
	}
}

//���W�擾
XMFLOAT3 GetPChimeraPos(int no)
{
	if (no < 0 || no >= PCHIMERA_MAX)
		return XMFLOAT3(0.0f, 0.0f, 0.0f);
	return playermodel[no].pos;
}

//�T�C�Y�擾
XMFLOAT3 GetPChimeraSize(int no)
{
	if (no < 0 || no >= PCHIMERA_MAX)
		return XMFLOAT3(0, 0, 0);
	//return XMFLOAT3(10.0f,10.0f,10.0f);
	XMFLOAT3 box = g_model[0].GetBBox();
	return XMFLOAT3(box.x / 2 * playermodel[no].scl.x, box.y / 2 * playermodel[no].scl.y, box.z / 2 * playermodel[no].scl.z);
}

float CheckCollisionRay(XMVECTOR pos, float fRadius)
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

void Sflag()
{
	sflag = false;
}