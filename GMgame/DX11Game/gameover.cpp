//=============================================================================
//
// �T���v���\������ [gameover.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "gameover.h"
#include "polygon.h"
#include "Texture.h"
#include "sceneClear.h"
#include "input.h"		//�ړ�������Ƃ���ɂ����

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �e�N�X�`���t�@�C����
#define GAMEOVER_TEXTURENAME	_T("data/texture/Gameover.png")
#define GAMECLEAR_TEXTURENAME	_T("data/texture/Gameclear0.png")


#define GAMEOVER_POS_X	(0)					//�����ʒuX
#define GAMEOVER_POS_Y	(100)					//�����ʒuY
#define GAMEOVER_SIZE_X	(200)		//����
#define GAMEOVER_SIZE_Y	(200)		//�c��

#define GAMECLEAR_POS_X	(0)	
#define GAMECLEAR_POS_Y	(100)
#define GAMECLEAR_SIZE_X	(600)
#define GAMECLEAR_SIZE_Y	(120)


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTexture[2];				// �e�N�X�`��

static XMFLOAT3 g_Position;//�ʒu
static float    g_Angle;   //�p�x
static XMFLOAT2 g_Size;    //�T�C�Y
static XMFLOAT2 g_Scale;   //�g�嗦 
static int		g_nCnt;
static float	g_Alpha = 1.0f;   //����


//=============================================================================
// �T���v���\���̏���������
//=============================================================================
HRESULT InitGameover()
{
	HRESULT hr;

	// �e�N�X�`���ǂݍ���
	hr = CreateTextureFromFile(GetDevice(), GAMEOVER_TEXTURENAME, &g_pTexture[0]);
	if (FAILED(hr))return hr;
	// �e�N�X�`���ǂݍ���
	hr = CreateTextureFromFile(GetDevice(), GAMECLEAR_TEXTURENAME, &g_pTexture[1]);
	if (FAILED(hr))return hr;

	g_Position = XMFLOAT3(GAMEOVER_POS_X, GAMEOVER_POS_Y, 0.0f);
	

	return hr;
}

//=============================================================================
// �T���v���\���̏I������
//=============================================================================
void UninitGameover()
{
	// �e�N�X�`���J��
	SAFE_RELEASE(g_pTexture[0]);
	SAFE_RELEASE(g_pTexture[1]);
}

//=============================================================================
// �T���v���\���̍X�V����
//=============================================================================
void UpdateGameover()
{


}

//=============================================================================
// �T���v���\������
//=============================================================================
void DrawGameover()
{
	// �T���v���`��
	//�|���S�����ݒ�
	SetPolygonPos(g_Position.x, g_Position.y);			//���W
	SetPolygonSize(GAMEOVER_SIZE_X, GAMEOVER_SIZE_Y);		//�傫��
	SetPolygonAngle(0.0f);				//�p�x
	SetPolygonColor(1.0f, 1.0f, 1.0f);	//�F(RGB)
	SetPolygonAlpha(g_Alpha);				//���l(�����x)

	SetPolygonUV(0.0f, 0.0f);			//0�Ԃ̃e�N�X�`��
	SetPolygonFrameSize(1.0f, 1.0f);	//�e�N�X�`��
	SetPolygonTexture(g_pTexture[0]);		//�e�N�X�`��


	//�|���S���̕`�揈��
	DrawPolygon(GetDeviceContext());

}

void DrawGameclear()
{
	// �T���v���`��
	//�|���S�����ݒ�
	SetPolygonPos(g_Position.x, g_Position.y);			//���W
	SetPolygonSize(GAMEOVER_SIZE_X, GAMEOVER_SIZE_Y);		//�傫��
	SetPolygonAngle(0.0f);				//�p�x
	SetPolygonColor(1.0f, 1.0f, 1.0f);	//�F(RGB)
	SetPolygonAlpha(g_Alpha);				//���l(�����x)

	SetPolygonUV(0.0f, 0.0f);			//0�Ԃ̃e�N�X�`��
	SetPolygonFrameSize(1.0f, 1.0f);	//�e�N�X�`��
	SetPolygonTexture(g_pTexture[1]);		//�e�N�X�`��


	//�|���S���̕`�揈��
	DrawPolygon(GetDeviceContext());

}

// �T���v���`��܂ł̎菇
// �@ gameover.h ��gameover.cpp�ɂ���֐��̃v���g�^�C�v�錾������
// �A main.cpp ��gameover.cpp�̏������A�I���A�X�V�A�`����Ăяo��
// �B gameover.cpp �ɍ��W�ƃT�C�Y�ƃe�N�X�`���[�t�@�C�����̒萔������
// �C InitGAMEOVER()�֐�����������
// �D DrawGAMEOVER()�֐�����������
