//=============================================================================
//
// �\������ [backtex.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "backtex.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"		//�ړ�������Ƃ���ɂ����

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �e�N�X�`���t�@�C����
#define BT_TEXTURENAME	_T("data/texture/backtex.png")

#define BT_POS_X	(-380)					//�����ʒuX
#define BT_POS_Y	(200)					//�����ʒuY
#define BT_SIZE_X	(450)		//����
#define BT_SIZE_Y	(250)		//�c��


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


//=============================================================================
// �^�C�g���\���̏���������
//=============================================================================
HRESULT InitBackTex()
{
	HRESULT hr;

	// �e�N�X�`���ǂݍ���
	hr = CreateTextureFromFile(GetDevice(), BT_TEXTURENAME, &g_pTexture);
	if (FAILED(hr))return hr;


	g_Position = XMFLOAT3(BT_POS_X, BT_POS_Y, 0.0f);
	g_Alpha = 1.0f;
	g_Scale = XMFLOAT2(1.0f, 1.0f);

	return hr;
}

//=============================================================================
// �^�C�g���\���̏I������
//=============================================================================
void UninitBackTex()
{
	// �e�N�X�`���J��
	SAFE_RELEASE(g_pTexture);
}

//=============================================================================
// �^�C�g���\���̍X�V����
//=============================================================================
void UpdateBackTex()
{

}

//=============================================================================
// �^�C�g���\������
//=============================================================================
void DrawBackTex()
{
	// �^�C�g���`��
	//�|���S�����ݒ�
	SetPolygonPos(g_Position.x, g_Position.y);			//���W
	SetPolygonSize(BT_SIZE_X, BT_SIZE_Y);		//�傫��
	SetPolygonAngle(g_Angle.y);				//�p�x
	SetPolygonColor(1.0f, 1.0f, 1.0f);	//�F(RGB)
	SetPolygonAlpha(g_Alpha);				//���l(�����x)
	
	SetPolygonUV(0.0f, 0.0f);			//0�Ԃ̃e�N�X�`��
	SetPolygonFrameSize(1.0f, 1.0f);	//�e�N�X�`��
	SetPolygonTexture(g_pTexture);		//�e�N�X�`��

	//�|���S���̕`�揈��
	DrawPolygon(GetDeviceContext());

}

void DrawBackTex1()
{
	// �^�C�g���`��
	//�|���S�����ݒ�
	SetPolygonPos(50, 250);			//���W
	SetPolygonSize(BT_SIZE_X, BT_SIZE_Y);		//�傫��
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
// �@ BT.h ��BT.cpp�ɂ���֐��̃v���g�^�C�v�錾������
// �A main.cpp ��BT.cpp�̏������A�I���A�X�V�A�`����Ăяo��
// �B BT.cpp �ɍ��W�ƃT�C�Y�ƃe�N�X�`���[�t�@�C�����̒萔������
// �C InitBT()�֐�����������
// �D DrawBT()�֐�����������
