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
#include "tab.h"
#include "tex.h"
#include "playercs.h"
#include "sceneTitle.h"
#include "Bpartsmenu.h"
#include "Mpartsmenu.h"
#include "partsmenu.h"
#include "Upartsmenu.h"



//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SAMPLE_TEXTURENAME	_T("data/texture/tex.png")


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

	InitBg();

	InitTab();

	InitTex();

	Sflag();

	// �J�����X�V
	GetCamera()->Init();

	player.Init();

	GetPlayer()->Percent();

	return hr;
}

//=============================================================================
// �^�C�g���\���̏I������
//=============================================================================
void UninitSceneChimera()
{

	UninitPChimera();

	UninitBg();

	UninitTab();

	UninitTex();

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

	UpdateTab();

	UpdateTex();

	
	switch (BCharaNo())
	{
	case 0://����
	{
		switch (CharaNo())//������
		{
		case 0: {	//����
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 1: {//���
			PartsGet()->Setscl(XMFLOAT3(1.8f, 1.8f, 1.8f));
			break; }
		case 2: {//�ς�
			PartsGet()->Setscl(XMFLOAT3(2.5f, 2.5f, 2.5f));
			break; }
		case 3: {//�Ƃ�
			PartsGet()->Setscl(XMFLOAT3(1.2f, 1.2f, 1.2f));
			break; }
		case 4: {//�Ԃ�
			PartsGet()->Setscl(XMFLOAT3(2.5f, 2.5f, 2.5f));
			break; }
		case 5: {//����
			PartsGet()->Setscl(XMFLOAT3(0.8f, 0.8f, 0.8f));
			break; }
		default:
			break;
		}
		switch (UCharaNo())//�����날��
		{
		case 0: {	//����
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 1: {//���
			UPartsGet()->Setscl(XMFLOAT3(1.1f, 1.1f, 1.1f));
			break; }
		case 2: {//�ς�
			UPartsGet()->Setscl(XMFLOAT3(2.2f, 2.2f, 2.2f));
			break; }
		case 3: {//�Ƃ�
			UPartsGet()->Setscl(XMFLOAT3(3.0f, 3.0f, 3.0f));
			break; }
		case 4: {//�Ԃ�
			UPartsGet()->Setscl(XMFLOAT3(2.5f, 2.5f, 2.5f));
			break; }
		case 5: {//����
			UPartsGet()->Setscl(XMFLOAT3(0.6f, 0.6f, 0.6f));
			break; }
		default:
			break;
		}
		switch (MCharaNo())//�܂�����
		{
		case 0: {	//����
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 1: {//���
			MPartsGet()->Setscl(XMFLOAT3(1.1f, 1.1f, 1.1f));
			break; }
		case 2: {//�ς�
			MPartsGet()->Setscl(XMFLOAT3(2.2f, 2.2f, 2.2f));
			break; }
		case 3: {//�Ƃ�
			MPartsGet()->Setscl(XMFLOAT3(3.0f, 3.0f, 3.0f));
			break; }
		case 4: {//�Ԃ�
			MPartsGet()->Setscl(XMFLOAT3(2.5f, 2.5f, 2.5f));
			break; }
		case 5: {//����
			MPartsGet()->Setscl(XMFLOAT3(0.6f, 0.6f, 0.6f));
			break; }
		default:
			break;
		}
		break;
	}
	case 1://���
	{
		switch (CharaNo())//������
		{
		case 0: {	//����
			PartsGet()->Setscl(XMFLOAT3(0.8f, 0.8f, 0.8f));
			break; }
		case 1: {//���
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 2: {//�ς�
			PartsGet()->Setscl(XMFLOAT3(1.6f, 1.6f, 1.6f));
			break; }
		case 3: {//�Ƃ�
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 4: {//�Ԃ�
			PartsGet()->Setscl(XMFLOAT3(1.6f, 1.6f, 1.6f));
			break; }
		case 5: {//����
			PartsGet()->Setscl(XMFLOAT3(0.5f, 0.5f, 0.5f));
			break; }
		default:
			break;
		}
		switch (UCharaNo())//�����날��
		{
		case 0: {	//����
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 1: {//���
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 2: {//�ς�
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 3: {//�Ƃ�
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 4: {//�Ԃ�
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 5: {//����
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		default:
			break;
		}
		switch (MCharaNo())//�܂�����
		{
		case 0: {	//����
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 1: {//���
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 2: {//�ς�
			MPartsGet()->Setscl(XMFLOAT3(1.6f, 1.6f, 1.6f));
			break; }
		case 3: {//�Ƃ�
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 4: {//�Ԃ�
			MPartsGet()->Setscl(XMFLOAT3(1.6f, 1.6f, 1.6f));
			break; }
		case 5: {//����
			MPartsGet()->Setscl(XMFLOAT3(0.4f, 0.4f, 0.4f));
			break; }
		default:
			break;
		}
		break;
	}
	case 2://�ς�
	{
		switch (CharaNo())
		{
		case 0: {	//����
			PartsGet()->Setscl(XMFLOAT3(0.5f, 0.5f, 0.5f));
			break; }
		case 1: {//���
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 2: {//�ς�
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 3: {//�Ƃ�
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 4: {//�Ԃ�
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 5: {//����
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		default:
			break;
		}
		switch (UCharaNo())
		{
		case 0: {	//����
			UPartsGet()->Setscl(XMFLOAT3(0.5f, 0.5f, 0.5f));
			break; }
		case 1: {//���
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 2: {//�ς�
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 3: {//�Ƃ�
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 4: {//�Ԃ�
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 5: {//����
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		default:
			break;
		}
		switch (MCharaNo())
		{
		case 0: {	//����
			MPartsGet()->Setscl(XMFLOAT3(0.5f, 0.5f, 0.5f));
			break; }
		case 1: {//���
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 2: {//�ς�
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 3: {//�Ƃ�
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 4: {//�Ԃ�
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 5: {//����
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		default:
			break;
		}
		break;
	}
	case 3://�Ƃ�
	{
		switch (CharaNo())
		{
		case 0: {	//����
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 1: {//���
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 2: {//�ς�
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 3: {//�Ƃ�
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 4: {//�Ԃ�
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 5: {//����
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		default:
			break;
		}
		switch (UCharaNo())
		{
		case 0: {	//����
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 1: {//���
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 2: {//�ς�
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 3: {//�Ƃ�
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 4: {//�Ԃ�
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 5: {//����
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		default:
			break;
		}
		switch (MCharaNo())
		{
		case 0: {	//����
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 1: {//���
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 2: {//�ς�
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 3: {//�Ƃ�
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 4: {//�Ԃ�
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 5: {//����
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		default:
			break;
		}
		break;
	}
	case 4://�Ԃ�
	{
		switch (CharaNo())
		{
		case 0: {	//����
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 1: {//���
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 2: {//�ς�
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 3: {//�Ƃ�
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 4: {//�Ԃ�
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 5: {//����
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		default:
			break;
		}
		switch (UCharaNo())
		{
		case 0: {	//����
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 1: {//���
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 2: {//�ς�
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 3: {//�Ƃ�
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 4: {//�Ԃ�
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 5: {//����
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		default:
			break;
		}
		switch (MCharaNo())
		{
		case 0: {	//����
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 1: {//���
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 2: {//�ς�
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 3: {//�Ƃ�
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 4: {//�Ԃ�
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 5: {//����
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		default:
			break;
		}
		break;
	}
	case 5://����
	{
		switch (CharaNo())
		{
		case 0: {	//����
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 1: {//���
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 2: {//�ς�
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 3: {//�Ƃ�
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 4: {//�Ԃ�
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 5: {//����
			PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		default:
			break;
		}
		switch (UCharaNo())
		{
		case 0: {	//����
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 1: {//���
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 2: {//�ς�
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 3: {//�Ƃ�
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 4: {//�Ԃ�
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 5: {//����
			UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		default:
			break;
		}
		switch (MCharaNo())
		{
		case 0: {	//����
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 1: {//���
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 2: {//�ς�
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 3: {//�Ƃ�
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 4: {//�Ԃ�
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		case 5: {//����
			MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break; }
		default:
			break;
		}
		break;
	}
	default:
		break;
	}

	Sflag();

	if (GetMouseTrigger(1))
	{
		StartFade(SCENE_SELECT);
	}

}

//=============================================================================
// �^�C�g���\������
//=============================================================================
void DrawSceneChimera()
{

	// Z�o�b�t�@����
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);
	//�������I�u�W�F�N�g(3D�̒��ł����ɂ���)
	//SetZWrite(true);

	//�w�i
	DrawBg1();

	DrawTex();

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

	DrawTab();

	player.Draw();

	GetPlayer()->DrawChimera(50,50);

}

Chimera* Get()
{
	return &player;
}
