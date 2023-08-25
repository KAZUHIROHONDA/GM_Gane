#include "Title.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"
#include "MessageManager.h"
#include "jyanken.h"
#include "fade.h"
#include "player.h"
#include "enemy.h"
#include "sceneGame.h"
#include "collision.h"
#include "sceneTitle.h"

#define gu 1-1
#define tyoki 2-1
#define pa 3-1

// �}�N����`
#define	NUM_JYANKEN_MENU		(3)			// �|�[�Y���j���[��
#define	JYANKEN_MENU_WIDTH	(100.0f)	// �|�[�Y���j���[��
#define	JYANKEN_MENU_HEIGHT	(100.0f)		// �|�[�Y���j���[����
#define	JYANKEN_MENU_INTERVAL	(100.0f)	// �|�[�Y���j���[�Ԋu
#define	JYANKEN_MENU_POS_X	(100.0f)		// �|�[�Y���j���[�ʒu(X���W)
#define	JYANKEN_MENU_POS_Y	(-250.0f)	// �|�[�Y���j���[�ʒu(Y���W)
#define	PLATE_WIDTH			(360.0f)	// �v���[�g�̕�
#define	PLATE_HEIGHT		(340.0f)	// �v���[�g�̕�
#define	PLATE_POS_X			(0.0f)		// �v���[�g�̈ʒu(X���W)
#define	PLATE_POS_Y			(0.0f)		// �v���[�g�̈ʒu(Y���W)

static ID3D11ShaderResourceView*	g_pTextures[3] = { nullptr };

// �e�N�X�`���t�@�C����
#define TITLE_TEXTURENAME	_T("data/texture/button.png")
static float g_fCurve = 0.0f;
static float g_fCol = 0.0f;
static int	 nStopTime = 0;

static LPCWSTR c_aFileNameJyankenMenu[NUM_JYANKEN_MENU] =
{
	L"data/TEXTURE/gu.png",	// �R���e�B�j���[
	L"data/TEXTURE/tyoki.png",
	L"data/TEXTURE/pa.png",
};

static JYANKEN_MENU g_nJyankenMenu = JYANKEN_MENU_1;	//	�I�𒆂̃��j���[No
static int te[5] = {-1,-1,-1,-1,-1};
static int selte[5] = { -1,-1,-1,-1,-1 };
static int aite[5] = { -1,-1,-1,-1,-1 };
static int result[4]; //������, ������, ��������, �A����
static int Sette[20] = { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};//���̎�D
static int Setaite[20] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };//���̎�D

bool jadge = false;
int z, i;
int j = 0;
int n = 0;
int f = 0;
int Cnt, Cnt1 = 0;
bool win = false;
bool lose = false;
int pCnt = 180;
bool useflag1 = true;
bool useflag2 = true;
bool useflag3 = true;
bool useflag4 = true;
bool useflag5 = true;
float  size = 100.0f;
int nPhase = 1;
int PAT, EAT = 0;
int Ac = 0;

//OK�{�^��
#define OK_TEXTURENAME	_T("data/texture/button.png")
#define HATENA_TEXTURENAME	_T("data/texture/1mdpi.png")
		
#define OK_POS_X	(300)					//�����ʒuX
#define OK_POS_Y	(-250)					//�����ʒuY
#define OK_SIZE_X	(100)		//����
#define OK_SIZE_Y	(100)		//�c��

#define HATENA_POS_X	(300)					//�����ʒuX
#define HATENA_POS_Y	(-250)					//�����ʒuY
#define HATENA_SIZE_X	(100)		//����
#define HATENA_SIZE_Y	(100)		//�c��

static ID3D11ShaderResourceView*	g_pTexture;				// �e�N�X�`��
static ID3D11ShaderResourceView*	g_pTexture1;				// �e�N�X�`��
static XMFLOAT3 g_Position;//�ʒu
static XMFLOAT2    g_Angle;   //�p�x
static XMFLOAT3	g_Rot;
static XMFLOAT2 g_Size;    //�T�C�Y
static XMFLOAT2 g_Scale;   //�g�嗦 
static float	g_Alpha;   //����

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

	// �e�N�X�`���ǂݍ���
	hr = CreateTextureFromFile(GetDevice(), TITLE_TEXTURENAME, &g_pTexture);
	if (FAILED(hr))return hr;
	hr = CreateTextureFromFile(GetDevice(), HATENA_TEXTURENAME, &g_pTexture1);
	if (FAILED(hr))return hr;


	for (int nCntJyankenMenu = 0; nCntJyankenMenu < NUM_JYANKEN_MENU; ++nCntJyankenMenu) {
		// �e�N�X�`���̓ǂݍ���
		hr = CreateTextureFromFile(pDevice,									// �f�o�C�X�ւ̃|�C���^
			c_aFileNameJyankenMenu[nCntJyankenMenu],	// �t�@�C���̖��O
			&g_pTextures[nCntJyankenMenu]);			// �ǂݍ��ރ������[
	}


	g_Position = XMFLOAT3(OK_POS_X, OK_POS_Y, 0.0f);
	g_Alpha = 1.0f;
	g_Scale = XMFLOAT2(1.0f, 1.0f);
    jadge = false;
	j = 0;
	n = 0;
	f = 0;
	Cnt, Cnt1 = 0;
	win = false;
	lose = false;
	pCnt = 180;
	useflag1 = true;
	useflag2 = true;
	useflag3 = true;
	useflag4 = true;
	useflag5 = true;
	size = 100.0f;
	nPhase = 1;
	pCnt = 180;
	GetPhase()->ChangePhase(STARTPHASE);
	g_nJyankenMenu = JYANKEN_MENU_1;
	g_fCurve = 0.0f;

	for (int i = 0; i < 20; i++)
	{
		Sette[i] = GetPlayer()->GetHand(i);
		Setaite[i] = GetEnemy()->GetHand(i);
	}
	
	return hr;
}

void UninitJyanken()
{
	// �e�N�X�`���̊J��
	for (int nCntJyankenMenu = 0; nCntJyankenMenu < NUM_JYANKEN_MENU; ++nCntJyankenMenu) {
		SAFE_RELEASE(g_pTextures[nCntJyankenMenu]);
	}
	// �e�N�X�`���J��
	SAFE_RELEASE(g_pTexture);
	SAFE_RELEASE(g_pTexture1);
}

void UpdateJyankenStart()
{
	pCnt--;
	if (pCnt < 0)
	{
		for (int f = 0; f < 5; f++)
		{
			aite[f] = Setaite[rand() % 20];//����
			selte[f] = Sette[rand() % 20];
			CameraDEBUG();
		}
		GetPhase()->ChangePhase(BATTLEPHASE);
	}
}

void UpdateJyankenSet()
{

	// �㉺�L�[�Ŋe���ڊԂ̈ړ�
	//if (GetKeyRepeat(VK_D) || GetKeyRepeat(VK_RIGHT)) {
	//	g_nJyankenMenu = (JYANKEN_MENU)((g_nJyankenMenu + NUM_JYANKEN_MENU - 1) % NUM_JYANKEN_MENU);
	//	SetJyankenMenu();
	//}
	//else if (GetKeyRepeat(VK_A) || GetKeyRepeat(VK_LEFT)) {
	//	g_nJyankenMenu = (JYANKEN_MENU)((g_nJyankenMenu + 1) % NUM_JYANKEN_MENU);
	//	SetJyankenMenu();
	//}

	switch (nPhase)
	{
	case 1:	size += 0.1f;	
		if (size > 120.0f) nPhase = -1;
		break;

	case -1:size -= 0.1f;				
		if (size < 110) nPhase = +1;
		break;
	}


	//�}�E�X
	POINT temp = (*GetMousePosition());
	XMFLOAT2 mousePos = XMFLOAT2(temp.x - SCREEN_CENTER_X, -(temp.y - SCREEN_CENTER_Y));
	XMFLOAT2 pos1 = XMFLOAT2(JYANKEN_MENU_POS_X, JYANKEN_MENU_POS_Y);
	XMFLOAT2 pos2 = XMFLOAT2(JYANKEN_MENU_POS_X - 1 * JYANKEN_MENU_INTERVAL, JYANKEN_MENU_POS_Y);
	XMFLOAT2 pos3 = XMFLOAT2(JYANKEN_MENU_POS_X - 2 * JYANKEN_MENU_INTERVAL, JYANKEN_MENU_POS_Y);
	XMFLOAT2 pos4 = XMFLOAT2(JYANKEN_MENU_POS_X - 3 * JYANKEN_MENU_INTERVAL, JYANKEN_MENU_POS_Y);
	XMFLOAT2 pos5 = XMFLOAT2(JYANKEN_MENU_POS_X - 4 * JYANKEN_MENU_INTERVAL, JYANKEN_MENU_POS_Y);
	XMFLOAT2 radius1 = XMFLOAT2(JYANKEN_MENU_WIDTH / 2, JYANKEN_MENU_HEIGHT / 2);
	XMFLOAT2 mpos2 = mousePos;
	XMFLOAT2 radius2 = XMFLOAT2(0.1, 0.1);
	if (CollisionBB(&pos1, &radius1, &mpos2, &radius2)&& useflag1 == true)
	{
		SetJyankenMenu1();
		if (GetMouseTrigger(0))
		{
			te[j] = selte[0];
			j++;
			useflag1 = false;
			if (useflag2 != false)
			{
				g_nJyankenMenu = JYANKEN_MENU_2;
			}
			else if (useflag3 != false)
			{
				g_nJyankenMenu = JYANKEN_MENU_3;
			}
			else if (useflag4 != false)
			{
				g_nJyankenMenu = JYANKEN_MENU_4;
			}
			else if (useflag5 != false)
			{
				g_nJyankenMenu = JYANKEN_MENU_5;
			}
			SetJyankenMenu();
		}
	}
	else if (CollisionBB(&pos2, &radius1, &mpos2, &radius2) && useflag2 == true)
	{
		SetJyankenMenu2();
		if (GetMouseTrigger(0))
		{
			te[j] = selte[1];
			j++;
			useflag2 = false;
			if (useflag1 != false)
			{
				g_nJyankenMenu = JYANKEN_MENU_1;
			}
			else if (useflag3 != false)
			{
				g_nJyankenMenu = JYANKEN_MENU_3;
			}
			else if (useflag4 != false)
			{
				g_nJyankenMenu = JYANKEN_MENU_4;
			}
			else if (useflag5 != false)
			{
				g_nJyankenMenu = JYANKEN_MENU_5;
			}
			SetJyankenMenu();
		}
	}
	else if (CollisionBB(&pos3, &radius1, &mpos2, &radius2) && useflag3 == true)
	{
		SetJyankenMenu3();
		if (GetMouseTrigger(0))
		{
			te[j] = selte[2];
			j++;
			useflag3 = false;
			if (useflag2 != false)
			{
				g_nJyankenMenu = JYANKEN_MENU_2;
			}
			else if (useflag1 != false)
			{
				g_nJyankenMenu = JYANKEN_MENU_1;
			}
			else if (useflag4 != false)
			{
				g_nJyankenMenu = JYANKEN_MENU_4;
			}
			else if (useflag5 != false)
			{
				g_nJyankenMenu = JYANKEN_MENU_5;
			}
			SetJyankenMenu();
		}
	}
	else if (CollisionBB(&pos4, &radius1, &mpos2, &radius2) && useflag4 == true)
	{
		SetJyankenMenu4();
		if (GetMouseTrigger(0))
		{
			te[j] = selte[3];
			j++;
			useflag4 = false;
			if (useflag2 != false)
			{
				g_nJyankenMenu = JYANKEN_MENU_2;
			}
			else if (useflag3 != false)
			{
				g_nJyankenMenu = JYANKEN_MENU_3;
			}
			else if (useflag1 != false)
			{
				g_nJyankenMenu = JYANKEN_MENU_1;
			}
			else if (useflag5 != false)
			{
				g_nJyankenMenu = JYANKEN_MENU_5;
			}
			SetJyankenMenu();
		}
	}
	else if (CollisionBB(&pos5, &radius1, &mpos2, &radius2) && useflag5 == true)
	{

		SetJyankenMenu5();
		if (GetMouseTrigger(0))
		{
			te[j] = selte[4];
			j++;
			useflag5 = false;
			if (useflag2 != false)
			{
				g_nJyankenMenu = JYANKEN_MENU_2;
			}
			else if (useflag3 != false)
			{
				g_nJyankenMenu = JYANKEN_MENU_3;
			}
			else if (useflag4 != false)
			{
				g_nJyankenMenu = JYANKEN_MENU_4;
			}
			else if (useflag1 != false)
			{
				g_nJyankenMenu = JYANKEN_MENU_1;
			}
			SetJyankenMenu();
		}
	}

	if (j >= 5)
	{
		XMFLOAT2 pos4 = XMFLOAT2(OK_POS_X, OK_POS_Y);
		XMFLOAT2 radius3 = XMFLOAT2(OK_SIZE_X / 2, OK_SIZE_X / 2);
		if (CollisionBB(&pos4, &radius3, &mpos2, &radius2))
		{
			if (GetMouseTrigger(0))
			{
				GetPhase()->ChangePhase(JUDGEPHASE);
				Cntadd();
				CameraFIXED();
			}
		}
	}

	// �g�̕����̐F�̕ω�(�O���f�[�V����)
	g_fCurve += XM_PI * 0.01f;
	if (g_fCurve > XM_PI) {
		g_fCurve -= XM_2PI;
	}

	g_fCol = cosf(g_fCurve) * 0.2f + 0.8f;

	//�܌ɂȂ�܂őI�ׂ�
	if (j < 5)
	{
		if (GetKeyTrigger(VK_RETURN) || GetJoyTrigger(0, 0))
		{
			//�I�𒆂̂��̂ɂ�蕪��
			JYANKEN_MENU menu = GetJyankenMenu();
			switch (menu)
			{
				//�O�[
			case JYANKEN_MENU_1:

				te[j] = selte[0];
				j++;
				useflag1 = false;
				if (useflag2 != false)
				{
					g_nJyankenMenu = JYANKEN_MENU_2;
				}
				else if (useflag3 != false)
				{
					g_nJyankenMenu = JYANKEN_MENU_3;
				}
				else if (useflag4 != false)
				{
					g_nJyankenMenu = JYANKEN_MENU_4;
				}
				else if (useflag5 != false)
				{
					g_nJyankenMenu = JYANKEN_MENU_5;
				}
				SetJyankenMenu();
				break;
				//�`���L
			case JYANKEN_MENU_2:

				te[j] = selte[1];
				j++;
				useflag2 = false;
				if (useflag1 != false)
				{
					g_nJyankenMenu = JYANKEN_MENU_1;
				}
				else if (useflag3 != false)
				{
					g_nJyankenMenu = JYANKEN_MENU_3;
				}
				else if (useflag4 != false)
				{
					g_nJyankenMenu = JYANKEN_MENU_4;
				}
				else if (useflag5 != false)
				{
					g_nJyankenMenu = JYANKEN_MENU_5;
				}
				SetJyankenMenu();
				break;
				//�p�[
			case JYANKEN_MENU_3:

				te[j] = selte[2];
				j++;
				useflag3 = false;
				if (useflag2 != false)
				{
					g_nJyankenMenu = JYANKEN_MENU_2;
				}
				else if (useflag1 != false)
				{
					g_nJyankenMenu = JYANKEN_MENU_1;
				}
				else if (useflag4 != false)
				{
					g_nJyankenMenu = JYANKEN_MENU_4;
				}
				else if (useflag5 != false)
				{
					g_nJyankenMenu = JYANKEN_MENU_5;
				}
				SetJyankenMenu();
				break;
			case JYANKEN_MENU_4:

				te[j] = selte[3];
				j++;
				useflag4 = false;
				if (useflag2 != false)
				{
					g_nJyankenMenu = JYANKEN_MENU_2;
				}
				else if (useflag3 != false)
				{
					g_nJyankenMenu = JYANKEN_MENU_3;
				}
				else if (useflag1 != false)
				{
					g_nJyankenMenu = JYANKEN_MENU_1;
				}
				else if (useflag5 != false)
				{
					g_nJyankenMenu = JYANKEN_MENU_5;
				}
				SetJyankenMenu();
				break;
				//�`���L
			case JYANKEN_MENU_5:

				te[j] = selte[4];
				j++;
				useflag5 = false;
				if (useflag2 != false)
				{
					g_nJyankenMenu = JYANKEN_MENU_2;
				}
				else if (useflag3 != false)
				{
					g_nJyankenMenu = JYANKEN_MENU_3;
				}
				else if (useflag4 != false)
				{
					g_nJyankenMenu = JYANKEN_MENU_4;
				}
				else if (useflag1 != false)
				{
					g_nJyankenMenu = JYANKEN_MENU_1;
				}
				SetJyankenMenu();
				break;
			}
		}
	}

}

void UpdateJyankenJadge()
{

    Cnt--;
	
    if(Cnt < 0)
	{
		jadge = false;
		win = false;
		lose = false;
		n++;
		Cnt = 35;
	}
	if (Cnt == 30)
	{
		Jyanken(n, result);
		jadge = true;
	}
	
	if (n > 4)
	{
		Cnt1 = 180;
		GetPhase()->ChangePhase(BATTLEPHASE);
	}

}

void UpdateJyankenBattle()
{

	Cnt1--;

	//�������Ƃ�
	if (result[0] > result[1])
	{
		Ac = rand() % 2;
		switch (Ac)
		{
		case 0: {Action(); break; }
		case 1: {Action2(); break; }	
		default:
			break;
		}

	}
	//��������
	if (result[0] < result[1])
	{
		Ac = rand() % 2;
		switch (Ac)
		{
		case 0: {EAction();; break; }
		case 1: {EAction2(); break; }
		default:
			break;
		}

	}

	if (Cnt1 < 0)
	{
		if (result[0] > result[1])
		{
			
			DamageEnemy(PAT);
			//if (result[0] >= 0)
			//	DamageEnemy(50 * result[0]);
		}
		if (result[0] < result[1])
		{
			
			DamagePlayer(EAT);
			//if (result[1] >= 0)
			//	DamagePlayer(50 * result[1]);
		}
		j = 0;
		n = 0;
		f = 0;

		 useflag1 = true;
		 useflag2 = true;
		 useflag3 = true;
		 useflag4 = true;
		 useflag5 = true;

		for (int r = 0; r < 4; r++)
		{
			result[r] = 0;
		}
		for (int p = 0; p < 5; p++)
		{
			te[p] = -1;
		}
		for (int f = 0; f < 5; f++)
		{
			aite[f] = Setaite[rand() % 20];//����
			selte[f] = Sette[rand() % 20];
		}
		win = false;
		lose = false;
		PAT = 0;
		EAT = 0;
		GetPhase()->ChangePhase(SETPHASE);
	}
	
}

