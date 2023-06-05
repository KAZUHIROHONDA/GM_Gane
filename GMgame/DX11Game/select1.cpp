//=============================================================================
//
// �|�[�Y���� [select1.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include <stdarg.h>
#include <string.h>
#include "select1.h"
#include "polygon.h"	// �|���S���g�p
#include "Texture.h"// �e�N�X�`���g�p
#include "input.h"
#include "sound.h"

// �}�N����`
#define	NUM_SELECT1_MENU		(3)			// �|�[�Y���j���[��
#define	SELECT1_MENU_WIDTH	(200.0f)	// �|�[�Y���j���[��
#define	SELECT1_MENU_HEIGHT	(40.0f)		// �|�[�Y���j���[����
#define	SELECT1_MENU_INTERVAL	(80.0f)	// �|�[�Y���j���[�Ԋu
#define	SELECT1_MENU_POS_X	(0.0f)		// �|�[�Y���j���[�ʒu(X���W)
#define	SELECT1_MENU_POS_Y	(-50.0f)	// �|�[�Y���j���[�ʒu(Y���W)
#define	PLATE_WIDTH			(360.0f)	// �v���[�g�̕�
#define	PLATE_HEIGHT		(340.0f)	// �v���[�g�̕�
#define	PLATE_POS_X			(0.0f)		// �v���[�g�̈ʒu(X���W)
#define	PLATE_POS_Y			(0.0f)		// �v���[�g�̈ʒu(Y���W)

// �\���̒�`
// �v���g�^�C�v�錾
// �O���[�o���ϐ�
static ID3D11ShaderResourceView*	g_pTextures[3] = { nullptr };	// �e�N�X�`���ւ̃|�C���^

static float g_fCurve = 0.0f;
static float g_fCol = 0.0f;
static int	 nStopTime = 0;
static float g_fAlpha = 1.0f;
static int   g_fCnt = 0;

static LPCWSTR c_aFileNameSelect1Menu[NUM_SELECT1_MENU] =
{
	L"data/TEXTURE/select0.png",	// �R���e�B�j���[
	L"data/TEXTURE/select1.png",	// ���g���C
	L"data/TEXTURE/select2.png",	// �N�C�b�g
};

static SELECT1_MENU g_nSelect1Menu = SELECT1_MENU_CONTINUE;	//	�I�𒆂̃��j���[No

// ����������
HRESULT InitSelect1( void )
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr;

	for (int nCntSelect1Menu = 0; nCntSelect1Menu < NUM_SELECT1_MENU; ++nCntSelect1Menu) {
		// �e�N�X�`���̓ǂݍ���
		hr = CreateTextureFromFile( pDevice,									// �f�o�C�X�ւ̃|�C���^
			c_aFileNameSelect1Menu[nCntSelect1Menu],	// �t�@�C���̖��O
			&g_pTextures[nCntSelect1Menu] );			// �ǂݍ��ރ������[
	}

	g_nSelect1Menu = SELECT1_MENU_CONTINUE;
	g_fCurve = 0.0f;

	return hr;
}

void UninitSelect1( void )
{
	// �e�N�X�`���̊J��
	for (int nCntSelect1Menu = 0; nCntSelect1Menu < NUM_SELECT1_MENU; ++nCntSelect1Menu) {
		SAFE_RELEASE( g_pTextures[nCntSelect1Menu] );
	}
}

