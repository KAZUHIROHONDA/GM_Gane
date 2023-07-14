//=============================================================================
//
// �^�C�g���V�[��[sceneChimera.cpp]
// Author : TAKUYA KATOU
//
//=============================================================================
#include "sceneChimera.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"		
#include "bg.h"
#include "collision.h"
#include "fade.h"
#include "chimeracs.h"
#include "pchimera.h"
#include "Light.h"
#include "Camera.h"
#include "partsmenu.h"



//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SAMPLE_TEXTURENAME	_T("data/texture/bg2.png")


#define SAMPLE_POS_X	(0)	// �����ʒuX
#define SAMPLE_POS_Y	(0)	// �����ʒuY
#define SAMPLE_SIZE_X	(SCREEN_WIDTH)	// ����
#define SAMPLE_SIZE_Y	(SCREEN_HEIGHT)	// �c��

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTexture;				// �e�N�X�`��
static XMFLOAT3 g_Position;	// ���W

Chimera player;//�v���C��

//=============================================================================
// �^�C�g���\���̏���������
//=============================================================================
HRESULT InitSceneChimera()
{
	// �e�N�X�`���ǂݍ���
	HRESULT hr =
		CreateTextureFromFile(GetDevice(),
			SAMPLE_TEXTURENAME, &g_pTexture);

	//HRESULT hr = S_OK;
	HWND hWnd = GetMainWnd();


	//�v���C���[�̏���������
	hr = InitPChimera();
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("�v���C���[�����������G���["), _T("�G���["), MB_OK | MB_ICONSTOP);
		return hr;
	}

	hr = InitParts();
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("�v���C���[�����������G���["), _T("�G���["), MB_OK | MB_ICONSTOP);
		return hr;
	}

	InitBg();

	// �J�����X�V
	GetCamera()->Init();

	player.Init();

	return hr;
}

//=============================================================================
// �^�C�g���\���̏I������
//=============================================================================
void UninitSceneChimera()
{
	UninitParts();

	UninitPChimera();

	UninitBg();

	player.Uninit();
}

//=============================================================================
// �^�C�g���\���̍X�V����
//=============================================================================
void UpdateSceneChimera()
{
	// �J�����X�V
	GetCamera()->Update();

	UpdatePChimera();


	UpdateBg();

	player.Update();
	
}

//=============================================================================
// �^�C�g���\������
//=============================================================================
void DrawSceneChimera()
{

	//�������I�u�W�F�N�g(3D�̒��ł����ɂ���)
	SetZWrite(true);

	//�w�i
	//DrawBg();

	// Z�o�b�t�@�L��
	SetZBuffer(true);

	//�v���C���[
	DrawPChimera();

	//������������
	GetLight()->SetDisable();
	
	//���������L��
	GetLight()->SetEnable();

	// Z�o�b�t�@����
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);


	//������������
	GetLight()->SetDisable();
	//�������I�u�W�F�N�g(3D�̒��ł����ɂ���)
	//�r���{�[�h�e�т傤��
	SetZWrite(false);
	

	//���������L��
	GetLight()->SetEnable();


	player.Draw();


}
