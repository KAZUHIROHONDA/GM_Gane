#include "Title.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"
#include "MessageManager.h"
#include "jyanken.h"
#include"fade.h"
#include"player.h"
#include"enemy.h"

#define gu 1-1
#define tyoki 2-1
#define pa 3-1

// �}�N����`
#define	NUM_JYANKEN_MENU		(3)			// �|�[�Y���j���[��
#define	JYANKEN_MENU_WIDTH	(200.0f)	// �|�[�Y���j���[��
#define	JYANKEN_MENU_HEIGHT	(200.0f)		// �|�[�Y���j���[����
#define	JYANKEN_MENU_INTERVAL	(200.0f)	// �|�[�Y���j���[�Ԋu
#define	JYANKEN_MENU_POS_X	(0.0f)		// �|�[�Y���j���[�ʒu(X���W)
#define	JYANKEN_MENU_POS_Y	(-50.0f)	// �|�[�Y���j���[�ʒu(Y���W)
#define	PLATE_WIDTH			(360.0f)	// �v���[�g�̕�
#define	PLATE_HEIGHT		(340.0f)	// �v���[�g�̕�
#define	PLATE_POS_X			(0.0f)		// �v���[�g�̈ʒu(X���W)
#define	PLATE_POS_Y			(0.0f)		// �v���[�g�̈ʒu(Y���W)

static ID3D11ShaderResourceView*	g_pTextures[3] = { nullptr };

// �e�N�X�`���t�@�C����
#define TITLE_TEXTURENAME	_T("data/texture/Title.png")
static float g_fCurve = 0.0f;
static float g_fCol = 0.0f;
static int	 nStopTime = 0;

static LPCWSTR c_aFileNameJyankenMenu[NUM_JYANKEN_MENU] =
{
	L"data/TEXTURE/gu.png",	// �R���e�B�j���[
	L"data/TEXTURE/tyoki.png",
	L"data/TEXTURE/pa.png",
};

static JYANKEN_MENU g_nJyankenMenu = JYANKEN_MENU_GU;	//	�I�𒆂̃��j���[No
static int te[5];
static int result[4]; //������, ������, ��������, �A����

int z, i, aite;
int j = 0;
int n = 0;

const tMessage testMessage[9] = {
	{L"����񂯂�T�񏟕��I�I",MESSAGE_TYPE::E_TYPE_ENTER,(long long int)&testMessage[1] },
	{L"�����I",MESSAGE_TYPE::E_TYPE_NORMAL},
	{L"�܂��I",MESSAGE_TYPE::E_TYPE_NORMAL},
	{L"�������I",MESSAGE_TYPE::E_TYPE_NORMAL},
	{L"�P���",MESSAGE_TYPE::E_TYPE_NORMAL},
	{L"�Q���",MESSAGE_TYPE::E_TYPE_NORMAL},
	{L"�R���",MESSAGE_TYPE::E_TYPE_NORMAL},
	{L"�S���",MESSAGE_TYPE::E_TYPE_NORMAL},
	{L"�T���",MESSAGE_TYPE::E_TYPE_NORMAL},
};

HRESULT InitJyanken()
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr;

	SetMessage((tMessage*)&testMessage[0]);
	for (int nCntJyankenMenu = 0; nCntJyankenMenu < NUM_JYANKEN_MENU; ++nCntJyankenMenu) {
		// �e�N�X�`���̓ǂݍ���
		hr = CreateTextureFromFile(pDevice,									// �f�o�C�X�ւ̃|�C���^
			c_aFileNameJyankenMenu[nCntJyankenMenu],	// �t�@�C���̖��O
			&g_pTextures[nCntJyankenMenu]);			// �ǂݍ��ރ������[
	}

	g_nJyankenMenu = JYANKEN_MENU_GU;
	g_fCurve = 0.0f;

	return hr;
}

void UninitJyanken()
{
	// �e�N�X�`���̊J��
	for (int nCntJyankenMenu = 0; nCntJyankenMenu < NUM_JYANKEN_MENU; ++nCntJyankenMenu) {
		SAFE_RELEASE(g_pTextures[nCntJyankenMenu]);
	}
}

void UpdateJyanken()
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
					g_nJyankenMenu = (JYANKEN_MENU)((g_nJyankenMenu + 1) % NUM_JYANKEN_MENU);
					SetJyankenMenu();
					nStopTime = 10;
				}
				if (angle < 135 || angle > 45)
				{
					len *= 0.3f;
					g_nJyankenMenu = (JYANKEN_MENU)((g_nJyankenMenu + NUM_JYANKEN_MENU - 1) % NUM_JYANKEN_MENU);
					SetJyankenMenu();
					nStopTime = 10;
				}


			}
		}
	}

	// �㉺�L�[�Ŋe���ڊԂ̈ړ�
	if (GetKeyRepeat(VK_A) || GetKeyRepeat(VK_RIGHT)) {
		g_nJyankenMenu = (JYANKEN_MENU)((g_nJyankenMenu + NUM_JYANKEN_MENU - 1) % NUM_JYANKEN_MENU);
		SetJyankenMenu();
	}
	else if (GetKeyRepeat(VK_D) || GetKeyRepeat(VK_LEFT)) {
		g_nJyankenMenu = (JYANKEN_MENU)((g_nJyankenMenu + 1) % NUM_JYANKEN_MENU);
		SetJyankenMenu();
	}

	// �g�̕����̐F�̕ω�(�O���f�[�V����)
	g_fCurve += XM_PI * 0.01f;
	if (g_fCurve > XM_PI) {
		g_fCurve -= XM_2PI;
	}

	g_fCol = cosf(g_fCurve) * 0.2f + 0.8f;


	if (j <= 5)
	{
		SetMessage((tMessage*)&testMessage[j + 4]);
		if (GetKeyTrigger(VK_RETURN) || GetJoyTrigger(0, 0))
		{
			//�I�𒆂̂��̂ɂ�蕪��
			JYANKEN_MENU menu = GetJyankenMenu();
			switch (menu)
			{
				//�O�[
			case JYANKEN_MENU_GU:
				te[j] = 0;
				j++;
				//Jyanken(0);
				break;
				//�`���L
			case JYANKEN_MENU_TYOKI:
				te[j] = 1;
				j++;
				//Jyanken(1);
				break;
				//�p�[
			case JYANKEN_MENU_PA:
				te[j] = 2;
				j++;
				//Jyanken(2);
				break;
			}
		}
	}

	if (j > 5)
	{
		if (n <= 4)
		{
			if (GetKeyTrigger(VK_RETURN) || GetJoyTrigger(0, 0))
			{
				Jyanken(te[n], result);
				n++;
			}
		}
		else if (n > 4)
		{
			j = 0;
			n = 0;
			for (int r = 0; r < 4; r++)
			{
				result[r] = 0;
			}
		}
	}
}


void DrawJyanken()
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();


	//�|�[�Y���j���[�̕\��
	SetPolygonSize(JYANKEN_MENU_WIDTH, JYANKEN_MENU_HEIGHT);
	for (int nCntJyankenMenu = 0; nCntJyankenMenu < NUM_JYANKEN_MENU; ++nCntJyankenMenu) {
		SetPolygonPos(JYANKEN_MENU_POS_X - nCntJyankenMenu * JYANKEN_MENU_INTERVAL, JYANKEN_MENU_POS_Y);



		//�I������Ă��郁�j���[��ڗ�������
		if (nCntJyankenMenu == g_nJyankenMenu)
		{
			SetPolygonColor(1.0f, 1.0f, 1.0f);
		}
		else
		{
			SetPolygonColor(0.3f, 0.3f, 0.3f);
		}

		// �e�N�X�`���̐ݒ�
		SetPolygonTexture(g_pTextures[nCntJyankenMenu]);
		// �|���S���̕`��
		DrawPolygon(pDeviceContext);
	}

	SetPolygonColor(1.0f, 1.0f, 1.0f);

}

//����񂯂�
void Jyanken(int no, int *cnt)
{
	i = no;

	aite = rand() % 3;//����


	if (aite == i)
	{
		SetMessage((tMessage*)&testMessage[3]);
		cnt[2]++;	// ������
		//StartFade(SCENE_TITLE,180);
		//z = z - 1;
	}
	//else if (i == 0)i = 3;
	//if (aite < i)
	else if ((i == 0 && aite == 1) || (i == 1 && aite == 2) || (i == 2 && aite == 0))
	{
		SetMessage((tMessage*)&testMessage[1]);
		DamageEnemy(50);
		if (cnt[0] >= 0)
			DamageEnemy(50 * cnt[0]);
		cnt[0]++;	// ����
		cnt[3]++;	// �A��
		//printf("����\n\n");
	}
	else
	{
		SetMessage((tMessage*)&testMessage[2]);
		DamagePlayer(50);
		if (cnt[1] >= 0)
			DamagePlayer(50 * cnt[1]);
		cnt[1]++;	// �܂�
		cnt[3] = 0;	// �A�����Z�b�g
		//printf("����\n\n");
	}


}

//=============================================================================
// �|�[�Y���j���[�̐ݒ�
//=============================================================================
void SetJyankenMenu(void)
{
	g_fCurve = 0.0f;
}

//=============================================================================
// �|�[�Y���j���[�̎擾
//=============================================================================
JYANKEN_MENU GetJyankenMenu(void)
{
	return g_nJyankenMenu;
}

//=============================================================================
// �|�[�Y���j���[�̃��Z�b�g
//=============================================================================
void ResetJyankenMenu(void)
{
	g_nJyankenMenu = JYANKEN_MENU_GU;
	SetJyankenMenu();
}