void DrawJyankenStart()
{

}


void DrawJyankenSet()
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();


	//�|�[�Y���j���[�̕\��
	SetPolygonSize(JYANKEN_MENU_WIDTH, JYANKEN_MENU_HEIGHT);
	for (int nCntJyankenMenu = 0; nCntJyankenMenu < 5; ++nCntJyankenMenu) {
		SetPolygonPos(JYANKEN_MENU_POS_X - nCntJyankenMenu * JYANKEN_MENU_INTERVAL, JYANKEN_MENU_POS_Y);


		//�I������Ă��郁�j���[��ڗ�������
		if (nCntJyankenMenu == g_nJyankenMenu)
		{
			SetPolygonSize(size, size);		//�傫��
		}
		else
		{
			SetPolygonSize(JYANKEN_MENU_WIDTH, JYANKEN_MENU_HEIGHT);
		}

		SetPolygonColor(1.0f, 1.0f, 1.0f);

		if (nCntJyankenMenu == 0 && useflag1 == false)
		{
			SetPolygonColor(0.3f, 0.3f, 0.3f);
		}
		if (nCntJyankenMenu == 1 && useflag2 == false)
		{
			SetPolygonColor(0.3f, 0.3f, 0.3f);
		}
		if (nCntJyankenMenu == 2 && useflag3 == false)
		{
			SetPolygonColor(0.3f, 0.3f, 0.3f);
		}
		if (nCntJyankenMenu == 3 && useflag4 == false)
		{
			SetPolygonColor(0.3f, 0.3f, 0.3f);
		}
		if (nCntJyankenMenu == 4 && useflag5 == false)
		{
			SetPolygonColor(0.3f, 0.3f, 0.3f);
		}

		// �e�N�X�`���̐ݒ�
		SetPolygonTexture(g_pTextures[selte[nCntJyankenMenu]]);
		// �|���S���̕`��
		DrawPolygon(pDeviceContext);
	}

	SetPolygonColor(1.0f, 1.0f, 1.0f);
	SetPolygonSize(JYANKEN_MENU_WIDTH, JYANKEN_MENU_HEIGHT);

	for (int k = 0; k < 5;k++)
	{
		if (te[k] != -1)
		{
			//�|���S�����ݒ�
			SetPolygonPos(-200 + 100 * k, -150);			//���W
			SetPolygonSize(JYANKEN_MENU_WIDTH, JYANKEN_MENU_HEIGHT);		//�傫��
			SetPolygonTexture(g_pTextures[te[k]]);		//�e�N�X�`��

			//�|���S���̕`�揈��
			DrawPolygon(GetDeviceContext());
		}
		if (aite[k] != -1)
		{
			//�|���S�����ݒ�
			SetPolygonPos(300 + 50 * k, -300);			//���W
			SetPolygonSize(JYANKEN_MENU_WIDTH/2, JYANKEN_MENU_HEIGHT/2);		//�傫��
			SetPolygonTexture(g_pTextures[aite[k]]);		//�e�N�X�`��

			//�|���S���̕`�揈��
			DrawPolygon(GetDeviceContext());
		}
	}

	if (j >= 5)
	{
		// �^�C�g���`��
	//�|���S�����ݒ�
		SetPolygonPos(g_Position.x, g_Position.y);			//���W
		SetPolygonSize(OK_SIZE_X, OK_SIZE_Y);		//�傫��
		SetPolygonAngle(g_Angle.y);				//�p�x
		SetPolygonAlpha(g_Alpha);				//���l(�����x)

		SetPolygonUV(0.0f, 0.0f);			//0�Ԃ̃e�N�X�`��
		SetPolygonFrameSize(1.0f, 1.0f);	//�e�N�X�`��
		SetPolygonTexture(g_pTexture);		//�e�N�X�`��
		SetPolygonColor(1.0f, 1.0f, 1.0f);	//�F(RGB)

		//�|���S���̕`�揈��
		DrawPolygon(GetDeviceContext());
	}

}

