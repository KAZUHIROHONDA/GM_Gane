//=============================================================================
//
// �w�i�\������ [bg.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "bg.h"
#include "polygon.h"
#include "Texture.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �e�N�X�`���t�@�C����
#define BG_TEXTURENAME	_T("data/texture/aiueo.png")
#define BG1_TEXTURENAME	_T("data/texture/setumei.jpg")

#define BG_POS_X	(0)					//�����ʒuX
#define BG_POS_Y	(0)					//�����ʒuY
#define BG_SIZE_X	(SCREEN_WIDTH)		//����
#define BG_SIZE_Y	(SCREEN_HEIGHT)		//�c��


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTexture[2];				// �e�N�X�`��

//=============================================================================
// �w�i�\���̏���������
//=============================================================================
HRESULT InitBg()
{
	HRESULT hr;

	// �e�N�X�`���ǂݍ���
	hr = CreateTextureFromFile(GetDevice(), BG_TEXTURENAME, &g_pTexture[0]);
	if (FAILED(hr))return hr;

	// �e�N�X�`���ǂݍ���
	hr = CreateTextureFromFile(GetDevice(), BG1_TEXTURENAME, &g_pTexture[1]);
	if (FAILED(hr))return hr;
	
	return hr;
}

//=============================================================================
// �w�i�\���̏I������
//=============================================================================
void UninitBg()
{
	// �e�N�X�`���J��
	SAFE_RELEASE(g_pTexture[0]);
	SAFE_RELEASE(g_pTexture[1]);
}

//=============================================================================
// �w�i�\���̍X�V����
//=============================================================================
void UpdateBg()
{
	// (�������Ȃ�)
}

//=============================================================================
// �w�i�\������
//=============================================================================
void DrawBg()
{
	// �w�i�`��
	//�|���S�����ݒ�
	SetPolygonPos(BG_POS_X, BG_POS_Y);			//���W
	SetPolygonSize(BG_SIZE_X, BG_SIZE_Y);		//�傫��
	SetPolygonAngle(0.0f);				//�p�x
	SetPolygonColor(1.0f, 1.0f, 1.0f);	//�F(RGB)
	SetPolygonAlpha(1.0f);				//���l(�����x)

	SetPolygonUV(0.0f, 0.0f);			//0�Ԃ̃e�N�X�`��
	SetPolygonFrameSize(1.0f, 1.0f);	//�e�N�X�`��
	SetPolygonTexture(g_pTexture[0]);		//�e�N�X�`��

	//�|���S���̕`�揈��
	DrawPolygon(GetDeviceContext());

}

void DrawBg1()
{
	// �w�i�`��
	//�|���S�����ݒ�
	SetPolygonPos(BG_POS_X, BG_POS_Y);			//���W
	SetPolygonSize(BG_SIZE_X, BG_SIZE_Y);		//�傫��
	SetPolygonAngle(0.0f);				//�p�x
	SetPolygonColor(1.0f, 1.0f, 1.0f);	//�F(RGB)
	SetPolygonAlpha(1.0f);				//���l(�����x)

	SetPolygonUV(0.0f, 0.0f);			//0�Ԃ̃e�N�X�`��
	SetPolygonFrameSize(1.0f, 1.0f);	//�e�N�X�`��
	SetPolygonTexture(g_pTexture[1]);		//�e�N�X�`��

	//�|���S���̕`�揈��
	DrawPolygon(GetDeviceContext());

}


// �w�i�`��܂ł̎菇
// �@ bg.h ��bg.cpp�ɂ���֐��̃v���g�^�C�v�錾������
// �A main.cpp ��bg.cpp�̏������A�I���A�X�V�A�`����Ăяo��
// �B bg.cpp �ɍ��W�ƃT�C�Y�ƃe�N�X�`���[�t�@�C�����̒萔������
// �C InitBG()�֐�����������
// �D DrawBG()�֐�����������
