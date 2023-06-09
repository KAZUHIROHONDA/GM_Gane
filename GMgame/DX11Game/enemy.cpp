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

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_ENEMY		"data/model/kamereon.obj"
//#define MODEL_ENEMY		"data/model/wani.obj"
#define MODEL_ENEMY01	"data/model/enemy01.fbx"
#define MODEL_ENEMY02	"data/model/enemy02.fbx"

#define VALUE_MOVE		(0.1f)		
#define VALUE_ROTATE	(2.0f)

#define ENEMY_RADIUS  (10.0f)

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

	int				nHP;

	int				nShadowIdx;		//�e�̔ԍ�

}tEnemy;

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static CAssimpModel		g_model[3];				//���f���f�[�^�i�[�p
static tEnemy			g_enemy[ENEMY_MAX];	//�v���C���[

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
		g_enemy[i].pos = XMFLOAT3(0.0f, 0.0f, 20.0f);
		g_enemy[i].rot = XMFLOAT3(0.0f, 180.0f, 0.0f);
		g_enemy[i].scl = XMFLOAT3(3.0f,3.0f,3.0f);
		g_enemy[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		//������
		g_enemy[i].nPhase = 1;
		g_enemy[i].nState = 0;		//�ŏ��͒ʏ�
		g_enemy[i].Tipe = 0;

		g_enemy[i].nHP = 1000;

		g_enemy[i].nShadowIdx = -1;
	}
	
	bfCnt = 0;

	// ���[���h�}�g���b�N�X�̏�����
	XMMATRIX mtxWorld;
	mtxWorld = XMMatrixIdentity();

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		XMStoreFloat4x4(&g_enemy[i].mtxWorld, mtxWorld);
	}

	// ���f���f�[�^�̓ǂݍ���
	hr = g_model[0].Load(pDevice, pDeviceContext,MODEL_ENEMY);
	// ���f���f�[�^�̓ǂݍ���
	hr = g_model[1].Load(pDevice, pDeviceContext,MODEL_ENEMY01);
	// ���f���f�[�^�̓ǂݍ���
	hr = g_model[2].Load(pDevice, pDeviceContext, MODEL_ENEMY02);
	
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

		//���x
		g_enemy[i].pos.x += g_enemy[i].vel.x;
		g_enemy[i].pos.y += g_enemy[i].vel.y;
		g_enemy[i].pos.z += g_enemy[i].vel.z;

		// ���f���̓������L�q����Ȃ炱��
		switch (g_enemy[i].nPhase)
		{
		case 1:g_enemy[i].pos.y+= VALUE_MOVE;				//�E��
			if (g_enemy[i].pos.y>= 15.0f) g_enemy[i].nPhase = -1;
			break;

		case -1:g_enemy[i].pos.y -= VALUE_MOVE;				//����
			if (g_enemy[i].pos.y <= 5.0f) g_enemy[i].nPhase = +1;
			break;
		}

		//��ʊO�ɍs���Ȃ�����
		XMFLOAT3 box = g_model[0].GetBBox();
		//[���[] 
		if (g_enemy[i].pos.x < -200 + box.x * g_enemy[i].scl.x)
		{
			g_enemy[i].pos.x = -200 + box.x * g_enemy[i].scl.x;
			g_enemy[i].vel.x *= -1.0f;
		}
		//[�E�[]
		else if (g_enemy[i].pos.x > 200 - box.x * g_enemy[i].scl.x)
		{
			g_enemy[i].pos.x = 200 - box.x * g_enemy[i].scl.x;
			g_enemy[i].vel.x *= -1.0f;
		}
		//[���[] �v���C���[�̍��[����ʂ̏�[
		if (g_enemy[i].pos.z > 200 - box.z * g_enemy[i].scl.z)
		{
			g_enemy[i].pos.z = 200 - box.z * g_enemy[i].scl.z;
			g_enemy[i].vel.z *= -1.0f;
		}
		//[��[] �v���C���[�̍��[����ʂ̉��[
		else if (g_enemy[i].pos.z < -200 + box.z * g_enemy[i].scl.z)
		{
			g_enemy[i].pos.z = -200 + box.z * g_enemy[i].scl.z;
			g_enemy[i].vel.z *= -1.0f;
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
		switch (g_enemy[i].Tipe)
		{  //�����ő��x��ς���
		case 0:
		{
			g_model[0].Draw(pDeviceContext, g_enemy[i].mtxWorld);

		} break;
		case 1:
		{
			g_model[1].Draw(pDeviceContext, g_enemy[i].mtxWorld);
		} break;
		case 2:
		{
			g_model[2].Draw(pDeviceContext, g_enemy[i].mtxWorld); 
		} break;
		
		}

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
int SetEnemy(XMFLOAT3 pos,int no)
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
}


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