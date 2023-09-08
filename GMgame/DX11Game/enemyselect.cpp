//=============================================================================
//
// �G���� [enemyselect.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "AssimpModel.h"
#include "enemyselect.h"
#include "main.h"
#include "shadow.h"		//�e������ɂ͓����
#include "flame.h"
#include"input.h"
#include"fade.h"
#include "Egauge.h"
#include "sceneTitle.h"
#include "sceneGame.h"
#include "enemymodel.h"
#include "sound.h"

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


#define MODELMAX	(40)

//�\���̒�`
typedef struct _tEnemyselect
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

}tEnemyselect;

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static CAssimpModel			g_model[MODELMAX];				//���f���f�[�^�i�[�p
static tEnemyselect			g_enemyselect[ENEMYSEL_MAX];	//�v���C���[
static tEnemyselect			g_enemyselectHD[ENEMYSEL_MAX];
static tEnemyselect			g_enemyselectAM[ENEMYSEL_MAX];
static tEnemyselect			g_enemyselectLG[ENEMYSEL_MAX];

static int enemyno;//�G�̔ԍ�

//=============================================================================
// ����������
//=============================================================================
HRESULT InitSelectEnemyselect(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	for (int i = 0; i < ENEMYSEL_MAX; i++)
	{
		g_enemyselect[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);//z100
		g_enemyselect[i].rot = XMFLOAT3(0.0f, 180.0f, 0.0f);
		g_enemyselect[i].scl = EnemyScal(i * 4);
		g_enemyselect[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);

		//������
		g_enemyselect[i].nPhase = 0;
		g_enemyselect[i].nState = 0;		//�ŏ��͒ʏ�
		g_enemyselect[i].Tipe = 0;

		g_enemyselect[i].nShadowIdx = -1;

		//�� wani
		g_enemyselectHD[i].pos = EnemyScal(1 + i * 4);//z-3
		g_enemyselectHD[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyselectHD[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_enemyselectHD[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyselectHD[i].nPhase = 0;
		g_enemyselectHD[i].nState = 1;	// puro�؂�͍ŏ�����o�ꂵ�Ă���
		g_enemyselectHD[i].nShadowIdx = -1;

		////�r-0.8f
		g_enemyselectAM[i].pos = EnemyScal(2 + i * 4);//-0.8,-1
		g_enemyselectAM[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyselectAM[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_enemyselectAM[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyselectAM[i].nPhase = 0;
		g_enemyselectAM[i].nState = 1;	// puro�؂�͍ŏ�����o�ꂵ�Ă���
		g_enemyselectAM[i].nShadowIdx = -1;

		//��
		g_enemyselectLG[i].pos = EnemyScal(3 + i * 4);
		g_enemyselectLG[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyselectLG[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_enemyselectLG[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyselectLG[i].nPhase = 0;
		g_enemyselectLG[i].nState = 1;	// puro�؂�͍ŏ�����o�ꂵ�Ă���
		g_enemyselectLG[i].nShadowIdx = -1;


		g_enemyselect[i].nStopTime = 0; //�ŏ��͓�����

		g_enemyselect[i].nShadowIdx = CreateShadow(g_enemyselect[i].pos, 20.0f);
	}

	// ���[���h�}�g���b�N�X�̏�����
	XMMATRIX mtxWorld;
	mtxWorld = XMMatrixIdentity();

	for (int i = 0; i < ENEMYSEL_MAX; i++)
	{
		XMStoreFloat4x4(&g_enemyselect[i].mtxWorld, mtxWorld);
		XMStoreFloat4x4(&g_enemyselectHD[i].mtxWorld, mtxWorld);
		XMStoreFloat4x4(&g_enemyselectAM[i].mtxWorld, mtxWorld);
		XMStoreFloat4x4(&g_enemyselectLG[i].mtxWorld, mtxWorld);

	}

	// ���f���f�[�^�̓ǂݍ���0
	hr = g_model[0].Load(pDevice, pDeviceContext,
		GetEnemychimera(0)->GetBody()); if (FAILED(hr)) return hr;

	hr = g_model[1].Load(pDevice, pDeviceContext,
		GetEnemychimera(0)->GetHead()); if (FAILED(hr)) return hr;

	hr = g_model[2].Load(pDevice, pDeviceContext,
		GetEnemychimera(0)->GetMae()); if (FAILED(hr)) return hr;

	hr = g_model[3].Load(pDevice, pDeviceContext,
		GetEnemychimera(0)->GetBack()); if (FAILED(hr)) return hr;

	// ���f���f�[�^�̓ǂݍ���1
	hr = g_model[4].Load(pDevice, pDeviceContext,
		GetEnemychimera(1)->GetBody()); if (FAILED(hr)) return hr;

	hr = g_model[5].Load(pDevice, pDeviceContext,
		GetEnemychimera(1)->GetHead()); if (FAILED(hr)) return hr;

	hr = g_model[6].Load(pDevice, pDeviceContext,
		GetEnemychimera(1)->GetMae()); if (FAILED(hr)) return hr;

	hr = g_model[7].Load(pDevice, pDeviceContext,
		GetEnemychimera(1)->GetBack()); if (FAILED(hr)) return hr;

	// ���f���f�[�^�̓ǂݍ���2
	hr = g_model[8].Load(pDevice, pDeviceContext,
		GetEnemychimera(2)->GetBody()); if (FAILED(hr)) return hr;

	hr = g_model[9].Load(pDevice, pDeviceContext,
		GetEnemychimera(2)->GetHead()); if (FAILED(hr)) return hr;

	hr = g_model[10].Load(pDevice, pDeviceContext,
		GetEnemychimera(2)->GetMae()); if (FAILED(hr)) return hr;

	hr = g_model[11].Load(pDevice, pDeviceContext,
		GetEnemychimera(2)->GetBack()); if (FAILED(hr)) return hr;

	// ���f���f�[�^�̓ǂݍ���3
	hr = g_model[12].Load(pDevice, pDeviceContext,
		GetEnemychimera(3)->GetBody()); if (FAILED(hr)) return hr;

	hr = g_model[13].Load(pDevice, pDeviceContext,
		GetEnemychimera(3)->GetHead()); if (FAILED(hr)) return hr;

	hr = g_model[14].Load(pDevice, pDeviceContext,
		GetEnemychimera(3)->GetMae()); if (FAILED(hr)) return hr;

	hr = g_model[15].Load(pDevice, pDeviceContext,
		GetEnemychimera(3)->GetBack()); if (FAILED(hr)) return hr;

	// ���f���f�[�^�̓ǂݍ���3
	hr = g_model[16].Load(pDevice, pDeviceContext,
		GetEnemychimera(4)->GetBody()); if (FAILED(hr)) return hr;

	hr = g_model[17].Load(pDevice, pDeviceContext,
		GetEnemychimera(4)->GetHead()); if (FAILED(hr)) return hr;

	hr = g_model[18].Load(pDevice, pDeviceContext,
		GetEnemychimera(4)->GetMae()); if (FAILED(hr)) return hr;

	hr = g_model[19].Load(pDevice, pDeviceContext,
		GetEnemychimera(4)->GetBack()); if (FAILED(hr)) return hr;

	// ���f���f�[�^�̓ǂݍ���4
	hr = g_model[20].Load(pDevice, pDeviceContext,
		GetEnemychimera(5)->GetBody()); if (FAILED(hr)) return hr;

	hr = g_model[21].Load(pDevice, pDeviceContext,
		GetEnemychimera(5)->GetHead()); if (FAILED(hr)) return hr;

	hr = g_model[22].Load(pDevice, pDeviceContext,
		GetEnemychimera(5)->GetMae()); if (FAILED(hr)) return hr;

	hr = g_model[23].Load(pDevice, pDeviceContext,
		GetEnemychimera(5)->GetBack()); if (FAILED(hr)) return hr;

	// ���f���f�[�^�̓ǂݍ���5
	hr = g_model[24].Load(pDevice, pDeviceContext,
		GetEnemychimera(6)->GetBody()); if (FAILED(hr)) return hr;

	hr = g_model[25].Load(pDevice, pDeviceContext,
		GetEnemychimera(6)->GetHead()); if (FAILED(hr)) return hr;

	hr = g_model[26].Load(pDevice, pDeviceContext,
		GetEnemychimera(6)->GetMae()); if (FAILED(hr)) return hr;

	hr = g_model[27].Load(pDevice, pDeviceContext,
		GetEnemychimera(6)->GetBack()); if (FAILED(hr)) return hr;

	// ���f���f�[�^�̓ǂݍ���6
	hr = g_model[28].Load(pDevice, pDeviceContext,
		GetEnemychimera(7)->GetBody()); if (FAILED(hr)) return hr;

	hr = g_model[29].Load(pDevice, pDeviceContext,
		GetEnemychimera(7)->GetHead()); if (FAILED(hr)) return hr;

	hr = g_model[30].Load(pDevice, pDeviceContext,
		GetEnemychimera(7)->GetMae()); if (FAILED(hr)) return hr;

	hr = g_model[31].Load(pDevice, pDeviceContext,
		GetEnemychimera(7)->GetBack()); if (FAILED(hr)) return hr;

	// ���f���f�[�^�̓ǂݍ���7
	hr = g_model[32].Load(pDevice, pDeviceContext,
		GetEnemychimera(8)->GetBody()); if (FAILED(hr)) return hr;

	hr = g_model[33].Load(pDevice, pDeviceContext,
		GetEnemychimera(8)->GetHead()); if (FAILED(hr)) return hr;

	hr = g_model[34].Load(pDevice, pDeviceContext,
		GetEnemychimera(8)->GetMae()); if (FAILED(hr)) return hr;

	hr = g_model[35].Load(pDevice, pDeviceContext,
		GetEnemychimera(8)->GetBack()); if (FAILED(hr)) return hr;

	// ���f���f�[�^�̓ǂݍ���8
	hr = g_model[36].Load(pDevice, pDeviceContext,
		GetEnemychimera(9)->GetBody()); if (FAILED(hr)) return hr;

	hr = g_model[37].Load(pDevice, pDeviceContext,
		GetEnemychimera(9)->GetHead()); if (FAILED(hr)) return hr;

	hr = g_model[38].Load(pDevice, pDeviceContext,
		GetEnemychimera(9)->GetMae()); if (FAILED(hr)) return hr;

	hr = g_model[39].Load(pDevice, pDeviceContext,
		GetEnemychimera(9)->GetBack()); if (FAILED(hr)) return hr;


	return hr;

}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemyselect(void)
{
	//�e�̉��
	for (int i = 0; i < ENEMYSEL_MAX; i++)
	{
		ReleaseShadow(g_enemyselect[i].nShadowIdx);
	}


	// ���f���̉��
	for (int i = 0; i < MODELMAX; i++)
	{
		g_model[i].Release();
	}
}

//=============================================================================
// �X�V����
//=============================================================================
//enemyselect�̃Z���N�g�A�b�v�f�[�g
void UpdateSelectEnemyselect(void)
{
	// �v���C���[�̓������L�q����Ȃ炱��
	for (int i = 0; i < ENEMYSEL_MAX; i++)
	{
		//���g�p
		if (g_enemyselect[i].nState == 0) continue;


		//�z�C�[����]
		int temp2 = GetMouseWheelDelta();
		wchar_t str[256];
		wsprintf(str,L"%d", temp2);
		OutputDebugString(str);
        g_enemyselect[i].pos.z += static_cast<float>(temp2) / 5;
		


		float hitLength[10];

		for (int i = 0; i < 10; i++)
		{//�ʒu�Ɣ��a���w��
			hitLength[i] = ECheckCollisionRay1(XMVectorSet(g_enemyselect[i].pos.x, g_enemyselect[i].pos.y, g_enemyselect[i].pos.z, 0.0f), 20.0f);
			if (hitLength[i] < 9999.9f)
			{
				g_enemyselect[i].rot.y += 0.1f;
				SelectEnemy(i);
				GetEnemy()->Percent();
				if (GetMouseTrigger(0))
				{
					PlaySound(SOUND_LABEL_SE_SELECTS);
					//�G�̑I��
					enemyno = i;
					StartFade(SCENE_GAME);
				}
			}
			else
			{
				g_enemyselect[i].rot = XMFLOAT3(0.0f, 90.0f, 0.0f);
			}
		}



		XMMATRIX mtxWorld, mtxRot, mtxScl,
			mtxTranslate;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_enemyselect[i].scl.x, g_enemyselect[i].scl.y, g_enemyselect[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);



		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(g_enemyselect[i].rot.x), XMConvertToRadians(g_enemyselect[i].rot.y + 180), XMConvertToRadians(g_enemyselect[i].rot.z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);


		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_enemyselect[i].pos.x, g_enemyselect[i].pos.y, g_enemyselect[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);


		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&g_enemyselect[i].mtxWorld, mtxWorld);

		//�e�̈ړ�
		MoveShadow(g_enemyselect[i].nShadowIdx, g_enemyselect[i].pos);


		//************************************************:
		//��

		XMMATRIX mtxWorldHD;
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorldHD = XMMatrixIdentity();
		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_enemyselectHD[i].scl.x, g_enemyselectHD[i].scl.y, g_enemyselectHD[i].scl.z);
		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxScl);
		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(g_enemyselectHD[i].rot.x),
			XMConvertToRadians(g_enemyselectHD[i].rot.y),//+180
			XMConvertToRadians(g_enemyselectHD[i].rot.z)
		);
		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_enemyselectHD[i].pos.x, g_enemyselectHD[i].pos.y, g_enemyselectHD[i].pos.z);

		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxTranslate);

		//�e�̍s����|����
		mtxWorldHD = XMMatrixMultiply(mtxWorldHD, mtxWorld);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&g_enemyselectHD[i].mtxWorld, mtxWorldHD);


		//************************************************:
		//�r

		XMMATRIX mtxWorldAM;
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorldAM = XMMatrixIdentity();
		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_enemyselectAM[i].scl.x, g_enemyselectAM[i].scl.y, g_enemyselectAM[i].scl.z);
		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxScl);
		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(g_enemyselectAM[i].rot.x),
			XMConvertToRadians(g_enemyselectAM[i].rot.y),//+180
			XMConvertToRadians(g_enemyselectAM[i].rot.z)
		);
		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_enemyselectAM[i].pos.x, g_enemyselectAM[i].pos.y, g_enemyselectAM[i].pos.z);

		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxTranslate);

		//�e�̍s����|����
		mtxWorldAM = XMMatrixMultiply(mtxWorldAM, mtxWorld);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&g_enemyselectAM[i].mtxWorld, mtxWorldAM);

		//************************************************:
		//��

		XMMATRIX mtxWorldLG;
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorldLG = XMMatrixIdentity();
		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_enemyselectLG[i].scl.x, g_enemyselectLG[i].scl.y, g_enemyselectLG[i].scl.z);
		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxScl);
		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(g_enemyselectLG[i].rot.x),
			XMConvertToRadians(g_enemyselectLG[i].rot.y),//+180
			XMConvertToRadians(g_enemyselectLG[i].rot.z)
		);
		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_enemyselectLG[i].pos.x, g_enemyselectLG[i].pos.y, g_enemyselectLG[i].pos.z);

		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxTranslate);

		//�e�̍s����|����
		mtxWorldLG = XMMatrixMultiply(mtxWorldLG, mtxWorld);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&g_enemyselectLG[i].mtxWorld, mtxWorldLG);
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemyselect(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	for (int i = 0; i < ENEMYSEL_MAX; i++)
	{
		//���g�p
		if (g_enemyselect[i].nState == 0) continue;

		// ���f���\��
		g_model[0 + i * 4].Draw(pDeviceContext, g_enemyselect[i].mtxWorld);
		g_model[1 + i * 4].Draw(pDeviceContext, g_enemyselectHD[i].mtxWorld);
		g_model[2 + i * 4].Draw(pDeviceContext, g_enemyselectAM[i].mtxWorld);
		g_model[3 + i * 4].Draw(pDeviceContext, g_enemyselectLG[i].mtxWorld);

	}

	GetEnemy()->DrawChimera(500, 0);
}

