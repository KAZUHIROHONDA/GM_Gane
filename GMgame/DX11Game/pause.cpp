//=============================================================================
//
// �|�[�Y���� [pause.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include <stdarg.h>
#include <string.h>
#include "pause.h"
#include "polygon.h"	// �|���S���g�p
#include "Texture.h"// �e�N�X�`���g�p
#include "input.h"
#include "collision.h"

// �}�N����`
#define	NUM_PAUSE_MENU		(3)			// �|�[�Y���j���[��
#define	PAUSE_MENU_WIDTH	(320.0f)	// �|�[�Y���j���[��
#define	PAUSE_MENU_HEIGHT	(60.0f)		// �|�[�Y���j���[����
#define	PAUSE_MENU_INTERVAL	(100.0f)	// �|�[�Y���j���[�Ԋu
#define	PAUSE_MENU_POS_X	(0.0f)		// �|�[�Y���j���[�ʒu(X���W)
#define	PAUSE_MENU_POS_Y	(PAUSE_MENU_INTERVAL)	// �|�[�Y���j���[�ʒu(Y���W)
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

static LPCWSTR c_aFileNamePauseMenu[NUM_PAUSE_MENU] =
{
	L"data/TEXTURE/pause0.png",	// �R���e�B�j���[
	L"data/TEXTURE/pause1.png",	// ���g���C
	L"data/TEXTURE/pause2.png",	// �N�C�b�g
};

static PAUSE_MENU g_nSelectMenu = PAUSE_MENU_CONTINUE;	//	�I�𒆂̃��j���[No

// ����������
HRESULT InitPause( void )
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr;

	for (int nCntPauseMenu = 0; nCntPauseMenu < NUM_PAUSE_MENU; ++nCntPauseMenu) {
		// �e�N�X�`���̓ǂݍ���
		hr = CreateTextureFromFile( pDevice,									// �f�o�C�X�ւ̃|�C���^
			c_aFileNamePauseMenu[nCntPauseMenu],	// �t�@�C���̖��O
			&g_pTextures[nCntPauseMenu] );			// �ǂݍ��ރ������[
	}

	g_nSelectMenu = PAUSE_MENU_CONTINUE;
	g_fCurve = 0.0f;

	return hr;
}

void UninitPause( void )
{
	// �e�N�X�`���̊J��
	for (int nCntPauseMenu = 0; nCntPauseMenu < NUM_PAUSE_MENU; ++nCntPauseMenu) {
		SAFE_RELEASE( g_pTextures[nCntPauseMenu] );
	}
}

void UpdatePause( void )
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
					g_nSelectMenu = (PAUSE_MENU)((g_nSelectMenu + 1) % NUM_PAUSE_MENU);
					SetPauseMenu();
					nStopTime = 10;
				}
				else
				{
					len *= 0.3f;
					g_nSelectMenu = (PAUSE_MENU)((g_nSelectMenu + NUM_PAUSE_MENU - 1) % NUM_PAUSE_MENU);
					SetPauseMenu();
					nStopTime = 10;
				}

			}
		}
	}

	//�}�E�X
	POINT temp = (*GetMousePosition());
	XMFLOAT2 mousePos = XMFLOAT2(temp.x - SCREEN_CENTER_X, -(temp.y - SCREEN_CENTER_Y));
	XMFLOAT2 pos1 = XMFLOAT2(PAUSE_MENU_POS_X, PAUSE_MENU_POS_Y);
	XMFLOAT2 pos2 = XMFLOAT2(PAUSE_MENU_POS_X , PAUSE_MENU_POS_Y - 1 * PAUSE_MENU_INTERVAL);
	XMFLOAT2 pos3 = XMFLOAT2(PAUSE_MENU_POS_X, PAUSE_MENU_POS_Y - 2 * PAUSE_MENU_INTERVAL);
	XMFLOAT2 radius1 = XMFLOAT2(PAUSE_MENU_WIDTH / 2, PAUSE_MENU_HEIGHT / 2);
	XMFLOAT2 mpos2 = mousePos;
	XMFLOAT2 radius2 = XMFLOAT2(0.1, 0.1);
	if (CollisionBB(&pos1, &radius1, &mpos2, &radius2))
	{
		ResetPauseMenu();
	}
	else if (CollisionBB(&pos2, &radius1, &mpos2, &radius2))
	{
		ResetPause1();
	}
	else if (CollisionBB(&pos3, &radius1, &mpos2, &radius2))
	{
		ResetPause2();
	}


	// �㉺�L�[�Ŋe���ڊԂ̈ړ�
	if (GetKeyRepeat( VK_W ) || GetKeyRepeat( VK_UP )) {
		g_nSelectMenu = (PAUSE_MENU)((g_nSelectMenu + NUM_PAUSE_MENU - 1) % NUM_PAUSE_MENU);
		SetPauseMenu();
	} else if (GetKeyRepeat( VK_S ) || GetKeyRepeat( VK_DOWN )) {
		g_nSelectMenu = (PAUSE_MENU)((g_nSelectMenu + 1) % NUM_PAUSE_MENU);
		SetPauseMenu();
	}



	// �g�̕����̐F�̕ω�(�O���f�[�V����)
	g_fCurve += XM_PI * 0.01f;
	if (g_fCurve > XM_PI) {
		g_fCurve -= XM_2PI;
	}

	g_fCol = cosf( g_fCurve ) * 0.2f + 0.8f;


}