void DrawJyankenJadge()
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	if (jadge == true)
	{
		if (te[n] != -1)
		{
			//�|���S�����ݒ�
			SetPolygonPos(-100, 0);			//���W
			SetPolygonTexture(g_pTextures[te[n]]);		//�e�N�X�`��
			if (win == true)
			{
				SetPolygonSize(JYANKEN_MENU_WIDTH*2, JYANKEN_MENU_HEIGHT*2);		//�傫��
			}
			else
			{
				SetPolygonSize(JYANKEN_MENU_WIDTH, JYANKEN_MENU_HEIGHT);		//�傫��
			}
			//�|���S���̕`�揈��
			DrawPolygon(GetDeviceContext());
		}
		if (aite[n] != -1)
		{
			//�|���S�����ݒ�
			SetPolygonPos(100, 0);			//���W
			SetPolygonTexture(g_pTextures[aite[n]]);		//�e�N�X�`��
			if (lose == true)
			{
				SetPolygonSize(JYANKEN_MENU_WIDTH * 2, JYANKEN_MENU_HEIGHT * 2);		//�傫��
			}
			else
			{
				SetPolygonSize(JYANKEN_MENU_WIDTH, JYANKEN_MENU_HEIGHT);		//�傫��
			}
			//�|���S���̕`�揈��
			DrawPolygon(GetDeviceContext());
		}
	}

	for (int k = 0; k < 5; k++)
	{
		if (te[k] != -1)
		{
			//�|���S�����ݒ�
			SetPolygonPos(-400 + 50 * k, -300);			//���W
			SetPolygonSize(JYANKEN_MENU_WIDTH/2, JYANKEN_MENU_HEIGHT/2);		//�傫��
			SetPolygonTexture(g_pTextures[te[k]]);		//�e�N�X�`��

			//�|���S���̕`�揈��
			DrawPolygon(GetDeviceContext());
		}
		if (aite[k] != -1)
		{
			//�|���S�����ݒ�
			SetPolygonPos(200 + 50 * k, -300);			//���W
			SetPolygonSize(JYANKEN_MENU_WIDTH / 2, JYANKEN_MENU_HEIGHT / 2);		//�傫��
			SetPolygonTexture(g_pTextures[aite[k]]);		//�e�N�X�`��

			//�|���S���̕`�揈��
			DrawPolygon(GetDeviceContext());
		}
	}

}


