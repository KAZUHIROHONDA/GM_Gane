//=============================================================================
//
// �X�R�A�摜�\������ [playerhp.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "playerhp.h"
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
#define PLAYERHP_TEXTURENAME	_T("data/texture/frame_score.png")

#define PLAYERHP_SIZE_X	(340)	// ����
#define PLAYERHP_SIZE_Y	(80)	// �c��
#define PLAYERHP_POS_X	(-SCREEN_WIDTH/2+PLAYERHP_SIZE_X/2)	// �����ʒuX
#define PLAYERHP_POS_Y	(-SCREEN_HEIGHT/2+PLAYERHP_SIZE_Y/2)	// �����ʒuY

#define PLAYERHP_FRAME_X	(1)	// �A�j���̉��̐�
#define PLAYERHP_FRAME_Y	(1)	// �A�j���̏c�̐�

#define PLAYERHP_DIGIT		(8)	// ����

#define NUMBER_POS_X	(PLAYERHP_POS_X-PLAYERHP_DIGIT/2*NUMBER_SIZE_X)
#define NUMBER_POS_Y	(PLAYERHP_POS_Y+NUMBER_SIZE_Y/2-8)

// �\���̒�`
typedef struct {
	int	nPatNo;		// �\���p�^�[���ԍ�
	int	nCount;		// �\���t���[����
}ANIM_PAT;

// �X�R�A�\����
typedef struct _tPlayerhp
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

	int			nPlayerhp;		// 
}tPlayerhp;

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
tPlayerhp g_playerhp[PLAYERHP_MAX];	// �X�R�A
//int g_nHiplayerhp = 1000;	//�n�C�X�R�A

//=============================================================================
// �X�R�A�摜�\���̏���������
//=============================================================================
HRESULT InitPlayerhp()
{
	// �e�N�X�`���ǂݍ���
	HRESULT hr = 
		CreateTextureFromFile(GetDevice(),
		PLAYERHP_TEXTURENAME, &g_pTexture);
	if (FAILED(hr)) return hr;

	// �ϐ�������
	for (int i = 0; i < PLAYERHP_MAX; i++)
	{
		g_playerhp[i].Position = XMFLOAT3(PLAYERHP_POS_X,
			PLAYERHP_POS_Y, 0.0f);
		g_playerhp[i].Size = XMFLOAT2(PLAYERHP_SIZE_X,
			PLAYERHP_SIZE_Y);
		g_playerhp[i].Angle = 0.0f;
		g_playerhp[i].Scale = XMFLOAT2(1.0f, 1.0f);
		g_playerhp[i].nDir = 0;	// �ŏ��͉�

		g_playerhp[i].nAnimNo = 0;
		g_playerhp[i].nCount = g_animPat[g_playerhp[i].nDir][g_playerhp[i].nAnimNo].nCount;
		// �ŏ��̌����̍ŏ��̃A�j���\������

		g_playerhp[i].Vel = XMFLOAT2(0.0f, 0.0f);
		g_playerhp[i].bUse = true;

		g_playerhp[i].nPlayerhp = 0;
	}

	///g_nHiplayerhp = 1000;	//�f�t�H���g�l
	//�t�@�C������ǂݍ���
	FILE* fp;
	if ((fp = fopen("savedata.dat", "r")) != NULL)
	{//�I�[�y������
		//fread(&g_nHiplayerhp, sizeof(int), 1, fp);
		//fclose(fp);
	}
	return hr;
}

//=============================================================================
// �X�R�A�摜�\���̏I������
//=============================================================================
void UninitPlayerhp()
{
	// �e�N�X�`���J��
	SAFE_RELEASE(g_pTexture);
	//�t�@�C������ǂݍ���
	FILE* fp;
	if ((fp = fopen("savedata.dat", "w")) != NULL)
	{//�I�[�y������
		//fwrite(&g_nHiplayerhp, sizeof(int), 1, fp);
		//fclose(fp);
	}
}

//=============================================================================
// �X�R�A�摜�\���̍X�V����
//=============================================================================
void UpdatePlayerhp()
{	// �X�V����

	for (int i = 0; i < PLAYERHP_MAX; i++)
	{
		// �g���ĂȂ���Ή������Ȃ�
		// �o�Ă��Ȃ��X�R�A�͉������Ȃ�
		if (g_playerhp[i].bUse == false) continue;

		g_playerhp[i].nPlayerhp = GetPlayerHp(i);

	}
}

//=============================================================================
// �X�R�A�摜�\������
//=============================================================================
void DrawPlayerhp()
{
	for (int i = 0; i < PLAYERHP_MAX; i++)
	{
		// �����˂̃X�R�A�͕\�����Ȃ�
		if (g_playerhp[i].bUse == false)	continue;

		// �X�R�A�摜�`��
		SetPolygonPos(g_playerhp[i].Position.x, g_playerhp[i].Position.y);	// ���W
		SetPolygonSize(g_playerhp[i].Size.x * g_playerhp[i].Scale.x,
			g_playerhp[i].Size.y * g_playerhp[i].Scale.y);	// �傫��
		SetPolygonAngle(g_playerhp[i].Angle);				// �p�x
		SetPolygonColor(1.0f, 1.0f, 1.0f);	// �F(RGB)
		SetPolygonAlpha(0.0f);				// ���l(�����x)

		int u, v;
		u = g_animPat[g_playerhp[i].nDir][g_playerhp[i].nAnimNo].nPatNo
			% PLAYERHP_FRAME_X;
		v = g_animPat[g_playerhp[i].nDir][g_playerhp[i].nAnimNo].nPatNo
			/ PLAYERHP_FRAME_X;
		SetPolygonUV(
			u / (float)PLAYERHP_FRAME_X,
			v / (float)PLAYERHP_FRAME_Y);			// 0�Ԃ̃e�N�X�`�����W
		SetPolygonFrameSize(
			1.0f / PLAYERHP_FRAME_X,
			1.0f / PLAYERHP_FRAME_Y);	// �e�N�X�`���̉���,�c��
		SetPolygonTexture(g_pTexture);		// �e�N�X�`���w��

		// �|���S���̕`�揈��
		DrawPolygon(GetDeviceContext());

		// ����(number)�̕`��
		XMFLOAT2 pos = XMFLOAT2(
			NUMBER_POS_X,NUMBER_POS_Y);
		
		SetPolygonColor(0.0f, 50.0f, 1.0f);	// �F(RGB)
		SetPolygonAlpha(1.0f);
		DrawNumber(&pos,		// ���W
			g_playerhp[i].nPlayerhp,	// ���l
			3,					// ����
			NUMBER_SIZE_X, NUMBER_SIZE_Y);// �T�C�Y
	}

	//�n�C�X�R�A
	//XMFLOAT2 pos = XMFLOAT2(
	//	NUMBER_POS_X, NUMBER_POS_Y - 64);
	//DrawNumber(&pos, g_nHiplayerhp, 8);

	// ���Ƃɖ߂�
	SetPolygonUV(0.0f, 0.0f);			// 0�Ԃ̃e�N�X�`�����W
	SetPolygonFrameSize(1.0f, 1.0f);	// �e�N�X�`���̉���,�c��
}

/*void AddPlayerhp(int num)
{
	g_playerhp[0].nPlayerhp += num;
	if (g_playerhp[0].nPlayerhp < 0)
		g_playerhp[0].nPlayerhp = 0;
	if (g_playerhp[0].nPlayerhp >
		pow(10, PLAYERHP_DIGIT) - 1)
	{	// �J���X�g
		g_playerhp[0].nPlayerhp =
			pow(10, PLAYERHP_DIGIT) - 1;
	}
	//if (g_playerhp[0].nPlayerhp > g_nHiplayerhp)
	//{  //�n�C�X�R�A�X�V
	//	g_nHiplayerhp = g_playerhp[0].nPlayerhp;

	//}
}*/
