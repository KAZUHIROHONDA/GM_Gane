//=============================================================================
//
// �^�C�g���V�[��[sceneTitle.cpp]
// Author : TAKUYA KATOU
//
//=============================================================================
#include "sceneTitle.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"		
#include "fade.h"
#include "bg.h"
#include "Title.h"
#include "enter.h"
#include "stage.h"
#include "select1.h"
#include "sceneClear.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//=============================================================================
// �^�C�g���\���̏���������
//=============================================================================
HRESULT InitSceneTitle()
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

	//�^�C�g��
	InitTitle();

	//�I��
	InitSelect1();

	//�X�e�[�W�̏�����
	SetStageNo(0);

	return hr;
}

//=============================================================================
// �^�C�g���\���̏I������
//=============================================================================
void UninitSceneTitle()
{

	//�w�i�̏I������
	UninitBg();

	//�^�C�g��
	UninitTitle();

	//�I��
	UninitSelect1();
}

//=============================================================================
// �^�C�g���\���̍X�V����
//=============================================================================
void UpdateSceneTitle()
{

	//�w�i�̍X�V����
	UpdateBg();

	//�^�C�g��
	UpdateTitle();
	//�I��
	UpdateSelect1();

	E_FADE fadeState = GetFade();
	if (fadeState == E_FADE_NONE)
	{
		if (GetKeyTrigger(VK_RETURN)|| GetJoyTrigger(0, 0))
		{
			//�I�𒆂̂��̂ɂ�蕪��
			SELECT1_MENU menu = GetSelect1Menu();
			switch (menu)
			{
				//�Q�[���X�^�[�g
			case SELECT1_MENU_CONTINUE:
				StartFade(SCENE_GAME);
				SetStageNo(0);
				break;
				//�������
			case SELECT1_MENU_RETRY:
				StartFade(SCENE_SETUMEI);
				break;
				//�X�e�[�W�Z���N�g
			case SELECT1_MENU_QUIT:
				StartFade(SCENE_SELECT);
				break;
			}
		}
	}

	
}

//=============================================================================
// �^�C�g���\������
//=============================================================================
void DrawSceneTitle()
{
	SetZWrite(false);
	//�w�i�̕`�揈��
	DrawBg();

	// Z�o�b�t�@�L��
	SetZBuffer(true);

	// Z�o�b�t�@����
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);
	

	//�^�C�g��
	DrawTitle();
	//�I��
	DrawSelect1();

	
}
