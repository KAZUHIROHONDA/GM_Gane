//=============================================================================
//
// �X�R�A�摜�\������ [enemyhp.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "enemyhp.h"
#include"enemy.h"
#include "polygon.h"
#include "Texture.h"
#include "number.h"	// �����`��ɕK�v
#include"player.h"
#ifdef _DEBUG
#include "debugproc.h"
#endif
#pragma warning(disable:4996)	//���܂��Ȃ�
//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �e�N�X�`���t�@�C����
#define ENEMYHP_TEXTURENAME	_T("data/texture/frame_score.png")

#define ENEMYHP_SIZE_X	(340)	// ����
#define ENEMYHP_SIZE_Y	(80)	// �c��
#define ENEMYHP_POS_X	(SCREEN_WIDTH/2-ENEMYHP_SIZE_X/2)	// �����ʒuX
#define ENEMYHP_POS_Y	(SCREEN_HEIGHT/2-ENEMYHP_SIZE_Y/2)	// �����ʒuY

#define ENEMYHP_FRAME_X	(1)	// �A�j���̉��̐�
#define ENEMYHP_FRAME_Y	(1)	// �A�j���̏c�̐�

#define ENEMYHP_DIGIT		(8)	// ����

#define NUMBER_POS_X	(ENEMYHP_POS_X-ENEMYHP_DIGIT/2*NUMBER_SIZE_X)
#define NUMBER_POS_Y	(ENEMYHP_POS_Y+NUMBER_SIZE_Y/2-8)

// �\���̒�`
typedef struct {
	int	nPatNo;		// �\���p�^�[���ԍ�
	int	nCount;		// �\���t���[����
}ANIM_PAT;

// �X�R�A�\����
typedef struct _tEnemyhp
{
	XMFLOAT3	Position;	// ���W
	XMFLOAT2	Vel;		// �ړ����x
	XMFLOAT2	Size;		// �T�C�Y
	float		Angle;		// �p�x
	XMFLOAT2	Scale;		// �g�嗦
	int			nDir;		// ����
	XMFLOAT4	col;

	int			nAnimNo;	// �\�����No
	int			nCount;		// �c��t���[����
	bool		bUse;		// �g�p�t���O

	int			nEnemyhp;		// 
}tEnemyhp;

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTexture;				// �e�N�X�`��
static ANIM_PAT g_animPat[][2] = {
	{{0,6},{-1,-1}},
	{{0,6},{-1,-1}},
	{{0,6},{-1,-1}},
	{{0,6},{-1,-1}},
};
tEnemyhp g_enemyhp[ENEMYHP_MAX];	// �X�R�A
//int g_nHienemyhp = 1000;	//�n�C�X�R�A

//=============================================================================
// �X�R�A�摜�\���̏���������
//=============================================================================
HRESULT InitEnemyhp()
{
	// �e�N�X�`���ǂݍ���
	HRESULT hr = 
		CreateTextureFromFile(GetDevice(),
		ENEMYHP_TEXTURENAME, &g_pTexture);
	if (FAILED(hr)) return hr;

	// �ϐ�������
	for (int i = 0; i < ENEMYHP_MAX; i++)
	{
		g_enemyhp[i].Position = XMFLOAT3(ENEMYHP_POS_X,
			ENEMYHP_POS_Y, 0.0f);
		g_enemyhp[i].Size = XMFLOAT2(ENEMYHP_SIZE_X,
			ENEMYHP_SIZE_Y);
		g_enemyhp[i].Angle = 0.0f;
		g_enemyhp[i].Scale = XMFLOAT2(1.0f, 1.0f);
		g_enemyhp[i].nDir = 0;	// �ŏ��͉�

		g_enemyhp[i].nAnimNo = 0;
		g_enemyhp[i].nCount = g_animPat[g_enemyhp[i].nDir][g_enemyhp[i].nAnimNo].nCount;
		// �ŏ��̌����̍ŏ��̃A�j���\������

		g_enemyhp[i].Vel = XMFLOAT2(0.0f, 0.0f);
		g_enemyhp[i].bUse = true;

		g_enemyhp[i].nEnemyhp = 0;
	}

	///g_nHienemyhp = 1000;	//�f�t�H���g�l
	//�t�@�C������ǂݍ���
	FILE* fp;
	if ((fp = fopen("savedata.dat", "r")) != NULL)
	{//�I�[�y������
		//fread(&g_nHienemyhp, sizeof(int), 1, fp);
		//fclose(fp);
	}
	return hr;
}