void UpdateSelect1( void )
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
					g_nSelect1Menu = (SELECT1_MENU)((g_nSelect1Menu + 1) % NUM_SELECT1_MENU);
					SetSelect1Menu();
					nStopTime = 10;
				}
				else
				{
					len *= 0.3f;
					g_nSelect1Menu = (SELECT1_MENU)((g_nSelect1Menu + NUM_SELECT1_MENU - 1) % NUM_SELECT1_MENU);
					SetSelect1Menu();
					nStopTime = 10;
				}

			}
		}
	}
	// �㉺�L�[�Ŋe���ڊԂ̈ړ�
	if (GetKeyRepeat( VK_W ) || GetKeyRepeat( VK_UP )) {
		g_nSelect1Menu = (SELECT1_MENU)((g_nSelect1Menu + NUM_SELECT1_MENU - 1) % NUM_SELECT1_MENU);
		PlaySound(SOUND_LABEL_SE_SELECT);
		SetSelect1Menu();
	} else if (GetKeyRepeat( VK_S ) || GetKeyRepeat( VK_DOWN )) {
		g_nSelect1Menu = (SELECT1_MENU)((g_nSelect1Menu + 1) % NUM_SELECT1_MENU);
		PlaySound(SOUND_LABEL_SE_SELECT);
		SetSelect1Menu();
	}

	// �g�̕����̐F�̕ω�(�O���f�[�V����)
	g_fCurve += XM_PI * 0.01f;
	if (g_fCurve > XM_PI) {
		g_fCurve -= XM_2PI;
	}

	g_fCnt++;
	if (g_fCnt <= 120)
	{
		g_fAlpha -= 0.01f;
	}
	if (g_fAlpha <= 0.02f)
	{
		g_fAlpha = 1.0f;
		g_fCnt = 0;
	}

	g_fCol = cosf( g_fCurve ) * 0.2f + 0.8f;
}

void DrawSelect1( void )
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	
	//�|�[�Y���j���[�̕\��
	SetPolygonSize( SELECT1_MENU_WIDTH, SELECT1_MENU_HEIGHT );
	for (int nCntSelect1Menu = 0; nCntSelect1Menu < NUM_SELECT1_MENU; ++nCntSelect1Menu) {
		SetPolygonPos( SELECT1_MENU_POS_X, SELECT1_MENU_POS_Y - nCntSelect1Menu * SELECT1_MENU_INTERVAL );

		//�I������Ă��郁�j���[��ڗ�������
		if (nCntSelect1Menu == g_nSelect1Menu)
		{
			SetPolygonColor(1.0f, 1.0f, 1.0f);
			SetPolygonAlpha(g_fAlpha);
		}
		else
		{
			SetPolygonColor(0.3f, 0.3f, 0.3f);
			SetPolygonAlpha(1.0f);
		}
		
		// �e�N�X�`���̐ݒ�
		SetPolygonTexture( g_pTextures[nCntSelect1Menu] );
		// �|���S���̕`��
		DrawPolygon( pDeviceContext );
	}

	SetPolygonColor( 1.0f, 1.0f, 1.0f );
}

void DrawSelect11(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();


	//�|�[�Y���j���[�̕\��
	SetPolygonSize(SELECT1_MENU_WIDTH, SELECT1_MENU_HEIGHT);
	for (int nCntSelect1Menu = 1; nCntSelect1Menu < NUM_SELECT1_MENU; ++nCntSelect1Menu) {
		SetPolygonPos(SELECT1_MENU_POS_X, SELECT1_MENU_POS_Y - nCntSelect1Menu * SELECT1_MENU_INTERVAL);

		//�I������Ă��郁�j���[��ڗ�������
		if (nCntSelect1Menu == g_nSelect1Menu)
		{
			SetPolygonColor(1.0f, 1.0f, 1.0f);
			SetPolygonAlpha(g_fAlpha);
		}
		else
		{
			SetPolygonColor(0.3f, 0.3f, 0.3f);
			SetPolygonAlpha(1.0f);
		}

		// �e�N�X�`���̐ݒ�
		SetPolygonTexture(g_pTextures[nCntSelect1Menu]);
		// �|���S���̕`��
		DrawPolygon(pDeviceContext);
	}

	SetPolygonColor(1.0f, 1.0f, 1.0f);
}

//=============================================================================
// �|�[�Y���j���[�̐ݒ�
//=============================================================================
void SetSelect1Menu( void )
{
	g_fCurve = 0.0f;
}

//=============================================================================
// �|�[�Y���j���[�̎擾
//=============================================================================
SELECT1_MENU GetSelect1Menu( void )
{
	return g_nSelect1Menu;
}

//=============================================================================
// �|�[�Y���j���[�̃��Z�b�g
//=============================================================================
void ResetSelect1Menu( void )
{
	g_nSelect1Menu = SELECT1_MENU_CONTINUE;
	SetSelect1Menu();
}
