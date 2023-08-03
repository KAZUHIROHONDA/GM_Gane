//=============================================================================
//
// �|�[�Y���� [tab.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include <stdarg.h>
#include <string.h>
#include "tab.h"
#include "polygon.h"	// �|���S���g�p
#include "Texture.h"// �e�N�X�`���g�p
#include "input.h"
#include "sound.h"
#include "collision.h"
#include "sceneChimera.h"
#include "chimeracs.h"

// �}�N����`
#define	NUM_TAB_MENU		(4)			// �|�[�Y���j���[��
#define	TAB_MENU_WIDTH	(28.0f)	// �|�[�Y���j���[��
#define	TAB_MENU_HEIGHT	(128.0f)		// �|�[�Y���j���[����
#define	TAB_MENU_INTERVAL	(130.0f)	// �|�[�Y���j���[�Ԋu
#define	TAB_MENU_POS_X	(50.0f)		// �|�[�Y���j���[�ʒu(X���W)
#define	TAB_MENU_POS_Y	(300.0f)	// �|�[�Y���j���[�ʒu(Y���W)
#define	PLATE_WIDTH			(360.0f)	// �v���[�g�̕�
#define	PLATE_HEIGHT		(340.0f)	// �v���[�g�̕�
#define	PLATE_POS_X			(0.0f)		// �v���[�g�̈ʒu(X���W)
#define	PLATE_POS_Y			(0.0f)		// �v���[�g�̈ʒu(Y���W)

// �\���̒�`
// �v���g�^�C�v�錾
// �O���[�o���ϐ�
static ID3D11ShaderResourceView*	g_pTextures[4] = { nullptr };	// �e�N�X�`���ւ̃|�C���^

static float g_fCurve = 0.0f;
static float g_fCol = 0.0f;
static int	 nStopTime = 0;

static LPCWSTR c_aFileNameTabMenu[NUM_TAB_MENU] =
{
	L"data/TEXTURE/Htab.png",	
	L"data/TEXTURE/Btab.png",	
	L"data/TEXTURE/Mtab.png",
	L"data/TEXTURE/Utab.png",	
};

static TAB_MENU g_nTabMenu = TAB_MENU_ATAMA;	//	�I�𒆂̃��j���[No

// ����������
HRESULT InitTab( void )
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr;

	for (int nCntTabMenu = 0; nCntTabMenu < NUM_TAB_MENU; ++nCntTabMenu) {
		// �e�N�X�`���̓ǂݍ���
		hr = CreateTextureFromFile( pDevice,									// �f�o�C�X�ւ̃|�C���^
			c_aFileNameTabMenu[nCntTabMenu],	// �t�@�C���̖��O
			&g_pTextures[nCntTabMenu] );			// �ǂݍ��ރ������[
	}

	g_nTabMenu = TAB_MENU_ATAMA;
	g_fCurve = 0.0f;

	return hr;
}

void UninitTab( void )
{
	// �e�N�X�`���̊J��
	for (int nCntTabMenu = 0; nCntTabMenu < NUM_TAB_MENU; ++nCntTabMenu) {
		SAFE_RELEASE( g_pTextures[nCntTabMenu] );
	}
}

void UpdateTab( void )
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
					g_nTabMenu = (TAB_MENU)((g_nTabMenu + 1) % NUM_TAB_MENU);
					SetTabMenu();
					nStopTime = 10;
				}
				if (angle < 135 || angle > 45)
				{
					len *= 0.3f;
					g_nTabMenu = (TAB_MENU)((g_nTabMenu + NUM_TAB_MENU - 1) % NUM_TAB_MENU);
					SetTabMenu();
					nStopTime = 10;
				}


			}
		}
	}

	//�}�E�X
	POINT temp = (*GetMousePosition());
	XMFLOAT2 mousePos = XMFLOAT2(temp.x - SCREEN_CENTER_X, -(temp.y - SCREEN_CENTER_Y));
	XMFLOAT2 pos1 = XMFLOAT2(TAB_MENU_POS_X, TAB_MENU_POS_Y);
	XMFLOAT2 pos2 = XMFLOAT2(TAB_MENU_POS_X, TAB_MENU_POS_Y - 1 * TAB_MENU_INTERVAL);
	XMFLOAT2 pos3 = XMFLOAT2(TAB_MENU_POS_X, TAB_MENU_POS_Y - 2 * TAB_MENU_INTERVAL);
	XMFLOAT2 pos4 = XMFLOAT2(TAB_MENU_POS_X, TAB_MENU_POS_Y - 3 * TAB_MENU_INTERVAL);
	XMFLOAT2 radius1 = XMFLOAT2(TAB_MENU_WIDTH / 2, TAB_MENU_HEIGHT / 2);
	XMFLOAT2 mpos2 = mousePos;
	XMFLOAT2 radius2 = XMFLOAT2(0.1, 0.1);
	if (CollisionBB(&pos1, &radius1, &mpos2, &radius2))
	{
		ResetTabMenu();
		if (GetMouseTrigger(0))
		{
			Get()->ChangeChimera(HEAD);
		}
	}
	else if (CollisionBB(&pos2, &radius1, &mpos2, &radius2))
	{
		ResetTabMenu1();
		if (GetMouseTrigger(0))
		{
			Get()->ChangeChimera(BODY);
		}
	}
	else if (CollisionBB(&pos3, &radius1, &mpos2, &radius2))
	{
		ResetTabMenu2();
		if (GetMouseTrigger(0))
		{
			Get()->ChangeChimera(FOREFOOT);
		}
	}
	else if (CollisionBB(&pos4, &radius1, &mpos2, &radius2))
	{
		ResetTabMenu3();
		if (GetMouseTrigger(0))
		{
			Get()->ChangeChimera(HINDFOOT);
		}
	}

	// �㉺�L�[�Ŋe���ڊԂ̈ړ�
	if (GetKeyRepeat( VK_W ) || GetKeyRepeat( VK_UP )) {
		g_nTabMenu = (TAB_MENU)((g_nTabMenu + NUM_TAB_MENU - 1) % NUM_TAB_MENU);
		SetTabMenu();
	} else if (GetKeyRepeat( VK_S ) || GetKeyRepeat( VK_DOWN )) {
		g_nTabMenu = (TAB_MENU)((g_nTabMenu + 1) % NUM_TAB_MENU);
		SetTabMenu();
	}

	// �g�̕����̐F�̕ω�(�O���f�[�V����)
	g_fCurve += XM_PI * 0.01f;
	if (g_fCurve > XM_PI) {
		g_fCurve -= XM_2PI;
	}

	g_fCol = cosf( g_fCurve ) * 0.2f + 0.8f;
}

void DrawTab( void )
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	
	//�|�[�Y���j���[�̕\��
	SetPolygonSize( TAB_MENU_WIDTH, TAB_MENU_HEIGHT );
	for (int nCntTabMenu = 0; nCntTabMenu < NUM_TAB_MENU; ++nCntTabMenu) {
		SetPolygonPos( TAB_MENU_POS_X, TAB_MENU_POS_Y - nCntTabMenu * TAB_MENU_INTERVAL );



		//�I������Ă��郁�j���[��ڗ�������
		if (nCntTabMenu == g_nTabMenu)
		{
			SetPolygonColor(1.0f, 1.0f, 1.0f);
		}
		else
		{
			SetPolygonColor(0.3f, 0.3f, 0.3f);
		}
		
		// �e�N�X�`���̐ݒ�
		SetPolygonTexture( g_pTextures[nCntTabMenu] );
		// �|���S���̕`��
		DrawPolygon( pDeviceContext );
	}

	SetPolygonColor( 1.0f, 1.0f, 1.0f );
}


//=============================================================================
// �|�[�Y���j���[�̐ݒ�
//=============================================================================
void SetTabMenu( void )
{
	g_fCurve = 0.0f;
}

//=============================================================================
// �|�[�Y���j���[�̎擾
//=============================================================================
TAB_MENU GetTabMenu( void )
{
	return g_nTabMenu;
}

//=============================================================================
// �|�[�Y���j���[�̃��Z�b�g
//=============================================================================
void ResetTabMenu( void )
{
	g_nTabMenu = TAB_MENU_ATAMA;
	SetTabMenu();
}
void ResetTabMenu1(void)
{
	g_nTabMenu = TAB_MENU_KARADA;
	SetTabMenu();
}
void ResetTabMenu2(void)
{
	g_nTabMenu = TAB_MENU_MAE;
	SetTabMenu();
}
void ResetTabMenu3(void)
{
	g_nTabMenu = TAB_MENU_USIRO;
	SetTabMenu();
}

