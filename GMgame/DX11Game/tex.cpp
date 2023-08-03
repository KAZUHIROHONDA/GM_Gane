//=============================================================================
//
// �w�i�\������ [tex.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "tex.h"
#include "polygon.h"
#include "Texture.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �e�N�X�`���t�@�C����
#define TEX_TEXTURENAME	_T("data/texture/tex.png")
#define TEX1_TEXTURENAME	_T("data/texture/tex1.png")

#define TEX_POS_X	(0)					//�����ʒuX
#define TEX_POS_Y	(0)					//�����ʒuY
#define TEX_SIZE_X	(SCREEN_WIDTH)		//����
#define TEX_SIZE_Y	(SCREEN_HEIGHT)		//�c��


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTexture[2];				// �e�N�X�`��

//=============================================================================
// �w�i�\���̏���������
//=============================================================================
HRESULT InitTex()
{
	HRESULT hr;

	// �e�N�X�`���ǂݍ���
	hr = CreateTextureFromFile(GetDevice(), TEX_TEXTURENAME, &g_pTexture[0]);
	if (FAILED(hr))return hr;

	// �e�N�X�`���ǂݍ���
	hr = CreateTextureFromFile(GetDevice(), TEX1_TEXTURENAME, &g_pTexture[1]);
	if (FAILED(hr))return hr;
	
	return hr;
}

//=============================================================================
// �w�i�\���̏I������
//=============================================================================
void UninitTex()
{
	// �e�N�X�`���J��
	SAFE_RELEASE(g_pTexture[0]);
	SAFE_RELEASE(g_pTexture[1]);
}

//=============================================================================
// �w�i�\���̍X�V����
//=============================================================================
void UpdateTex()
{
	// (�������Ȃ�)
}

//=============================================================================
// �w�i�\������
//=============================================================================
void DrawTex()
{
	// �w�i�`��
	//�|���S�����ݒ�
	SetPolygonPos(TEX_POS_X, TEX_POS_Y);			//���W
	SetPolygonSize(TEX_SIZE_X, TEX_SIZE_Y);		//�傫��
	SetPolygonAngle(0.0f);				//�p�x
	SetPolygonColor(1.0f, 1.0f, 1.0f);	//�F(RGB)
	SetPolygonAlpha(1.0f);				//���l(�����x)

	SetPolygonUV(0.0f, 0.0f);			//0�Ԃ̃e�N�X�`��
	SetPolygonFrameSize(1.0f, 1.0f);	//�e�N�X�`��
	SetPolygonTexture(g_pTexture[0]);		//�e�N�X�`��

	//�|���S���̕`�揈��
	DrawPolygon(GetDeviceContext());

}

void DrawTex1()
{
	// �w�i�`��
	//�|���S�����ݒ�
	SetPolygonPos(TEX_POS_X, TEX_POS_Y);			//���W
	SetPolygonSize(TEX_SIZE_X, TEX_SIZE_Y);		//�傫��
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
// �@ tex.h ��tex.cpp�ɂ���֐��̃v���g�^�C�v�錾������
// �A main.cpp ��tex.cpp�̏������A�I���A�X�V�A�`����Ăяo��
// �B tex.cpp �ɍ��W�ƃT�C�Y�ƃe�N�X�`���[�t�@�C�����̒萔������
// �C InitTEX()�֐�����������
// �D DrawTEX()�֐�����������
