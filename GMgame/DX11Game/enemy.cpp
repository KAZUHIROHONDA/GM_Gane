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
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_ENEMY		"data/model/wani2.obj"
#define MODEL_ENEMY1	"data/model/wani1.obj"
#define MODEL_ENEMY2	"data/model/wani3.obj"
#define MODEL_ENEMY3	"data/model/wani4.obj"

#define VALUE_MOVE		(0.1f)		
#define VALUE_ROTATE	(2.0f)

#define ENEMY_RADIUS  (10.0f)

#define ENEMY_EGAUGE_POS_X	(125.0f)
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


static int			g_nCnt1;//�s���J�E���g1�R��
static int			g_nCnt2;//�s���J�E���g2�R��
static int			g_nCnt3;//�s���J�E���g3�R��
static int			g_nCnt4;//�s���J�E���g4�R��
static bool				g_atama = true;//false;

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
		g_enemy[i].pos = XMFLOAT3(0.0f, 20.0f, 20.0f);
		g_enemy[i].rot = XMFLOAT3(0.0f, 180.0f, 0.0f);
		g_enemy[i].scl = XMFLOAT3(15.0f, 15.0f, 15.0f);
		g_enemy[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		//������
		g_enemy[i].nPhase = 0;
		g_enemy[i].nState = 1;		//�ŏ��͒ʏ�
		g_enemy[i].Tipe = 0;

		g_enemy[i].nHP = 1000;
		g_enemy[i].nEGauge = g_enemy[i].nHP / 10;
		g_enemy[i].nShadowIdx = -1;


		//��
		g_enemyHD[i].pos = XMFLOAT3(0.0f, 0.0f, -3.0f);
		g_enemyHD[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyHD[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_enemyHD[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyHD[i].nPhase = 0;
		g_enemyHD[i].nState = 1;	// puro�؂�͍ŏ�����o�ꂵ�Ă���
		g_enemyHD[i].nShadowIdx = -1;

		//�r
		g_enemyAM[i].pos = XMFLOAT3(0.0f, -0.8f, -1.0f);
		g_enemyAM[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyAM[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_enemyAM[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyAM[i].nPhase = 0;
		g_enemyAM[i].nState = 1;	// puro�؂�͍ŏ�����o�ꂵ�Ă���
		g_enemyAM[i].nShadowIdx = -1;

		//��
		g_enemyLG[i].pos = XMFLOAT3(0.0f, -0.8f, 0.0f);
		g_enemyLG[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyLG[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_enemyLG[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyLG[i].nPhase = 0;
		g_enemyLG[i].nState = 1;	// puro�؂�͍ŏ�����o�ꂵ�Ă���
		g_enemyLG[i].nShadowIdx = -1;

		g_nCnt1 = 0;					//
		g_nCnt2 = 0;					//
		g_nCnt3 = 0;
		g_nCnt4 = 0;

		g_enemy[i].nStopTime = 0; //�ŏ��͓�����

		g_enemy[i].nShadowIdx = CreateShadow(g_enemy[i].pos, 20.0f);
	}
	
	bfCnt = 0;

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

		if (GetKeyPress(VK_Z))
		{
			//g_enemy[i].rot.y+=5;
			g_enemyHD[i].rot.z += 5;
			//g_enemyAM[i].rot.x += 5;
			g_enemyLG[i].rot.x += 5;

		}


		if (GetKeyPress(VK_7))
		{
			g_nCnt1++;
			g_enemy[i].pos.z = 30;
			g_enemy[i].rot.x = 30;
		}




		g_nCnt4++;
		if (GetKeyPress(VK_0))//�����A�j���[�V����
		{
			g_enemy[i].rot.z = 180;
			if (g_nCnt4 >= 20)
			{
				g_enemyHD[i].pos.z -= 0.05f;
				g_enemyHD[i].rot.z -= 1.0f;
				g_enemyHD[i].rot.y -= 1.0f;

				g_enemyAM[i].pos.x -= 0.05f;
				g_enemyAM[i].rot.z -= 1.0f;
				g_enemyAM[i].rot.y -= 1.0f;

				g_enemyLG[i].pos.z += 0.05f;
				g_enemyLG[i].rot.z -= 1.0f;
				g_enemyLG[i].rot.y -= 1.0f;
			}

		}




		//�Q�[�W�̓����̏���
		if (g_enemy[i].nEGauge >= g_enemy[i].nHP / 10)
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
/*int SetEnemy(XMFLOAT3 pos,int no)
{
	tEnemy* pEnemy = g_enemy;
	for (int i = 0; i < ENEMY_MAX; ++i, ++pEnemy)
	{	// �o�����Ă���G�̓X���[
		if (pEnemy->nState > 0) continue;

		pEnemy->pos = pos;
		switch (no)
		{  
		case 0:
		{
			g_enemy[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
			g_enemy[i].Tipe = 0;
		} break;
		case 1:
		{
			g_enemy[i].vel = XMFLOAT3(-VALUE_MOVE, 0.0f, 0.0f);
			g_enemy[i].Tipe = 1;
		} break;
		case 2: 
		{
			g_enemy[i].vel = XMFLOAT3(VALUE_MOVE,  0.0f, 0.0f);
			g_enemy[i].Tipe = 1; 
		} break;
		case 3: 
		{
			g_enemy[i].vel = XMFLOAT3(VALUE_MOVE*5, 0.0f, 0.0f);
			g_enemy[i].Tipe = 2; 
		} break;
		case 4:
		{
			g_enemy[i].vel = XMFLOAT3(-VALUE_MOVE*5, 0.0f, 0.0f);
			g_enemy[i].Tipe = 2;
		} break;
		case 5:
		{
			g_enemy[i].vel = XMFLOAT3( 0.0f, 0.0f, -VALUE_MOVE*5);
			g_enemy[i].Tipe = 2;
		} break;
		case 6:
		{
			g_enemy[i].vel = XMFLOAT3( 0.0f, 0.0f, VALUE_MOVE*5);
			g_enemy[i].Tipe = 2;
		} break;
		case 7:
		{
			g_enemy[i].vel = XMFLOAT3(-VALUE_MOVE * 5, 0.0f, -VALUE_MOVE * 5);
			g_enemy[i].Tipe = 2;
		} break;
		case 8:
		{
			g_enemy[i].vel = XMFLOAT3(VALUE_MOVE * 5, 0.0f, VALUE_MOVE * 5);
			g_enemy[i].Tipe = 2;
		} break;
		}
		pEnemy->nState = 1;	// �o��
		pEnemy->nShadowIdx =
			CreateShadow(pEnemy->pos, ENEMY_RADIUS);
		//pEnemy->rot.y = XMConvertToDegrees(atan2f(dir.x, dir.z));//���f���̌���
		return i;	// �ԍ���Ԃ�
	}

	return -1;	// ������Ȃ�����
}*/


int GetEnemyHp(int no)
{
	if (no < 0 || no >= ENEMY_MAX)	return(0);

	return g_enemy[no].nHP;


}

void DamageEnemy(int damage)
{

	g_enemy[0].nHP -= damage;
	if (g_enemy[0].nHP <= 0)
	{
		g_enemy[0].nHP = 0;

		DestroyEnemy(0);


		//StartSceneChange(SCENE_GAMECLEAR);
	}
}

void DestroyEnemy(int no)
{
	if (no < 0 || no >= ENEMY_MAX)	return;

	g_enemy[no].nState = 0;

	//�e�̊J��
	ReleaseShadow(g_enemy[no].nShadowIdx);
	g_enemy[no].nShadowIdx = -1;

	StartFade(SCENE_CLEAR);

}
void DrawEnemyEGauge()
{
	const XMFLOAT2 pos = { ENEMY_EGAUGE_POS_X, ENEMY_EGAUGE_POS_Y };

	DrawEGauge(pos, (float)g_enemy[0].nEGauge / EGAUGE_MAX);
}

void ResetEPos(int no)
{
	if (no < 0 || no >= ENEMY_MAX)	return;

	g_enemy[no].pos = XMFLOAT3(0.0f, 20.0f, 20.0f);
	g_enemy[no].rot = XMFLOAT3(0.0f, 180.0f, 0.0f);
	g_enemy[no].scl = XMFLOAT3(8.0f, 8.0f, 8.0f);
	g_enemy[no].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_enemyHD[no].pos = XMFLOAT3(0.0f, 0.0f, -3.0f);
	g_enemyHD[no].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_enemyHD[no].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
	g_enemyHD[no].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);

	g_nCnt1 = 0;
	g_nCnt2 = 0;
}