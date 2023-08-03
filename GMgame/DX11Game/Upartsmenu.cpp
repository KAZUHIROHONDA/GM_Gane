//=============================================================================
//
// �v���C���[���� [Upartsmenu.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include <stdarg.h>
#include <string.h>
#include "Upartsmenu.h"
#include "polygon.h"	// �|���S���g�p
#include "Texture.h"// �e�N�X�`���g�p
#include "input.h"
#include "sound.h"
#include "collision.h"
#include "model.h"
#include "pchimera.h"
#include "playercs.h"

// �}�N����`
#define	NUM_UPARTS_MENU		(2)			// �|�[�Y���j���[��
#define	UPARTS_MENU_WIDTH	(200.0f)	// �|�[�Y���j���[��
#define	UPARTS_MENU_HEIGHT	(200.0f)		// �|�[�Y���j���[����
#define	UPARTS_MENU_INTERVAL	(300.0f)	// �|�[�Y���j���[�Ԋu
#define	UPARTS_MENU_POS_X	(200.0f)		// �|�[�Y���j���[�ʒu(X���W)
#define	UPARTS_MENU_POS_Y	(200.0f)	// �|�[�Y���j���[�ʒu(Y���W)
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

static LPCWSTR c_aFileNameUPartsMenu[NUM_UPARTS_MENU] =
{
	L"data/TEXTURE/kabu3.png",	// �R���e�B�j���[
	L"data/TEXTURE/wani3.png",	// ���g���C
};

static UPARTS_MENU g_nUPartsMenu = UPARTS_MENU_KABU;	//	�I�𒆂̃��j���[No

Player UParts;

// ����������
HRESULT InitUParts(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr;

	for (int nCntUPartsMenu = 0; nCntUPartsMenu < NUM_UPARTS_MENU; ++nCntUPartsMenu) {
		// �e�N�X�`���̓ǂݍ���
		hr = CreateTextureFromFile(pDevice,									// �f�o�C�X�ւ̃|�C���^
			c_aFileNameUPartsMenu[nCntUPartsMenu],	// �t�@�C���̖��O
			&g_pTextures[nCntUPartsMenu]);			// �ǂݍ��ރ������[
	}

	UParts.SetHP(10);
	UParts.SetPAat(15);
	UParts.SetGUat(20);
	UParts.SetTYOKIat(25);

	g_nUPartsMenu = UPARTS_MENU_KABU;
	g_fCurve = 0.0f;

	return hr;
}

void UninitUParts(void)
{
	// �e�N�X�`���̊J��
	for (int nCntUPartsMenu = 0; nCntUPartsMenu < NUM_UPARTS_MENU; ++nCntUPartsMenu) {
		SAFE_RELEASE(g_pTextures[nCntUPartsMenu]);
	}
}

void UpdateUParts(void)
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
					g_nUPartsMenu = (UPARTS_MENU)((g_nUPartsMenu + 1) % NUM_UPARTS_MENU);
					SetUPartsMenu();
					nStopTime = 10;
				}
				if (angle < 135 || angle > 45)
				{
					len *= 0.3f;
					g_nUPartsMenu = (UPARTS_MENU)((g_nUPartsMenu + NUM_UPARTS_MENU - 1) % NUM_UPARTS_MENU);
					SetUPartsMenu();
					nStopTime = 10;
				}


			}
		}
	}

	//�}�E�X
	POINT temp = (*GetMousePosition());
	XMFLOAT2 mousePos = XMFLOAT2(temp.x - SCREEN_CENTER_X, -(temp.y - SCREEN_CENTER_Y));
	XMFLOAT2 pos1 = XMFLOAT2(UPARTS_MENU_POS_X, UPARTS_MENU_POS_Y);
	XMFLOAT2 pos2 = XMFLOAT2(UPARTS_MENU_POS_X + 1 * UPARTS_MENU_INTERVAL, UPARTS_MENU_POS_Y );
	XMFLOAT2 radius1 = XMFLOAT2(UPARTS_MENU_WIDTH / 2, UPARTS_MENU_HEIGHT / 2);
	XMFLOAT2 mpos2 = mousePos;
	XMFLOAT2 radius2 = XMFLOAT2(0.1, 0.1);
	if (CollisionBB(&pos1, &radius1, &mpos2, &radius2))
	{
		ResetUPartsMenu();
		if (GetMouseTrigger(0))
		{
			SetBack(MODEL_KABU3);
			UParts.SetHP(10);
			UParts.SetPAat(15);
			UParts.SetGUat(20);
			UParts.SetTYOKIat(25);
			InitPChimera();
		}
	}
	else if (CollisionBB(&pos2, &radius1, &mpos2, &radius2))
	{
		ResetUPartsMenu1();
		if (GetMouseTrigger(0))
		{
			SetBack(MODEL_wani3);
			UParts.SetHP(10);
			UParts.SetPAat(15);
			UParts.SetGUat(20);
			UParts.SetTYOKIat(25);
			InitPChimera();
		}
	}


	// �㉺�L�[�Ŋe���ڊԂ̈ړ�
	if (GetKeyRepeat(VK_W) || GetKeyRepeat(VK_UP)) {
		g_nUPartsMenu = (UPARTS_MENU)((g_nUPartsMenu + NUM_UPARTS_MENU - 1) % NUM_UPARTS_MENU);
		SetUPartsMenu();
	}
	else if (GetKeyRepeat(VK_S) || GetKeyRepeat(VK_DOWN)) {
		g_nUPartsMenu = (UPARTS_MENU)((g_nUPartsMenu + 1) % NUM_UPARTS_MENU);
		SetUPartsMenu();
	}

	// �g�̕����̐F�̕ω�(�O���f�[�V����)
	g_fCurve += XM_PI * 0.01f;
	if (g_fCurve > XM_PI) {
		g_fCurve -= XM_2PI;
	}

	g_fCol = cosf(g_fCurve) * 0.2f + 0.8f;
}

void DrawUParts(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();


	//�|�[�Y���j���[�̕\��
	SetPolygonSize(UPARTS_MENU_WIDTH, UPARTS_MENU_HEIGHT);
	for (int nCntUPartsMenu = 0; nCntUPartsMenu < NUM_UPARTS_MENU; ++nCntUPartsMenu) {
		SetPolygonPos(UPARTS_MENU_POS_X + nCntUPartsMenu * UPARTS_MENU_INTERVAL, UPARTS_MENU_POS_Y );

		// �e�N�X�`���̐ݒ�
		SetPolygonTexture(g_pTextures[nCntUPartsMenu]);
		// �|���S���̕`��
		DrawPolygon(pDeviceContext);
	}

	SetPolygonColor(1.0f, 1.0f, 1.0f);
}


//=============================================================================
// �|�[�Y���j���[�̐ݒ�
//=============================================================================
void SetUPartsMenu(void)
{
	g_fCurve = 0.0f;
}

//=============================================================================
// �|�[�Y���j���[�̎擾
//=============================================================================
UPARTS_MENU GetUPartsMenu(void)
{
	return g_nUPartsMenu;
}

//=============================================================================
// �|�[�Y���j���[�̃��Z�b�g
//=============================================================================
void ResetUPartsMenu(void)
{
	g_nUPartsMenu = UPARTS_MENU_KABU;
	SetUPartsMenu();
}
void ResetUPartsMenu1(void)
{
	g_nUPartsMenu = UPARTS_MENU_WANI;
	SetUPartsMenu();
}
void ResetUPartsMenu2(void)
{
	//g_nUPartsMenu = UPARTS_MENU_QUIT;
	//SetUPartsMenu();
}

Player* UPartsGet()
{
	return &UParts;
}
