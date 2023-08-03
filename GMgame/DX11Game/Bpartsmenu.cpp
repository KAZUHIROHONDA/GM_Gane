//=============================================================================
//
// �v���C���[���� [Bpartsmenu.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include <stdarg.h>
#include <string.h>
#include "Bpartsmenu.h"
#include "polygon.h"	// �|���S���g�p
#include "Texture.h"// �e�N�X�`���g�p
#include "input.h"
#include "sound.h"
#include "collision.h"
#include "model.h"
#include "pchimera.h"
#include "sceneTitle.h"

// �}�N����`
#define	NUM_BPARTS_MENU		(2)			// �|�[�Y���j���[��
#define	BPARTS_MENU_WIDTH	(200.0f)	// �|�[�Y���j���[��
#define	BPARTS_MENU_HEIGHT	(200.0f)		// �|�[�Y���j���[����
#define	BPARTS_MENU_INTERVAL	(300.0f)	// �|�[�Y���j���[�Ԋu
#define	BPARTS_MENU_POS_X	(200.0f)		// �|�[�Y���j���[�ʒu(X���W)
#define	BPARTS_MENU_POS_Y	(200.0f)	// �|�[�Y���j���[�ʒu(Y���W)
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

static LPCWSTR c_aFileNameBPartsMenu[NUM_BPARTS_MENU] =
{
	L"data/TEXTURE/kabu1.png",	// �R���e�B�j���[
	L"data/TEXTURE/wani1.png",	// ���g���C
};

static BPARTS_MENU g_nBPartsMenu = BPARTS_MENU_KABU;	//	�I�𒆂̃��j���[No

Player BParts;

// ����������
HRESULT InitBParts(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr;

	for (int nCntBPartsMenu = 0; nCntBPartsMenu < NUM_BPARTS_MENU; ++nCntBPartsMenu) {
		// �e�N�X�`���̓ǂݍ���
		hr = CreateTextureFromFile(pDevice,									// �f�o�C�X�ւ̃|�C���^
			c_aFileNameBPartsMenu[nCntBPartsMenu],	// �t�@�C���̖��O
			&g_pTextures[nCntBPartsMenu]);			// �ǂݍ��ރ������[
	}

	GetPlayer()->SetName("�J�u�g���V");
	BParts.SetHP(10);
	BParts.SetPAat(10);
	BParts.SetGUat(10);
	BParts.SetTYOKIat(10);

	g_nBPartsMenu = BPARTS_MENU_KABU;
	g_fCurve = 0.0f;

	return hr;
}

void UninitBParts(void)
{
	// �e�N�X�`���̊J��
	for (int nCntBPartsMenu = 0; nCntBPartsMenu < NUM_BPARTS_MENU; ++nCntBPartsMenu) {
		SAFE_RELEASE(g_pTextures[nCntBPartsMenu]);
	}
}

void UpdateBParts(void)
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
					g_nBPartsMenu = (BPARTS_MENU)((g_nBPartsMenu + 1) % NUM_BPARTS_MENU);
					SetBPartsMenu();
					nStopTime = 10;
				}
				if (angle < 135 || angle > 45)
				{
					len *= 0.3f;
					g_nBPartsMenu = (BPARTS_MENU)((g_nBPartsMenu + NUM_BPARTS_MENU - 1) % NUM_BPARTS_MENU);
					SetBPartsMenu();
					nStopTime = 10;
				}


			}
		}
	}

	//�}�E�X
	POINT temp = (*GetMousePosition());
	XMFLOAT2 mousePos = XMFLOAT2(temp.x - SCREEN_CENTER_X, -(temp.y - SCREEN_CENTER_Y));
	XMFLOAT2 pos1 = XMFLOAT2(BPARTS_MENU_POS_X, BPARTS_MENU_POS_Y);
	XMFLOAT2 pos2 = XMFLOAT2(BPARTS_MENU_POS_X + 1 * BPARTS_MENU_INTERVAL, BPARTS_MENU_POS_Y );
	XMFLOAT2 radius1 = XMFLOAT2(BPARTS_MENU_WIDTH / 2, BPARTS_MENU_HEIGHT / 2);
	XMFLOAT2 mpos2 = mousePos;
	XMFLOAT2 radius2 = XMFLOAT2(0.1, 0.1);
	if (CollisionBB(&pos1, &radius1, &mpos2, &radius2))
	{
		ResetBPartsMenu();
		if (GetMouseTrigger(0))
		{
			SetBody(MODEL_KABU1);
			GetPlayer()->SetName("�J�u�g���V");
			BParts.SetHP(10);
			BParts.SetPAat(10);
			BParts.SetGUat(10);
			BParts.SetTYOKIat(10);
		    InitPChimera();
		}
	}
	else if (CollisionBB(&pos2, &radius1, &mpos2, &radius2))
	{
		ResetBPartsMenu1();
		if (GetMouseTrigger(0))
		{
			SetBody(MODEL_wani1);
			GetPlayer()->SetName("���j");
			BParts.SetHP(20);
			BParts.SetPAat(20);
			BParts.SetGUat(20);
			BParts.SetTYOKIat(20);
			InitPChimera();
		}
	}


	// �㉺�L�[�Ŋe���ڊԂ̈ړ�
	if (GetKeyRepeat(VK_W) || GetKeyRepeat(VK_UP)) {
		g_nBPartsMenu = (BPARTS_MENU)((g_nBPartsMenu + NUM_BPARTS_MENU - 1) % NUM_BPARTS_MENU);
		SetBPartsMenu();
	}
	else if (GetKeyRepeat(VK_S) || GetKeyRepeat(VK_DOWN)) {
		g_nBPartsMenu = (BPARTS_MENU)((g_nBPartsMenu + 1) % NUM_BPARTS_MENU);
		SetBPartsMenu();
	}

	// �g�̕����̐F�̕ω�(�O���f�[�V����)
	g_fCurve += XM_PI * 0.01f;
	if (g_fCurve > XM_PI) {
		g_fCurve -= XM_2PI;
	}

	g_fCol = cosf(g_fCurve) * 0.2f + 0.8f;
}

void DrawBParts(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();


	//�|�[�Y���j���[�̕\��
	SetPolygonSize(BPARTS_MENU_WIDTH, BPARTS_MENU_HEIGHT);
	for (int nCntBPartsMenu = 0; nCntBPartsMenu < NUM_BPARTS_MENU; ++nCntBPartsMenu) {
		SetPolygonPos(BPARTS_MENU_POS_X + nCntBPartsMenu * BPARTS_MENU_INTERVAL, BPARTS_MENU_POS_Y );



		//�I������Ă��郁�j���[��ڗ�������
		//if (nCntBPartsMenu == g_nBPartsMenu)
		//{
		//	SetPolygonColor(1.0f, 1.0f, 1.0f);
		//}
		//else
		//{
		//	SetPolygonColor(0.3f, 0.3f, 0.3f);
		//}

		// �e�N�X�`���̐ݒ�
		SetPolygonTexture(g_pTextures[nCntBPartsMenu]);
		// �|���S���̕`��
		DrawPolygon(pDeviceContext);
	}

	SetPolygonColor(1.0f, 1.0f, 1.0f);
}


//=============================================================================
// �|�[�Y���j���[�̐ݒ�
//=============================================================================
void SetBPartsMenu(void)
{
	g_fCurve = 0.0f;
}

//=============================================================================
// �|�[�Y���j���[�̎擾
//=============================================================================
BPARTS_MENU GetBPartsMenu(void)
{
	return g_nBPartsMenu;
}

//=============================================================================
// �|�[�Y���j���[�̃��Z�b�g
//=============================================================================
void ResetBPartsMenu(void)
{
	g_nBPartsMenu = BPARTS_MENU_KABU;
	SetBPartsMenu();
}
void ResetBPartsMenu1(void)
{
	g_nBPartsMenu = BPARTS_MENU_WANI;
	SetBPartsMenu();
}

Player* BPartsGet()
{
	return &BParts;
}

void ResetBPartsMenu2(void)
{
	//g_nBPartsMenu = BPARTS_MENU_QUIT;
	//SetBPartsMenu();
}