//�����m�F
bool IsEnemyselect(int no)
{
	if (no < 0 || no >= ENEMYSEL_MAX) return false;
	return g_enemyselect[no].nState > 0 ?
		true : false;
}


//���W�擾
XMFLOAT3 GetEnemyselectPos(int no)
{
	if (no < 0 || no >= ENEMYSEL_MAX)
		return XMFLOAT3(0.0f, 0.0f, 0.0f);
	return g_enemyselect[no].pos;
}

//�T�C�Y�擾
XMFLOAT3 GetEnemyselectSize(int no)
{
	if (no < 0 || no >= ENEMYSEL_MAX)
		return XMFLOAT3(0,0,0);
	//return XMFLOAT3(10.0f,10.0f,10.0f);
	XMFLOAT3 box = g_model[0].GetBBox();
	return XMFLOAT3(box.x / 2 * g_enemyselect[no].scl.x, box.y / 2 * g_enemyselect[no].scl.y, box.z / 2 * g_enemyselect[no].scl.z);
}

int Enemyno()
{
	return enemyno;
}

//�G�̒l�Z�b�g
void SelectEnemy(int val)
{
	switch (val)
	{
	case 0: 
	{
		GetEnemy()->Setmodel(0);
		GetEnemy()->Setmodel(10);
		GetEnemy()->Setmodel(20);
		GetEnemy()->Setmodel(30);
		GetEnemy()->SetName("�J�u�g���V");
		break;
	}
	case 1:
	{
		GetEnemy()->Setmodel(1);
		GetEnemy()->Setmodel(11);
		GetEnemy()->Setmodel(21);
		GetEnemy()->Setmodel(31);
		GetEnemy()->SetName("���j");
		break;
	}
	case 2:
	{
		GetEnemy()->Setmodel(2);
		GetEnemy()->Setmodel(12);
		GetEnemy()->Setmodel(22);
		GetEnemy()->Setmodel(32);
		GetEnemy()->SetName("�p���_");
		break;
	}
	case 3:
	{
		GetEnemy()->Setmodel(3);
		GetEnemy()->Setmodel(13);
		GetEnemy()->Setmodel(23);
		GetEnemy()->Setmodel(33);
		GetEnemy()->SetName("�g��");
		break;
	}
	case 4:
	{
		GetEnemy()->Setmodel(4);
		GetEnemy()->Setmodel(14);
		GetEnemy()->Setmodel(24);
		GetEnemy()->Setmodel(34);
		GetEnemy()->SetName("�Ԃ�");
		break;
	}
	case 5:
	{
		GetEnemy()->Setmodel(2);
		GetEnemy()->Setmodel(12);
		GetEnemy()->Setmodel(22);
		GetEnemy()->Setmodel(32);
		GetEnemy()->SetName("�E�V");
		break;
	}
	case 6:
	{
		GetEnemy()->Setmodel(2);
		GetEnemy()->Setmodel(12);
		GetEnemy()->Setmodel(22);
		GetEnemy()->Setmodel(32);
		GetEnemy()->SetName("�L�����P");
		break;
	}
	case 7:
	{
		GetEnemy()->Setmodel(2);
		GetEnemy()->Setmodel(12);
		GetEnemy()->Setmodel(22);
		GetEnemy()->Setmodel(32);
		GetEnemy()->SetName("�L�����Q");
		break;
	}
	case 8:
	{
		GetEnemy()->Setmodel(2);
		GetEnemy()->Setmodel(12);
		GetEnemy()->Setmodel(22);
		GetEnemy()->Setmodel(32);
		GetEnemy()->SetName("�L�����R");
		break;
	}
	case 9:
	{
		GetEnemy()->Setmodel(2);
		GetEnemy()->Setmodel(12);
		GetEnemy()->Setmodel(22);
		GetEnemy()->Setmodel(32);
		GetEnemy()->SetName("�L�����S");
		break;
	}

	default:
		break;
	}
}


float ECheckCollisionRay1(XMVECTOR pos, float fRadius)
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

int SetEnemyselect(XMFLOAT3 pos, XMFLOAT3 rot)
{
	tEnemyselect* pEnemy = g_enemyselect;
	for (int i = 0; i < ENEMYSEL_MAX; ++i, ++pEnemy)
	{	// �o�����Ă���G�̓X���[
		if (pEnemy->nState > 0) continue;

		pEnemy->pos = pos;

		g_enemyselect[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_enemyselect[i].Tipe = 0;

		pEnemy->nState = 1;	// �o��
		pEnemy->nShadowIdx =
			CreateShadow(pEnemy->pos, ENEMY_RADIUS);
		pEnemy->rot = rot;//���f���̌���
		return i;	// �ԍ���Ԃ�
	}

	return -1;	// ������Ȃ�����
}
