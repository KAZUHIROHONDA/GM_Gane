//=============================================================================
//
// �v���C���[���� [Mpartsmenu.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include <stdarg.h>
#include <string.h>
#include "Mpartsmenu.h"
#include "polygon.h"	// �|���S���g�p
#include "Texture.h"// �e�N�X�`���g�p
#include "input.h"
#include "sound.h"
#include "collision.h"
#include "model.h"
#include "pchimera.h"
#include "sceneTitle.h"

// �}�N����`
#define	NUM_MPARTS_MENU		(4)			// �|�[�Y���j���[��
#define	MPARTS_MENU_WIDTH	(200.0f)	// �|�[�Y���j���[��
#define	MPARTS_MENU_HEIGHT	(200.0f)		// �|�[�Y���j���[����
#define	MPARTS_MENU_INTERVAL	(300.0f)	// �|�[�Y���j���[�Ԋu
#define	MPARTS_MENU_POS_X	(200.0f)		// �|�[�Y���j���[�ʒu(X���W)
#define	MPARTS_MENU_POS_Y	(200.0f)	// �|�[�Y���j���[�ʒu(Y���W)
#define	PLATE_WIDTH			(360.0f)	// �v���[�g�̕�
#define	PLATE_HEIGHT		(340.0f)	// �v���[�g�̕�
#define	PLATE_POS_X			(0.0f)		// �v���[�g�̈ʒu(X���W)
#define	PLATE_POS_Y			(0.0f)		// �v���[�g�̈ʒu(Y���W)

// �\���̒�`
// �v���g�^�C�v�錾
// �O���[�o���ϐ�
static ID3D11ShaderResourceView*	g_pTextures[5] = { nullptr };	// �e�N�X�`���ւ̃|�C���^

static float g_fCurve = 0.0f;
static float g_fCol = 0.0f;
static int	 nStopTime = 0;

static LPCWSTR c_aFileNameMPartsMenu[NUM_MPARTS_MENU] =
{
	L"data/TEXTURE/kabu2.png",	// �R���e�B�j���[
	L"data/TEXTURE/wani2.png",	// ���g���C
	L"data/TEXTURE/panda2.png",	// ���g���C
	L"data/TEXTURE/wani2.png",	// ���g���C
};

static MPARTS_MENU g_nMPartsMenu = MPARTS_MENU_KABU;	//	�I�𒆂̃��j���[No

Player MParts;

// ����������
HRESULT InitMParts(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr;

	for (int nCntMPartsMenu = 0; nCntMPartsMenu < NUM_MPARTS_MENU; ++nCntMPartsMenu) {
		// �e�N�X�`���̓ǂݍ���
		hr = CreateTextureFromFile(pDevice,									// �f�o�C�X�ւ̃|�C���^
			c_aFileNameMPartsMenu[nCntMPartsMenu],	// �t�@�C���̖��O
			&g_pTextures[nCntMPartsMenu]);			// �ǂݍ��ރ������[
	}

	MParts.SetHP(10);
	MParts.SetPAat(15);
	MParts.SetGUat(20);
	MParts.SetTYOKIat(25);

	g_nMPartsMenu = MPARTS_MENU_KABU;
	g_fCurve = 0.0f;

	return hr;
}

void UninitMParts(void)
{
	// �e�N�X�`���̊J��
	for (int nCntMPartsMenu = 0; nCntMPartsMenu < NUM_MPARTS_MENU; ++nCntMPartsMenu) {
		SAFE_RELEASE(g_pTextures[nCntMPartsMenu]);
	}
}

void UpdateMParts(void)
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
					g_nMPartsMenu = (MPARTS_MENU)((g_nMPartsMenu + 1) % NUM_MPARTS_MENU);
					SetMPartsMenu();
					nStopTime = 10;
				}
				if (angle < 135 || angle > 45)
				{
					len *= 0.3f;
					g_nMPartsMenu = (MPARTS_MENU)((g_nMPartsMenu + NUM_MPARTS_MENU - 1) % NUM_MPARTS_MENU);
					SetMPartsMenu();
					nStopTime = 10;
				}


			}
		}
	}

	//�}�E�X
	POINT temp = (*GetMousePosition());
	XMFLOAT2 mousePos = XMFLOAT2(temp.x - SCREEN_CENTER_X, -(temp.y - SCREEN_CENTER_Y));
	XMFLOAT2 pos1 = XMFLOAT2(MPARTS_MENU_POS_X, MPARTS_MENU_POS_Y);
	XMFLOAT2 pos2 = XMFLOAT2(MPARTS_MENU_POS_X + 1 * MPARTS_MENU_INTERVAL, MPARTS_MENU_POS_Y );
	XMFLOAT2 pos3 = XMFLOAT2(MPARTS_MENU_POS_X, MPARTS_MENU_POS_Y - 1 * 250);
	XMFLOAT2 pos4 = XMFLOAT2(MPARTS_MENU_POS_X + 1 * 250, MPARTS_MENU_POS_Y - 1 * 250);
	XMFLOAT2 radius1 = XMFLOAT2(MPARTS_MENU_WIDTH / 2, MPARTS_MENU_HEIGHT / 2);
	XMFLOAT2 mpos2 = mousePos;
	XMFLOAT2 radius2 = XMFLOAT2(0.1, 0.1);
	if (CollisionBB(&pos1, &radius1, &mpos2, &radius2))
	{
		ResetMPartsMenu();
		if (GetMouseTrigger(0))
		{
			SetMae(MODEL_KABU2);
			MParts.SetHP(10);
			MParts.SetPAat(15);
			MParts.SetGUat(20);
			MParts.SetTYOKIat(25);
			InitPChimera();
		}
	}
	else if (CollisionBB(&pos2, &radius1, &mpos2, &radius2))
	{
		ResetMPartsMenu1();
		if (GetMouseTrigger(0))
		{
			SetMae(MODEL_wani2);
			MParts.SetHP(20);
			MParts.SetPAat(35);
			MParts.SetGUat(40);
			MParts.SetTYOKIat(55);
			InitPChimera();
		}
	}
	else if (CollisionBB(&pos3, &radius1, &mpos2, &radius2))
	{

		if (GetMouseTrigger(0))
		{
			SetMae(MODEL_PANDA3);
			MParts.SetHP(50);
			MParts.SetPAat(50);
			MParts.SetGUat(50);
			MParts.SetTYOKIat(50);
			InitPChimera();
		}
	}
	else if (CollisionBB(&pos4, &radius1, &mpos2, &radius2))
	{

		if (GetMouseTrigger(0))
		{
			SetMae(MODEL_TORI3);
			MParts.SetHP(50);
			MParts.SetPAat(50);
			MParts.SetGUat(50);
			MParts.SetTYOKIat(50);
			InitPChimera();
		}
	}


	// �㉺�L�[�Ŋe���ڊԂ̈ړ�
	if (GetKeyRepeat(VK_W) || GetKeyRepeat(VK_UP)) {
		g_nMPartsMenu = (MPARTS_MENU)((g_nMPartsMenu + NUM_MPARTS_MENU - 1) % NUM_MPARTS_MENU);
		SetMPartsMenu();
	}
	else if (GetKeyRepeat(VK_S) || GetKeyRepeat(VK_DOWN)) {
		g_nMPartsMenu = (MPARTS_MENU)((g_nMPartsMenu + 1) % NUM_MPARTS_MENU);
		SetMPartsMenu();
	}

	// �g�̕����̐F�̕ω�(�O���f�[�V����)
	g_fCurve += XM_PI * 0.01f;
	if (g_fCurve > XM_PI) {
		g_fCurve -= XM_2PI;
	}

	g_fCol = cosf(g_fCurve) * 0.2f + 0.8f;
}

void DrawMParts(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	int nCnt = 0;

	//�|�[�Y���j���[�̕\��
	SetPolygonSize(MPARTS_MENU_WIDTH, MPARTS_MENU_HEIGHT);
	for (int nCntBPartsMenu = 0; nCntBPartsMenu < 2; ++nCntBPartsMenu) {

		for (int nCntBPartsMenu1 = 0; nCntBPartsMenu1 < 2; ++nCntBPartsMenu1) {
			SetPolygonPos(MPARTS_MENU_POS_X + nCntBPartsMenu1 * MPARTS_MENU_INTERVAL,MPARTS_MENU_POS_Y - nCntBPartsMenu * 250);

			// �e�N�X�`���̐ݒ�
			SetPolygonTexture(g_pTextures[nCnt]);
			// �|���S���̕`��
			DrawPolygon(pDeviceContext);
			nCnt++;
		}
	}

	SetPolygonColor(1.0f, 1.0f, 1.0f);
}


//=============================================================================
// �|�[�Y���j���[�̐ݒ�
//=============================================================================
void SetMPartsMenu(void)
{
	g_fCurve = 0.0f;
}

//=============================================================================
// �|�[�Y���j���[�̎擾
//=============================================================================
MPARTS_MENU GetMPartsMenu(void)
{
	return g_nMPartsMenu;
}

//=============================================================================
// �|�[�Y���j���[�̃��Z�b�g
//=============================================================================
void ResetMPartsMenu(void)
{
	g_nMPartsMenu = MPARTS_MENU_KABU;
	SetMPartsMenu();
}
void ResetMPartsMenu1(void)
{
	g_nMPartsMenu = MPARTS_MENU_WANI;
	SetMPartsMenu();
}
void ResetMPartsMenu2(void)
{
	//g_nMPartsMenu = MPARTS_MENU_QUIT;
	//SetMPartsMenu();
}

Player* MPartsGet()
{
	return &MParts;
}
