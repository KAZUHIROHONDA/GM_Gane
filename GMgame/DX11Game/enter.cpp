//=============================================================================
//
// �G���^�[�\������ [enter.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "enter.h"
#include "polygon.h"
#include "Texture.h"



//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �e�N�X�`���t�@�C����
#define ENTER_TEXTURENAME	_T("data/texture/select.png")

#define ENTER_POS_X	(0)					//�����ʒuX
#define ENTER_POS_Y	(-200)					//�����ʒuY
#define ENTER_SIZE_X	(360)		//����
#define ENTER_SIZE_Y	(80)		//�c��

#define ENTER_FRAME_X (1)			//�A�j���̉��̐�
#define ENTER_FRAME_Y (1)			//�A�j���̏c�̐�

//�\���̒�`
typedef struct {
	int nPatNo;		//�\���p�^�[���ԍ�
	int nCount;		//�\���t���[����
}ANIM_PAT;

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTexture; // �e�N�X�`��

static ANIM_PAT g_animPat[] = {
	{0,6},
    {-1,-1} //�ԕ�
};

static XMFLOAT3 g_Position;//�ʒu
static float    g_Angle;   //�p�x
static XMFLOAT2 g_Size;    //�T�C�Y
static XMFLOAT2 g_Scale;   //�g�嗦 
static float	g_Alpha;   //����

static int g_nAnimNo;		//�\�����No
static int g_nCount;		//�c��t���[����
static int g_nCnt;

//=============================================================================
// �G���^�[�\���̏���������
//=============================================================================
HRESULT InitEnter()
{
	HRESULT hr;

	// �e�N�X�`���ǂݍ���
	hr = CreateTextureFromFile(GetDevice(), ENTER_TEXTURENAME, &g_pTexture);
	if (FAILED(hr))return hr;
	
	//�ϐ�������
	g_Position = XMFLOAT3(ENTER_POS_X, ENTER_POS_Y, 0.0f);
	g_Size = XMFLOAT2(ENTER_SIZE_X, ENTER_SIZE_Y);
	g_Angle = 0.0f;
	g_Scale = XMFLOAT2(1.0f, 1.0f);

	g_nAnimNo = 0;
	g_nCount = g_animPat[g_nAnimNo].nCount;
	g_Alpha = 1.0f;
				//�ŏ��̃A�j���\������

	return hr;
}

//=============================================================================
// �G���^�[�\���̏I������
//=============================================================================
void UninitEnter()
{
	// �e�N�X�`���J��
	SAFE_RELEASE(g_pTexture);
}

//=============================================================================
// �G���^�[�\���̍X�V����
//=============================================================================
void UpdateEnter()
{
	g_nCnt++;

	if (g_Alpha > 0)
	{
		g_Alpha -= 0.01;
	}
	else if (g_Alpha < 0)
	{
		g_Alpha = 1.0;
	}
	

	// �A�j���[�V�������̍X�V
	g_nCount--;		//�c��J�E���g�����炷
	if (g_nCount <= 0)
	{
		//���̊G�͏I���
		g_nAnimNo++;		//���̊G�ɐi�߂�
		if (g_animPat[g_nAnimNo].nPatNo < 0)
		{
			//�ԕ����Q�Ƃ����̂�
			g_nAnimNo = 0;	//�擪�ɖ߂�
		}
		//�J�E���^�����̊G�̕\���t���[�����ɐݒ�
		g_nCount = g_animPat[g_nAnimNo].nCount;
	}


}

//=============================================================================
// �G���^�[�\������
//=============================================================================
void DrawEnter()
{
	// �G���^�[�`��
	//�|���S�����ݒ�
	SetPolygonPos(g_Position.x, g_Position.y);			//���W
	SetPolygonSize(g_Size.x*g_Scale.x, g_Size.y*g_Scale.y);		//�傫��
	SetPolygonAngle(g_Angle);				//�p�x
	SetPolygonColor(1.0f, 1.0f, 1.0f);	//�F(RGB)
	SetPolygonAlpha(g_Alpha);				//���l(�����x)

	int u, v;
	u = g_animPat[g_nAnimNo].nPatNo % ENTER_FRAME_X;
	v = g_animPat[g_nAnimNo].nPatNo / ENTER_FRAME_X;
	SetPolygonUV(u / (float)ENTER_FRAME_X,v / (float)ENTER_FRAME_Y);	
	SetPolygonFrameSize(1.0f/ENTER_FRAME_X, 1.0f/ ENTER_FRAME_Y);	//�e�N�X�`��
	SetPolygonTexture(g_pTexture);		//�e�N�X�`��

	//�|���S���̕`�揈��
	DrawPolygon(GetDeviceContext());

	//���ɖ߂�
	SetPolygonUV(0.0f, 0.0f);			//0�Ԃ̃e�N�X�`��
	SetPolygonFrameSize(1.0f, 1.0f);	//�e�N�X�`��

}

void DrawEnter1()
{
	// �G���^�[�`��
	//�|���S�����ݒ�
	SetPolygonPos(0, -200);			//���W
	SetPolygonSize(g_Size.x*g_Scale.x, g_Size.y*g_Scale.y);		//�傫��
	SetPolygonAngle(g_Angle);				//�p�x
	SetPolygonColor(1.0f, 1.0f, 1.0f);	//�F(RGB)
	SetPolygonAlpha(g_Alpha);				//���l(�����x)

	int u, v;
	u = g_animPat[g_nAnimNo].nPatNo % ENTER_FRAME_X;
	v = g_animPat[g_nAnimNo].nPatNo / ENTER_FRAME_X;
	SetPolygonUV(u / (float)ENTER_FRAME_X, v / (float)ENTER_FRAME_Y);
	SetPolygonFrameSize(1.0f / ENTER_FRAME_X, 1.0f / ENTER_FRAME_Y);	//�e�N�X�`��
	SetPolygonTexture(g_pTexture);		//�e�N�X�`��

	//�|���S���̕`�揈��
	DrawPolygon(GetDeviceContext());

	//���ɖ߂�
	SetPolygonUV(0.0f, 0.0f);			//0�Ԃ̃e�N�X�`��
	SetPolygonFrameSize(1.0f, 1.0f);	//�e�N�X�`��
}


// �G���^�[�`��܂ł̎菇
// �@ enter.h ��enter.cpp�ɂ���֐��̃v���g�^�C�v�錾������
// �A main.cpp ��enter.cpp�̏������A�I���A�X�V�A�`����Ăяo��
// �B enter.cpp �ɍ��W�ƃT�C�Y�ƃe�N�X�`���[�t�@�C�����̒萔������
// �C InitENTER()�֐�����������
// �D DrawENTER()�֐�����������
