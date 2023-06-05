//=============================================================================
//
// �|�[�Y���� [selectstage.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include <stdarg.h>
#include <string.h>
#include "selectstage.h"
#include "polygon.h"	// �|���S���g�p
#include "Texture.h"// �e�N�X�`���g�p
#include "input.h"
#include "sound.h"

// �}�N����`
#define	NUM_SELECT_STAGE		(9)			// �|�[�Y���j���[��
#define	SELECT_STAGE_WIDTH	(200.0f)	// �|�[�Y���j���[��
#define	SELECT_STAGE_HEIGHT	(40.0f)		// �|�[�Y���j���[����
#define	SELECT_STAGE_INTERVAL	(200.0f)	// �|�[�Y���j���[�Ԋu
#define	SELECT_STAGE_POS_X	(0.0f)		// �|�[�Y���j���[�ʒu(X���W)
#define	SELECT_STAGE_POS_Y	(200.0f)	// �|�[�Y���j���[�ʒu(Y���W)
#define	PLATE_WIDTH			(360.0f)	// �v���[�g�̕�
#define	PLATE_HEIGHT		(340.0f)	// �v���[�g�̕�
#define	PLATE_POS_X			(0.0f)		// �v���[�g�̈ʒu(X���W)
#define	PLATE_POS_Y			(0.0f)		// �v���[�g�̈ʒu(Y���W)

// �\���̒�`
// �v���g�^�C�v�錾
// �O���[�o���ϐ�
static ID3D11ShaderResourceView*	g_pTextures[NUM_SELECT_STAGE] = { nullptr };	// �e�N�X�`���ւ̃|�C���^

static float g_fCurve = 0.0f;
static float g_fCol = 0.0f;
static int	 nStopTime = 0;
static int   posx = -300;

static LPCWSTR c_aFileNameSelectMenu[NUM_SELECT_STAGE] =
{
	L"data/TEXTURE/stage1.png",	// �R���e�B�j���[
	L"data/TEXTURE/stage2.png",	// ���g���C
	L"data/TEXTURE/stage3.png",	// �N�C�b�g
	L"data/TEXTURE/stage4.png",
	L"data/TEXTURE/stage5.png",
	L"data/TEXTURE/stage6.png",
	L"data/TEXTURE/stage7.png",
	L"data/TEXTURE/stage8.png",
	L"data/TEXTURE/stage9.png",
};

static SELECT_STAGE g_nSelectMenu = SELECT_STAGE_1;	//	�I�𒆂̃��j���[No

// ����������
HRESULT InitSelectStage( void )
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr;

	for (int nCntSelectMenu = 0; nCntSelectMenu < NUM_SELECT_STAGE; ++nCntSelectMenu) {
		// �e�N�X�`���̓ǂݍ���
		hr = CreateTextureFromFile( pDevice,									// �f�o�C�X�ւ̃|�C���^
			c_aFileNameSelectMenu[nCntSelectMenu],	// �t�@�C���̖��O
			&g_pTextures[nCntSelectMenu] );			// �ǂݍ��ރ������[
	}

	g_nSelectMenu = SELECT_STAGE_1;
	g_fCurve = 0.0f;

	return hr;
}

void UninitSelectStage( void )
{
	// �e�N�X�`���̊J��
	for (int nCntSelectMenu = 0; nCntSelectMenu < NUM_SELECT_STAGE; ++nCntSelectMenu) {
		SAFE_RELEASE( g_pTextures[nCntSelectMenu] );
	}
}

void UpdateSelectStage( void )
{
	//����s�\����
	if (nStopTime > 0)
	{
		nStopTime--;
	}
	if (nStopTime == 0)
	{
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
				{
					len *= 0.3f;
					g_nSelectMenu = (SELECT_STAGE)
						 ((g_nSelectMenu + 1) % NUM_SELECT_STAGE);
					SetSelectStageMenu();
					nStopTime = 10;
				}
				else 
				{
					len *= 0.3f;
					g_nSelectMenu = (SELECT_STAGE)
						 ((g_nSelectMenu + NUM_SELECT_STAGE - 1) % NUM_SELECT_STAGE);
					SetSelectStageMenu();
					nStopTime = 10;
				}

			}
		}
	}
	
	// �㉺�L�[�Ŋe���ڊԂ̈ړ�
	if (GetKeyRepeat( VK_W ) || GetKeyRepeat( VK_UP )) {
		g_nSelectMenu = (SELECT_STAGE)((g_nSelectMenu + NUM_SELECT_STAGE - 1) % NUM_SELECT_STAGE);
		PlaySound(SOUND_LABEL_SE_SELECT);
		SetSelectStageMenu();
	} else if (GetKeyRepeat( VK_S ) || GetKeyRepeat( VK_DOWN )) {
		g_nSelectMenu = (SELECT_STAGE)((g_nSelectMenu + 1) % NUM_SELECT_STAGE);
		PlaySound(SOUND_LABEL_SE_SELECT);
		SetSelectStageMenu();
	}
	else if (GetKeyRepeat(VK_A) || GetKeyRepeat(VK_LEFT)) {
		g_nSelectMenu = (SELECT_STAGE)((g_nSelectMenu + NUM_SELECT_STAGE - 3)% NUM_SELECT_STAGE);
		PlaySound(SOUND_LABEL_SE_SELECT);
		SetSelectStageMenu();
	}
	else if (GetKeyRepeat(VK_D) || GetKeyRepeat(VK_RIGHT)) {
		g_nSelectMenu = (SELECT_STAGE)((g_nSelectMenu + 3) % NUM_SELECT_STAGE);
		PlaySound(SOUND_LABEL_SE_SELECT);
		SetSelectStageMenu();
	}

	// �g�̕����̐F�̕ω�(�O���f�[�V����)
	g_fCurve += XM_PI * 0.01f;
	if (g_fCurve > XM_PI) {
		g_fCurve -= XM_2PI;
	}

	g_fCol = cosf( g_fCurve ) * 0.2f + 0.8f;
}

void DrawSelectStage( void )
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	int nCnt = 0;
	posx = -300;
	 
	//�|�[�Y���j���[�̕\��
	SetPolygonSize( SELECT_STAGE_WIDTH, SELECT_STAGE_HEIGHT );
	for (int CntSelectMenu = 0; CntSelectMenu < 3; ++CntSelectMenu)
	{

		for (int nCntSelectMenu = 0; nCntSelectMenu < 3; ++nCntSelectMenu) 
		{
			SetPolygonPos(posx, SELECT_STAGE_POS_Y - nCntSelectMenu * SELECT_STAGE_INTERVAL);

			//�I������Ă��郁�j���[��ڗ�������
			if (nCntSelectMenu + CntSelectMenu * 3 == g_nSelectMenu)
			{
				SetPolygonColor(1.0f, 1.0f, 1.0f);
			}
			else
			{
				SetPolygonColor(0.3f, 0.3f, 0.3f);
			}

			// �e�N�X�`���̐ݒ�
			SetPolygonTexture(g_pTextures[nCnt]);
			// �|���S���̕`��
			DrawPolygon(pDeviceContext);

			nCnt++;
		}

		posx += 300;
	}

	SetPolygonColor( 1.0f, 1.0f, 1.0f );
}


//=============================================================================
// �|�[�Y���j���[�̐ݒ�
//=============================================================================
void SetSelectStageMenu( void )
{
	g_fCurve = 0.0f;
}

//=============================================================================
// �|�[�Y���j���[�̎擾
//=============================================================================
SELECT_STAGE GetSelectStageMenu( void )
{
	return g_nSelectMenu;
}

//=============================================================================
// �|�[�Y���j���[�̃��Z�b�g
//=============================================================================
void ResetSelectStageMenu( void )
{
	g_nSelectMenu = SELECT_STAGE_1;
	SetSelectStageMenu();
}
