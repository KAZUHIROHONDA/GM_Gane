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

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_PLAYER		"data/model/kabuBody.obj"
#define MODEL_PLAYER1		"data/model/kabu2.obj"
#define MODEL_PLAYER2		"data/model/kabu3.obj"
#define MODEL_PLAYER3		"data/model/kabu4.obj"



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

	//�|�[�Y���̍��ڌ���

	E_FADE fadeState = GetFade();
	if (fadeState == E_FADE_NONE)
	{
		if (GetKeyTrigger(VK_RETURN) || GetJoyTrigger(0, 0) || GetMouseTrigger(0))
		{
			//�I�𒆂̂��̂ɂ�蕪��
			SELECT_STAGE menu = GetSelectStageMenu();
			switch (menu)
			{
				//�Q�[���ɖ߂�
			case SELECT_STAGE_1:
				StartFade(SCENE_GAME);
				break;
				//���g���C
			case SELECT_STAGE_2:
				StartFade(SCENE_TITLE);
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
	SetZWrite(false);
	//�w�i�̕`�揈��
	DrawBg1();

	// Z�o�b�t�@�L��
	SetZBuffer(true);

	// Z�o�b�t�@����
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);

	DrawSelectStage();

}
