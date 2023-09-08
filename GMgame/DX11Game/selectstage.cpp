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
#include "collision.h"
#include "fade.h"

// �}�N����`
#define	NUM_SELECT_STAGE		(2)			// �|�[�Y���j���[��
#define	SELECT_STAGE_WIDTH	(570.0f)	// �|�[�Y���j���[��
#define	SELECT_STAGE_HEIGHT	(410.0f)		// �|�[�Y���j���[����
#define	SELECT_STAGE_INTERVAL	(500.0f)	// �|�[�Y���j���[�Ԋu
#define	SELECT_STAGE_POS_X	(200.0f)		// �|�[�Y���j���[�ʒu(X���W)
#define	SELECT_STAGE_POS_Y	(0.0f)	// �|�[�Y���j���[�ʒu(Y���W)
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
	L"data/TEXTURE/sel1.png",	
	L"data/TEXTURE/sel2.png",	

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
	if (GetKeyRepeat(VK_A) || GetKeyRepeat(VK_LEFT)) {
		g_nSelectMenu = (SELECT_STAGE)((g_nSelectMenu + NUM_SELECT_STAGE - 1)% NUM_SELECT_STAGE);
		
		SetSelectStageMenu();
	}
	else if (GetKeyRepeat(VK_D) || GetKeyRepeat(VK_RIGHT)) {
		g_nSelectMenu = (SELECT_STAGE)((g_nSelectMenu + 1) % NUM_SELECT_STAGE);
		
		SetSelectStageMenu();
	}

	POINT temp = (*GetMousePosition());
	XMFLOAT2 mousePos = XMFLOAT2(temp.x - SCREEN_CENTER_X, -(temp.y - SCREEN_CENTER_Y));
	//�}�E�X
	XMFLOAT2 pos1 = XMFLOAT2(SELECT_STAGE_POS_X, SELECT_STAGE_POS_Y);
	XMFLOAT2 pos2 = XMFLOAT2(SELECT_STAGE_POS_X - 1 * SELECT_STAGE_INTERVAL, SELECT_STAGE_POS_Y);
	XMFLOAT2 radius1 = XMFLOAT2(SELECT_STAGE_WIDTH / 2, SELECT_STAGE_HEIGHT / 2);
	XMFLOAT2 mpos2 = mousePos;
	XMFLOAT2 radius2 = XMFLOAT2(0.1, 0.1);
	if (CollisionBB(&pos1, &radius1, &mpos2, &radius2))
	{
		SetSelectStageMenu1();

	}
	else if (CollisionBB(&pos2, &radius1, &mpos2, &radius2))
	{
		SetSelectStageMenu2();

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
	posx = -250;
	 
	//�|�[�Y���j���[�̕\��
	SetPolygonSize( SELECT_STAGE_WIDTH, SELECT_STAGE_HEIGHT );

		for (int nCntSelectMenu = 0; nCntSelectMenu < 2; ++nCntSelectMenu) 
		{
			SetPolygonPos(posx + nCntSelectMenu * SELECT_STAGE_INTERVAL, SELECT_STAGE_POS_Y);

			//�I������Ă��郁�j���[��ڗ�������
			if (nCntSelectMenu == g_nSelectMenu)
			{
				SetPolygonColor(1.0f, 1.0f, 1.0f);
			}
			else
			{
				SetPolygonColor(0.3f, 0.3f, 0.3f);
			}

			// �e�N�X�`���̐ݒ�
			SetPolygonTexture(g_pTextures[nCntSelectMenu]);
			// �|���S���̕`��
			DrawPolygon(pDeviceContext);
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

void SetSelectStageMenu1(void)
{
	g_nSelectMenu = SELECT_STAGE_2;
	SetSelectStageMenu();
}

void SetSelectStageMenu2(void)
{
	g_nSelectMenu = SELECT_STAGE_1;
	SetSelectStageMenu();
}