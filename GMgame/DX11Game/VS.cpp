//=============================================================================
//
// �^�C�g���\������ [VS.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "VS.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"		//�ړ�������Ƃ���ɂ����

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �e�N�X�`���t�@�C����
#define VS_TEXTURENAME	_T("data/texture/VS.png")

#define VS_POS_X	(0)					//�����ʒuX
#define VS_POS_Y	(0)					//�����ʒuY
#define VS_SIZE_X	(300)		//����
#define VS_SIZE_Y	(300)		//�c��


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
HRESULT InitVS()
{
	HRESULT hr;

	// �e�N�X�`���ǂݍ���
	hr = CreateTextureFromFile(GetDevice(), VS_TEXTURENAME, &g_pTexture);
	if (FAILED(hr))return hr;


	g_Position = XMFLOAT3(VS_POS_X, VS_POS_Y, 0.0f);
	g_Alpha = 1.0f;
	g_Scale = XMFLOAT2(1.0f, 1.0f);

	return hr;
}

//=============================================================================
// �^�C�g���\���̏I������
//=============================================================================
void UninitVS()
{
	// �e�N�X�`���J��
	SAFE_RELEASE(g_pTexture);
}

//=============================================================================
// �^�C�g���\���̍X�V����
//=============================================================================
void UpdateVS()
{

}

//=============================================================================
// �^�C�g���\������
//=============================================================================
void DrawVS()
{
	// �^�C�g���`��
	//�|���S�����ݒ�
	SetPolygonPos(g_Position.x, g_Position.y);			//���W
	SetPolygonSize(VS_SIZE_X, VS_SIZE_Y);		//�傫��
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
// �@ VS.h ��VS.cpp�ɂ���֐��̃v���g�^�C�v�錾������
// �A main.cpp ��VS.cpp�̏������A�I���A�X�V�A�`����Ăяo��
// �B VS.cpp �ɍ��W�ƃT�C�Y�ƃe�N�X�`���[�t�@�C�����̒萔������
// �C InitVS()�֐�����������
// �D DrawVS()�֐�����������
