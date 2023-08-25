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

	GetPlayer()->SetHP(BPartsGet()->GetHP());
	GetPlayer()->SetGUat(BPartsGet()->GetGUat() + MPartsGet()->GetGUat() + UPartsGet()->GetGUat() + PartsGet()->GetGUat());
	GetPlayer()->SetPAat(BPartsGet()->GetPAat() + MPartsGet()->GetPAat() + UPartsGet()->GetPAat() + PartsGet()->GetPAat());
	GetPlayer()->SetTYOKIat(BPartsGet()->GetTYOKIat() + MPartsGet()->GetTYOKIat() + UPartsGet()->GetTYOKIat() + PartsGet()->GetTYOKIat());

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

	GetPlayer()->SetHP(BPartsGet()->GetHP());
	GetPlayer()->SetGUat(BPartsGet()->GetGUat() + MPartsGet()->GetGUat() + UPartsGet()->GetGUat() + PartsGet()->GetGUat());
	GetPlayer()->SetPAat(BPartsGet()->GetPAat() + MPartsGet()->GetPAat() + UPartsGet()->GetPAat() + PartsGet()->GetPAat());
	GetPlayer()->SetTYOKIat(BPartsGet()->GetTYOKIat() + MPartsGet()->GetTYOKIat() + UPartsGet()->GetTYOKIat() + PartsGet()->GetTYOKIat());

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
