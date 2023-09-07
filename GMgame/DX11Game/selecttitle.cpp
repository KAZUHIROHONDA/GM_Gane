//=============================================================================
//
// �w�i�\������ [seltitle.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "selecttitle.h"
#include "polygon.h"
#include "Texture.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �e�N�X�`���t�@�C����
#define SETI_TEXTURENAME	_T("data/texture/gamesel.png")
#define SETI1_TEXTURENAME	_T("data/texture/stagesel.png")

#define SETI_POS_X	(-400)					//�����ʒuX
#define SETI_POS_Y	(300)					//�����ʒuY
#define SETI_SIZE_X	(360)		//����
#define SETI_SIZE_Y	(80)		//�c��


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTexture[2];				// �e�N�X�`��

//=============================================================================
// �w�i�\���̏���������
//=============================================================================
HRESULT InitSelTitle()
{
	HRESULT hr;

	// �e�N�X�`���ǂݍ���
	hr = CreateTextureFromFile(GetDevice(), SETI_TEXTURENAME, &g_pTexture[0]);
	if (FAILED(hr))return hr;

	// �e�N�X�`���ǂݍ���
	hr = CreateTextureFromFile(GetDevice(), SETI1_TEXTURENAME, &g_pTexture[1]);
	if (FAILED(hr))return hr;
	
	return hr;
}

//=============================================================================
// �w�i�\���̏I������
//=============================================================================
void UninitSelTitle()
{
	// �e�N�X�`���J��
	SAFE_RELEASE(g_pTexture[0]);
	SAFE_RELEASE(g_pTexture[1]);
}

//=============================================================================
// �w�i�\���̍X�V����
//=============================================================================
void UpdateSelTitle()
{
	// (�������Ȃ�)
}

//=============================================================================
// �w�i�\������
//=============================================================================
void DrawSelTitle()
{
	// �w�i�`��
	//�|���S�����ݒ�
	SetPolygonPos(SETI_POS_X, SETI_POS_Y);			//���W
	SetPolygonSize(SETI_SIZE_X, SETI_SIZE_Y);		//�傫��
	SetPolygonAngle(0.0f);				//�p�x
	SetPolygonColor(1.0f, 1.0f, 1.0f);	//�F(RGB)
	SetPolygonAlpha(1.0f);				//���l(�����x)

	SetPolygonUV(0.0f, 0.0f);			//0�Ԃ̃e�N�X�`��
	SetPolygonFrameSize(1.0f, 1.0f);	//�e�N�X�`��
	SetPolygonTexture(g_pTexture[0]);		//�e�N�X�`��

	//�|���S���̕`�揈��
	DrawPolygon(GetDeviceContext());

}

void DrawSelTitle1()
{
	// �w�i�`��
	//�|���S�����ݒ�
	SetPolygonPos(SETI_POS_X, SETI_POS_Y);			//���W
	SetPolygonSize(SETI_SIZE_X, SETI_SIZE_Y);		//�傫��
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
// �@ seltitle.h ��seltitle.cpp�ɂ���֐��̃v���g�^�C�v�錾������
// �A main.cpp ��seltitle.cpp�̏������A�I���A�X�V�A�`����Ăяo��
// �B seltitle.cpp �ɍ��W�ƃT�C�Y�ƃe�N�X�`���[�t�@�C�����̒萔������
// �C InitSETI()�֐�����������
// �D DrawSETI()�֐�����������
