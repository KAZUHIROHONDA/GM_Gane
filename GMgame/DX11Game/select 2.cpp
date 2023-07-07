//=============================================================================
//
// �|�[�Y���� [select2.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include <stdarg.h>
#include <string.h>
#include "select 2.h"
#include "polygon.h"	// �|���S���g�p
#include "Texture.h"// �e�N�X�`���g�p
#include "input.h"
#include "sound.h"
#include "collision.h"

// �}�N����`
#define	NUM_SELECT2_MENU		(2)			// �|�[�Y���j���[��
#define	SELECT2_MENU_WIDTH	(200.0f)	// �|�[�Y���j���[��
#define	SELECT2_MENU_HEIGHT	(40.0f)		// �|�[�Y���j���[����
#define	SELECT2_MENU_INTERVAL	(50.0f)	// �|�[�Y���j���[�Ԋu
#define	SELECT2_MENU_POS_X	(0.0f)		// �|�[�Y���j���[�ʒu(X���W)
#define	SELECT2_MENU_POS_Y	(-50.0f)	// �|�[�Y���j���[�ʒu(Y���W)
#define	PLATE_WIDTH			(360.0f)	// �v���[�g�̕�
#define	PLATE_HEIGHT		(340.0f)	// �v���[�g�̕�
#define	PLATE_POS_X			(0.0f)		// �v���[�g�̈ʒu(X���W)
#define	PLATE_POS_Y			(0.0f)		// �v���[�g�̈ʒu(Y���W)

// �\���̒�`
// �v���g�^�C�v�錾
// �O���[�o���ϐ�
static ID3D11ShaderResourceView*	g_pTextures[2] = { nullptr };	// �e�N�X�`���ւ̃|�C���^

static float g_fCurve = 0.0f;
static float g_fCol = 0.0f;
static int	 nStopTime = 0;

static LPCWSTR c_aFileNameSelect2Menu[NUM_SELECT2_MENU] =
{
	L"data/TEXTURE/select001.png",	// �R���e�B�j���[
	L"data/TEXTURE/select02.png",	// ���g���C
};

static SELECT2_MENU g_nSelect2Menu = SELECT2_MENU_CONTINUE;	//	�I�𒆂̃��j���[No

// ����������
HRESULT InitSelect2( void )
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr;

	for (int nCntSelect2Menu = 0; nCntSelect2Menu < NUM_SELECT2_MENU; ++nCntSelect2Menu) {
		// �e�N�X�`���̓ǂݍ���
		hr = CreateTextureFromFile( pDevice,									// �f�o�C�X�ւ̃|�C���^
			c_aFileNameSelect2Menu[nCntSelect2Menu],	// �t�@�C���̖��O
			&g_pTextures[nCntSelect2Menu] );			// �ǂݍ��ރ������[
	}

	g_nSelect2Menu = SELECT2_MENU_CONTINUE;
	g_fCurve = 0.0f;

	return hr;
}

void UninitSelect2( void )
{
	// �e�N�X�`���̊J��
	for (int nCntSelect2Menu = 0; nCntSelect2Menu < NUM_SELECT2_MENU; ++nCntSelect2Menu) {
		SAFE_RELEASE( g_pTextures[nCntSelect2Menu] );
	}
}

void UpdateSelect2( void )
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
					g_nSelect2Menu = (SELECT2_MENU)((g_nSelect2Menu + 1) % NUM_SELECT2_MENU);
					SetSelect2Menu();
					nStopTime = 10;
				}
				else
				{
					len *= 0.3f;
					g_nSelect2Menu = (SELECT2_MENU)((g_nSelect2Menu + NUM_SELECT2_MENU - 1) % NUM_SELECT2_MENU);
					SetSelect2Menu();
					nStopTime = 10;
				}

			}
		}
	}
	//�}�E�X
	POINT temp = (*GetMousePosition());
	XMFLOAT2 mousePos = XMFLOAT2(temp.x - SCREEN_CENTER_X, -(temp.y - SCREEN_CENTER_Y));
	XMFLOAT2 pos1 = XMFLOAT2(SELECT2_MENU_POS_X, SELECT2_MENU_POS_Y);
	XMFLOAT2 pos2 = XMFLOAT2(SELECT2_MENU_POS_X, SELECT2_MENU_POS_Y - 1 * SELECT2_MENU_INTERVAL);
	XMFLOAT2 radius1 = XMFLOAT2(SELECT2_MENU_WIDTH / 2, SELECT2_MENU_HEIGHT / 2);
	XMFLOAT2 mpos2 = mousePos;
	XMFLOAT2 radius2 = XMFLOAT2(0.1, 0.1);
	if (CollisionBB(&pos1, &radius1, &mpos2, &radius2))
	{
		ResetSelect2Menu();
	}
	else if (CollisionBB(&pos2, &radius1, &mpos2, &radius2))
	{
		ResetSelect2Menu1();
	}


	// �㉺�L�[�Ŋe���ڊԂ̈ړ�
	if (GetKeyRepeat( VK_W ) || GetKeyRepeat( VK_UP )) {
		g_nSelect2Menu = (SELECT2_MENU)((g_nSelect2Menu + NUM_SELECT2_MENU - 1) % NUM_SELECT2_MENU);
		PlaySound(SOUND_LABEL_SE_SELECT);
		SetSelect2Menu();
	} else if (GetKeyRepeat( VK_S ) || GetKeyRepeat( VK_DOWN )) {
		g_nSelect2Menu = (SELECT2_MENU)((g_nSelect2Menu + 1) % NUM_SELECT2_MENU);
		PlaySound(SOUND_LABEL_SE_SELECT);
		SetSelect2Menu();
	}

	// �g�̕����̐F�̕ω�(�O���f�[�V����)
	g_fCurve += XM_PI * 0.01f;
	if (g_fCurve > XM_PI) {
		g_fCurve -= XM_2PI;
	}

	g_fCol = cosf( g_fCurve ) * 0.2f + 0.8f;
}

void DrawSelect2( void )
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	
	//�|�[�Y���j���[�̕\��
	SetPolygonSize( SELECT2_MENU_WIDTH, SELECT2_MENU_HEIGHT );
	for (int nCntSelect2Menu = 0; nCntSelect2Menu < NUM_SELECT2_MENU; ++nCntSelect2Menu) {
		SetPolygonPos( SELECT2_MENU_POS_X, SELECT2_MENU_POS_Y - nCntSelect2Menu * SELECT2_MENU_INTERVAL );

		//�I������Ă��郁�j���[��ڗ�������
		if (nCntSelect2Menu == g_nSelect2Menu)
		{
			SetPolygonColor(1.0f, 1.0f, 1.0f);
		}
		else
		{
			SetPolygonColor(0.3f, 0.3f, 0.3f);
		}
		
		// �e�N�X�`���̐ݒ�
		SetPolygonTexture( g_pTextures[nCntSelect2Menu] );
		// �|���S���̕`��
		DrawPolygon( pDeviceContext );
	}

	SetPolygonColor( 1.0f, 1.0f, 1.0f );
}


//=============================================================================
// �|�[�Y���j���[�̐ݒ�
//=============================================================================
void SetSelect2Menu( void )
{
	g_fCurve = 0.0f;
}

//=============================================================================
// �|�[�Y���j���[�̎擾
//=============================================================================
SELECT2_MENU GetSelect2Menu( void )
{
	return g_nSelect2Menu;
}

//=============================================================================
// �|�[�Y���j���[�̃��Z�b�g
//=============================================================================
void ResetSelect2Menu( void )
{
	g_nSelect2Menu = SELECT2_MENU_CONTINUE;
	SetSelect2Menu();
}
void ResetSelect2Menu1(void)
{
	g_nSelect2Menu = SELECT2_MENU_RETRY;
	SetSelect2Menu();
}

