//=============================================================================
//
// �^�C�g���V�[��[sceneSelect.cpp]
// Author : TAKUYA KATOU
//
//=============================================================================
#include "sceneSelect.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"		
#include "fade.h"
#include "bg.h"
#include "selectstage.h"
#include "player.h"
#include "Camera.h"
#include "pchimera.h"
#include "Light.h"
#include "selecttitle.h"
#include"sound.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************



//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//=============================================================================
// �^�C�g���\���̏���������
//=============================================================================
HRESULT InitSceneSelect()
{
	HRESULT hr = S_OK;
	HWND hWnd = GetMainWnd();

	//�w�i�̏���������
	hr = InitBg();
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("�w�i�����������G���["), _T("�G���["), MB_OK | MB_ICONSTOP);
		return hr;
	}

	hr = InitSelectStage();
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("�Z���N�g�摜�����������G���["), _T("�G���["), MB_OK | MB_ICONSTOP);
		return hr;
	}

	//�v���C���[�̏���������
	hr = InitPChimera();
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("�v���C���[�����������G���["), _T("�G���["), MB_OK | MB_ICONSTOP);
		return hr;
	}

	InitSelTitle();

	PlaySound(SOUND_LABEL_BGM_4);

	// �J�����X�V
	GetCamera()->Init();

	return hr;
}

//=============================================================================
// �^�C�g���\���̏I������
//=============================================================================
void UninitSceneSelect()
{
	//�w�i�̏I������
	UninitBg();
	
	//�Z���N�g�̏I������
	UninitSelectStage();
	
	UninitSelTitle();
	
	UninitPChimera();
<<<<<<< HEAD

=======
	StopSound();
>>>>>>> feature/matsui3
}

//=============================================================================
// �^�C�g���\���̍X�V����
//=============================================================================
void UpdateSceneSelect()
{
	//�w�i�̍X�V����
	UpdateBg();

	//�I������
	UpdateSelectStage();

	// �J�����X�V
	GetCamera()->Update();

	UpdatePChimera();

	UpdateSelTitle();

	//�|�[�Y���̍��ڌ���

	E_FADE fadeState = GetFade();
	if (fadeState == E_FADE_NONE)
	{
		if (GetKeyTrigger(VK_RETURN) || GetJoyTrigger(0, 0)||GetMouseTrigger(0))
		{
			PlaySound(SOUND_LABEL_SE_SELECTS);
			//�I�𒆂̂��̂ɂ�蕪��
			SELECT_STAGE menu = GetSelectStageMenu();
			switch (menu)
			{
				//�Q�[���ɖ߂�
			case SELECT_STAGE_1:
				StartFade(SCENE_BASELECT);
				break;
				//���g���C
			case SELECT_STAGE_2:
				StartFade(SCENE_CHIMERA);
				break;
			}
		}
	}



}

//=============================================================================
// �^�C�g���\������
//=============================================================================
void DrawSceneSelect()
{

	// Z�o�b�t�@����
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);
	//�������I�u�W�F�N�g(3D�̒��ł����ɂ���)
	//SetZWrite(true);

	//�w�i
	DrawBg1();

	DrawSelectStage();

	DrawSelTitle();

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




}