//����񂯂�
void Jyanken(int no, int *cnt)
{


	if ((te[no] == 0 && aite[no] == 1) || (te[no] == 1 && aite[no] == 2) || (te[no] == 2 && aite[no] == 0))
	{
		win = true;
		cnt[0]++;	// ����
		cnt[3]++;	// �A��
		switch (te[no])
		{
		case 0: { PAT += GetPlayer()->GetGUat(); break; }
		case 1: { PAT += GetPlayer()->GetTYOKIat(); break; }
		case 2: { PAT += GetPlayer()->GetPAat(); break; }
		default:
			break;
		}

	}
	else if (aite[no] == te[no])
	{
		cnt[2]++;	// ������
		//z = z - 1;
	}
	else
	{
		lose = true;
		cnt[1]++;	// �܂�
		cnt[3] = 0;	// �A�����Z�b�g
		switch (aite[no])
		{
		case 0: { EAT += GetEnemy()->GetGUat(); break; }
		case 1: { EAT += GetEnemy()->GetTYOKIat(); break; }
		case 2: { EAT += GetEnemy()->GetPAat(); break; }
		default:
			break;
		}
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
	g_nJyankenMenu = JYANKEN_MENU_1;
	SetJyankenMenu();
}

void SetJyankenMenu1(void)
{
	g_nJyankenMenu = JYANKEN_MENU_1;
	SetJyankenMenu();
}
void SetJyankenMenu2(void)
{
	g_nJyankenMenu = JYANKEN_MENU_2;
	SetJyankenMenu();
}
void SetJyankenMenu3(void)
{
	g_nJyankenMenu = JYANKEN_MENU_3;
	SetJyankenMenu();
}
void SetJyankenMenu4(void)
{
	g_nJyankenMenu = JYANKEN_MENU_4;
	SetJyankenMenu();
}
void SetJyankenMenu5(void)
{
	g_nJyankenMenu = JYANKEN_MENU_5;
	SetJyankenMenu();
}


void Cntadd()
{
	Cnt = 60;
}