//=============================================================================
// �X�R�A�摜�\���̏I������
//=============================================================================
void UninitEnemyhp()
{
	// �e�N�X�`���J��
	SAFE_RELEASE(g_pTexture);
	//�t�@�C������ǂݍ���
	FILE* fp;
	if ((fp = fopen("savedata.dat", "w")) != NULL)
	{//�I�[�y������
		//fwrite(&g_nHienemyhp, sizeof(int), 1, fp);
		//fclose(fp);
	}
}

//=============================================================================
// �X�R�A�摜�\���̍X�V����
//=============================================================================
void UpdateEnemyhp()
{	// �X�V����

	for (int i = 0; i < ENEMYHP_MAX; i++)
	{
		// �g���ĂȂ���Ή������Ȃ�
		// �o�Ă��Ȃ��X�R�A�͉������Ȃ�
		if (g_enemyhp[i].bUse == false) continue;

		g_enemyhp[i].nEnemyhp = GetEnemyHp(i);

	}
}

//=============================================================================
// �X�R�A�摜�\������
//=============================================================================
void DrawEnemyhp()
{
	for (int i = 0; i < ENEMYHP_MAX; i++)
	{
		// �����˂̃X�R�A�͕\�����Ȃ�
		if (g_enemyhp[i].bUse == false)	continue;

		// �X�R�A�摜�`��
		SetPolygonPos(g_enemyhp[i].Position.x, g_enemyhp[i].Position.y);	// ���W
		SetPolygonSize(g_enemyhp[i].Size.x * g_enemyhp[i].Scale.x,
			g_enemyhp[i].Size.y * g_enemyhp[i].Scale.y);	// �傫��
		SetPolygonAngle(g_enemyhp[i].Angle);				// �p�x
		SetPolygonColor(0.0f, 1.0f, 1.0f);	// �F(RGB)
		SetPolygonAlpha(0.0f);				// ���l(�����x)

		int u, v;
		u = g_animPat[g_enemyhp[i].nDir][g_enemyhp[i].nAnimNo].nPatNo
			% ENEMYHP_FRAME_X;
		v = g_animPat[g_enemyhp[i].nDir][g_enemyhp[i].nAnimNo].nPatNo
			/ ENEMYHP_FRAME_X;
		SetPolygonUV(
			u / (float)ENEMYHP_FRAME_X,
			v / (float)ENEMYHP_FRAME_Y);			// 0�Ԃ̃e�N�X�`�����W
		SetPolygonFrameSize(
			1.0f / ENEMYHP_FRAME_X,
			1.0f / ENEMYHP_FRAME_Y);	// �e�N�X�`���̉���,�c��
		SetPolygonTexture(g_pTexture);		// �e�N�X�`���w��

		// �|���S���̕`�揈��
		DrawPolygon(GetDeviceContext());

		// ����(number)�̕`��
		XMFLOAT2 pos = XMFLOAT2(
			NUMBER_POS_X,NUMBER_POS_Y);
		
		SetPolygonColor(100.0f, 00.0f, 1.0f);	// �F(RGB)
		SetPolygonAlpha(1.0f);
		DrawNumber(&pos,		// ���W
			g_enemyhp[i].nEnemyhp,	// ���l
			3,					// ����
			NUMBER_SIZE_X, NUMBER_SIZE_Y);// �T�C�Y
	}

	//�n�C�X�R�A
	//XMFLOAT2 pos = XMFLOAT2(
	//	NUMBER_POS_X, NUMBER_POS_Y - 64);
	//DrawNumber(&pos, g_nHienemyhp, 8);

	// ���Ƃɖ߂�
	SetPolygonUV(0.0f, 0.0f);			// 0�Ԃ̃e�N�X�`�����W
	SetPolygonFrameSize(1.0f, 1.0f);	// �e�N�X�`���̉���,�c��
}

/*void AddEnemyhp(int num)
{
	g_enemyhp[0].nEnemyhp += num;
	if (g_enemyhp[0].nEnemyhp < 0)
		g_enemyhp[0].nEnemyhp = 0;
	if (g_enemyhp[0].nEnemyhp >
		pow(10, ENEMYHP_DIGIT) - 1)
	{	// �J���X�g
		g_enemyhp[0].nEnemyhp =
			pow(10, ENEMYHP_DIGIT) - 1;
	}
	//if (g_enemyhp[0].nEnemyhp > g_nHienemyhp)
	//{  //�n�C�X�R�A�X�V
	//	g_nHienemyhp = g_enemyhp[0].nEnemyhp;

	//}
}*/
