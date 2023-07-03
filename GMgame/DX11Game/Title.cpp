//=============================================================================
//
// �^�C�g���\������ [Title.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "Title.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"		//�ړ�������Ƃ���ɂ����

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �e�N�X�`���t�@�C����
#define TITLE_TEXTURENAME	_T("data/texture/KIMERA.png")

#define TITLE_POS_X	(0)					//�����ʒuX
#define TITLE_POS_Y	(150)					//�����ʒuY
#define TITLE_SIZE_X	(800)		//����
#define TITLE_SIZE_Y	(400)		//�c��


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTexture;				// �e�N�X�`��

static XMFLOAT3 g_Position;//�ʒu
static XMFLOAT2    g_Angle;   //�p�x
static XMFLOAT3	g_Rot;
static XMFLOAT2 g_Size;    //�T�C�Y
static XMFLOAT2 g_Scale;   //�g�嗦 
static float	g_Alpha;   //����

int				g_nPhase;
static float g_nCnt;

//=============================================================================
// �^�C�g���\���̏���������
//=============================================================================
HRESULT InitTitle()
{
	HRESULT hr;

	// �e�N�X�`���ǂݍ���
	hr = CreateTextureFromFile(GetDevice(), TITLE_TEXTURENAME, &g_pTexture);
	if (FAILED(hr))return hr;


	g_Position = XMFLOAT3(TITLE_POS_X, TITLE_POS_Y, 0.0f);
	g_Alpha = 1.0f;
	g_Scale = XMFLOAT2(1.0f, 1.0f);

	g_nCnt = 0.0f;
	g_nPhase = 1;
	return hr;
}

//=============================================================================
// �^�C�g���\���̏I������
//=============================================================================
void UninitTitle()
{
	// �e�N�X�`���J��
	SAFE_RELEASE(g_pTexture);
}

//=============================================================================
// �^�C�g���\���̍X�V����
//=============================================================================
void UpdateTitle()
{
	g_nCnt++;
	g_Scale.x--;
}

//=============================================================================
// �^�C�g���\������
//=============================================================================
void DrawTitle()
{
	// �^�C�g���`��
	//�|���S�����ݒ�
	SetPolygonPos(g_Position.x, g_Position.y);			//���W
	SetPolygonSize(TITLE_SIZE_X, TITLE_SIZE_Y);		//�傫��
	SetPolygonAngle(g_Angle.y);				//�p�x
	SetPolygonColor(1.0f, 1.0f, 1.0f);	//�F(RGB)
	SetPolygonAlpha(g_Alpha);				//���l(�����x)
	
	SetPolygonUV(0.0f, 0.0f);			//0�Ԃ̃e�N�X�`��
	SetPolygonFrameSize(1.0f, 1.0f);	//�e�N�X�`��
	SetPolygonTexture(g_pTexture);		//�e�N�X�`��

	//�|���S���̕`�揈��
	DrawPolygon(GetDeviceContext());

}


// �^�C�g���`��܂ł̎菇
// �@ Title.h ��Title.cpp�ɂ���֐��̃v���g�^�C�v�錾������
// �A main.cpp ��Title.cpp�̏������A�I���A�X�V�A�`����Ăяo��
// �B Title.cpp �ɍ��W�ƃT�C�Y�ƃe�N�X�`���[�t�@�C�����̒萔������
// �C InitTITLE()�֐�����������
// �D DrawTITLE()�֐�����������
