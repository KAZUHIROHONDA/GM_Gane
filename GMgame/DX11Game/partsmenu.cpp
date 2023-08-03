//=============================================================================
//
// �v���C���[���� [partsmenu.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include <stdarg.h>
#include <string.h>
#include "partsmenu.h"
#include "polygon.h"	// �|���S���g�p
#include "Texture.h"// �e�N�X�`���g�p
#include "input.h"
#include "sound.h"
#include "collision.h"
#include "model.h"
#include "pchimera.h"
#include "playercs.h"

// �}�N����`
#define	NUM_PARTS_MENU		(2)			// �|�[�Y���j���[��
#define	PARTS_MENU_WIDTH	(200.0f)	// �|�[�Y���j���[��
#define	PARTS_MENU_HEIGHT	(200.0f)		// �|�[�Y���j���[����
#define	PARTS_MENU_INTERVAL	(300.0f)	// �|�[�Y���j���[�Ԋu
#define	PARTS_MENU_POS_X	(200.0f)		// �|�[�Y���j���[�ʒu(X���W)
#define	PARTS_MENU_POS_Y	(200.0f)	// �|�[�Y���j���[�ʒu(Y���W)
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

static LPCWSTR c_aFileNamePartsMenu[NUM_PARTS_MENU] =
{
	L"data/TEXTURE/kabu.png",	// �R���e�B�j���[
	L"data/TEXTURE/wani.png",	// ���g���C
};

static PARTS_MENU g_nPartsMenu = PARTS_MENU_KABU;	//	�I�𒆂̃��j���[No

Player Parts;

// ����������
HRESULT InitParts(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr;

	for (int nCntPartsMenu = 0; nCntPartsMenu < NUM_PARTS_MENU; ++nCntPartsMenu) {
		// �e�N�X�`���̓ǂݍ���
		hr = CreateTextureFromFile(pDevice,									// �f�o�C�X�ւ̃|�C���^
			c_aFileNamePartsMenu[nCntPartsMenu],	// �t�@�C���̖��O
			&g_pTextures[nCntPartsMenu]);			// �ǂݍ��ރ������[
	}

	Parts.SetHP(10);
	Parts.SetPAat(10);
	Parts.SetGUat(10);
	Parts.SetTYOKIat(10);

	g_nPartsMenu = PARTS_MENU_KABU;
	g_fCurve = 0.0f;

	return hr;
}

void UninitParts(void)
{
	// �e�N�X�`���̊J��
	for (int nCntPartsMenu = 0; nCntPartsMenu < NUM_PARTS_MENU; ++nCntPartsMenu) {
		SAFE_RELEASE(g_pTextures[nCntPartsMenu]);
	}
}

void UpdateParts(void)
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
					g_nPartsMenu = (PARTS_MENU)((g_nPartsMenu + 1) % NUM_PARTS_MENU);
					SetPartsMenu();
					nStopTime = 10;
				}
				if (angle < 135 || angle > 45)
				{
					len *= 0.3f;
					g_nPartsMenu = (PARTS_MENU)((g_nPartsMenu + NUM_PARTS_MENU - 1) % NUM_PARTS_MENU);
					SetPartsMenu();
					nStopTime = 10;
				}


			}
		}
	}

	//�}�E�X
	POINT temp = (*GetMousePosition());
	XMFLOAT2 mousePos = XMFLOAT2(temp.x - SCREEN_CENTER_X, -(temp.y - SCREEN_CENTER_Y));
	XMFLOAT2 pos1 = XMFLOAT2(PARTS_MENU_POS_X, PARTS_MENU_POS_Y);
	XMFLOAT2 pos2 = XMFLOAT2(PARTS_MENU_POS_X + 1 * PARTS_MENU_INTERVAL, PARTS_MENU_POS_Y );
	XMFLOAT2 radius1 = XMFLOAT2(PARTS_MENU_WIDTH / 2, PARTS_MENU_HEIGHT / 2);
	XMFLOAT2 mpos2 = mousePos;
	XMFLOAT2 radius2 = XMFLOAT2(0.1, 0.1);
	if (CollisionBB(&pos1, &radius1, &mpos2, &radius2))
	{
		ResetPartsMenu();
		if (GetMouseTrigger(0))
		{
			SetHead(MODEL_KABU);
			Parts.SetHP(10);
			Parts.SetPAat(10);
			Parts.SetGUat(10);
			Parts.SetTYOKIat(10);
			InitPChimera();
		}
	}
	else if (CollisionBB(&pos2, &radius1, &mpos2, &radius2))
	{
		ResetPartsMenu1();
		if (GetMouseTrigger(0))
		{
			SetHead(MODEL_wani);
			Parts.SetHP(15);
			Parts.SetPAat(15);
			Parts.SetGUat(15);
			Parts.SetTYOKIat(15);
			InitPChimera();
		}
	}


	// �㉺�L�[�Ŋe���ڊԂ̈ړ�
	if (GetKeyRepeat(VK_W) || GetKeyRepeat(VK_UP)) {
		g_nPartsMenu = (PARTS_MENU)((g_nPartsMenu + NUM_PARTS_MENU - 1) % NUM_PARTS_MENU);
		SetPartsMenu();
	}
	else if (GetKeyRepeat(VK_S) || GetKeyRepeat(VK_DOWN)) {
		g_nPartsMenu = (PARTS_MENU)((g_nPartsMenu + 1) % NUM_PARTS_MENU);
		SetPartsMenu();
	}

	// �g�̕����̐F�̕ω�(�O���f�[�V����)
	g_fCurve += XM_PI * 0.01f;
	if (g_fCurve > XM_PI) {
		g_fCurve -= XM_2PI;
	}

	g_fCol = cosf(g_fCurve) * 0.2f + 0.8f;
}

void DrawParts(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();


	//�|�[�Y���j���[�̕\��
	SetPolygonSize(PARTS_MENU_WIDTH, PARTS_MENU_HEIGHT);
	for (int nCntPartsMenu = 0; nCntPartsMenu < NUM_PARTS_MENU; ++nCntPartsMenu) {
		SetPolygonPos(PARTS_MENU_POS_X + nCntPartsMenu * PARTS_MENU_INTERVAL, PARTS_MENU_POS_Y );


		// �e�N�X�`���̐ݒ�
		SetPolygonTexture(g_pTextures[nCntPartsMenu]);
		// �|���S���̕`��
		DrawPolygon(pDeviceContext);
	}

	SetPolygonColor(1.0f, 1.0f, 1.0f);
}


//=============================================================================
// �|�[�Y���j���[�̐ݒ�
//=============================================================================
void SetPartsMenu(void)
{
	g_fCurve = 0.0f;
}

//=============================================================================
// �|�[�Y���j���[�̎擾
//=============================================================================
PARTS_MENU GetPartsMenu(void)
{
	return g_nPartsMenu;
}

//=============================================================================
// �|�[�Y���j���[�̃��Z�b�g
//=============================================================================
void ResetPartsMenu(void)
{
	g_nPartsMenu = PARTS_MENU_KABU;
	SetPartsMenu();
}
void ResetPartsMenu1(void)
{
	g_nPartsMenu = PARTS_MENU_WANI;
	SetPartsMenu();
}
void ResetPartsMenu2(void)
{
	//g_nPartsMenu = PARTS_MENU_QUIT;
	//SetPartsMenu();
}

Player* PartsGet()
{
	return &Parts;
}
