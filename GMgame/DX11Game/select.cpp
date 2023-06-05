//=============================================================================
//
// �|�[�Y���� [select.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include <stdarg.h>
#include <string.h>
#include "select.h"
#include "polygon.h"	// �|���S���g�p
#include "Texture.h"// �e�N�X�`���g�p
#include "input.h"
#include "sound.h"

// �}�N����`
#define	NUM_SELECT_MENU		(3)			// �|�[�Y���j���[��
#define	SELECT_MENU_WIDTH	(200.0f)	// �|�[�Y���j���[��
#define	SELECT_MENU_HEIGHT	(40.0f)		// �|�[�Y���j���[����
#define	SELECT_MENU_INTERVAL	(50.0f)	// �|�[�Y���j���[�Ԋu
#define	SELECT_MENU_POS_X	(0.0f)		// �|�[�Y���j���[�ʒu(X���W)
#define	SELECT_MENU_POS_Y	(-50.0f)	// �|�[�Y���j���[�ʒu(Y���W)
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

static LPCWSTR c_aFileNameSelectMenu[NUM_SELECT_MENU] =
{
	L"data/TEXTURE/select000.png",	// �R���e�B�j���[
	L"data/TEXTURE/select001.png",	// ���g���C
	L"data/TEXTURE/select002.png",	// �N�C�b�g
};

static SELECT_MENU g_nSelectMenu = SELECT_MENU_CONTINUE;	//	�I�𒆂̃��j���[No

// ����������
HRESULT InitSelect( void )
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr;

	for (int nCntSelectMenu = 0; nCntSelectMenu < NUM_SELECT_MENU; ++nCntSelectMenu) {
		// �e�N�X�`���̓ǂݍ���
		hr = CreateTextureFromFile( pDevice,									// �f�o�C�X�ւ̃|�C���^
			c_aFileNameSelectMenu[nCntSelectMenu],	// �t�@�C���̖��O
			&g_pTextures[nCntSelectMenu] );			// �ǂݍ��ރ������[
	}

	g_nSelectMenu = SELECT_MENU_CONTINUE;
	g_fCurve = 0.0f;

	return hr;
}

void UninitSelect( void )
{
	// �e�N�X�`���̊J��
	for (int nCntSelectMenu = 0; nCntSelectMenu < NUM_SELECT_MENU; ++nCntSelectMenu) {
		SAFE_RELEASE( g_pTextures[nCntSelectMenu] );
	}
}

void UpdateSelect( void )
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
					g_nSelectMenu = (SELECT_MENU)((g_nSelectMenu + 1) % NUM_SELECT_MENU);
					SetSelectMenu();
					nStopTime = 10;
				}
				if (angle < 135 || angle > 45)
				{
					len *= 0.3f;
					g_nSelectMenu = (SELECT_MENU)((g_nSelectMenu + NUM_SELECT_MENU - 1) % NUM_SELECT_MENU);
					SetSelectMenu();
					nStopTime = 10;
				}


			}
		}
	}
	
	// �㉺�L�[�Ŋe���ڊԂ̈ړ�
	if (GetKeyRepeat( VK_W ) || GetKeyRepeat( VK_UP )) {
		g_nSelectMenu = (SELECT_MENU)((g_nSelectMenu + NUM_SELECT_MENU - 1) % NUM_SELECT_MENU);
		PlaySound(SOUND_LABEL_SE_SELECT);
		SetSelectMenu();
	} else if (GetKeyRepeat( VK_S ) || GetKeyRepeat( VK_DOWN )) {
		g_nSelectMenu = (SELECT_MENU)((g_nSelectMenu + 1) % NUM_SELECT_MENU);
		PlaySound(SOUND_LABEL_SE_SELECT);
		SetSelectMenu();
	}

	// �g�̕����̐F�̕ω�(�O���f�[�V����)
	g_fCurve += XM_PI * 0.01f;
	if (g_fCurve > XM_PI) {
		g_fCurve -= XM_2PI;
	}

	g_fCol = cosf( g_fCurve ) * 0.2f + 0.8f;
}

void DrawSelect( void )
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	
	//�|�[�Y���j���[�̕\��
	SetPolygonSize( SELECT_MENU_WIDTH, SELECT_MENU_HEIGHT );
	for (int nCntSelectMenu = 0; nCntSelectMenu < NUM_SELECT_MENU; ++nCntSelectMenu) {
		SetPolygonPos( SELECT_MENU_POS_X, SELECT_MENU_POS_Y - nCntSelectMenu * SELECT_MENU_INTERVAL );



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
		SetPolygonTexture( g_pTextures[nCntSelectMenu] );
		// �|���S���̕`��
		DrawPolygon( pDeviceContext );
	}

	SetPolygonColor( 1.0f, 1.0f, 1.0f );
}


//=============================================================================
// �|�[�Y���j���[�̐ݒ�
//=============================================================================
void SetSelectMenu( void )
{
	g_fCurve = 0.0f;
}

//=============================================================================
// �|�[�Y���j���[�̎擾
//=============================================================================
SELECT_MENU GetSelectMenu( void )
{
	return g_nSelectMenu;
}

//=============================================================================
// �|�[�Y���j���[�̃��Z�b�g
//=============================================================================
void ResetSelectMenu( void )
{
	g_nSelectMenu = SELECT_MENU_CONTINUE;
	SetSelectMenu();
}