void DrawPause( void )
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// �^�C�g���`��
   //�|���S�����ݒ�
	SetPolygonPos(0,0);			//���W
	SetPolygonSize(SCREEN_WIDTH, SCREEN_HEIGHT);		//�傫��
	SetPolygonColor(1.0f, 1.0f, 1.0f);	//�F(RGB)
	SetPolygonAlpha(0.5f);				//���l(�����x)

	SetPolygonUV(0.0f, 0.0f);			//0�Ԃ̃e�N�X�`��
	SetPolygonFrameSize(1.0f, 1.0f);	//�e�N�X�`��
	SetPolygonTexture(NULL);		//�e�N�X�`��

	//�|���S���̕`�揈��
	DrawPolygon(GetDeviceContext());
	SetPolygonAlpha(1.0f);				//���l(�����x)

	//�|�[�Y���j���[�̕\��
	SetPolygonSize( PAUSE_MENU_WIDTH, PAUSE_MENU_HEIGHT );
	for (int nCntPauseMenu = 0; nCntPauseMenu < NUM_PAUSE_MENU; ++nCntPauseMenu) {
		SetPolygonPos( PAUSE_MENU_POS_X, PAUSE_MENU_POS_Y - nCntPauseMenu * PAUSE_MENU_INTERVAL );

		//�I������Ă��郁�j���[��ڗ�������
		if (nCntPauseMenu == g_nSelectMenu)
		{
			SetPolygonColor(1.0f, 1.0f, 1.0f);
		}
		else
		{
			SetPolygonColor(0.3f, 0.3f, 0.3f);
		}
		
		// �e�N�X�`���̐ݒ�
		SetPolygonTexture( g_pTextures[nCntPauseMenu] );
		// �|���S���̕`��
		DrawPolygon( pDeviceContext );
	}

	SetPolygonColor( 1.0f, 1.0f, 1.0f );
}

//=============================================================================
// �|�[�Y���j���[�̐ݒ�
//=============================================================================
void SetPauseMenu( void )
{
	g_fCurve = 0.0f;
}

//=============================================================================
// �|�[�Y���j���[�̎擾
//=============================================================================
PAUSE_MENU GetPauseMenu( void )
{
	return g_nSelectMenu;
}

//=============================================================================
// �|�[�Y���j���[�̃��Z�b�g
//=============================================================================
void ResetPauseMenu( void )
{
	g_nSelectMenu = PAUSE_MENU_CONTINUE;
	SetPauseMenu();
}

void ResetPause1(void)
{
	g_nSelectMenu = PAUSE_MENU_RETRY;
	SetPauseMenu();
}
void ResetPause2(void)
{
	g_nSelectMenu = PAUSE_MENU_QUIT;
	SetPauseMenu();
}